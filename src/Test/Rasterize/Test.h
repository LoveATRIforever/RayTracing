#pragma once
#include"../../Rasterize/Renderer.h"
#include"glad/glad.h"
#include"Camera.h"

class Test
{
private:
	Renderer m_Renderer;
	std::shared_ptr<Texture> m_ColorAttachment;
	std::shared_ptr<Texture> m_DepthAttachment;
	std::shared_ptr<Shader> m_TriangleShader;
	std::shared_ptr<Shader> m_CubeShader;
	std::shared_ptr<Shader> m_LineShader;
	std::shared_ptr<Shader> m_PointShader;
	std::shared_ptr<Shader> m_BoxShader;

	std::shared_ptr<Texture> m_Texture1;
	std::shared_ptr<Texture> m_Texture2;

	uint32_t m_OpenGLTexture;
	EditorCamera m_Camera = EditorCamera(CameraType::ePerspective);
	bool m_IsCameraRotate = false;
	glm::vec2 m_LastMousePos;

	int32_t example = 0;
public:
	Test();
	~Test();
	
	void Render(float tick);
	
private:
	void DrawTriangle();
	void DrawCube();
	void DrawLine();
	void DrawPoint();
	void DrawBox();

	void CreateOpenGLTextureForColorAttachment();
	void UpdateCamera(float tick);
};