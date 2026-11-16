#include"Cube.h"


glm::vec4 Cube::VertexShader(const float* vertexData)
{
	index++;

	glm::mat4 projection = Uniform<glm::mat4>(0);
	glm::mat4 view = Uniform<glm::mat4>(16);
	glm::mat4 model = Uniform<glm::mat4>(32);
	glm::vec4 vPosition(vertexData[0], vertexData[1], vertexData[2], 1.0f);
	glm::vec3 vNormal(vertexData[3], vertexData[4], vertexData[5]);
	glm::vec2 vUv(vertexData[6], vertexData[7]);
	
	Data[index].position = model * vPosition;
	glm::mat3 tmp = glm::transpose(glm::inverse(glm::mat3(model)));
	Data[index].normal = glm::normalize(tmp * vNormal);
	Data[index].uv = vUv;
	return projection * view * model * vPosition;

}

void Cube::FragmentShader(glm::vec3 barycentricCoordinates, glm::ivec2 pixel)
{
	glm::vec3 lightPosition = glm::vec3(5, 5, 5);
	glm::vec3 position = Interpolation3(Data, position, barycentricCoordinates);
	glm::vec3 normal = glm::normalize(Interpolation3(Data, normal, barycentricCoordinates));
	glm::vec2 uv = Interpolation3(Data, uv, barycentricCoordinates);
	glm::vec3 viewPosition = Uniform<glm::vec3>(48);

	glm::vec3 lightDirection = glm::normalize(lightPosition - position);
	glm::vec3 viewDirection = glm::normalize(viewPosition - position);
	glm::vec3 halfVector = glm::normalize(lightDirection + viewDirection);
	
	glm::vec3 diffuseColor = glm::vec3(SampleTexture(0, uv));
	glm::vec3 specularColor = glm::vec3(SampleTexture(1, uv));

	float diffuse = glm::max(glm::dot(normal, lightDirection), 0.0f);
	float specular = glm::max(glm::dot(normal, halfVector), 0.0f);
	specular *= specular;
	specular *= specular;
	specular *= specular;
	specular *= specular;
	specular *= specular;
	
	glm::vec3 color = diffuseColor * glm::vec3(diffuse + 0.1) + specularColor * glm::vec3(specular);
	color = glm::vec4(1, 0, 0, 1);
	WriteColorAttachment(0, pixel, glm::vec4(color, 1.0f));
}

void Cube::Clear()
{
	index = -1;
}

uint32_t Cube::VertexStride()
{
	return 8;
}

float* Cube::GetMidData(uint32_t k)
{
	return (float*)&Data[k];
}

uint32_t Cube::GetMidDataCount()
{
	return sizeof(Data[0]) / 4;
}
