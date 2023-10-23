#pragma once
#include "Camera.h"

namespace ProjectGE {
	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float FOV, float width, float height, float zNear = 1.0f, float zFar = 1000.0f);
	};
};


