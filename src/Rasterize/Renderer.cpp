#include"Renderer.h"
#include <execution>
#include<vector>

#include<iostream>
#include<format>

inline float cross2(glm::vec2 a, glm::vec2 b) {
	return a.x * b.y - a.y * b.x;
}

void Renderer::DrawIndexed(const std::vector<float>& vertexs, const std::vector<uint32_t>& index)
{
	if (!ReadyForDraw())
		return;
	
	uint32_t stride = m_Shader->VertexStride();
	PolygonType type = m_Shader->Polygon();
	if (type == PolygonType::eTriangle)
	{
		std::array<const float*, 3> pData;
		for (int i = 0; i < index.size() / 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				pData[j] = vertexs.data() + stride * index[3 * i + j];
			}
			DrawTriangle(pData);
		}
	}
	else if (type == PolygonType::eLine)
	{
		std::array<const float*, 2> pData;
		for (int i = 0; i < index.size() / 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				pData[j] = vertexs.data() + stride * index[2 * i + j];
			}
			DrawLine(pData);
		}
	}
	else if (type == PolygonType::ePoint)
	{
		for (int i = 0; i < index.size(); i++)
		{
			DrawPoint(vertexs.data() + stride * index[i]);
		}
	}
}

void Renderer::SetShader(std::shared_ptr<Shader> shader)
{
	glm::vec4 defaultColor(0, 0, 0, 1);
	std::array<float, 16> defaultUniform{0};
	if (!shader)
		return;
	m_Shader = shader;
	m_Shader->SetSampleTextureFunc([this , defaultColor](uint32_t index, glm::vec2 uv) {
		if (index < 0 || index >= MaxTexture)
			return defaultColor;
		if (!m_Textures[index])
			return defaultColor;
		return m_Textures[index]->Sample(uv);
		});
	m_Shader->SetWriteColorAttachmentFunc([this](uint32_t index, glm::ivec2 position, glm::vec4 color) {
		color = glm::clamp(color, 0.0f, 1.0f);
		m_ColorAttachment->Write(position, color);
		});
	m_Shader->SetRandomUniform([this, defaultUniform](uint32_t offset, uint32_t count)
		{
			if (offset < 0)
				return defaultUniform.data();
			if (count == 0 || offset + count > MaxUniform)
				return defaultUniform.data();
			return (const float*)&m_Uniforms[offset];
		});
}

void Renderer::SetUniformBuffer(float* data, uint32_t size)
{
	size = glm::max(size, (uint32_t)sizeof(float) * MaxUniform);
	memcpy(m_Uniforms.data(), data, size);
}

bool Renderer::ReadyForDraw()
{
	return m_Shader && m_ColorAttachment;
}

