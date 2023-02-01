#pragma once
#include "Window.h"
#include "Renderers.h"
#include "Timer.h"

class RenderApplication
{
public:
	RenderApplication(RendererOptions renderType = RendererOptions::DirectX);
	~RenderApplication();

	int Execute();
private:
	void Tick();
private:
	Window wnd;
	Timer timer;
	std::unique_ptr<Renderer> renderer;

};

