#include "gepch.h"
#include "Transform.h"

Transform::Transform() : m_Position(0,0,0), m_Rotation(0,0,0), m_Scale(1, 1, 1)
{
	UpdateMatrix();
}

void Transform::UpdateMatrix()
{
	m_ModelMatrix = glm::translate(glm::mat4(1.0f), m_Position) * 
		glm::rotate(glm::mat4(1.0f), m_Rotation.x, glm::vec3(1, 0, 0)) *
		glm::rotate(glm::mat4(1.0f), m_Rotation.y, glm::vec3(0, 1, 0)) *
		glm::rotate(glm::mat4(1.0f), m_Rotation.z, glm::vec3(0, 0, 1)) * 
		glm::scale(glm::mat4(1.0f), m_Scale);
}
