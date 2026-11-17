#pragma once
#include"../../RayTracing/Material.h"
#include"SolidColor.h"
#include<memory>

class DiffuseLight :public Material
{
private:
	std::shared_ptr<RTexture> m_Emit;
public:
	DiffuseLight(const glm::vec3 color)
		:m_Emit(std::make_shared<SolidColor>(color)){ }
	DiffuseLight(std::shared_ptr<RTexture> tex)
		:m_Emit(tex){ }

	bool Scatter(Ray& ray, HitRecord& hitRecord, ScatterRecord& scatterRecord) override;
	glm::vec3 Emitted(Ray& ray, HitRecord& hitRecord) override;
};