void Renderer::DrawTriangle(const std::array<const float*, 3>& pData)
{
	uint32_t width = m_ColorAttachment->Width();
	uint32_t height = m_ColorAttachment->Height();

	std::array<glm::vec4, 3> point;
	std::array<glm::vec4, 3> point2;
	m_Shader->Clear();
	for (int i = 0; i < 3; i++)
	{
		point[i] = m_Shader->VertexShader(pData[i]);
	}

	int count = 0;
	count = std::count_if(point.begin(), point.end(), [](const glm::vec4& v) {return v.w + v.z < 0; });
	int countZero = std::count_if(point.begin(), point.end(), [](const glm::vec4& v) {return v.w + v.z == 0; });
	if (count == 3)
	{
		return;
	}
	else if(count == 0)
	{
		RasterizeTriangle(point);
	}
	else if(count == 2)
	{
		if(countZero == 1)
		{
			return;
		}
		else
		{
			uint32_t midDataCount = m_Shader->GetMidDataCount();
			float k0 = point[0].w + point[0].z;
			float k1 = point[1].w + point[1].z;
			float k2 = point[2].w + point[2].z;
			float* data0 = m_Shader->GetMidData(0);
			float* data1 = m_Shader->GetMidData(1);
			float* data2 = m_Shader->GetMidData(2);
			if (k0 > 0)
			{
				point2[0] = point[0];

				float t = k0 / (k0 - k1);
				for (int i = 0; i < midDataCount; i++)
				{
					data1[i] = glm::mix(data0[i], data1[i], t);
				}
				point2[1] = glm::mix(point[0], point[1], t);

				t = k0 / (k0 - k2);
				for (int i = 0; i < midDataCount; i++)
				{
					data2[i] = glm::mix(data0[i], data2[i], t);
				}
				point2[2] = glm::mix(point[0], point[2], t);
				RasterizeTriangle(point2);
			}
			else if(k1 > 0)
			{
				point2[1] = point[1];

				float t = k1 / (k1 - k0);
				for (int i = 0; i < midDataCount; i++)
				{
					data0[i] = glm::mix(data1[i], data0[i], t);
				}
				point2[0] = glm::mix(point[1], point[0], t);

				t = k1 / (k1 - k2);
				for (int i = 0; i < midDataCount; i++)
				{
					data2[i] = glm::mix(data1[i], data2[i], t);
				}
				point2[2] = glm::mix(point[1], point[2], t);
				RasterizeTriangle(point2);
			}
			else
			{
				point2[2] = point[2];

				float t = k2 / (k2 - k0);
				for (int i = 0; i < midDataCount; i++)
				{
					data0[i] = glm::mix(data2[i], data0[i], t);
				}
				point2[0] = glm::mix(point[2], point[0], t);

				t = k2 / (k2 - k1);
				for (int i = 0; i < midDataCount; i++)
				{
					data1[i] = glm::mix(data2[i], data1[i], t);
				}
				point2[1] = glm::mix(point[2], point[1], t);
				RasterizeTriangle(point2);
			}
		}
	}
	else
	{
		if (countZero == 2)
		{
			return;
		}
		else if (countZero == 1)
		{
			uint32_t midDataCount = m_Shader->GetMidDataCount();
			float k0 = point[0].w + point[0].z;
			float k1 = point[1].w + point[1].z;
			float k2 = point[2].w + point[2].z;
			float* data0 = m_Shader->GetMidData(0);
			float* data1 = m_Shader->GetMidData(1);
			float* data2 = m_Shader->GetMidData(2);
			if (k0 > 0)
			{
				if (k1 < 0)
				{
					float t = k0 / (k0 - k1);
					point2[0] = point[0];
					point2[2] = point[2];
					for (int i = 0; i < midDataCount; i++)
					{
						data1[i] = glm::mix(data0[i], data1[i], t);
					}
					point2[1] = glm::mix(point[0], point[1], t);
					RasterizeTriangle(point2);
				}
				else
				{
					float t = k0 / (k0 - k2);
					point2[0] = point[0];
					point2[1] = point[1];
					for (int i = 0; i < midDataCount; i++)
					{
						data1[i] = glm::mix(data0[i], data2[i], t);
					}
					point2[2] = glm::mix(point[0], point[2], t);
					RasterizeTriangle(point2);
				}
			}
			else if (k1 > 0)
			{
				if (k0 < 0)
				{
					float t = k1 / (k1 - k0);
					point2[1] = point[1];
					point2[2] = point[2];
					for (int i = 0; i < midDataCount; i++)
					{
						data0[i] = glm::mix(data1[i], data0[i], t);
					}
					point2[0] = glm::mix(point[1], point[0], t);
					RasterizeTriangle(point2);
				}
				else
				{
					float t = k1 / (k1 - k2);
					point2[0] = point[0];
					point2[1] = point[1];
					for (int i = 0; i < midDataCount; i++)
					{
						data2[i] = glm::mix(data1[i], data2[i], t);
					}
					point2[2] = glm::mix(point[1], point[2], t);
					RasterizeTriangle(point2);
				}
			}
			else
			{
				if (k0 < 0)
				{
					float t = k2 / (k2 - k0);
					point2[1] = point[1];
					point2[2] = point[2];
					for (int i = 0; i < midDataCount; i++)
					{
						data0[i] = glm::mix(data2[i], data0[i], t);
					}
					point2[0] = glm::mix(point[2], point[0], t);
					RasterizeTriangle(point2);
				}
				else
				{
					float t = k2 / (k2 - k1);
					point2[0] = point[0];
					point2[2] = point[2];
					for (int i = 0; i < midDataCount; i++)
					{
						data1[i] = glm::mix(data2[i], data1[i], t);
					}
					point2[1] = glm::mix(point[2], point[1], t);
					RasterizeTriangle(point2);
				}
			}
		}
		else
		{
			uint32_t midDataCount = m_Shader->GetMidDataCount();
			float k0 = point[0].w + point[0].z;
			float k1 = point[1].w + point[1].z;
			float k2 = point[2].w + point[2].z;
			float* data0 = m_Shader->GetMidData(0);
			float* data1 = m_Shader->GetMidData(1);
			float* data2 = m_Shader->GetMidData(2);
			std::vector<float> tmpData(midDataCount);
			if (k0 < 0)
			{
				point2[1] = point[1];
				point2[2] = point[2];
				float t = k2 / (k2 - k0);
				for (int i = 0; i < midDataCount; i++)
				{
					tmpData[i] = data0[i];
					data0[i] = glm::mix(data2[i], data0[i], t);
				}
				point2[0] = glm::mix(point[2], point[0], t);
				RasterizeTriangle(point2);

				point2[2] = point2[0];
				t = k1 / (k1 - k0);
				for (int i = 0; i < midDataCount; i++)
				{
					data2[i] = data0[i];
					data0[i] = glm::mix(data1[i], tmpData[i], t);
				}
				point2[0] = glm::mix(point[1], point[0], t);
				RasterizeTriangle(point2);
			}
			else if (k1 < 0)
			{
				point2[2] = point[2];
				point2[0] = point[0];
				float t = k0 / (k0 - k1);
				for (int i = 0; i < midDataCount; i++)
				{
					tmpData[i] = data1[i];
					data1[i] = glm::mix(data0[i], data1[i], t);
				}
				point2[1] = glm::mix(point[0], point[1], t);
				RasterizeTriangle(point2);

				point2[0] = point2[1];
				t = k2 / (k2 - k1);
				for (int i = 0; i < midDataCount; i++)
				{
					data0[i] = data1[i];
					data1[i] = glm::mix(data2[i], tmpData[i], t);
				}
				point2[1] = glm::mix(point[2], point[1], t);
				RasterizeTriangle(point2);
			}
			else
			{
				point2[0] = point[0];
				point2[1] = point[1];
				float t = k1 / (k1 - k2);
				for (int i = 0; i < midDataCount; i++)
				{
					tmpData[i] = data2[i];
					data2[i] = glm::mix(data1[i], data2[i], t);
				}
				point2[2] = glm::mix(point[1], point[2], t);
				RasterizeTriangle(point2);

				point2[1] = point2[2];
				t = k0 / (k0 - k2);
				for (int i = 0; i < midDataCount; i++)
				{
					data1[i] = data2[i];
					data2[i] = glm::mix(data0[i], tmpData[i], t);
				}
				point2[2] = glm::mix(point[0], point[2], t);
				RasterizeTriangle(point2);
			}
		}
	}
	
	
	

}

