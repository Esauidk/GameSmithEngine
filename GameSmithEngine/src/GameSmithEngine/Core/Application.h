#pragma once

#include "Core.h"
#include "Window.h"
#include "Timer.h"

#include "GameSmithEngine/Events/Event.h"
#include "GameSmithEngine/Core/MixStack.h"
#include "GameSmithEngine/Core/Layer.h"
#include "GameSmithEngine/Core/Timer.h"
#include "GameSmithEngine/Core/FrameRateController.h"

#include "GameSmithEngine/ImGui/ImGuiLayer.h"



namespace GameSmith {
	struct CommandLineArgs {
		int Count = 0;
		char** Args = nullptr;
	};

	struct ApplicationSpecs {
		std::string Name = "Game Smith Engine";
		CommandLineArgs CommandLineArgs;
	};

	// The class is the abstract representation of the Engine's structure
	// Ex: Layers(Submodules), Execution Loop, OS Window, etc
	class GE_API Application
	{
	public:
		Application(ApplicationSpecs& appSpecs);
		virtual ~Application();

		// Add a layer into the Engine Structure
		void PushLayer(Layer* layer);
		// Add a layer as an overlay in the Engine Structure
		void PushOverlay(Layer* layer);

		// Start executing the game engine (will block until the engine has been dismissed/shutdown)
		void Execute();

		inline static Application& Get() { return *s_Instance; }
		inline Window* GetWindow() { return m_Window.get();}
		inline Timer& GetTimer() { return m_Timer; }
		inline ImGuiLayer* GetImGuiInstance() { return m_ImGuiLayer; }
		inline const ApplicationSpecs& GetApplicationSpecs() const { return m_AppSpecs; }
	private:
		bool OnWindowClose(WindowCloseEvent& evn);
	private:
		ApplicationSpecs m_AppSpecs;

		bool m_Running = true;
		bool m_LoopStarted = false;
		Timer m_Timer;
		FrameRateController m_FrameRateController;;

		Scope<Window> m_Window;
		std::queue<Layer*> m_PendingLayers;
		std::queue<Layer*> m_PendingSpecialLayers;
		ImGuiLayer* m_ImGuiLayer;
		MixStack<Layer> m_LayerStack;


		static Application* s_Instance;
	};

	// Needs to be defined by CLIENT
	// This will tell the engine which application to use during the entry point phase
	Application* CreateApplication(CommandLineArgs cmdArgs);
};


