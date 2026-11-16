#include"Test.h"
#include"Triangle.h"
#include"Cube.h"
#include"Line.h"
#include"Point.h"
#include"imgui.h"
#include"glm/gtc/matrix_transform.hpp"


Test::Test()
{
	m_ColorAttachment = std::make_shared<Texture>(800, 600, TextureFormat::eRGB8);
	m_DepthAttachment = std::make_shared<Texture>(800, 600, TextureFormat::eR32F);

	m_TriangleShader = std::make_shared<Triangle>();
	m_CubeShader = std::make_shared<Cube>();
	m_LineShader = std::make_shared<Line>();
	m_PointShader = std::make_shared<Point>();
	m_Renderer.SetColorAttachment(m_ColorAttachment);
	m_Renderer.SetDepthAttachment(m_DepthAttachment);
	m_Texture1 = std::make_shared<Texture>("assets/texture/img3.png");
	m_Renderer.SetTexture(0, m_Texture1);
	m_Texture2 = std::make_shared<Texture>("assets/texture/img4.png");
	m_Renderer.SetTexture(1, m_Texture2);


	CreateOpenGLTextureForColorAttachment();
	m_Renderer.SetShader(m_CubeShader);
}

Test::~Test()
{
	glDeleteTextures(1, &m_OpenGLTexture);
}

