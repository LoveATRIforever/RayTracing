#pragma once
#define GLFW_INCLUDE_NONE
#include"GLFW/glfw3.h"
#include"glad/glad.h"

#include<chrono>

class Application
{
private:
	GLFWwindow* m_Window;
	std::chrono::high_resolution_clock::time_point m_LastTimePointer;
	float m_Tick = 0.0f;
public:
	void Init();
	void Shutdown();
	void Run();
private:
	void Render();

	void InitImGui();
	void ShutdownImGui();
	void BeginImGuiFrame();
	void EndImGuiFrame();
	void SetImGuiStyle();
	
	void UpdateTick();
};