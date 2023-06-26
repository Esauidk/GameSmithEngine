#include "gepch.h"
#include "Application.h"

#include "Log.h"

namespace ProjectGE {
	Application* Application::s_Instance = nullptr;;

	Application::Application() {
		GE_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		m_Timer = std::unique_ptr<Timer>(new Timer());

		ProjectGE::WindowProps props;
		props.renderOption = RenderOptions::DIRECTX12;

		m_Window = std::unique_ptr<Window>(Window::Create(props));

		const std::vector<EventDispatcherBase*> dispatchers = m_Window->GetDistpachers();
		for (auto dispatcher : dispatchers) {
			
			bool reg = RegisterEvent<WindowCloseEvent>(dispatcher, GE_BIND_EVENT_FN(Application, OnWindowClose), false);

			if (reg) {
				continue;
			}
		}
	}

	Application::~Application() {
	
	}

	bool Application::OnWindowClose(WindowCloseEvent& evn) {
		m_Running = false;
		return true;
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.Push(layer);
		layer->EventSubscribe(m_Window->GetDistpachers(), false);
	}

	void Application::PushOverlay(Layer* layer) {
		m_LayerStack.PushSpecial(layer);
		layer->EventSubscribe(m_Window->GetDistpachers(), true);
	}

	void Application::Execute() {

		while (m_Running) {
			m_Window->OnPreUpdate();
			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}
			m_Window->OnUpdate();
		}
	}

	
};
