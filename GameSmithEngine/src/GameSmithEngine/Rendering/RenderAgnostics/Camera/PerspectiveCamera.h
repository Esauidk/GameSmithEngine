#pragma once
#include "Camera.h"

namespace GameSmith {
	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float FOV, float width, float height, float zNear = 0.2f, float zFar = 500.0f);
	};
};


