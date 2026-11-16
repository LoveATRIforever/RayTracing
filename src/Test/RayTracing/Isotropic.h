#pragma once
#include"../../RayTracing/Material.h"

class Isotropic :public Material
{
private:
	glm::vec3 m_Albedo;
public:
	Isotropic(const glm::vec3& albedo)
		:m_Albedo(albedo){ }


	bool Scatter(Ray& ray, HitRecord& hitRecord, glm::vec3& attenuation, Ray& scattered) override;
};