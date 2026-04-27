#pragma once

#include "Camera.h"
#include <GameSmithEngine/Core/Core.h>

namespace GameSmith {
	// This class representes an Orthographic Camera (Orthographics Projection)
	class GE_API OrthoCamera : public Camera
	{
	public:
		OrthoCamera(float left = 0.0f, float right = 0.0f, float bottom = 1.0f, float top = 1.0f, float zNear = -1000.0f, float zFar = 1000.0f);
	};
};


