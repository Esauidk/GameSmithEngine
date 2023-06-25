#include "gepch.h"
#include "Application.h"

#include "Log.h"

namespace ProjectGE {
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;;

	Application::Application() {
		GE_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		ProjectGE::WindowProps props;
		props.renderOption = RenderOptions::DIRECTX12;

		m_Window = std::unique_ptr<Window>(Window::Create(props));

		const std::vector<EventDispatcherBase*> dispatchers = m_Window->GetDistpachers();
		for (auto dispatcher : dispatchers) {
			EventDispatcher<WindowCloseEvent>* close_event;

			if (EVENT_CAST(WindowCloseEvent, dispatcher, close_event)) {
				close_event->AddListener(BIND_EVENT_FN(OnWindowClose));
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
