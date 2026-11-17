#pragma once
#include"../../RayTracing/Material.h"
#include"../../RayTracing/RTexture.h"
#include<memory>
#include"SolidColor.h"


class Lambertian :public Material
{
private:
	std::shared_ptr<RTexture> m_Albedo;
public:
	Lambertian(std::shared_ptr<RTexture> albedo)
		:m_Albedo(albedo) {}
	Lambertian(glm::vec3 albedo)
		:m_Albedo(std::make_shared<SolidColor>(albedo)) {
	}

	float ScatterPdf(Ray& ray, HitRecord& hitRecord, Ray& scattered) override;
	bool Scatter(Ray& ray, HitRecord& hitRecord, ScatterRecord& scatterRecord) override;
};