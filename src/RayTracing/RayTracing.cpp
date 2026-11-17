#include "RayTracing.h"
#include"Ray.h"
#include"Random.h"
#include <execution>
#include<mutex>
#include<iostream>
#include<format>
#include"../Test/RayTracing/Pdfs.h"

RayTracing::RayTracing()
{
}

void RayTracing::Draw()
{
	uint32_t width = m_ColorAttachment->Width();
	uint32_t height = m_ColorAttachment->Height();

	glm::vec3 front = glm::normalize(m_LookAt - m_CameraPosition);
	glm::vec3 right = glm::normalize(glm::cross(m_Up, -front));
	glm::vec3 up = glm::normalize(glm::cross(-front, right));

	float aspectRatio = width / (float)height;
	float halfH = glm::tan(glm::radians(m_Fov / 2)) * m_FocusDistance;
	float halfW = halfH * aspectRatio;

	float dx = halfW * 2 / width;
	float dy = halfH * 2 / height;

	float x = -halfW + 0.5 * dx;
	float y = -halfH + 0.5 * dy;

	float defocusRadius = m_FocusDistance * glm::tan(glm::radians(m_DefocusAngle / 2));

	glm::vec3 LeftBottom = m_CameraPosition + front * m_FocusDistance + x * right + y * up;

	if (0)
	{
		for (int indexY = 0; indexY < height; indexY++)
		{
			for (int indexX = 0; indexX < width; indexX++)
			{
				glm::ivec2 pixel(indexX, indexY);
				glm::vec3 pixelPosition = LeftBottom + (float)indexX * right * dx + (float)indexY * up * dy;
				glm::vec4 color(0);
				for (int i = 0; i < m_SamplePerPixel; i++)
				{
					glm::vec3 pixelP = pixelPosition + (Random01() - 0.5f) * dx * right + (Random01() - 0.5f) * dy * up;
					glm::vec3 center = m_CameraPosition;
					if (m_DefocusAngle > 0)
					{
						glm::vec2 uv = RandomDirection2D() * defocusRadius;
						center = center + uv.x * right + uv.y * up;
					}
					Ray ray(center, glm::normalize(pixelP - center));
					color += RayColor(ray);
				}
				color /= m_SamplePerPixel;
				color = glm::vec4(glm::pow(glm::vec3(color), glm::vec3(1 / 2.2f)), 1);
				m_ColorAttachment->Write(pixel, color);
			}
		}
	}
	else
	{

		std::vector<glm::ivec4> tiles;
		int32_t countX = width / m_TileSize;
		int32_t countY = height / m_TileSize;
		if (countX * m_TileSize < width)
			countX++;
		if (countY * m_TileSize < height)
			countY++;
		tiles.reserve(countX * countY);
		for (int x = 0; x < width; x += m_TileSize)
		{
			for (int y = 0; y < height; y += m_TileSize)
			{
				tiles.emplace_back(glm::ivec4(x, y, glm::min(x + m_TileSize, (int32_t)width), glm::min(y + m_TileSize, (int32_t)height)));
			}
		}

		std::mutex mtx;
		int count = 0;
		float percent = 100.0f / (countX * countY);

		std::for_each(std::execution::par, tiles.begin(), tiles.end(),
			[&](const glm::ivec4& tile)
			{
				for (int indexY = tile[1]; indexY < tile[3]; indexY++)
				{
					for (int indexX = tile[0]; indexX < tile[2]; indexX++)
					{
						glm::ivec2 pixel(indexX, indexY);
						glm::vec3 pixelPosition = LeftBottom + (float)indexX * right * dx + (float)indexY * up * dy;
						glm::vec4 color(0);
						if (!m_EnableJittering)
						{
							for (int i = 0; i < m_SamplePerPixel; i++)
							{
								glm::vec3 pixelP = pixelPosition + (Random01() - 0.5f) * dx * right + (Random01() - 0.5f) * dy * up;
								glm::vec3 center = m_CameraPosition;
								if (m_DefocusAngle > 0)
								{
									glm::vec2 uv = RandomDirection2D() * defocusRadius;
									center = center + uv.x * right + uv.y * up;
								}
								Ray ray(center, glm::normalize(pixelP - center));
								color += RayColor(ray);
							}
						}
						else
						{
							int sqrtSPP = glm::sqrt(m_SamplePerPixel);
							for (int i = 0; i < sqrtSPP; i++)
							{
								for (int j = 0; j < sqrtSPP; j++)
								{
									glm::vec3 pixelP = pixelPosition + ((i + Random01()) / sqrtSPP - 0.5f) * dx * right + ((j + Random01()) / sqrtSPP - 0.5f) * dy * up;
									glm::vec3 center = m_CameraPosition;
									if (m_DefocusAngle > 0)
									{
										glm::vec2 uv = RandomDirection2D() * defocusRadius;
										center = center + uv.x * right + uv.y * up;
									}
									Ray ray(center, glm::normalize(pixelP - center));
									color += RayColor(ray);
								}
							}
						}
						color /= m_SamplePerPixel;
						color = glm::min(color, glm::vec4(1));
						color = glm::vec4(glm::pow(glm::vec3(color), glm::vec3(1 / 2.2f)), 1);
						m_ColorAttachment->Write(pixel, color);
					}
				}
				std::unique_lock<std::mutex>(mtx);
				count++;
				std::cout << std::format("{}%\n", count * percent);
			});
	}
}

glm::vec4 RayTracing::RayColor(Ray ray, int depth)
{
	if (depth > m_MaxDepth)
		return glm::vec4(0, 0, 0, 1);
	HitRecord record;
	if (m_Shape->RayCheck(ray, record))
	{
		Ray newRay;
		ScatterRecord scatter;
		float pdfValue;
		std::shared_ptr<Pdf> pdf;
		
		glm::vec3 emit = record.mat->Emitted(ray, record);
		
		if (!record.mat->Scatter(ray, record, scatter))
			return glm::vec4(emit, 1.0f);
		
		if (scatter.skipPdf)
			return glm::vec4(scatter.attenuation, 1.0f) * RayColor(scatter.ray, depth + 1);
		
		if (m_Light)
		{
			auto lightPdf = std::make_shared<ShapePdf>(m_Light, record.p);
			pdf = std::make_shared<MixturePdf>(lightPdf, scatter.pdf);
		}
		else
		{
			pdf = scatter.pdf;
		}
		
		newRay = Ray(record.p, pdf->Gen());
		pdfValue = pdf->Value(newRay.Direction());

		float scatterPdf = record.mat->ScatterPdf(ray, record, newRay);
		
		return glm::vec4(scatter.attenuation * scatterPdf / pdfValue, 1.0f) * RayColor(newRay, depth + 1) + glm::vec4(emit, 1);
	}
	else
	{
		return glm::vec4(m_BkColor, 1.0f);
	}
}



