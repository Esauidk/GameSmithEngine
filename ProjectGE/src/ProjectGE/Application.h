#pragma once

#include "Core.h"
#include "Window.h"
#include "Timer.h"

#include "ProjectGE/Events/Event.h"
#include "ProjectGE/MixStack.h"
#include "ProjectGE/Layer.h"

#include "ProjectGE/ImGui/ImGuiLayer.h"

// Temporary
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/VertexBuffer.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/IndexBuffer.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/ShaderArguementDefiner.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/Shader.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/PipelineStateObject.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/Topology.h"

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


		std::unique_ptr<PipelineStateObject> m_State;
		std::unique_ptr<ShaderArguementDefiner> m_Root;
		std::unique_ptr<Topology> m_Topo;
		std::unique_ptr<Shader> m_VS;
		std::unique_ptr<Shader> m_PS;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
	};

	// Needs to be defined by CLIENT
	// This will tell the engine which application to use during the entry point phase
	Application* CreateApplication();
};


