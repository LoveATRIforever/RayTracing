#pragma once
#include"../../RayTracing/RTexture.h"

class CheckerTexture :public RTexture
{
private:
	glm::vec3 m_Color1;
	glm::vec3 m_Color2;
	float m_Scale;
public:
	CheckerTexture(const glm::vec3& color1, const glm::vec3& color2, float scale = 1)
		:m_Color1(color1), m_Color2(color2), m_Scale(1 / scale) { }
	

	glm::vec3 Sample(const glm::vec2& uv, const glm::vec3& point) override;
};