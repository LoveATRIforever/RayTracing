#pragma once
#include"Shader.h"
#include"Texture.h"
#include<cstdint>
#include<vector>
#include<memory>
#include<Array>

class Renderer
{
private:
	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<Texture> m_ColorAttachment;
	std::shared_ptr<Texture> m_DepthAttachment;


	struct
	{
		bool Wireframe = true;
		bool FaceCull = true;
	}Config;

	const uint32_t MaxUniform = 100;
	std::array<float, 100> m_Uniforms{0};
	const uint32_t MaxTexture = 16;
	std::array<std::shared_ptr<Texture>, 16> m_Textures;
public:
	void DrawIndexed(const std::vector<float>& vertexs, const std::vector<uint32_t>& index);
	void SetShader(std::shared_ptr<Shader> shader);
	void SetColorAttachment(std::shared_ptr<Texture> colorAttachment) { m_ColorAttachment = colorAttachment; }
	void SetDepthAttachment(std::shared_ptr<Texture> depthAttachment) { m_DepthAttachment = depthAttachment; }
	void SetUniformBuffer(float* data, uint32_t size);
	void SetTexture(uint32_t index, std::shared_ptr<Texture> texture)
	{
		if (index < 0 || index >= MaxTexture)
			return;
		m_Textures[index] = texture;
	}

	void EnableWireframe(bool enable) { Config.Wireframe = enable; }
	bool IsEnableWireframe() { return Config.Wireframe; }

	void EnableFaceCull(bool enable) { Config.FaceCull = enable; }
	bool IsEnableFaceCull() { return Config.FaceCull; }
private:
	bool ReadyForDraw();

	void DrawTriangle(const std::array<const float*, 3>& pData);
	bool IsPixelInTriangle(glm::vec2 pixel, const std::array<glm::vec2, 3>& triangle);
	glm::vec3 ComputeBarycentricCoordinates(glm::vec2 pixel, const std::array<glm::vec2, 3>& triangle);
	
	void RasterizeTriangle(const std::array<glm::vec4, 3>& point);

	void DrawPoint(const float* pData);

	void DrawLine(const std::array<const float*, 2>& pData);
	void RasterizeLine(const std::array<glm::vec4, 2> point);
};