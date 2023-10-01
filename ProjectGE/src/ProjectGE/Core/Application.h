#pragma once

#include "Core.h"
#include "Window.h"
#include "Timer.h"

#include "ProjectGE/Events/Event.h"
#include "ProjectGE/Core/MixStack.h"
#include "ProjectGE/Core/Layer.h"
#include "ProjectGE/Core/Timer.h"
#include "ProjectGE/Core/SubsystemManager.h"

#include "ProjectGE/ImGui/ImGuiLayer.h"



namespace ProjectGE{
	// The class is the abstract representation of the Engine's structure
	// Ex: Layers(Submodules), Execution Loop, OS Window, etc
	class GE_API Application
	{
	public:
		Application();
		virtual ~Application() = default;

		// Add a layer into the Engine Structure
		void PushLayer(Layer* layer);
		// Add a layer as an overlay in the Engine Structure
		void PushOverlay(Layer* layer);

		// Start executing the game engine (will block until the engine has been dismissed/shutdown)
		void Execute();

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window;}
		inline Timer& GetTimer() { return m_Timer; }
	private:
		bool OnWindowClose(WindowCloseEvent& evn);
	private:
		bool m_Running = true;
		Timer m_Timer;
		SubsystemManager m_SubSystems;

		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		MixStack<Layer> m_LayerStack;


		static Application* s_Instance;
	};

	// Needs to be defined by CLIENT
	// This will tell the engine which application to use during the entry point phase
	Application* CreateApplication();
};


