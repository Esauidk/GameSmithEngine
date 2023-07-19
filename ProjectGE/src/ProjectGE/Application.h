#pragma once

#include "Core.h"
#include "Window.h"
#include "Timer.h"

#include "ProjectGE/Events/Event.h"
#include "ProjectGE/MixStack.h"
#include "ProjectGE/Layer.h"

#include "ProjectGE/ImGui/ImGuiLayer.h"

namespace ProjectGE{
	class GE_API Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void Execute();

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window;}
		inline Timer& GetTimer() { return *m_Timer; }
	private:
		bool OnWindowClose(WindowCloseEvent& evn);
		
		bool m_Running = true;
		std::unique_ptr<Window> m_Window;
		std::unique_ptr<Timer> m_Timer;
		ImGuiLayer* m_ImGuiLayer;
		MixStack<Layer> m_LayerStack;


		static Application* s_Instance;
	};

	// Needs to be defined by CLIENT
	// This will tell the engine which application to use during the entry point phase
	Application* CreateApplication();
};


