#pragma once
#include"glm/glm.hpp"
#include"Ray.h"
#include"Pdf.h"
#include<memory>

class HitRecord;

struct ScatterRecord
{
	glm::vec3 attenuation;
	std::shared_ptr<Pdf> pdf;
	bool skipPdf;
	Ray ray;
};

class Material
{
public:
	virtual bool Scatter(Ray& ray, HitRecord& hitRecord, ScatterRecord& scatterRecord) = 0;
	virtual float ScatterPdf(Ray& ray, HitRecord& hitRecord, Ray& scattered) { return 0; }
	
	virtual glm::vec3 Emitted(Ray& ray, HitRecord& hitRecord) { return glm::vec3(0); }
};