void Renderer::RasterizeTriangle(const std::array<glm::vec4, 3>& point)
{
	uint32_t width = m_ColorAttachment->Width();
	uint32_t height = m_ColorAttachment->Height();

	std::array<glm::vec3, 3> ndc;
	std::array<glm::vec2, 3> screenPosition;
	for (int i = 0; i < 3; i++)
	{
		ndc[i] = glm::vec3(point[i]) * (1 / point[i].w);
		screenPosition[i].x = (ndc[i].x + 1) / 2 * width;
		screenPosition[i].y = (ndc[i].y + 1) / 2 * height;
	}

	if (Config.FaceCull)
	{
		glm::vec2 d1(ndc[1] - ndc[0]);
		glm::vec2 d2(ndc[2] - ndc[1]);
		float direct = cross2(d1, d2);
		if (direct < 0)
			return;
	}

	if (Config.Wireframe)
	{
		float k = (screenPosition[1].y - screenPosition[0].y) / (screenPosition[1].x - screenPosition[0].x);
		if (k > 1 || k < -1)
		{
			k = 1 / k;
			float x = screenPosition[0].x;
			float y = screenPosition[0].y;
			float finalY = screenPosition[1].y;
			float dy = 1;
			if (y > finalY)
			{
				dy = -1;
				k = -k;
			}
			for (;dy == 1 && y < finalY || dy == -1 && y > finalY; y += dy,x += k)
			{
				if (x<0 || x>=width || y<0 || y>=height)
					continue;
				glm::ivec2 pixel = { x,y };
				glm::vec2 pixelPosition = pixel;
				pixelPosition += glm::vec2(0.5f);
				glm::vec3 barycentricCoordinates = ComputeBarycentricCoordinates(pixelPosition, screenPosition);
				float k1 = barycentricCoordinates.x / point[0].w;
				float k2 = barycentricCoordinates.y / point[1].w;
				float k3 = barycentricCoordinates.z / point[2].w;
				float sumK = 1 / (k1 + k2 + k3);
				barycentricCoordinates.x = k1 * sumK;
				barycentricCoordinates.y = k2 * sumK;
				barycentricCoordinates.z = k3 * sumK;
				float depth = (glm::dot(barycentricCoordinates, glm::vec3(ndc[0].z, ndc[1].z, ndc[2].z)) + 1) / 2;
				if (depth >= 0 && depth < m_DepthAttachment->SamplePixel({ pixel.x,pixel.y }).r)
				{
					m_DepthAttachment->Write({ pixel.x,pixel.y }, { depth,0,0,0 });
					m_Shader->FragmentShader(barycentricCoordinates, glm::ivec2(pixel.x, pixel.y));
				}
			}
		}
		else
		{
			float x = screenPosition[0].x;
			float y = screenPosition[0].y;
			float finalX = screenPosition[1].x;
			float dx = 1;
			if (x > finalX)
			{
				dx = -1;
				k = -k;
			}
			for (;dx>0 && x < finalX || dx == -1 && x> finalX; x += dx,y += k)
			{
				if (x<0 || x>=width || y<0 || y>=height)
					continue;
				glm::ivec2 pixel = { x,y };
				glm::vec2 pixelPosition = pixel;
				pixelPosition += glm::vec2(0.5f);
				glm::vec3 barycentricCoordinates = ComputeBarycentricCoordinates(pixelPosition, screenPosition);
				float k1 = barycentricCoordinates.x / point[0].w;
				float k2 = barycentricCoordinates.y / point[1].w;
				float k3 = barycentricCoordinates.z / point[2].w;
				float sumK = 1 / (k1 + k2 + k3);
				barycentricCoordinates.x = k1 * sumK;
				barycentricCoordinates.y = k2 * sumK;
				barycentricCoordinates.z = k3 * sumK;
				float depth = (glm::dot(barycentricCoordinates, glm::vec3(ndc[0].z, ndc[1].z, ndc[2].z)) + 1) / 2;
				if (depth >= 0 && depth < m_DepthAttachment->SamplePixel({ pixel.x,pixel.y }).r)
				{
					m_DepthAttachment->Write({ pixel.x,pixel.y }, { depth,0,0,0 });
					m_Shader->FragmentShader(barycentricCoordinates, glm::ivec2(pixel.x, pixel.y));
				}
			}
		}

		k = (screenPosition[2].y - screenPosition[0].y) / (screenPosition[2].x - screenPosition[0].x);
		if (k > 1 || k < -1)
		{
			k = 1 / k;
			float x = screenPosition[0].x;
			float y = screenPosition[0].y;
			float finalY = screenPosition[2].y;
			float dy = 1;
			if (y > finalY)
			{
				dy = -1;
				k = -k;
			}
			for (; dy == 1 && y < finalY || dy == -1 && y > finalY; y += dy, x += k)
			{
				if (x<0 || x>=width || y<0 || y>=height)
					continue;
				glm::ivec2 pixel = { x,y };
				glm::vec2 pixelPosition = pixel;
				pixelPosition += glm::vec2(0.5f);
				glm::vec3 barycentricCoordinates = ComputeBarycentricCoordinates(pixelPosition, screenPosition);
				float k1 = barycentricCoordinates.x / point[0].w;
				float k2 = barycentricCoordinates.y / point[1].w;
				float k3 = barycentricCoordinates.z / point[2].w;
				float sumK = 1 / (k1 + k2 + k3);
				barycentricCoordinates.x = k1 * sumK;
				barycentricCoordinates.y = k2 * sumK;
				barycentricCoordinates.z = k3 * sumK;
				float depth = (glm::dot(barycentricCoordinates, glm::vec3(ndc[0].z, ndc[1].z, ndc[2].z)) + 1) / 2;
				if (depth >= 0 && depth < m_DepthAttachment->SamplePixel({ pixel.x,pixel.y }).r)
				{
					m_DepthAttachment->Write({ pixel.x,pixel.y }, { depth,0,0,0 });
					m_Shader->FragmentShader(barycentricCoordinates, glm::ivec2(pixel.x, pixel.y));
				}
			}
		}
		else
		{
			float x = screenPosition[0].x;
			float y = screenPosition[0].y;
			float finalX = screenPosition[2].x;
			float dx = 1;
			if (x > finalX)
			{
				dx = -1;
				k = -k;
			}
			for (; dx > 0 && x < finalX || dx == -1 && x> finalX; x += dx, y += k)
			{
				if (x<0 || x>=width || y<0 || y>=height)
					continue;
				glm::ivec2 pixel = { x,y };
				glm::vec2 pixelPosition = pixel;
				pixelPosition += glm::vec2(0.5f);
				glm::vec3 barycentricCoordinates = ComputeBarycentricCoordinates(pixelPosition, screenPosition);
				float k1 = barycentricCoordinates.x / point[0].w;
				float k2 = barycentricCoordinates.y / point[1].w;
				float k3 = barycentricCoordinates.z / point[2].w;
				float sumK = 1 / (k1 + k2 + k3);
				barycentricCoordinates.x = k1 * sumK;
				barycentricCoordinates.y = k2 * sumK;
				barycentricCoordinates.z = k3 * sumK;
				float depth = (glm::dot(barycentricCoordinates, glm::vec3(ndc[0].z, ndc[1].z, ndc[2].z)) + 1) / 2;
				if (depth >= 0 && depth < m_DepthAttachment->SamplePixel({ pixel.x,pixel.y }).r)
				{
					m_DepthAttachment->Write({ pixel.x,pixel.y }, { depth,0,0,0 });
					m_Shader->FragmentShader(barycentricCoordinates, glm::ivec2(pixel.x, pixel.y));
				}
			}
		}

		k = (screenPosition[1].y - screenPosition[2].y) / (screenPosition[1].x - screenPosition[2].x);
		if (k > 1 || k < -1)
		{
			k = 1 / k;
			float x = screenPosition[2].x;
			float y = screenPosition[2].y;
			float finalY = screenPosition[1].y;
			float dy = 1;
			if (y > finalY)
			{
				dy = -1;
				k = -k;
			}
			for (; dy == 1 && y < finalY || dy == -1 && y > finalY; y += dy, x += k)
			{
				if (x<0 || x>=width || y<0 || y>=height)
					continue;
				glm::ivec2 pixel = { x,y };
				glm::vec2 pixelPosition = pixel;
				pixelPosition += glm::vec2(0.5f);
				glm::vec3 barycentricCoordinates = ComputeBarycentricCoordinates(pixelPosition, screenPosition);
				float k1 = barycentricCoordinates.x / point[0].w;
				float k2 = barycentricCoordinates.y / point[1].w;
				float k3 = barycentricCoordinates.z / point[2].w;
				float sumK = 1 / (k1 + k2 + k3);
				barycentricCoordinates.x = k1 * sumK;
				barycentricCoordinates.y = k2 * sumK;
				barycentricCoordinates.z = k3 * sumK;
				float depth = (glm::dot(barycentricCoordinates, glm::vec3(ndc[0].z, ndc[1].z, ndc[2].z)) + 1) / 2;
				if (depth >= 0 && depth < m_DepthAttachment->SamplePixel({ pixel.x,pixel.y }).r)
				{
					m_DepthAttachment->Write({ pixel.x,pixel.y }, { depth,0,0,0 });
					m_Shader->FragmentShader(barycentricCoordinates, glm::ivec2(pixel.x, pixel.y));
				}
			}
		}
		else
		{
			float x = screenPosition[2].x;
			float y = screenPosition[2].y;
			float finalX = screenPosition[1].x;
			float dx = 1;
			if (x > finalX)
			{
				dx = -1;
				k = -k;
			}
			for (; dx > 0 && x < finalX || dx == -1 && x> finalX; x += dx, y += k)
			{
				if (x<0 || x>=width || y<0 || y>=height)
					continue;
				glm::ivec2 pixel = { x,y };
				glm::vec2 pixelPosition = pixel;
				pixelPosition += glm::vec2(0.5f);
				glm::vec3 barycentricCoordinates = ComputeBarycentricCoordinates(pixelPosition, screenPosition);
				float k1 = barycentricCoordinates.x / point[0].w;
				float k2 = barycentricCoordinates.y / point[1].w;
				float k3 = barycentricCoordinates.z / point[2].w;
				float sumK = 1 / (k1 + k2 + k3);
				barycentricCoordinates.x = k1 * sumK;
				barycentricCoordinates.y = k2 * sumK;
				barycentricCoordinates.z = k3 * sumK;
				float depth = (glm::dot(barycentricCoordinates, glm::vec3(ndc[0].z, ndc[1].z, ndc[2].z)) + 1) / 2;
				if (depth >= 0 && depth < m_DepthAttachment->SamplePixel({ pixel.x,pixel.y }).r)
				{
					m_DepthAttachment->Write({ pixel.x,pixel.y }, { depth,0,0,0 });
					m_Shader->FragmentShader(barycentricCoordinates, glm::ivec2(pixel.x, pixel.y));
				}
			}
		}
	}
	else
	{
		glm::vec2 minf = screenPosition[0];
		glm::vec2 maxf = screenPosition[0];
		for (int i = 1; i < 3; i++)
		{
			minf = glm::min(minf, screenPosition[i]);
			maxf = glm::max(maxf, screenPosition[i]);
		}
		glm::ivec2 mini = glm::floor(minf);
		glm::ivec2 maxi = glm::ceil(maxf);
		mini = glm::clamp(mini, glm::ivec2(0), glm::ivec2(width, height));
		maxi = glm::clamp(maxi + glm::ivec2(1, 1), glm::ivec2(0), glm::ivec2(width, height));

		int32_t tileSize = 32;
		int32_t countX = (maxi.x - mini.x) / tileSize;
		if (countX * tileSize < maxi.x - mini.x)
			countX++;
		int32_t countY = (maxi.y - mini.y) / tileSize;
		if (countY * tileSize < maxi.y - mini.y)
			countY++;
		std::vector<glm::ivec4> tiles;
		tiles.reserve(countX * countY);
		for (int x = mini.x; x < maxi.x; x += tileSize)
		{
			for (int y = mini.y; y < maxi.y; y += tileSize)
			{
				tiles.emplace_back(glm::ivec4{ x,y,glm::min(maxi.x,x + tileSize),glm::min(maxi.y,y + tileSize) });
			}
		}

		std::for_each(std::execution::par,tiles.begin(), tiles.end(),
			[&](const glm::ivec4& tile)
			{
				for (int indexY = tile[1]; indexY < tile[3]; indexY++)
				{
					for (int indexX = tile[0]; indexX < tile[2]; indexX++)
					{
						glm::vec2 pixelPosition(indexX + 0.5f, indexY + 0.5f);
						if (IsPixelInTriangle(pixelPosition, screenPosition))
						{
							glm::vec3 barycentricCoordinates = ComputeBarycentricCoordinates(pixelPosition, screenPosition);
							float k1 = barycentricCoordinates.x / point[0].w;
							float k2 = barycentricCoordinates.y / point[1].w;
							float k3 = barycentricCoordinates.z / point[2].w;
							float k = 1 / (k1 + k2 + k3);
							barycentricCoordinates.x = k1 * k;
							barycentricCoordinates.y = k2 * k;
							barycentricCoordinates.z = k3 * k;
							float depth = (glm::dot(barycentricCoordinates, glm::vec3(ndc[0].z, ndc[1].z, ndc[2].z)) + 1) / 2;
							if (depth >= 0 && depth < m_DepthAttachment->SamplePixel({ indexX,indexY }).r)
							{
								m_DepthAttachment->Write({ indexX,indexY }, { depth,0,0,0 });
								m_Shader->FragmentShader(barycentricCoordinates, glm::ivec2(indexX, indexY));
							}
						}
					}
				}
			});
	}
}

