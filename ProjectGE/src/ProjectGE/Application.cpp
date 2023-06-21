#include "gepch.h"
#include "Application.h"

#include "Log.h"

namespace ProjectGE {
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)


	bool testPrint(Event& evn) {
		GE_APP_INFO(evn);

		return true;
	}


	Application::Application() {
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->GetDispatcherClose().AddListener(BIND_EVENT_FN(OnWindowClose));

		// Testing
		m_Window->GetDispatcherMouseMove() += testPrint;
		m_Window->GetDispatcherMove() += testPrint;
		m_Window->GetDispatcherKeyPress() += testPrint;
		m_Window->GetDispatcherKeyRelease() += testPrint;
		m_Window->GetDispatcherMousePress() += testPrint;
		m_Window->GetDispatcherMouseRelease() += testPrint;
		m_Window->GetDispatcherResize() += testPrint;
		m_Window->GetDispatcherMouseScroll() += testPrint;
		m_Window->GetDispatcherFocus() += testPrint;
		m_Window->GetDispatcherFocusLost() += testPrint;
	}

	Application::~Application() {
	
	}

	bool Application::OnWindowClose(WindowCloseEvent& evn) {
		m_Running = false;
		return true;
	}

	void Application::Execute() {

		while (m_Running) {
			m_Window->OnUpdate();
		}
	}

	
};
