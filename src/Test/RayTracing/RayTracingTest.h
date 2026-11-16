#pragma once
#include"../../RayTracing/RayTracing.h"
#include"../../RayTracing/Shape.h"

class RayTracingTest
{
private:
	RayTracing m_RayTracing;
	std::shared_ptr<Texture> m_ColorAttachment;
	uint32_t m_OpenGLTexture;

	float m_RenderTime = 0;
	int32_t m_Example = 0;

	int m_ShowScale = 3;
public:
	RayTracingTest();
	~RayTracingTest();

	void Render(float tick);
private:
	void CopyTextureData();
	void CreateOpenGLTexture();

	void ChangeColorAttachment(uint32_t width, uint32_t height);

	void Draw();

	void DrawSphere();
	void DrawEarth();
	void DrawManySphere();
	void DrawPerlin();
	void DrawQuad();
	void DrawSimpleLight();
	void DrawCornellBox();
	void DrawSmokeCornellBox();
	void DrawFinal(int imageWidth, int sample, int depth, int scale);
};