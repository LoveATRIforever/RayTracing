#pragma once
#include"../../RayTracing/Shape.h"
#include"../../RayTracing/Material.h"
#include<memory>

class Sphere :public Shape
{
private:
	glm::vec3 m_Center;
	float m_Radius;
	std::shared_ptr<Material> m_Material;
public:
	Sphere() = default;
	Sphere(glm::vec3 center, float radius, std::shared_ptr<Material> material);

	bool RayCheck(Ray& ray, HitRecord& hitRecord) override;
};