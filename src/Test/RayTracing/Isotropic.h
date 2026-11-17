#pragma once
#include"../../RayTracing/Material.h"

class Isotropic :public Material
{
private:
	glm::vec3 m_Albedo;
public:
	Isotropic(const glm::vec3& albedo)
		:m_Albedo(albedo){ }

	float ScatterPdf(Ray& ray, HitRecord& hitRecord, Ray& scattered) override;
	bool Scatter(Ray& ray, HitRecord& hitRecord, ScatterRecord& scatterRecord) override;
};