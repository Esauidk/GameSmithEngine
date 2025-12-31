#include "gepch.h"
#include "Application.h"

#include "Log.h"


#include "GameSmithEngine/Core/Layers/EntitySystemLayer.h"
#include "GameSmithEngine/Core/Layers/RenderLayer.h"
#include "GameSmithEngine/Core/Layers/ResourceLayer.h"


#include "GameSmithEngine/Core/JobSystem/JobManager.h"


namespace GameSmith {
	Application* Application::s_Instance = nullptr;

	Application::Application(ApplicationSpecs& appSpecs) : m_AppSpecs(appSpecs) {
		GE_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		// TODO Setup default layers
		PushLayer(new ResourceLayer());
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
	}

	bool Application::OnWindowClose(WindowCloseEvent& evn) {
		m_Running = false;
		return true;
	}

	void Application::PushLayer(Layer* layer) {
		if (m_LoopStarted) {
			m_PendingLayers.push(layer);
		}
		else {
			layer->OnAttach();
			m_LayerStack.Push(layer);
		}
		
	}

	void Application::PushOverlay(Layer* layer) {
		if (m_LoopStarted) {
			m_PendingSpecialLayers.push(layer);
		}
		else {
			layer->OnAttach();
			m_LayerStack.PushSpecial(layer);
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

			while (!m_PendingSpecialLayers.empty()) {
				auto layer = m_PendingSpecialLayers.front();
				m_PendingSpecialLayers.pop();
				layer->OnAttach();
				m_LayerStack.PushSpecial(layer);
			}

			while (!m_PendingLayers.empty()) {
				auto layer = m_PendingLayers.front();
				m_PendingLayers.pop();
				layer->OnAttach();
				m_LayerStack.Push(layer);
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
