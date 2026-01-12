#include "gepch.h"
#include "Application.h"

#include "Log.h"


#include "GameSmithEngine/Core/Layers/EntitySystemLayer.h"
#include "GameSmithEngine/Core/Layers/RenderLayer.h"
#include "GameSmithEngine/Core/Layers/ResourceLayer.h"
#include "GameSmithEngine/EngineConfiguration/ConfigManager.h"


#include "GameSmithEngine/Core/JobSystem/JobManager.h"


namespace GameSmith {
	Application* Application::s_Instance = nullptr;

	Application::Application(ApplicationSpecs& appSpecs) : m_AppSpecs(appSpecs) {
		GE_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		ConfigManager::Init();

		// TODO Setup default layers
		PushLayer(new ResourceLayer()); // It is important that ResourceLayer is the first layer pushed as other layers may depend on it
		PushLayer(new RenderLayer());
		PushLayer(new EntitySystemLayer());

		GameSmith::WindowProps props;
		props.Title = appSpecs.Name;

		m_Window = std::unique_ptr<Window>(Window::Create(props));
		RegisterEvent<WindowCloseEvent>(&Window::s_Close, GE_BIND_EVENT_FN(Application::OnWindowClose, this), false);
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_Window->SetVSync(true);

		m_FrameRateController.SetFrameRateLimiting(!m_Window->IsVSync());

		JobManager::Init();
	}

	Application::~Application()
	{
		for (Layer* layer : m_LayerStack) {
			layer->OnDetach();
		}

		ConfigManager::Shutdown();
	}

	bool Application::OnWindowClose(WindowCloseEvent& evn) {
		m_Running = false;
		return true;
	}

	void Application::PushLayer(Layer* layer) {
		if (m_LoopStarted) {
			m_PendingAddLayers.push(layer);
		}
		else {
			layer->OnAttach(m_AppSpecs);
			m_LayerStack.Push(layer);
		}
		
	}

	void Application::PushOverlay(Layer* layer) {
		if (m_LoopStarted) {
			m_PendingAddSpecialLayers.push(layer);
		}
		else {
			layer->OnAttach(m_AppSpecs);
			m_LayerStack.PushSpecial(layer);
		}
		
	}

	void Application::PopLayer(Layer* layer)
	{
		if (m_LoopStarted) {
			m_PendingRemoveLayers.push(layer);
		}
		else {
			m_LayerStack.Pop(layer);
			layer->OnDetach();
			delete layer;
		}
	}

	void Application::Execute() {
		m_Timer.Reset();
		while (m_Running) {
			if (!m_LoopStarted) {
				m_LoopStarted = true;
			}

			m_FrameRateController.CheckAndPerformDelay();
			float dt = m_Timer.Mark();

			m_Window->OnUpdate();

			while (!m_PendingAddSpecialLayers.empty()) {
				auto layer = m_PendingAddSpecialLayers.front();
				m_PendingAddSpecialLayers.pop();
				layer->OnAttach(m_AppSpecs);
				m_LayerStack.PushSpecial(layer);
			}

			while (!m_PendingAddLayers.empty()) {
				auto layer = m_PendingAddLayers.front();
				m_PendingAddLayers.pop();
				layer->OnAttach(m_AppSpecs);
				m_LayerStack.Push(layer);
			}

			while (!m_PendingRemoveLayers.empty()) {
				auto layer = m_PendingRemoveLayers.front();
				m_PendingRemoveLayers.pop();
				m_LayerStack.Pop(layer);
				layer->OnDetach();
				delete layer;
			}

			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate(dt);
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();
		}
	}

	
};
