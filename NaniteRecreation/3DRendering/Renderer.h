#pragma once
#include "PreWin.h"
// A Renderer System to render 3D/2D or any graphical piece in an application
class Renderer {
public:
	// Initialize: Any initialization to be done with this renderer
	//	hWnd: The application window/GUI

	// Returns a bool for successful initialization
	virtual bool Initialize(HWND hWnd) = 0;

	// Render: This is called to render a frame in the application
	virtual void Render() = 0;
};