#include "Triangle.h"

glm::vec4 Triangle::VertexShader(const float* vertexData)
{
    index++;
	Data[index].color = glm::vec3(vertexData[2], vertexData[3], vertexData[4]);
    return glm::vec4(vertexData[0], vertexData[1], 0.0f, 1.0f);
}

void Triangle::FragmentShader(glm::vec3 barycentricCoordinates, glm::ivec2 pixel)
{
    glm::vec4 color = glm::vec4(Interpolation3(Data, color, barycentricCoordinates), 1.0f);
    WriteColorAttachment(0, pixel, color);
}

void Triangle::Clear()
{
    index = -1;
}

uint32_t Triangle::VertexStride()
{
    return 5;
}

float* Triangle::GetMidData(uint32_t k)
{
    return (float*)&Data[k];
}

uint32_t Triangle::GetMidDataCount()
{
    return sizeof(Data[0]) / 4;
}


