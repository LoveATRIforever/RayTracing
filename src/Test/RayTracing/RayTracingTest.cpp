#pragma once
#include"RayTracingTest.h"
#include"glad/glad.h"
#include"imgui.h"
#include"stb_image.h"

#include"Sphere.h"
#include"ShapeArray.h"
#include"Lambertian.h"
#include"Metal.h"
#include"Dielectric.h"
#include"../../RayTracing/Random.h"
#include"BVHNode.h"
#include<chrono>
#include"CheckerTexture.h"
#include"ImageTexture.h"
#include"PerlinTexture.h"
#include"Quad.h"
#include"DiffuseLight.h"
#include"Transform.h"
#include"ConstantMedium.h"

class Timer
{
private:
	float& m_Delta;
	std::chrono::high_resolution_clock::time_point m_Begin;
public:
	Timer(float& out)
		:m_Delta(out)
	{
		m_Begin = std::chrono::high_resolution_clock::now();
	}
	~Timer()
	{
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> deltaTime = end - m_Begin;
		m_Delta = deltaTime.count();
	}
};

inline std::string GetExampleName(int32_t example)
{
	switch (example)
	{
	case 0:	return "Sphere";
	case 1:	return "Spheres";
	case 2:	return "Earth";
	case 3:	return "Perlin";
	case 4:	return "Quad";
	case 5:	return "SimpleLight";
	case 6:	return "CornellBox";
	case 7:	return "SmokeCornellBox";
	case 8:	return "Final";
	case 9:	return "SuperFinal";
	}
}

RayTracingTest::RayTracingTest()
{
	m_ColorAttachment = std::make_shared<Texture>(400, 225, TextureFormat::eRGB8);
	m_RayTracing.SetColorAttachment(m_ColorAttachment);
	CreateOpenGLTexture();

	DrawSphere();
}

RayTracingTest::~RayTracingTest()
{
	glDeleteTextures(1, &m_OpenGLTexture);
}