bool Renderer::IsPixelInTriangle(glm::vec2 pixel, const std::array<glm::vec2, 3>& triangle)
{
	glm::vec2 AP = pixel - triangle[0];
	glm::vec2 AB = triangle[1] - triangle[0];
	float k1 = cross2(AP, AB);

	glm::vec2 BP = pixel - triangle[1];
	glm::vec2 BC = triangle[2] - triangle[1];
	float k2 = cross2(BP, BC);

	glm::vec2 CP = pixel - triangle[2];
	glm::vec2 CA = triangle[0] - triangle[2];
	float k3 = cross2(CP, CA);

	if (k1 >= 0 && k2 >= 0 && k3 >= 0 ||
		k1 < 0 && k2 < 0 && k3 < 0)
		return true;
	return false;
}

glm::vec3 Renderer::ComputeBarycentricCoordinates(glm::vec2 pixel, const std::array<glm::vec2, 3>& triangle)
{
	glm::vec2 AP = pixel - triangle[0];
	glm::vec2 AB = triangle[1] - triangle[0];
	float k1 = cross2(AP, AB);

	glm::vec2 BP = pixel - triangle[1];
	glm::vec2 BC = triangle[2] - triangle[1];
	float k2 = cross2(BP, BC);

	glm::vec2 CP = pixel - triangle[2];
	glm::vec2 CA = triangle[0] - triangle[2];
	float k3 = cross2(CP, CA);

	float k = 1 /(k1 + k2 + k3);
	return { k2 * k,k3 * k,k1 * k };
}


