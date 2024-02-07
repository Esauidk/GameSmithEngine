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
	}

	bool Application::OnWindowClose(WindowCloseEvent& evn) {
		m_Running = false;
		return true;
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.Push(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) {
		m_LayerStack.PushSpecial(layer);
		layer->OnAttach();
	}

	void Application::Execute() {
		m_Timer.Reset();
		while (m_Running) {
			GE_CORE_INFO("Staring update iteration");
			m_Timer.Mark();
			m_Window->OnUpdate();

			m_SubSystems.Update();
			GE_CORE_INFO("Updaing Layers");
			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			GE_CORE_INFO("Rendering Layers");
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_SubSystems.EndUpdate();
			GE_CORE_INFO("Ending update iteration");
		}
	}

	
};
