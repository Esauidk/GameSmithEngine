#pragma once

namespace ProjectGE {

	struct RenderInitialData {
		std::string debugName;
	};

	enum class RenderOptions {
		NONE,
		DIRECTX12
	};

	// A Renderer System to render 3D/2D or any graphical piece in an application
	class Renderer {
	public:
		// Initialize: Any initialization to be done with this renderer
		virtual void Init() = 0;

		virtual void Swap() = 0;

		virtual void Resize(float width, float height) = 0;

		// Create an object in the scene
		virtual void CreateObject() = 0;

		// Draws an object to the screen (TODO: Add parameters to fully define)
		virtual void DrawObject() = 0;
	};

};
