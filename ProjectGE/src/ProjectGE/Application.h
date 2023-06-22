#pragma once

#include "Core.h"
#include "Window.h"

#include "ProjectGE/Events/Event.h"
#include "ProjectGE/MixStack.h"
#include "ProjectGE/Layer.h"

namespace ProjectGE{
	class GE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void Execute();
	private:
		bool OnWindowClose(WindowCloseEvent& evn);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		MixStack<Layer> m_LayerStack;
	};

	// Needs to be defined by CLIENT
	// This will tell the engine which application to use during the entry point phase
	Application* CreateApplication();
};


