#pragma once
#include"../../RayTracing/Shape.h"
#include<memory>

class Translate :public Shape
{
private:
	std::shared_ptr<Shape> m_Shape;
	glm::vec3 m_Translate;
public:
	Translate(std::shared_ptr<Shape> shape, glm::vec3 translate);
	
	bool RayCheck(Ray& ray, HitRecord& hitRecord) override;
};

//按世界坐标轴旋转
class RotateY :public Shape
{
private:
	std::shared_ptr<Shape> m_Shape;
	float m_Angle;
	float m_Cos;
	float m_Sin;
public:
	RotateY(std::shared_ptr<Shape> shape, float angle);

	bool RayCheck(Ray& ray, HitRecord& hitRecord) override;
};