#pragma once
#include"../../RayTracing/RTexture.h"

class SolidColor :public RTexture
{
private:
	glm::vec3 m_Color;
public:
	SolidColor(const glm::vec3 color)
		:m_Color(color) {
	}

	glm::vec3 Sample(const glm::vec2& uv, const glm::vec3& point) override
	{
		return m_Color;
	}
};