#pragma once
#include "Window.h"
class RenderApplication
{
public:
	RenderApplication();
	~RenderApplication();

	int Execute();
private:
	Window wnd;
};

