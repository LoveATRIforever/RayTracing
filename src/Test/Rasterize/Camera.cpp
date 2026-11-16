#pragma once
#include"Camera.h"
#include"glm/gtc/matrix_transform.hpp"

RenderCamera LogicCamera::GetRenderCameraPerspective(PerspectiveCamera camera)
{
	RenderCamera renderCamera;
	renderCamera.ViewPosition = camera.Pos;
	renderCamera.View = glm::lookAtRH(camera.Pos, camera.Pos + camera.Front, STD_DIR_UP);
	renderCamera.Projection = glm::perspectiveRH_NO(glm::radians(camera.Fov), camera.AspectRatio, camera.Near, camera.Far);
	return renderCamera;
}

RenderCamera LogicCamera::GetRenderCameraOrtho(OrthoCamera camera)
{
	RenderCamera renderCamera;
	renderCamera.ViewPosition = camera.Pos;
	renderCamera.View = glm::lookAtRH(camera.Pos, camera.Pos + camera.Front, STD_DIR_UP);
	float halfWidth = camera.Width / 2;
	float halfHeight = camera.Height / 2;
	renderCamera.Projection = glm::orthoRH_NO(-halfWidth, halfWidth, -halfHeight, halfHeight, camera.Near, camera.Far);
	return renderCamera;
}


//正交和投影相机的相互转换

OrthoCamera LogicCamera::Switch(PerspectiveCamera camera)
{
	OrthoCamera orthoCamera;

	return orthoCamera;
}
PerspectiveCamera LogicCamera::Switch(OrthoCamera camera)
{
	PerspectiveCamera perspectiveCamera;

	return perspectiveCamera;
}

EditorCamera::EditorCamera(CameraType type)
	:m_Type(type)
{
}

void EditorCamera::SetCameraType(CameraType type, bool enableSwitch)
{
	if (type == m_Type)
		return;
	if (enableSwitch)
	{
		if (type == CameraType::ePerspective)
			m_PerspectiveData = m_LogicCamera.Switch(m_OrthoData);
		else
			m_OrthoData = m_LogicCamera.Switch(m_PerspectiveData);
	}
	m_Type = type;
}

RenderCamera EditorCamera::GetRenderCamera()
{
	if (m_Type == CameraType::ePerspective)
	{
		m_PerspectiveData.Pos = m_Pos;
		m_PerspectiveData.Front = m_Front;
		return m_LogicCamera.GetRenderCameraPerspective(m_PerspectiveData);
	}
	else
	{
		m_OrthoData.Pos = m_Pos;
		m_OrthoData.Front = m_Front;
		return m_LogicCamera.GetRenderCameraOrtho(m_OrthoData);
	}
}

void EditorCamera::Update(const glm::vec3& move, const glm::vec2 rotate)
{
	//右手系
	m_Pos += (m_Right * move.x + m_Up * move.y + m_Front * move.z);
	if (rotate.x != 0 || rotate.y != 0)
	{
		glm::vec2 alpha(-rotate.x / 2, -rotate.y / 2);
		glm::quat qX = { glm::cos(alpha.x),0,glm::sin(alpha.x),0 };
		glm::quat qY = { glm::cos(alpha.y),glm::sin(alpha.y),0,0 };
		m_Rotate = glm::normalize(qX * m_Rotate * qY);
		UpdateDirection();
	}
}

void EditorCamera::UpdateDirection()
{
	glm::quat tmp = { m_Rotate.w,-m_Rotate.x,-m_Rotate.y,-m_Rotate.z };
	glm::quat res = m_Rotate * STD_ROTATION * tmp;
	m_Front = { res.x,res.y,res.z };
	m_Front = glm::normalize(m_Front);
	m_Right = glm::cross(m_Front, STD_DIR_UP);
	m_Right = glm::normalize(m_Right);
	m_Up = glm::cross(m_Right, m_Front);
	m_Up = glm::normalize(m_Up);
}

void EditorCamera::SetViewportSize(uint32_t width, uint32_t height)
{
	//更新透视
	m_PerspectiveData.AspectRatio = (float)width / (float)height;
	//更新正交
}

void EditorCamera::Reset()
{
	m_Pos = glm::vec3{ 0 };
	m_Front = STD_DIR_FRONT;
	m_Right = STD_DIR_RIGHT;
	m_Up = STD_DIR_UP;
	m_Rotate = STD_ROTATION;
}
