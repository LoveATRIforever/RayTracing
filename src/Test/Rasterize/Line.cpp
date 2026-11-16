#include "Line.h"

glm::vec4 Line::VertexShader(const float* vertexData)
{
	index++;

	glm::mat4 projection = Uniform<glm::mat4>(0);
	glm::mat4 view = Uniform<glm::mat4>(16);
	glm::mat4 model = Uniform<glm::mat4>(32);
	glm::vec4 vPosition(vertexData[0], vertexData[1], vertexData[2], 1.0f);
	glm::vec3 vColor(vertexData[3], vertexData[4], vertexData[5]);

	Data[index].color = vColor;
	return projection * view * model * vPosition;
}

void Line::FragmentShader(glm::vec3 barycentricCoordinates, glm::ivec2 pixel)
{
	glm::vec3 color = glm::vec4(Interpolation2(Data, color, barycentricCoordinates), 1.0);
	WriteColorAttachment(0, pixel, glm::vec4(color, 1.0f));
}

void Line::Clear()
{
	index = -1;
}

uint32_t Line::VertexStride()
{
    return 6;
}


float* Line::GetMidData(uint32_t k)
{
    return (float*)&Data[k];
}

uint32_t Line::GetMidDataCount()
{
    return 3;
}
