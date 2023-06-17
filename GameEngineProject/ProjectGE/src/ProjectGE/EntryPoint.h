#pragma once

#ifdef GE_PLATFORM_WINDOWS

extern ProjectGE::Application* ProjectGE::CreateApplication();



int main(int argc, char** argv) {

	ProjectGE::Log::Init();
	GE_CORE_WARN("Initialized Log!");
	int a = 255;
	GE_APP_INFO("Hello! Var={0}", a);

	ProjectGE::Application* app = ProjectGE::CreateApplication();
	app->Execute();
	delete app;
}

#else
	#error ONLY WINDOWS IS SUPPORTED!
#endif
