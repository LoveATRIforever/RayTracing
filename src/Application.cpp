#include"Application.h"
#include"Log.h"
#include"imgui.h"
#include "backends/imgui_impl_glfw.h"
#include"backends/imgui_impl_opengl3.h"

#include<format>

#include"Test/Rasterize/Test.h"
#include"Test/RayTracing/RayTracingTest.h"


void GLFWErrorCallback(int error, const char* description)
{
	LOG(description);
}

static void OpenGLLogMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		LOG(std::format("[OpenGL Debug HIGH] {0}", message));
		ASSERT(false, "GL_DEBUG_SEVERITY_HIGH");
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		LOG(std::format("[OpenGL Debug MEDIUM] {0}", message));
		break;
	case GL_DEBUG_SEVERITY_LOW:
		LOG(std::format("[OpenGL Debug LOW] {0}", message));
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		// s_Context->Log(std::format("[OpenGL Debug NOTIFICATION] {0}", message));
		break;
	}
}

void Application::Init()
{
	ASSERT(glfwInit(), "Failed to initialize GLFW");
	glfwSetErrorCallback(GLFWErrorCallback);

	m_Window = nullptr;
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_Window = glfwCreateWindow(1280, 720, "Render", nullptr, nullptr);
	ASSERT(m_Window != nullptr, "Failed to create GLFW window");
	
	glfwMakeContextCurrent(m_Window);

	ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize GLAD");
	glDebugMessageCallback(OpenGLLogMessage, nullptr);

	InitImGui();
}

void  Application::Shutdown()
{
	ShutdownImGui();
	glfwTerminate();
}

void Application::Run()
{
	m_LastTimePointer = std::chrono::high_resolution_clock::now();
	RayTracingTest test;
	while (!glfwWindowShouldClose(m_Window))
	{
		UpdateTick();
		glfwPollEvents();
		BeginImGuiFrame();

		Render();
		test.Render(m_Tick);

		ImGui::ShowDemoWindow();

		EndImGuiFrame();
		glfwSwapBuffers(m_Window);
	}

}

void Application::Render()
{
	
}

void Application::InitImGui()
{
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
	ImGui_ImplOpenGL3_Init();

	io.FontDefault = io.Fonts->AddFontFromFileTTF("resources/fonts/Delius/Delius-Regular.ttf", 20.0f);
	ImGui::StyleColorsDark();
	SetImGuiStyle();
}

void Application::ShutdownImGui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Application::BeginImGuiFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
}

void Application::EndImGuiFrame()
{
	ImGui::Render();
	ImDrawData* main_draw_data = ImGui::GetDrawData();
	ImGui_ImplOpenGL3_RenderDrawData(main_draw_data);
	ImGuiIO io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void Application::SetImGuiStyle()
{
	auto& colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

	// Headers
	colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Frame BG
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
}

void Application::UpdateTick()
{
	auto tmp = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> deltaTime = tmp - m_LastTimePointer;
	m_LastTimePointer = tmp;
	m_Tick = deltaTime.count();
}
