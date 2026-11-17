#pragma once
#include"../../RayTracing/Shape.h"

#include<memory>

class ShapeArray :public Shape
{
private:
	std::vector<std::shared_ptr<Shape>> m_Array;
public:
	ShapeArray() = default;
	void Clear() { m_Array.clear(); }
	void Add(std::shared_ptr<Shape> shape) { m_Array.push_back(shape); m_BoundingBox.Add(shape->GetBoundingBox()); }
	bool RayCheck(Ray& ray, HitRecord& hitRecord) override;

	float PdfValue(const glm::vec3& ori, const glm::vec3& dir) override;
	glm::vec3 Random(const glm::vec3& ori) override;
	
};