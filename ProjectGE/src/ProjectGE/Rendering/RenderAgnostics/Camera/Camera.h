#pragma once
#include "ProjectGE/EntitySystem/Components/Transform.h"
namespace ProjectGE {

	// An abstract representation of a Camera in 3D space
	// Takes care of computing transformation matricies related to a camera (View-Projection Matrix)

	// Note: Camera base class does not compute any special projection matrix, inheritors hold the responsibility of setting the projection matrix
	class Camera
	{
	public:
		Camera();
		inline Transform& GetTransform() { return m_Transform; }
		inline glm::mat4 GetMatrix() { UpdateMatrix();  return m_VP; }
	protected:
		void UpdateMatrix();
	protected:
		glm::mat4 m_Projection;
	private:
		glm::mat4 m_VP;
		glm::mat4 m_View;
		Transform m_Transform;
	};
};


