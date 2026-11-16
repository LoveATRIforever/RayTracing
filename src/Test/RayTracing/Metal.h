#pragma once
#include"../../RayTracing/Material.h"

class Metal :public Material
{
private:
	glm::vec3 m_Albedo;
	float m_Fuzz;
public:
	Metal(glm::vec3 albedo, float fuzz)
		:m_Albedo(albedo), m_Fuzz(fuzz) {}

	bool Scatter(Ray& ray, HitRecord& hitRecord, glm::vec3& attenuation, Ray& scattered) override;
};