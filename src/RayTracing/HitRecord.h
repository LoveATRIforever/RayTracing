#pragma once
#include"glm/glm.hpp"
#include"Material.h"
#include<memory>

struct HitRecord
{
	glm::vec3 normal;
	glm::vec3 p;
	float t;
	glm::vec2 uv;
	std::shared_ptr<Material> mat;

	float minT;

	HitRecord()
	{
		t = std::numeric_limits<float>::infinity();
		minT = 0.001;
	}
};