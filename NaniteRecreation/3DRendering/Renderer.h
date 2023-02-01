#pragma once
#include "PreWin.h"
// A Renderer System to render 3D/2D or any graphical piece in an application
class Renderer {
public:
	// Initialize: Any initialization to be done with this renderer
	//	hWnd: The application window/GUI

	// Returns a bool for successful initialization
	virtual bool Initialize(HWND hWnd) = 0;

	// StartFrame: This is called to start the render of a frame in the application
	// EndFrame: This is called to end the rendering of a frame and usual present it to the screen
	virtual void StartFrame() = 0;
	virtual void EndFrame() = 0;
};