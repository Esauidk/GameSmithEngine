#pragma once
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/Texture.h"

namespace GameSmith {
	// Represents a Render Display (swap chain or other architecture) along with metadata related to the display
	class RendererContext {
	public:
		virtual ~RendererContext() = default;
		// Initialize: Any initialization to be done with this renderer
		virtual void Init() = 0;
		
		// Swap the frame buffers currently being displayed
		virtual bool Swap() = 0;

		// Resize the Render Display
		virtual void Resize(float width, float height, bool useForRender) = 0;

		virtual void ResizeRenderSpace(float posX, float posY, float width, float height) = 0;
		
		// Change the synchronization settings with the display refresh rate
		virtual void SetVsync(bool vsync) = 0;

		// Change the clear color when the frame buffers get cleared
		virtual void SetClearColor(float r, float g, float b, float a) = 0;
	};

};
