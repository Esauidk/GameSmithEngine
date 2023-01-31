#pragma once
#include "Window.h"
#include "Renderers.h"

class RenderApplication
{
public:
	RenderApplication(RendererOptions renderType = RendererOptions::DirectX);
	~RenderApplication();

	int Execute();
private:
	Window wnd;
	std::unique_ptr<Renderer> renderer;

};

