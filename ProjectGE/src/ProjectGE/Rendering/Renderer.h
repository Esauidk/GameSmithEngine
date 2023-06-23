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

		// StartFrame: This is called to start the render of a frame in the application
		// EndFrame: This is called to end the rendering of a frame and present it to the screen
		virtual void StartFrame() = 0;
		virtual void EndFrame() = 0;

		// Create an object in the scene
		virtual void CreateObject() = 0;

		// Draws an object to the screen (TODO: Add parameters to fully define)
		virtual void DrawObject() = 0;
	};

};
