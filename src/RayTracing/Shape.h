#pragma once
#include"Ray.h"
#include"HitRecord.h"

class BoundingBox
{
public:
	glm::vec3 Min;
	glm::vec3 Max;
public:
	void Add(glm::vec3 point)
	{
		Max = glm::max(Max, point);
		Min = glm::min(Min, point);
	}

	void Add(BoundingBox box)
	{
		Max = glm::max(Max, box.Max);
		Min = glm::min(Min, box.Min);
	}

	BoundingBox()
	{
		Min = glm::vec3(std::numeric_limits<float>::infinity());
		Max = glm::vec3(-std::numeric_limits<float>::infinity());
	}

	BoundingBox(glm::vec3 min, glm::vec3 max)
	{
		Min = min;
		Max = max;
	}

	bool RayCheck(Ray& ray, float max);

	void Padding(float pad);
};

class Shape
{
protected:
	BoundingBox m_BoundingBox;
public:
	virtual bool RayCheck(Ray& ray, HitRecord& hitRecord) = 0;

	virtual float PdfValue(const glm::vec3& ori, const glm::vec3& dir)
	{
		return 0;
	}

	virtual glm::vec3 Random(const glm::vec3& ori)
	{
		return glm::vec3(1, 0, 0);
	}

	BoundingBox GetBoundingBox() { return m_BoundingBox; }

};