#include "gepch.h"
#include "PerspectiveCamera.h"

namespace GameSmith {
	PerspectiveCamera::PerspectiveCamera(float FOV, float width, float height, float zNear, float zFar) : Camera()
	{
		m_FOV = FOV;
		m_Width = width;
		m_Height = height;
		m_ZNear = zNear;
		m_ZFar = zFar;
		
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

	void PerspectiveCamera::UpdateProjectionMatrix()
	{
		m_Projection = glm::transpose(glm::perspectiveFovLH(m_FOV, m_Width, m_Height, m_ZNear, m_ZFar));
		UpdateMatrix();
	}
};
