#pragma once
#include"../../RayTracing/Shape.h"
#include"../../RayTracing/Material.h"
#include<memory>

class Quad :public Shape
{
private:
	glm::vec3 m_Q;
	glm::vec3 m_U;
	glm::vec3 m_V;
	std::shared_ptr<Material> m_Mat;

	glm::vec3 m_Normal;
	float m_D;
	glm::vec3 m_W;
public:
	Quad(const glm::vec3& o, const glm::vec3& u, const glm::vec3& v, std::shared_ptr<Material> mat);
		
	bool RayCheck(Ray& ray, HitRecord& hitRecord) override;
};