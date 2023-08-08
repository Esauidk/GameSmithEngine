#include "gepch.h"
#include "Camera.h"

namespace ProjectGE {
	Camera::Camera() : m_Position({0,0,0}), m_Rotation(0) {
		UpdateMatrix();
	}

	void Camera::UpdateMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0, 0, 1));
		m_View = glm::inverse(transform);
		m_VP = m_Projection * m_View;
	}
};