#pragma once

#include"glm/glm.hpp"

class Pdf
{
public:
	virtual float Value(const glm::vec3& dir) = 0;
	virtual glm::vec3 Gen() = 0;
};