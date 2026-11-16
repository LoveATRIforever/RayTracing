#pragma once
#include"glm/glm.hpp"



class Ray
{
private:
	glm::vec3 m_Origin;
	glm::vec3 m_Direction;
public:
	Ray() = default;
	Ray(const glm::vec3& origin, const glm::vec3& direction)
		:m_Origin(origin),m_Direction(glm::normalize(direction)){ }

	glm::vec3 At(float t) { return m_Origin + t * m_Direction; }
	glm::vec3& Origin() { return m_Origin; }
	glm::vec3& Direction() { return m_Direction; }
};