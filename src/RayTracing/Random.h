#pragma once
#include"glm/glm.hpp"
#include <random>

inline float Random01() {
	static std::uniform_real_distribution<float> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

inline glm::vec3 RandomDirectionOnHemisphere(glm::vec3 normal)
{
	glm::vec3 res;
	res = glm::vec3(Random01() * 2 - 1, Random01() * 2 - 1, Random01() * 2 - 1);
	const glm::vec3 center(0);
	while (glm::dot(res, res) > 1 && glm::dot(res, normal) < 0)
	{
		res = glm::vec3(Random01() * 2 - 1, Random01() * 2 - 1, Random01() * 2 - 1);
	}
	return glm::normalize(res);
}

inline glm::vec3 RandomDirection()
{
	glm::vec3 res;
	res = glm::vec3(Random01() * 2 - 1, Random01() * 2 - 1, Random01() * 2 - 1);
	while (glm::dot(res, res) > 1)
	{
		res = glm::vec3(Random01() * 2 - 1, Random01() * 2 - 1, Random01() * 2 - 1);
	}
	return glm::normalize(res);
}

inline glm::vec2 RandomDirection2D()
{
	glm::vec2 res;
	res = glm::vec2(Random01() * 2 - 1, Random01() * 2 - 1);
	while (glm::dot(res, res) > 1)
	{
		res = glm::vec2(Random01() * 2 - 1, Random01() * 2 - 1);
	}
	return res;
}

inline glm::vec3 RandomColor()
{
	return glm::vec3(Random01(), Random01(), Random01());
}

inline glm::vec3 RandomCosineDirection()
{
	float t1 = Random01();
	float phi = t1 * 2 * 3.1416;;
	float t2 = Random01();
	glm::vec3 res;
	res.x = glm::cos(phi) * glm::sqrt(t2);
	res.y = glm::sin(phi) * glm::sqrt(t2);
	res.z = glm::sqrt(1 - t2);
	return res;
}
