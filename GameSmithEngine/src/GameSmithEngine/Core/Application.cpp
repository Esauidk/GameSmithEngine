#include "gepch.h"
#include "Application.h"

#include "Log.h"


namespace GameSmith {
	Application* Application::s_Instance = nullptr;

	Application::Application(std::string appName) {
		GE_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		GameSmith::WindowProps props;
		props.Title = appName;

		m_Window = std::unique_ptr<Window>(Window::Create(props));
		RegisterEvent<WindowCloseEvent>(&Window::s_Close, GE_BIND_EVENT_FN(Application::OnWindowClose, this), false);
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_Window->SetVSync(true);

		m_FrameRateController.SetFrameRateLimiting(!m_Window->IsVSync());
	}

	bool Application::OnWindowClose(WindowCloseEvent& evn) {
		m_Running = false;
		return true;
	}

	void Application::PushLayer(Layer* layer) {
		m_PendingLayers.push(layer);
	}

	void Application::PushOverlay(Layer* layer) {
		m_PendingSpecialLayers.push(layer);
	}

	void Application::Execute() {
		m_Timer.Reset();
		while (m_Running) {
			m_FrameRateController.CheckAndPerformDelay();
			float dt = m_Timer.Mark();

			m_Window->OnUpdate();

			m_SubSystems.Update(dt);

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
				layer->OnUpdate();
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_SubSystems.EndUpdate();
		}
	}

	
};
