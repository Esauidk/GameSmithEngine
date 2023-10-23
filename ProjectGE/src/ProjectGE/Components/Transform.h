#pragma once
#include "ProjectGE/ThirdPartySetups/GLM/GLMDefines.h"
#include <glm/gtc/matrix_transform.hpp>
class Transform
{
public:
	Transform();
	inline void SetPosition(glm::vec3 newPos) { m_Position = newPos; UpdateMatrix(); }
	inline glm::vec3 GetPosition() const { return m_Position; }
	inline void SetRotation(glm::vec3 newRotation) { m_Rotation = newRotation; UpdateMatrix(); }
	inline glm::vec3 GetRotation() const { return m_Rotation; }
	inline void SetScale(glm::vec3 newScale) { m_Scale = newScale; UpdateMatrix(); }
	inline glm::vec3 GetScale() const { return m_Scale; }

	inline glm::mat4 GetModelMatrix() const { return m_ModelMatrix; }
private:
	void UpdateMatrix();
private:
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;
	glm::mat4 m_ModelMatrix;
};

