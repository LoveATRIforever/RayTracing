#pragma once
#include"glm/glm.hpp"
#include"glm/gtc/quaternion.hpp"


//右手系
constexpr const glm::vec3 STD_DIR_UP = { 0,1,0 };
constexpr const glm::vec3 STD_DIR_RIGHT = { 1,0,0 };
constexpr const glm::vec3 STD_DIR_FRONT = { 0,0,-1 };
constexpr const glm::quat STD_ROTATION = { 0,0,0,-1 };

//用于渲染的最终相机数据
struct RenderCamera
{
	glm::mat4 View = glm::mat4(1.0f);
	glm::mat4 Projection = glm::mat4(1.0f);
	glm::vec3 ViewPosition = glm::vec3(0.0f);
};

enum class CameraType
{
	ePerspective = 0,	//投影
	eOrtho = 1			//正交
};

//用来计算RenderCamera的投影相机数据
struct PerspectiveCamera
{
	glm::vec3 Pos{ 0 };
	glm::vec3 Front = STD_DIR_FRONT;
	float Fov = 45.0f;
	float AspectRatio = 16.0f / 9.0f;
	float Near = 0.1f;
	float Far = 1000.0f;
};

//用来计算RenderCamera的正交相机数据
struct OrthoCamera
{
	glm::vec3 Pos{ 0 };
	glm::vec3 Front = STD_DIR_FRONT;
	float Width = 16.0f;
	float Height = 9.0f;
	float Near = 0.0f;
	float Far = 25.0f;
};

//相机的计算方法，用于计算相机组件和编辑器相机
class LogicCamera
{
public:
	RenderCamera GetRenderCameraPerspective(PerspectiveCamera camera);
	RenderCamera GetRenderCameraOrtho(OrthoCamera camera);

	//相机转换
	OrthoCamera Switch(PerspectiveCamera camera);
	PerspectiveCamera Switch(OrthoCamera camera);
};

class EditorCamera
{
public:
	LogicCamera m_LogicCamera;

	CameraType m_Type;

	glm::vec3 m_Pos{ 0 };
	glm::vec3 m_Front = STD_DIR_FRONT;
	glm::vec3 m_Right = STD_DIR_RIGHT;
	glm::vec3 m_Up = STD_DIR_UP;
	glm::quat m_Rotate = STD_ROTATION;

	//透视
	PerspectiveCamera m_PerspectiveData;
	//正交
	OrthoCamera m_OrthoData;
public:
	EditorCamera(CameraType type);

	//如果 enableSwitch=true 将在转换时通过当前相机类型的数据推算出转过后
	//相机类型的数据
	void SetCameraType(CameraType type, bool enableSwitch);
	CameraType GetCameraType() { return m_Type; }

	RenderCamera GetRenderCamera();

	void Update(const glm::vec3& move, const glm::vec2 rotate);

	void SetViewportSize(uint32_t width, uint32_t height);
	void Reset();
private:
	void UpdateDirection();
};