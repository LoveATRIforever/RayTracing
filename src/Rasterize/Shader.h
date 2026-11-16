#pragma once
#include"glm/glm.hpp"
#include<vector>
#include<functional>
#include<memory>

#define Interpolation3(array,value,weight) (array[0].value * weight[0] + array[1].value * weight[1] + array[2].value * weight[2])
#define Interpolation2(array,value,weight) (array[0].value * weight[0] + array[1].value * weight[1])

enum class PolygonType
{
	eTriangle,
	eLine,
	ePoint
};

class Shader
{
protected:
	std::function<glm::vec4(uint32_t, glm::vec2)> SampleTexture;
	std::function<void(uint32_t, glm::ivec2, glm::vec4)> WriteColorAttachment;
	std::function<const float* (uint32_t, uint32_t)> RandomUniform;

	template<typename T>
	T Uniform(uint32_t offset)
	{
		T t;
		const float* pData = RandomUniform(offset, sizeof(T) / 4);
		memcpy(&t, pData, sizeof(T));
		return t;
	}

public:
	virtual glm::vec4 VertexShader(const float* vertexData) = 0;
	virtual void FragmentShader(glm::vec3 barycentricCoordinates, glm::ivec2 pixel) = 0;
	virtual void Clear() = 0;
	virtual uint32_t VertexStride() = 0;
	virtual PolygonType Polygon() = 0;

	virtual float* GetMidData(uint32_t k) = 0;
	virtual uint32_t GetMidDataCount() = 0;

	void SetSampleTextureFunc(std::function<glm::vec4(uint32_t, glm::vec2)> sampleTexture)
	{
		SampleTexture = sampleTexture;
	}

	void SetWriteColorAttachmentFunc(std::function<void(uint32_t, glm::ivec2, glm::vec4)> writeColorAttachment)
	{
		WriteColorAttachment = writeColorAttachment;
	}

	void SetRandomUniform(std::function<const float* (uint32_t offset, uint32_t count)> randomUniform)
	{
		RandomUniform = randomUniform;
	}

	
};