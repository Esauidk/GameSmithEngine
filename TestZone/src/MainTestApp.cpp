#include <ProjectGE.h>

bool printTest(ProjectGE::KeyPressedEvent& test) {
	if (test.GetKeyCode() == GE_KEY_TAB) {
		GE_APP_TRACE("Tab key is pressed (event)");
	}

	return false;
}

class ExampleLayer : public ProjectGE::Layer {
public:

	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override {
		if (ProjectGE::Input::IsKeyPressed(GE_KEY_TAB)) {
			GE_APP_TRACE("Tab Key is pressed (polled)");
		}
		
	}

	void EventSubscribe(std::vector<ProjectGE::EventDispatcherBase*> dispatchers, bool overlay) override {
		for (auto dispatcher : dispatchers) {

			bool reg = RegisterEvent<ProjectGE::KeyPressedEvent>(dispatcher, printTest, false);

			if (reg) {
				continue;
			}
		}
	}

	
};
class TestZone : public ProjectGE::Application {
public:
	TestZone() {
		PushLayer(new ExampleLayer());
		PushLayer(new ProjectGE::ImGuiLayer());
	}
	~TestZone() {

	}
};

ProjectGE::Application* ProjectGE::CreateApplication() {
	return new TestZone();
}
