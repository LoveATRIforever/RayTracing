#pragma once
#include"../../RayTracing/Shape.h"
#include"Isotropic.h"

class ConstantMedium :public Shape
{
private:
	std::shared_ptr<Material> m_Mat;
	std::shared_ptr<Shape> m_Shape;
	float m_C;
public:
	ConstantMedium(std::shared_ptr<Shape> shape, float d, const glm::vec3& color)
		:m_Shape(shape), m_Mat(std::make_shared<Isotropic>(color)), m_C(-1 / d) 
	{
		m_BoundingBox = m_Shape->GetBoundingBox();
	}

	bool RayCheck(Ray& ray, HitRecord& hitRecord) override;
};