#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

namespace ProjectGE{
	class GE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Execute();
	private:
		bool OnWindowClose(WindowCloseEvent& evn);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// Needs to be defined by CLIENT
	// This will tell the engine which application to use during the entry point phase
	Application* CreateApplication();
};


