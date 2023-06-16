#pragma once

#ifdef GE_PLATFORM_WINDOWS

extern ProjectGE::Application* ProjectGE::CreateApplication();

int main(int argc, char** argv) {
	ProjectGE::Application* app = ProjectGE::CreateApplication();
	app->Execute();
	delete app;
}

#else
	#error ONLY WINDOWS IS SUPPORTED!
#endif
