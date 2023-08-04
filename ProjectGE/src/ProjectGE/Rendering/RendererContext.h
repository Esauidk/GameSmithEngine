#pragma once

namespace ProjectGE {
	// A RendererContext System to render 3D/2D or any graphical piece in an application
	class RendererContext {
	public:
		// Initialize: Any initialization to be done with this renderer
		virtual void Init() = 0;
 
		virtual void Swap() = 0;

		virtual void Resize(float width, float height) = 0;

		virtual void SetClearColor(float r, float g, float b, float a) = 0;
		virtual void AttachContextResources() = 0;
	};

};