void Renderer::DrawPoint(const float* pData)
{
	uint32_t width = m_ColorAttachment->Width();
	uint32_t height = m_ColorAttachment->Height();

	glm::vec4 point = m_Shader->VertexShader(pData);
	if (point.w <= 0)
		return;
	glm::vec3 ndc = glm::vec3(point) / point.w;
	float screenPositionX = (ndc.x + 1) / 2 * width;
	float screenPositionY = (ndc.y + 1) / 2 * height;
	if (screenPositionX < 0 || screenPositionX >= width || screenPositionY < 0 || screenPositionY >= height)
		return;
	glm::ivec2 pixel(screenPositionX, screenPositionY);
	float depth = (ndc.z + 1) / 2;
	if (depth >= 0 && depth < m_DepthAttachment->SamplePixel(pixel).r)
	{
		m_DepthAttachment->Write(pixel, glm::vec4(depth, 0, 0, 1));
		m_Shader->FragmentShader(glm::vec3(1, 0, 0), pixel);
	}
}

void Renderer::DrawLine(const std::array<const float*, 2>& pData)
{
	std::array<glm::vec4, 2> point;
	m_Shader->Clear();
	for (int i = 0; i < 2; i++)
	{
		point[i] = m_Shader->VertexShader(pData[i]);
	}

	float h0 = point[0].z + point[0].w;
	float h1 = point[1].z + point[1].w;
	
	uint32_t count = 0;
	uint32_t countZero = 0;
	if (h0 < 0)
		count++;
	else if (h0 == 0)
		countZero++;
	if (h1 < 0)
		count++;
	else if (h1 == 0)
		countZero++;

	if (count + countZero == 2)
	{
		return;
	}
	else if (count == 0)
	{
		RasterizeLine(point);
	}
	else
	{
		uint32_t midDataCount = m_Shader->GetMidDataCount();
		float* data0 = m_Shader->GetMidData(0);
		float* data1 = m_Shader->GetMidData(1);
		if (h0 < 0)
		{
			float k = h1 / (h1 - h0);
			for (int i = 0; i < midDataCount; i++)
			{
				data0[i] = glm::mix(data1[i], data0[i], k);
			}
			point[0] = glm::mix(point[1], point[0], k);
			RasterizeLine(point);
		}
		else
		{
			float k = h0 / (h0 - h1);
			for (int i = 0; i < midDataCount; i++)
			{
				data1[i] = glm::mix(data0[i], data1[i], k);
			}
			point[1] = glm::mix(point[0], point[1], k);
			RasterizeLine(point);
		}
	}
	

}

