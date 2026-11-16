#pragma once
#include"../../RayTracing/Shape.h"
#include<memory>
#include<vector>

class BVHNode :public Shape
{
private:
	std::shared_ptr<Shape> m_Left;
	std::shared_ptr<Shape> m_Right;
public:
	BVHNode(const std::vector<std::shared_ptr<Shape>>& shapes);
	BVHNode(const std::vector<std::shared_ptr<Shape>>& shapes, size_t begin, size_t end);

	bool RayCheck(Ray& ray, HitRecord& hitRecord) override;
};