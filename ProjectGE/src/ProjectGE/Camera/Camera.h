#pragma once
#include <glm/gtc/matrix_transform.hpp>
namespace ProjectGE {

	class Camera
	{
	public:
		Camera();
		inline void SetPosition(glm::vec3& position) { m_Position = position; UpdateMatrix(); }
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


