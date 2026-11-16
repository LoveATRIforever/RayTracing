#pragma once
#include"../../RayTracing/RTexture.h"
#include"Perlin.h"


class PerlinTexture :public RTexture
{
private:
	Perlin m_Perlin;
	float m_Scale;
public:
	PerlinTexture(float scale = 0)
		:m_Scale(scale){ }
	glm::vec3 Sample(const glm::vec2& uv, const glm::vec3& point) override;

};