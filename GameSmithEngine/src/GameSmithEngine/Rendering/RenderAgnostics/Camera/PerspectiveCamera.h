#pragma once
#include "Camera.h"
#include <GameSmithEngine/Core/Core.h>

namespace GameSmith {
	class GE_API PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float width, float height, float FOV = glm::radians(60.0f), float zNear = 0.2f, float zFar = 500.0f);
		PerspectiveCamera(float FOV = glm::radians(60.0f), float zNear = 0.2f, float zFar = 500.0f);
		void SetWidth(float width);
		void SetHeight(float height);
		void SetFOV(float fov);
	private:
		void UpdateProjectionMatrix();
	private:
		float m_FOV;
		float m_Width;
		float m_Height;
		float m_ZNear;
		float m_ZFar;
	};
};


