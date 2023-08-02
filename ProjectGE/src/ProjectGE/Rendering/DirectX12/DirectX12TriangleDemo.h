#pragma once

#include "ProjectGE/Rendering/RenderAgnostics/TriangleDemo.h"

namespace ProjectGE {
	class DirectX12TriangleDemo : public TriangleDemo {
	public:
		DirectX12TriangleDemo();
		void Draw(RendererContext* renderer) override;
	};
};


