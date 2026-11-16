#pragma once
#include"../../Rasterize/Shader.h"

class Cube :public Shader
{
private:
	int32_t index = -1;
	struct
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;
	}Data[3];
public:
	glm::vec4 VertexShader(const float* vertexData) override;
	void FragmentShader(glm::vec3 barycentricCoordinates, glm::ivec2 pixel) override;
	void Clear() override;
	uint32_t VertexStride() override;
	PolygonType Polygon() override { return PolygonType::eTriangle; }

	float* GetMidData(uint32_t k) override;
	uint32_t GetMidDataCount() override;
};