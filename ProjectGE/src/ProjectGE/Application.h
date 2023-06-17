#pragma once

#include "Core.h"
namespace ProjectGE{
	class GE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Execute();
	};

	// Needs to be defined by CLIENT
	// This will tell the engine which application to use during the entry point phase
	Application* CreateApplication();
};


