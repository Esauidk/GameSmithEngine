#include "gepch.h"
#include "Camera.h"

namespace ProjectGE {
	Camera::Camera() {
		UpdateMatrix();
	}

	void Camera::UpdateMatrix()
	{
		glm::mat4 transform = m_Transform.GetModelMatrix();
		m_View = glm::inverse(transform);
		// Convert from Collumn Major to Row Major
		m_VP = glm::transpose(m_Projection * m_View);
	}
};