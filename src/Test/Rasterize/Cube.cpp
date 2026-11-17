#include"Cube.h"


glm::vec4 Cube::VertexShader(const float* vertexData)
{
	index++;

	glm::mat4 projection = Uniform<glm::mat4>(0);
	glm::mat4 view = Uniform<glm::mat4>(16);
	glm::mat4 model = Uniform<glm::mat4>(32);
	glm::vec4 vPosition(vertexData[0], vertexData[1], vertexData[2], 1.0f);

	return projection * view * model * vPosition;
}

void Cube::FragmentShader(glm::vec3 barycentricCoordinates, glm::ivec2 pixel)
{
	WriteColorAttachment(0, pixel, glm::vec4(1, 0, 0, 1));
}

void Cube::Clear()
{
	index = -1;
}

uint32_t Cube::VertexStride()
{
	return 3;
}

float* Cube::GetMidData(uint32_t k)
{
	return nullptr;
}

uint32_t Cube::GetMidDataCount()
{
	return 0;
}
