#pragma once
#include"../Rasterize/Texture.h"
#include"Shape.h"

#include<memory>


class RayTracing
{
private:
	std::shared_ptr<Texture> m_ColorAttachment;

	std::shared_ptr<Shape> m_Shape;
	std::shared_ptr<Shape> m_Light;

public:
	int32_t m_SamplePerPixel = 100;
	int32_t m_MaxDepth = 50;

	float m_Fov = 20.0;
	glm::vec3 m_CameraPosition = { -2,2,1 };
	glm::vec3 m_LookAt = { 0,0,-1 };
	glm::vec3 m_Up = { 0,1,0 };

	float m_FocusDistance = 3.4;
	float m_DefocusAngle = 10;

	glm::vec3 m_BkColor;

	bool m_EnableJittering = false;
	

	int32_t m_TileSize = 32;
public:
	RayTracing();
	
	void Draw();
	void SetColorAttachment(std::shared_ptr<Texture> attachment) { m_ColorAttachment = attachment; }
	void SetShape(std::shared_ptr<Shape> shape) { m_Shape = shape; }
	void SetLight(std::shared_ptr<Shape> light) { m_Light = light; }

	glm::vec4 RayColor(Ray ray, int depth = 0);
};