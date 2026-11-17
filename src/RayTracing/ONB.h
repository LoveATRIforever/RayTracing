#pragma once
#include"glm/glm.hpp"

class ONB
{
private:
	glm::mat3 m_TBN;
public:
	ONB(glm::vec3 n)
	{
		glm::vec3 a = glm::abs(n.x) > 0.9 ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0);
		glm::vec3 t = glm::normalize(glm::cross(n, a));
		glm::vec3 b = glm::cross(n, t);
		m_TBN = glm::mat3(t, b, n);
	}

	glm::vec3& operator[](int i) { return m_TBN[i]; }

	glm::vec3 ToGlobal(const glm::vec3& v) { return glm::normalize(m_TBN * v); }
};