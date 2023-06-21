#include "gepch.h"
#include "Application.h"


#include "Events/ApplicationEvents.h"
#include "Log.h"

namespace ProjectGE {
	Application::Application() {
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application() {
	
	}

	void Application::Execute() {

		while (m_Running) {
			m_Window->OnUpdate();
		}
	}

	
};
