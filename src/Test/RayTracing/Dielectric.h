#pragma once
#include"../../RayTracing/Shape.h"
#include"../../RayTracing/Material.h"
#include<memory>

class Dielectric :public Material
{
private:
	float m_Ir;
public:
	Dielectric(float ir)
		:m_Ir(ir) {}

	bool Scatter(Ray& ray, HitRecord& hitRecord, ScatterRecord& scatterRecord) override;
};