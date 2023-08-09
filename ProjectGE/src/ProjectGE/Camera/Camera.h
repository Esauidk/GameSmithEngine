#pragma once
#include <glm/gtc/matrix_transform.hpp>
namespace ProjectGE {

	// An abstract representation of a Camera in 3D space
	// Takes care of computing transformation matricies related to a camera (Model-View-Projection Matrix)

	// Note: Camera base class does not compute any special projection matrix, inheritors hold the responsibility of setting the projection matrix
	class Camera
	{
	public:
		Camera();
		inline void SetPosition(const glm::vec3& position) { m_Position = position; UpdateMatrix(); }
		inline glm::vec3 GetPosition() const { return m_Position;}
		inline void SetRotation(float rotation) { m_Rotation = rotation; UpdateMatrix(); }
		inline float GetRotation() const { return m_Rotation; }
		inline glm::mat4 GetMatrix() const { return m_VP; }
	protected:
		void UpdateMatrix();

		glm::mat4 m_Projection;
	private:
		glm::mat4 m_VP;
		glm::mat4 m_View;
		glm::vec3 m_Position;
		float m_Rotation;
	};
};


