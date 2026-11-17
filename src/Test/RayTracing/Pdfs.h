#pragma once
#include"../../RayTracing/Pdf.h"
#include"../../RayTracing/ONB.h"
#include"../../RayTracing/Shape.h"
#include<memory>

class SpherePdf :public Pdf
{
public:
	float Value(const glm::vec3& dir) override;
	glm::vec3 Gen() override;
};

class CosinePdf :public Pdf
{
private:
	ONB m_Onb;
	glm::vec3 m_Normal;
public:
	CosinePdf(const glm::vec3 n)
		:m_Onb(n),m_Normal(n)
	{
	}

	float Value(const glm::vec3& dir) override;
	glm::vec3 Gen() override;
};

class ShapePdf :public Pdf
{
private:
	std::shared_ptr<Shape> m_Shape;
	glm::vec3 m_Ori;
public:
	ShapePdf(std::shared_ptr<Shape> shape, glm::vec3 ori)
		:m_Shape(shape), m_Ori(ori){ }

	float Value(const glm::vec3& dir) override;
	glm::vec3 Gen() override;
};

class MixturePdf : public Pdf
{
private:
	std::shared_ptr<Pdf> m_Pdf[2];
public:
	MixturePdf(std::shared_ptr<Pdf> pdf1, std::shared_ptr<Pdf> pdf2)
	{
		m_Pdf[0] = pdf1;
		m_Pdf[1] = pdf2;
	}

	// 通过 Pdf 继承
	float Value(const glm::vec3& dir) override;

	glm::vec3 Gen() override;

};