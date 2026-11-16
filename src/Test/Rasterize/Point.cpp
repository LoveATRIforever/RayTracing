#include "Point.h"

glm::vec4 Point::VertexShader(const float* vertexData)
{
	glm::mat4 projection = Uniform<glm::mat4>(0);
	glm::mat4 view = Uniform<glm::mat4>(16);
	glm::mat4 model = Uniform<glm::mat4>(32);
	glm::vec4 vPosition(vertexData[0], vertexData[1], vertexData[2], 1.0f);
	glm::vec3 vColor(vertexData[3], vertexData[4], vertexData[5]);

	Data.color = vColor;
	return projection * view * model * vPosition;
}

void Point::FragmentShader(glm::vec3 barycentricCoordinates, glm::ivec2 pixel)
{
	glm::vec3 color = glm::vec4(Data.color, 1.0);
	WriteColorAttachment(0, pixel, glm::vec4(color, 1.0f));
}

void Point::Clear()
{
}

uint32_t Point::VertexStride()
{
    return 6;
}

float* Point::GetMidData(uint32_t k)
{
	return (float*)&Data;
}

uint32_t Point::GetMidDataCount()
{
    return 3;
}
