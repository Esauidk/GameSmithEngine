#pragma once

#include "Camera.h"

namespace ProjectGE {
	// This class representes an Orthographic Camera (Orthographics Projection)
	class OrthoCamera : public Camera
	{
	public:
		OrthoCamera(float left, float right, float bottom, float top, float zNear = -1000.0f, float zFar = 1000.0f);
	};
};


