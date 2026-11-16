#pragma once
#include"../../Rasterize/Shader.h"

class Point :public Shader
{
private:
	struct
	{
		glm::vec3 color;
	}Data;
public:
	glm::vec4 VertexShader(const float* vertexData) override;
	void FragmentShader(glm::vec3 barycentricCoordinates, glm::ivec2 pixel) override;
	void Clear() override;
	uint32_t VertexStride() override;
	PolygonType Polygon() override { return PolygonType::ePoint; }

	float* GetMidData(uint32_t k) override;
	uint32_t GetMidDataCount() override;

};