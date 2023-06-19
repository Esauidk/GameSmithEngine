#include "Application.h"
#include "Events/MouseEvent.h"
#include "Log.h"

namespace ProjectGE {
	Application::Application() {
	}

	Application::~Application() {
	
	}
	 // NOTE: TESTING CODE, REMOVE LATER
	bool eventFired(MouseMoveEvent& env) {
		GE_APP_WARN(env);
		return true;
	}

	void Application::Execute() {
		// TESTING CODE
		MouseMoveEvent e(1.2f, 5.2f);
		GE_APP_TRACE(e);

		EventDispatcher<MouseMoveEvent> dispatcher;
		dispatcher.AddListener(eventFired);
		dispatcher.Dispatch(e);
		// END OF TESTING

		while (true);
	}

	
};
