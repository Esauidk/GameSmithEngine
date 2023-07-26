#pragma once

#include "ProjectGE/Rendering/Renderer.h"

namespace ProjectGE {
	class TriangleDemo {
	public:
		virtual void Draw(Renderer* renderer) = 0;
	};
}
