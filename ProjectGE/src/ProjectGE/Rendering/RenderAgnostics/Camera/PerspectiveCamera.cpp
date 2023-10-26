#include "gepch.h"
#include "PerspectiveCamera.h"

namespace ProjectGE {
	PerspectiveCamera::PerspectiveCamera(float FOV, float width, float height, float zNear, float zFar) : Camera()
	{
		m_Projection = glm::transpose(glm::perspectiveFovLH(FOV, width, height, zNear, zFar));
		UpdateMatrix();
	}
};
