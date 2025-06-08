#pragma once

#ifdef GE_PLATFORM_WINDOWS

extern GameSmith::Application* GameSmith::CreateApplication(GameSmith::CommandLineArgs cmdArgs);

int main(int argc, char** argv) {

	// Testing Logging
	GameSmith::Log::Init();
	GE_CORE_INFO("Initialized Log!");

	GameSmith::Application* app = GameSmith::CreateApplication({argc, argv});
	app->Execute();
	delete app;
}
#else
	#error ONLY WINDOWS IS SUPPORTED!
#endif
