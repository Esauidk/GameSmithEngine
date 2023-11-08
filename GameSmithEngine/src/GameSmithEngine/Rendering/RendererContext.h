#pragma once

namespace GameSmith {
	// Represents a Render Display (swap chain or other architecture) along with metadata related to the display
	class RendererContext {
	public:
		// Initialize: Any initialization to be done with this renderer
		virtual void Init() = 0;
		
		// Swap the frame buffers currently being displayed
		virtual void Swap() = 0;

		// Resize the Render Display
		virtual void Resize(float width, float height) = 0;
		
		// Change the synchronization settings with the display refresh rate
		virtual void SetVsync(bool vsync) = 0;

		// Change the clear color when the frame buffers get cleared
		virtual void SetClearColor(float r, float g, float b, float a) = 0;

		// Attach any display related resources to the pipeline
		virtual void AttachContextResources() = 0;
	};

};
