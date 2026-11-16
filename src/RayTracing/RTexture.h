#pragma once
#include"glm/glm.hpp"

class RTexture
{
public:
	virtual glm::vec3 Sample(const glm::vec2& uv, const glm::vec3& point) = 0;
};