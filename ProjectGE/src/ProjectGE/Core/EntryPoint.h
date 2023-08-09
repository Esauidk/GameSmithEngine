#pragma once

#ifdef GE_PLATFORM_WINDOWS

extern ProjectGE::Application* ProjectGE::CreateApplication();



int main(int argc, char** argv) {

	// Testing Logging
	ProjectGE::Log::Init();
	GE_CORE_INFO("Initialized Log!");

	ProjectGE::Application* app = ProjectGE::CreateApplication();
	app->Execute();
	delete app;
}
#else
	#error ONLY WINDOWS IS SUPPORTED!
#endif
