#pragma once
#include "Camera.h"

namespace GameSmith {
	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float FOV, float width, float height, float zNear = 0.2f, float zFar = 500.0f);
		void SetWidth(float width);
		void SetHeight(float height);
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


