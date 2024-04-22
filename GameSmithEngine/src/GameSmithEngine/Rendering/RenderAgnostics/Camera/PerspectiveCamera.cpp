#include "gepch.h"
#include "PerspectiveCamera.h"
#include "GameSmithEngine/Core/Application.h"

namespace GameSmith {
	PerspectiveCamera::PerspectiveCamera(float width, float height, float FOV, float zNear, float zFar) : Camera()
	{
		m_FOV = FOV;
		m_Width = width;
		m_Height = height;
		m_ZNear = zNear;
		m_ZFar = zFar;
		
		UpdateProjectionMatrix();
	}

	PerspectiveCamera::PerspectiveCamera(float FOV, float zNear, float zFar)
	{
		m_FOV = FOV;
		m_ZNear = zNear;
		m_ZFar = zFar;

		auto wnd = Application::Get().GetWindow();
		m_Width = (float)wnd->GetWidth();
		m_Height = (float)wnd->GetHeight();

		UpdateProjectionMatrix();
	}

	void PerspectiveCamera::SetWidth(float width)
	{
		m_Width = width;
		UpdateProjectionMatrix();
	}

	void PerspectiveCamera::SetHeight(float height)
	{
		m_Height = height;
		UpdateProjectionMatrix();
	}

	void PerspectiveCamera::SetFOV(float fov)
	{
		m_FOV = fov;
		UpdateProjectionMatrix();
	}

	void PerspectiveCamera::UpdateProjectionMatrix()
	{
		m_Projection = glm::transpose(glm::perspectiveFovLH(m_FOV, m_Width, m_Height, m_ZNear, m_ZFar));
		UpdateMatrix();
	}
};
