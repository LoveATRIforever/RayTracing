#pragma once
#include"glm/glm.hpp"
#include<array>

class Perlin
{
private:
	std::array<glm::vec3, 256> m_RanFloat;
	std::array<int, 256> m_X;
	std::array<int, 256> m_Y;
	std::array<int, 256> m_Z;
public:
	Perlin();
	
	float Noise(const glm::vec3& point);
	float Turb(const glm::vec3& point, int depth);
};