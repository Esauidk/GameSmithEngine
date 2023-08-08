#pragma once

#include "Camera.h"

namespace ProjectGE {
	class OrthoCamera : public Camera
	{
	public:
		OrthoCamera(float left, float right, float bottom, float top, float zNear = -1000.0f, float zFar = 1000.0f);
	};
};