void RayTracingTest::Render(float tick)
{
	
	
	ImGui::Begin("RenderInfo");
	ImGui::Text("Frame Time: %.3f ms", tick * 1000.0f);
	ImGui::Text("Render Time: %.3f s", m_RenderTime);
	ImGui::End();

	ImGui::Begin("Config");

	ImGui::Text("Example:");

	if (ImGui::RadioButton("Sphere", &m_Example, 0))
	{
		DrawSphere();
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Spheres", &m_Example, 1))
	{
		DrawManySphere();
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Earth", &m_Example, 2))
	{
		DrawEarth();
	}
	
	if (ImGui::RadioButton("Perlin", &m_Example, 3))
	{
		DrawPerlin();
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Quad", &m_Example, 4))
	{
		DrawQuad();
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("SimpleLight", &m_Example, 5))
	{
		DrawSimpleLight();
	}

	if (ImGui::RadioButton("CornellBox", &m_Example, 6))
	{
		DrawCornellBox();
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("SmokeCornellBox", &m_Example, 7))
	{
		DrawSmokeCornellBox();
	}

	if (ImGui::RadioButton("Final", &m_Example, 8))
	{
		DrawFinal(400, 1000, 20, 3);
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("SuperFinal", &m_Example, 9))
	{
		DrawFinal(800, 10000, 40, 1);
	}

	ImGui::Text("Setting:");
	if (ImGui::Checkbox("Jittering", &m_RayTracing.m_EnableJittering))
	{

	}

	if (ImGui::Button("Draw"))
	{
		Draw();
	}
	if (ImGui::Button("Save"))
	{
		std::string path = "src/Test/RayTracing/result/" + GetExampleName(m_Example) + ".jpg";
		m_ColorAttachment->Save(path);
	}
	ImGui::End();

	ImGui::Begin("Viewport");

	ImGui::ImageButton("color", (ImTextureID)m_OpenGLTexture, ImVec2(m_ColorAttachment->Width() * m_ShowScale, m_ColorAttachment->Height() * m_ShowScale), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
}

void RayTracingTest::Draw()
{
	m_ColorAttachment->Clear();
	{
		Timer t(m_RenderTime);
		m_RayTracing.Draw();
	}
	CopyTextureData();
}

void RayTracingTest::DrawSphere()
{
	auto shapeArray = std::make_shared<ShapeArray>();
	shapeArray->Add(std::make_shared<Sphere>(glm::vec3(0, 0, -1), 0.5, std::make_shared<Lambertian>(glm::vec3(0.7, 0.3, 0.3))));
	shapeArray->Add(std::make_shared<Sphere>(glm::vec3(0, -100.5, -1), 100, std::make_shared<Lambertian>(glm::vec3(0.8, 0.8, 0.0))));
	shapeArray->Add(std::make_shared<Sphere>(glm::vec3(-1, 0, -1), 0.5, std::make_shared<Dielectric>(1.5)));
	shapeArray->Add(std::make_shared<Sphere>(glm::vec3(-1, 0, -1), -0.4, std::make_shared<Dielectric>(1.5)));
	shapeArray->Add(std::make_shared<Sphere>(glm::vec3(1, 0, -1), 0.5, std::make_shared<Metal>(glm::vec3(0.8, 0.6, 0.2),0.0)));

	m_RayTracing.m_SamplePerPixel = 500;
	m_RayTracing.m_MaxDepth = 50;
	m_RayTracing.m_Fov = 90;
	m_RayTracing.m_CameraPosition = glm::vec3(0, 0, 0);
	m_RayTracing.m_LookAt = glm::vec3(0, 0, -1);
	m_RayTracing.m_DefocusAngle = 0;
	m_RayTracing.m_FocusDistance = 1;
	m_RayTracing.m_BkColor = glm::vec3(0.70, 0.80, 1.00);

	m_RayTracing.SetShape(shapeArray);
	ChangeColorAttachment(400, 225);
	m_ShowScale = 3;
}

void RayTracingTest::DrawEarth()
{
	auto earth = std::make_shared<Sphere>(glm::vec3(0, 0, 0), 2, std::make_shared<Lambertian>(std::make_shared<ImageTexture>("assets/texture/earthmap.jpg")));

	m_RayTracing.m_SamplePerPixel = 100;
	m_RayTracing.m_MaxDepth = 50;
	m_RayTracing.m_Fov = 20;
	m_RayTracing.m_CameraPosition = glm::vec3(0, 0, 12);
	m_RayTracing.m_LookAt = glm::vec3(0, 0, 0);
	m_RayTracing.m_DefocusAngle = 0;
	m_RayTracing.m_FocusDistance = 1;
	m_RayTracing.m_BkColor = glm::vec3(0.70, 0.80, 1.00);

	m_RayTracing.SetShape(earth);
	ChangeColorAttachment(400, 225);
	m_ShowScale = 3;
}

void RayTracingTest::DrawManySphere()
{
	std::vector<std::shared_ptr<Shape>> shapes;
	shapes.push_back(std::make_shared<Sphere>(glm::vec3{ 0,-1000,0 }, 1000, std::make_shared<Lambertian>(std::make_shared<CheckerTexture>(glm::vec3(.2, .3, .1), glm::vec3(.9, .9, .9), 0.32))));
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			auto chooseMat = Random01();
			glm::vec3 center(a + 0.9 * Random01(), 0.2, b + 0.9 * Random01());

			if (glm::distance(center, glm::vec3(4.0f, 0.2f, 0.0f)) > 0.9)
			{
				std::shared_ptr<Material> sphereMat;

				if (chooseMat < 0.8)
				{
					glm::vec3 albedo = RandomColor() * RandomColor();
					sphereMat = std::make_shared<Lambertian>(albedo);
					shapes.push_back(std::make_shared<Sphere>(center, 0.2, sphereMat));
				}
				else if (chooseMat < 0.95)
				{
					glm::vec3 albedo = RandomColor() * 0.5f + 0.5f;
					float fuzz = Random01() * 0.5f;
					sphereMat = std::make_shared<Metal>(albedo, fuzz);
					shapes.push_back(std::make_shared<Sphere>(center, 0.2, sphereMat));
				}
				else
				{
					sphereMat = std::make_shared<Dielectric>(1.5);
					shapes.push_back(std::make_shared<Sphere>(center, 0.2, sphereMat));
				}
			}
		}
	}

	shapes.push_back(std::make_shared<Sphere>(glm::vec3(0, 1, 0), 1, std::make_shared<Dielectric>(1.5)));
	shapes.push_back(std::make_shared<Sphere>(glm::vec3(-4, 1, 0), 1, std::make_shared<Lambertian>(glm::vec3(0.4, 0.2, 0.1))));
	shapes.push_back(std::make_shared<Sphere>(glm::vec3(4, 1, 0), 1, std::make_shared<Metal>(glm::vec3(0.7, 0.6, 0.5), 0)));

	m_RayTracing.m_SamplePerPixel = 500;
	m_RayTracing.m_MaxDepth = 50;
	m_RayTracing.m_Fov = 20;
	m_RayTracing.m_CameraPosition = glm::vec3(13, 2, 3);
	m_RayTracing.m_LookAt = glm::vec3(0, 0, 0);
	m_RayTracing.m_DefocusAngle = 0.6;
	m_RayTracing.m_FocusDistance = 10.0;
	m_RayTracing.m_BkColor = glm::vec3(0.70, 0.80, 1.00);

	m_RayTracing.SetShape(std::make_shared<BVHNode>(shapes));
	ChangeColorAttachment(400, 225);
	m_ShowScale = 3;
}

void RayTracingTest::DrawPerlin()
{
	auto perlin = std::make_shared <PerlinTexture>(4);

	auto shapeArray = std::make_shared<ShapeArray>();
	shapeArray->Add(std::make_shared<Sphere>(glm::vec3(0, -1000, 0), 1000, std::make_shared<Lambertian>(perlin)));
	shapeArray->Add(std::make_shared<Sphere>(glm::vec3(0, 2, 0), 2, std::make_shared<Lambertian>(perlin)));


	m_RayTracing.m_SamplePerPixel = 500;
	m_RayTracing.m_MaxDepth = 50;
	m_RayTracing.m_Fov = 20;
	m_RayTracing.m_CameraPosition = glm::vec3(13, 2, 3);
	m_RayTracing.m_LookAt = glm::vec3(0, 0, 0);
	m_RayTracing.m_DefocusAngle = 0;
	m_RayTracing.m_FocusDistance = 1;
	m_RayTracing.m_BkColor = glm::vec3(0.70, 0.80, 1.00);

	m_RayTracing.SetShape(shapeArray);
	ChangeColorAttachment(400, 225);
	m_ShowScale = 3;
}

void RayTracingTest::DrawQuad()
{
	auto shapeArray = std::make_shared<ShapeArray>();
	shapeArray->Add(std::make_shared<Quad>(glm::vec3(-3, -2, 5), glm::vec3(0, 0, -4), glm::vec3(0, 4, 0), std::make_shared<Lambertian>(glm::vec3(1, 0.2, 0.2))));
	shapeArray->Add(std::make_shared<Quad>(glm::vec3(-2, -2, 0), glm::vec3(4, 0, 0), glm::vec3(0, 4, 0), std::make_shared<Lambertian>(glm::vec3(0.2, 1, 0.2))));
	shapeArray->Add(std::make_shared<Quad>(glm::vec3(3, -2, 1), glm::vec3(0, 0, 4), glm::vec3(0, 4, 0), std::make_shared<Lambertian>(glm::vec3(0.2, 0.2, 1))));
	shapeArray->Add(std::make_shared<Quad>(glm::vec3(-2, 3, 1), glm::vec3(4, 0, 0), glm::vec3(0, 0, 4), std::make_shared<Lambertian>(glm::vec3(1, 0.5, 0.0))));
	shapeArray->Add(std::make_shared<Quad>(glm::vec3(-2, -3, 5), glm::vec3(4, 0, 0), glm::vec3(0, 0, -4), std::make_shared<Lambertian>(glm::vec3(0.2, 0.8, 0.8))));


	m_RayTracing.m_SamplePerPixel = 100;
	m_RayTracing.m_MaxDepth = 50;
	m_RayTracing.m_Fov = 80;
	m_RayTracing.m_CameraPosition = glm::vec3(0, 0, 9);
	m_RayTracing.m_LookAt = glm::vec3(0, 0, 0);
	m_RayTracing.m_DefocusAngle = 0;
	m_RayTracing.m_FocusDistance = 1;
	m_RayTracing.m_BkColor = glm::vec3(0.70, 0.80, 1.00);

	m_RayTracing.SetShape(shapeArray);
	ChangeColorAttachment(400, 225);
	m_ShowScale = 3;
}

void RayTracingTest::DrawSimpleLight()
{
	auto shapeArray = std::make_shared<ShapeArray>();
	auto perlin = std::make_shared<PerlinTexture>(4);
	shapeArray->Add(std::make_shared<Sphere>(glm::vec3(0, -1000, 0), 1000, std::make_shared<Lambertian>(perlin)));
	shapeArray->Add(std::make_shared<Sphere>(glm::vec3(0, 2, 0), 2, std::make_shared<Lambertian>(perlin)));

	shapeArray->Add(std::make_shared<Sphere>(glm::vec3(0, 7, 0), 2, std::make_shared<DiffuseLight>(glm::vec3(4))));
	shapeArray->Add(std::make_shared<Quad>(glm::vec3(3, 1, -2), glm::vec3(2, 0, 0), glm::vec3(0, 2, 0), std::make_shared<DiffuseLight>(glm::vec3(4, 4, 4))));

	m_RayTracing.m_SamplePerPixel = 100;
	m_RayTracing.m_MaxDepth = 50;
	m_RayTracing.m_Fov = 20;
	m_RayTracing.m_CameraPosition = glm::vec3(26, 3, 6);
	m_RayTracing.m_LookAt = glm::vec3(0, 2, 0);
	m_RayTracing.m_DefocusAngle = 0;
	m_RayTracing.m_FocusDistance = 1;
	m_RayTracing.m_BkColor = glm::vec3(0);

	m_RayTracing.SetShape(shapeArray);
	ChangeColorAttachment(400, 225);
	m_ShowScale = 3;
}

inline std::shared_ptr<ShapeArray> CreateBox(const glm::vec3& a, const glm::vec3& b, std::shared_ptr<Material> mat)
{
	auto array = std::make_shared<ShapeArray>();
	
	auto min = glm::min(a, b);
	auto max = glm::max(a, b);

	auto dx = glm::vec3(max.x - min.x, 0, 0);
	auto dy = glm::vec3(0, max.y - min.y, 0);
	auto dz = glm::vec3(0, 0, max.z - min.z);

	array->Add(std::make_shared<Quad>(glm::vec3(min.x, min.y, max.z), dx, dy, mat)); // 前
	array->Add(std::make_shared<Quad>(glm::vec3(max.x, min.y, max.z), -dz, dy, mat)); // 右
	array->Add(std::make_shared<Quad>(glm::vec3(max.x, min.y, min.z), -dx, dy, mat)); // 后
	array->Add(std::make_shared<Quad>(glm::vec3(min.x, min.y, min.z), dz, dy, mat)); // 左
	array->Add(std::make_shared<Quad>(glm::vec3(min.x, max.y, max.z), dx, -dz, mat)); // 顶
	array->Add(std::make_shared<Quad>(glm::vec3(min.x, min.y, min.z), dx, dz, mat)); // 底

	return array;
}

void RayTracingTest::DrawCornellBox()
{
	auto shapeArray = std::make_shared<ShapeArray>();
	
	auto red = std::make_shared<Lambertian>(glm::vec3(.65, .05, .05));
	auto white = std::make_shared<Lambertian>(glm::vec3(.73, .73, .73));
	auto green = std::make_shared<Lambertian>(glm::vec3(.12, .45, .15));
	auto light = std::make_shared<DiffuseLight>(glm::vec3(15));

	shapeArray->Add(std::make_shared<Quad>(glm::vec3(555, 0, 0), glm::vec3(0, 555, 0), glm::vec3(0, 0, 555), green));
	shapeArray->Add(std::make_shared<Quad>(glm::vec3(0, 0, 0), glm::vec3(0, 555, 0), glm::vec3(0, 0, 555), red));
	shapeArray->Add(std::make_shared<Quad>(glm::vec3(343, 554, 332), glm::vec3(-130, 0, 0), glm::vec3(0, 0, -105), light));
	shapeArray->Add(std::make_shared<Quad>(glm::vec3(0, 0, 0), glm::vec3(555, 0, 0), glm::vec3(0, 0, 555), white));
	shapeArray->Add(std::make_shared<Quad>(glm::vec3(555, 555, 555), glm::vec3(-555, 0, 0), glm::vec3(0, 0, -555), white));
	shapeArray->Add(std::make_shared<Quad>(glm::vec3(0, 0, 555), glm::vec3(555, 0, 0), glm::vec3(0, 555, 0), white));

	shapeArray->Add(std::make_shared<Translate>(std::make_shared<RotateY>(CreateBox(glm::vec3(0, 0, 0), glm::vec3(165, 330, 165), white), 15), glm::vec3(265, 0, 295)));
	shapeArray->Add(std::make_shared<Translate>(std::make_shared<RotateY>(CreateBox(glm::vec3(0, 0, 0), glm::vec3(165, 165, 165), white), -18), glm::vec3(130, 0, 65)));
	m_RayTracing.m_SamplePerPixel = 200;
	m_RayTracing.m_MaxDepth = 50;
	m_RayTracing.m_Fov = 40;
	m_RayTracing.m_CameraPosition = glm::vec3(278, 278, -800);
	m_RayTracing.m_LookAt = glm::vec3(278, 278, 0);
	m_RayTracing.m_DefocusAngle = 0;
	m_RayTracing.m_FocusDistance = 1;
	m_RayTracing.m_BkColor = glm::vec3(0);

	m_RayTracing.SetShape(shapeArray);
	ChangeColorAttachment(600, 600);
	m_ShowScale = 2;
}

void RayTracingTest::DrawSmokeCornellBox()
{
	auto shapeArray = std::make_shared<ShapeArray>();

	auto red = std::make_shared<Lambertian>(glm::vec3(.65, .05, .05));
	auto white = std::make_shared<Lambertian>(glm::vec3(.73, .73, .73));
	auto green = std::make_shared<Lambertian>(glm::vec3(.12, .45, .15));
	auto light = std::make_shared<DiffuseLight>(glm::vec3(7));

	shapeArray->Add(std::make_shared<Quad>(glm::vec3(555, 0, 0), glm::vec3(0, 555, 0), glm::vec3(0, 0, 555), green));
	shapeArray->Add(std::make_shared<Quad>(glm::vec3(0, 0, 0), glm::vec3(0, 555, 0), glm::vec3(0, 0, 555), red));
	shapeArray->Add(std::make_shared<Quad>(glm::vec3(113, 554, 127), glm::vec3(330, 0, 0), glm::vec3(0, 0, 305), light));
	shapeArray->Add(std::make_shared<Quad>(glm::vec3(0, 0, 0), glm::vec3(555, 0, 0), glm::vec3(0, 0, 555), white));
	shapeArray->Add(std::make_shared<Quad>(glm::vec3(555, 555, 555), glm::vec3(-555, 0, 0), glm::vec3(0, 0, -555), white));
	shapeArray->Add(std::make_shared<Quad>(glm::vec3(0, 0, 555), glm::vec3(555, 0, 0), glm::vec3(0, 555, 0), white));
	
	shapeArray->Add(std::make_shared<ConstantMedium>(std::make_shared<Translate>(std::make_shared<RotateY>(CreateBox(glm::vec3(0, 0, 0), glm::vec3(165, 330, 165), white), 15), glm::vec3(265, 0, 295)), 0.01, glm::vec3(0)));
	shapeArray->Add(std::make_shared<ConstantMedium>(std::make_shared<Translate>(std::make_shared<RotateY>(CreateBox(glm::vec3(0, 0, 0), glm::vec3(165, 165, 165), white), -18), glm::vec3(130, 0, 65)), 0.01, glm::vec3(1)));
	
	m_RayTracing.m_SamplePerPixel = 200;
	m_RayTracing.m_MaxDepth = 50;
	m_RayTracing.m_Fov = 40;
	m_RayTracing.m_CameraPosition = glm::vec3(278, 278, -800);
	m_RayTracing.m_LookAt = glm::vec3(278, 278, 0);
	m_RayTracing.m_DefocusAngle = 0;
	m_RayTracing.m_FocusDistance = 1;
	m_RayTracing.m_BkColor = glm::vec3(0);

	m_RayTracing.SetShape(shapeArray);
	ChangeColorAttachment(600, 600);
	m_ShowScale = 2;
}

void RayTracingTest::DrawFinal(int imageWidth, int sample, int depth, int scale)
{
	auto boxes1 = std::vector<std::shared_ptr<Shape>>();
	auto ground = std::make_shared<Lambertian>(glm::vec3(0.48, 0.83, 0.53));
	int boxPerSide = 20;
	for (int i = 0; i < boxPerSide; i++)
	{
		for (int j = 0; j < boxPerSide; j++)
		{
			float w = 100;
			float x0 = -1000 + i * w;
			float z0 = -1000 + j * w;
			float y0 = 0;
			float x1 = x0 + w;
			float y1 = int(Random01() * 101) + 1;
			float z1 = z0 + w;
			
			boxes1.push_back(CreateBox(glm::vec3(x0, y0, z0), glm::vec3(x1, y1, z1), ground));
		}
	}

	auto world = std::make_shared<ShapeArray>();

	world->Add(std::make_shared<BVHNode>(boxes1));

	auto light = std::make_shared<DiffuseLight>(glm::vec3(7));
	world->Add(std::make_shared<Quad>(glm::vec3(123, 554, 147), glm::vec3(300, 0, 0), glm::vec3(0, 0, 265), light));
	
	auto center1 = glm::vec3(400, 400, 200);
	auto center2 = center1 + glm::vec3(30, 0, 0);
	auto sphere_material = std::make_shared<Lambertian>(glm::vec3(0.7, 0.3, 0.1));
	world->Add(std::make_shared<Sphere>(center1, 50, sphere_material));

	world->Add(std::make_shared<Sphere>(glm::vec3(260, 150, 45), 50, std::make_shared<Dielectric>(1.5)));
	world->Add(std::make_shared<Sphere>(
		glm::vec3(0, 150, 145), 50, std::make_shared<Metal>(glm::vec3(0.8, 0.8, 0.9), 1.0)
	));

	auto boundary = std::make_shared<Sphere>(glm::vec3(360, 150, 145), 70, std::make_shared<Dielectric>(1.5));
	world->Add(boundary);
	world->Add(make_shared<ConstantMedium>(boundary, 0.2, glm::vec3(0.2, 0.4, 0.9)));
	boundary = std::make_shared<Sphere>(glm::vec3(0, 0, 0), 5000, std::make_shared<Dielectric>(1.5));
	world->Add(std::make_shared<ConstantMedium>(boundary, .0001, glm::vec3(1, 1, 1)));

	auto emat = std::make_shared<Lambertian>(std::make_shared<ImageTexture>("assets/texture/earthmap.jpg"));
	world->Add(make_shared<Sphere>(glm::vec3(400, 200, 400), 100, emat));
	auto pertext = std::make_shared<PerlinTexture>(0.1);
	world->Add(make_shared<Sphere>(glm::vec3(220, 280, 300), 80, make_shared<Lambertian>(pertext)));

	auto boxes2 = std::vector<std::shared_ptr<Shape>>();;
	auto white = std::make_shared<Lambertian>(glm::vec3(.73, .73, .73));
	int ns = 1000;
	for (int j = 0; j < ns; j++) {
		boxes2.push_back(std::make_shared<Sphere>(glm::vec3(Random01()*165, Random01() * 165, Random01() * 165), 10, white));
	}

	world->Add(std::make_shared<Translate>(
		std::make_shared<RotateY>(
			make_shared<BVHNode>(boxes2), 15),
		glm::vec3(-100, 270, 395)
	)
	);

	m_RayTracing.m_SamplePerPixel = sample;
	m_RayTracing.m_MaxDepth = depth;
	m_RayTracing.m_Fov = 40;
	m_RayTracing.m_CameraPosition = glm::vec3(478, 278, -600);
	m_RayTracing.m_LookAt = glm::vec3(278, 278, 0);
	m_RayTracing.m_DefocusAngle = 0;
	m_RayTracing.m_FocusDistance = 1;
	m_RayTracing.m_BkColor = glm::vec3(0);

	m_RayTracing.SetShape(world);
	ChangeColorAttachment(imageWidth, imageWidth);
	m_ShowScale = scale;
}

void RayTracingTest::CopyTextureData()
{
	glTextureSubImage2D(m_OpenGLTexture, 0, 0, 0, m_ColorAttachment->Width(), m_ColorAttachment->Height(), GL_RGB, GL_UNSIGNED_BYTE, m_ColorAttachment->Data());
}

void RayTracingTest::CreateOpenGLTexture()
{
	glCreateTextures(GL_TEXTURE_2D, 1, &m_OpenGLTexture);
	glTextureStorage2D(m_OpenGLTexture, 1, GL_RGB8, 400, 225);
}

void RayTracingTest::ChangeColorAttachment(uint32_t width, uint32_t height)
{
	m_ColorAttachment = std::make_shared<Texture>(width, height, TextureFormat::eRGB8);
	m_RayTracing.SetColorAttachment(m_ColorAttachment);

	glDeleteTextures(1, &m_OpenGLTexture);
	glCreateTextures(GL_TEXTURE_2D, 1, &m_OpenGLTexture);
	glTextureStorage2D(m_OpenGLTexture, 1, GL_RGB8, width, height);
}