void Test::Render(float tick)
{
	
	switch (example)
	{
	case 0:
		DrawCube();
		break;
	case 1:
		DrawLine();
		break;
	case 2:
		DrawPoint();
		break;
	}
	ImGui::Begin("Viewport");
	UpdateCamera(tick);
	ImGui::ImageButton("color", (ImTextureID)m_OpenGLTexture, ImVec2(800, 600), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
	ImGui::Begin("RenderInfo");
	ImGui::Text("Frame Time: %.3f ms", tick * 1000.0f);
	ImGui::End();

	ImGui::Begin("Config");

	if (ImGui::RadioButton("Cube", &example, 0))
	{
		m_Renderer.SetShader(m_CubeShader);
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Line", &example, 1))
	{
		m_Renderer.SetShader(m_LineShader);
	}
	ImGui::SameLine();
	if(ImGui::RadioButton("Point", &example, 2))
	{
		m_Renderer.SetShader(m_PointShader);
	}

	bool wireframe = m_Renderer.IsEnableWireframe();
	if (ImGui::Checkbox("wireframe", &wireframe))
	{
		m_Renderer.EnableWireframe(wireframe);
	}

	bool faceCull = m_Renderer.IsEnableFaceCull();
	if (ImGui::Checkbox("faceCull", &faceCull))
	{
		m_Renderer.EnableFaceCull(faceCull);
	}
	ImGui::End();
}

void Test::DrawTriangle()
{
	std::vector<float> vertex =
	{
		-0.5,-0.5,1,1,0,
		0.5,-0.5,0,1,0,
		0,0.5,0,0,1
	};
	std::vector<uint32_t> index =
	{
		0,1,2
	};
	m_ColorAttachment->Clear();
	m_DepthAttachment->Clear({ 1,0,0,0 });
	m_Renderer.DrawIndexed(vertex, index);
	glTextureSubImage2D(m_OpenGLTexture, 0, 0, 0, m_ColorAttachment->Width(), m_ColorAttachment->Height(), GL_RGB, GL_UNSIGNED_BYTE, m_ColorAttachment->Data());
}

void Test::DrawCube()
{
	std::vector<float> vertex =
	{
		// pos(x,y,z)            normal(x,y,z)          uv(u,v)
		// +X
		 0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f, 0.0f,

		 // -X
		 -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,     0.0f, 0.0f,
		 -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,     1.0f, 0.0f,
		 -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,     1.0f, 1.0f,
		 -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,     0.0f, 1.0f,

		 // +Y
		 -0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     0.0f, 1.0f,
		 -0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     0.0f, 0.0f,
		  0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f, 0.0f,
		  0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f, 1.0f,

		  // -Y
		  -0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     0.0f, 0.0f,
		   0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f, 0.0f,
		   0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f, 1.0f,
		  -0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     0.0f, 1.0f,

		  // +Z
		  -0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     0.0f, 0.0f,
		   0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f, 0.0f,
		   0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f, 1.0f,
		  -0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     0.0f, 1.0f,

		  // -Z
		  -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f, 0.0f,
		  -0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f, 1.0f,
		   0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     0.0f, 1.0f,
		   0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     0.0f, 0.0f,
	};
	std::vector<uint32_t> index =
	{
		0,  1,  2,   0,  2,  3,   // +X
		4,  5,  6,   4,  6,  7,   // -X
		8,  9, 10,   8, 10, 11,   // +Y
		12, 13, 14,  12, 14, 15,   // -Y
		16, 17, 18,  16, 18, 19,   // +Z
		20, 21, 22,  20, 22, 23   // -Z
	};
	
	glm::vec3 viewPosition = m_Camera.m_Pos;
	
	glm::mat4 model = glm::mat4(1);
	glm::mat4 view = glm::lookAt(viewPosition, viewPosition + m_Camera.m_Front, glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::mat4 view = glm::lookAt(viewPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	struct UniformBufferObject
	{
		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 model;
		glm::vec3 viewPosition;
	};
	
	UniformBufferObject ubo;
	ubo.model = model;
	ubo.view = view;
	ubo.projection = projection;
	ubo.viewPosition = viewPosition;
	m_Renderer.SetUniformBuffer((float*)& ubo, sizeof(UniformBufferObject));
	m_ColorAttachment->Clear();
	m_DepthAttachment->Clear({ 1,0,0,0 });
	m_Renderer.DrawIndexed(vertex, index);
	glTextureSubImage2D(m_OpenGLTexture, 0, 0, 0, m_ColorAttachment->Width(), m_ColorAttachment->Height(), GL_RGB, GL_UNSIGNED_BYTE, m_ColorAttachment->Data());
}

void Test::DrawLine()
{
	std::vector<float> vertex =
	{
		-0.5,-0.5,0,1,0,0,
		0.5,-0.5,0,1,0,0,
		0.5,0.5,0,1,0,0,
		-0.5,0.5,0,1,0,0
	};

	std::vector<uint32_t> index =
	{
		0,1,
		1,2,
		2,3,
		3,0
	};

	glm::vec3 viewPosition = m_Camera.m_Pos;

	glm::mat4 model = glm::mat4(1);
	glm::mat4 view = glm::lookAt(viewPosition, viewPosition + m_Camera.m_Front, glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::mat4 view = glm::lookAt(viewPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	struct UniformBufferObject
	{
		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 model;
		glm::vec3 viewPosition;
	};

	UniformBufferObject ubo;
	ubo.model = model;
	ubo.view = view;
	ubo.projection = projection;
	ubo.viewPosition = viewPosition;
	m_Renderer.SetUniformBuffer((float*)&ubo, sizeof(UniformBufferObject));
	m_ColorAttachment->Clear();
	m_DepthAttachment->Clear({ 1,0,0,0 });

	m_Renderer.DrawIndexed(vertex, index);
	glTextureSubImage2D(m_OpenGLTexture, 0, 0, 0, m_ColorAttachment->Width(), m_ColorAttachment->Height(), GL_RGB, GL_UNSIGNED_BYTE, m_ColorAttachment->Data());
}

void Test::DrawPoint()
{
	std::vector<float> vertex =
	{
		-0.5,-0.5,0,1,0,0,
		0.5,-0.5,0,1,0,0,
		0.5,0.5,0,1,0,0,
		-0.5,0.5,0,1,0,0
	};

	std::vector<uint32_t> index =
	{
		0,1,2,3
	};

	glm::vec3 viewPosition = m_Camera.m_Pos;

	glm::mat4 model = glm::mat4(1);
	glm::mat4 view = glm::lookAt(viewPosition, viewPosition + m_Camera.m_Front, glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::mat4 view = glm::lookAt(viewPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	struct UniformBufferObject
	{
		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 model;
		glm::vec3 viewPosition;
	};

	UniformBufferObject ubo;
	ubo.model = model;
	ubo.view = view;
	ubo.projection = projection;
	ubo.viewPosition = viewPosition;
	m_Renderer.SetUniformBuffer((float*)&ubo, sizeof(UniformBufferObject));
	m_ColorAttachment->Clear();
	m_DepthAttachment->Clear({ 1,0,0,0 });

	m_Renderer.DrawIndexed(vertex, index);
	glTextureSubImage2D(m_OpenGLTexture, 0, 0, 0, m_ColorAttachment->Width(), m_ColorAttachment->Height(), GL_RGB, GL_UNSIGNED_BYTE, m_ColorAttachment->Data());
}

void Test::CreateOpenGLTextureForColorAttachment()
{
	glCreateTextures(GL_TEXTURE_2D, 1, &m_OpenGLTexture);
	glTextureStorage2D(m_OpenGLTexture, 1, GL_RGB8, 800, 600);
}

void Test::UpdateCamera(float tick)
{

	if (!ImGui::IsWindowFocused() || !ImGui::IsWindowHovered() || !ImGui::IsKeyDown(ImGuiKey_LeftShift))
	{
		m_IsCameraRotate = false;
		return;
	}
	float delta = tick;
	glm::vec3 move{ 0 };
	glm::vec2 rotate{ 0 };
	if (ImGui::IsKeyDown(ImGuiKey_D))
		move.x += 1;
	if (ImGui::IsKeyDown(ImGuiKey_A))
		move.x -= 1;
	if (ImGui::IsKeyDown(ImGuiKey_F))
		move.y += 1;
	if (ImGui::IsKeyDown(ImGuiKey_C))
		move.y -= 1;
	if (ImGui::IsKeyDown(ImGuiKey_W))
		move.z += 1;
	if (ImGui::IsKeyDown(ImGuiKey_S))
		move.z -= 1;
	move *= delta;

	if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
	{
		auto tmp = ImGui::GetMousePos();
		glm::vec2 mousePos = { tmp.x,tmp.y };
		if (m_IsCameraRotate)
		{
			
			rotate = mousePos - m_LastMousePos;
			//屏幕空间原点在左上
			rotate *= 0.003f;
			m_LastMousePos = mousePos;
		}
		else
		{
			m_IsCameraRotate = true;
			m_LastMousePos = mousePos;
		}
	}
	else
	{
		m_IsCameraRotate = false;
	}
	//屏幕上x轴对应世界空间右，y轴对应世界空间上
	m_Camera.Update(move, { rotate.x, -rotate.y });

}
