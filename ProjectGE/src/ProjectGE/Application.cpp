#include "gepch.h"
#include "Application.h"
#include "Events/MouseEvents.h"
#include "Events/ApplicationEvents.h"
#include "Log.h"

namespace ProjectGE {
	Application::Application() {
	}

	Application::~Application() {
	
	}
	 // NOTE: TESTING CODE, REMOVE LATER
	bool eventFired(MouseMoveEvent& env) {
		GE_APP_INFO(env);
		return false;
	}

	bool eventFiredStop(MouseMoveEvent& env) {
		GE_APP_INFO(env);
		return true;
	}

	bool renderFired(AppRenderEvent& env) {
		GE_APP_ERROR(env);
		return false;
	}

	bool renderFiredStop(AppRenderEvent& env) {
		GE_APP_ERROR(env);
		return true;
	}

	void Application::Execute() {
		// TESTING CODE
		MouseMoveEvent e(1.2f, 5.2f);
		AppRenderEvent ap;
		GE_APP_TRACE(e);
		GE_APP_TRACE(ap);

		EventDispatcher<MouseMoveEvent> mouseDispatcher;
		EventDispatcher<AppRenderEvent> appRenderDispatcher;

		mouseDispatcher.AddListener(eventFired);
		mouseDispatcher.AddListener(eventFired);
		mouseDispatcher += eventFired;
		mouseDispatcher += eventFiredStop;
		mouseDispatcher += eventFired;

		appRenderDispatcher.AddListener(renderFired);
		appRenderDispatcher.AddListener(renderFired);
		appRenderDispatcher += renderFired;
		appRenderDispatcher += renderFiredStop;
		appRenderDispatcher += renderFired;

		mouseDispatcher.Dispatch(e);
		appRenderDispatcher.Dispatch(ap);

		// END OF TESTING

		while (true);
	}

	
};
