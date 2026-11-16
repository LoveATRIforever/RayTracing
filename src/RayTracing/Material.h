#pragma once
#include"glm/glm.hpp"

class HitRecord;
class Ray;

class Material
{
public:
	virtual bool Scatter(Ray& ray, HitRecord& hitRecord, glm::vec3& attenuation, Ray& scattered) = 0;
	virtual glm::vec3 Emitted(HitRecord& hitRecord) { return glm::vec3(0); }
};