void Renderer::RasterizeLine(const std::array<glm::vec4, 2> point)
{
	uint32_t width = m_ColorAttachment->Width();
	uint32_t height = m_ColorAttachment->Height();

	std::array<glm::vec3, 2> ndc;
	std::array<glm::vec2, 2> screenPosition;
	for (int i = 0; i < 2; i++)
	{
		ndc[i] = glm::vec3(point[i]) / point[i].w;
		screenPosition[i].x = (ndc[i].x + 1) / 2 * width;
		screenPosition[i].y = (ndc[i].y + 1) / 2 * height;
	}

	float k = (screenPosition[0].y - screenPosition[1].y) / (screenPosition[0].x - screenPosition[1].x);
	if (k > 1 || k < -1)
	{
		k = 1 / k;
		float x = screenPosition[0].x;
		float y = screenPosition[0].y;
		float finalY = screenPosition[1].y;
		float dy = 1;
		if (y > finalY)
		{
			dy = -1;
			k = -k;
		}
		for (; dy == 1 && y<finalY || dy == -1 && y>finalY; y += dy, x += k)
		{
			if (x < 0 || x >= width || y < 0 || y >= height)
				continue;
			float k0 = screenPosition[0].y - y;
			float k1 = y - screenPosition[1].y;
			float sumK = 1 / (k0 + k1);
			k0 = k0 * sumK;
			k1 = k1 * sumK;
			k0 /= point[0].w;
			k1 /= point[1].w;
			sumK = 1 / (k0 + k1);
			k0 = k0 * sumK;
			k1 = k1 * sumK;
			glm::ivec2 pixel(x, y);
			float depth = k0 * ndc[0].z + k1 * ndc[1].z;
			if (depth >= 0 && depth < m_DepthAttachment->SamplePixel(pixel).r)
			{
				m_DepthAttachment->Write(pixel, glm::vec4(depth, 0, 0, 1));
				m_Shader->FragmentShader(glm::vec3(k0, k1, 0), pixel);
			}
		}
	}
	else
	{
		float x = screenPosition[0].x;
		float y = screenPosition[0].y;
		float finalX = screenPosition[1].x;
		float dx = 1;
		if (x > finalX)
		{
			dx = -1;
			k = -k;
		}
		for (; dx == 1 && x<finalX || dx == -1 && x>finalX; x += dx, y += k)
		{
			if (x < 0 || x >= width || y < 0 || y >= height)
				continue;
			float k0 = screenPosition[0].x - x;
			float k1 = x - screenPosition[1].x;
			float sumK = 1 / (k0 + k1);
			k0 = k0 * sumK;
			k1 = k1 * sumK;
			k0 /= point[0].w;
			k1 /= point[1].w;
			sumK = 1 / (k0 + k1);
			k0 = k0 * sumK;
			k1 = k1 * sumK;
			glm::ivec2 pixel(x, y);
			float depth = k0 * ndc[0].z + k1 * ndc[1].z;
			if (depth >= 0 && depth < m_DepthAttachment->SamplePixel(pixel).r)
			{
				m_DepthAttachment->Write(pixel, glm::vec4(depth, 0, 0, 1));
				m_Shader->FragmentShader(glm::vec3(k0, k1, 0), pixel);
			}
		}
	}


}
