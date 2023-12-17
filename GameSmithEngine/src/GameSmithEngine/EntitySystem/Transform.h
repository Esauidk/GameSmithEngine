#pragma once
#include "GameSmithEngine/ThirdPartySetups/GLM/GLMDefines.h"
#include <glm/gtc/matrix_transform.hpp>

namespace GameSmith {
	class Transform
	{
	public:
		Transform();
		inline bool HasChanged() { return m_ModelMatrix != m_LastMatrix; }
		inline void SetPosition(glm::vec3 newPos) { m_Position = newPos; UpdateMatrix(); }
		inline glm::vec3 GetPosition() const { return m_Position; }
		inline void SetRotation(glm::vec3 newRotation) { m_Rotation = newRotation; UpdateMatrix(); }
		inline glm::vec3 GetRotation() const { return m_Rotation; }
		inline void SetScale(glm::vec3 newScale) { m_Scale = newScale; UpdateMatrix(); }
		inline glm::vec3 GetScale() const { return m_Scale; }

		inline glm::mat4 GetModelMatrix() const { return glm::transpose(m_ModelMatrix); }
	private:
		void UpdateMatrix();
	private:
		bool m_Changed;

		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;
		glm::mat4 m_ModelMatrix;

		glm::mat4 m_LastMatrix;
	};
};


