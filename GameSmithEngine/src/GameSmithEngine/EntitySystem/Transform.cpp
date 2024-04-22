#include "gepch.h"
#include "Transform.h"

namespace GameSmith {
	Transform::Transform() : m_Position(0, 0, 0), m_Rotation(0, 0, 0), m_Scale(1, 1, 1), m_CoordinateFrame(nullptr)
	{
		UpdateMatrix();
	}

	void Transform::RemoveChildCoordinateFrame(Transform* oldFrame)
	{
		for (auto it = m_ChildrenFrames.begin(); it != m_ChildrenFrames.end(); it++) {
			if (*it == oldFrame) {
				m_ChildrenFrames.erase(it);
				break;
			}
		}
	}

	void Transform::UpdateMatrix()
	{
		glm::mat4 coordFrame = m_CoordinateFrame == nullptr ? glm::mat4(1) : m_CoordinateFrame->m_ModelMatrix;

		m_ModelMatrix = coordFrame * glm::translate(glm::mat4(1.0f), m_Position) *
			glm::scale(glm::mat4(1.0f), m_Scale) *
			glm::rotate(glm::mat4(1.0f), m_Rotation.z, glm::vec3(0, 0, 1)) *
			glm::rotate(glm::mat4(1.0f), m_Rotation.x, glm::vec3(1, 0, 0)) *
			glm::rotate(glm::mat4(1.0f), m_Rotation.y, glm::vec3(0, 1, 0));

		for (auto children : m_ChildrenFrames) {
			children->UpdateMatrix();
		}
	}
};

