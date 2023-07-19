#include <ProjectGE.h>

#include "imgui.h"


bool printTest(ProjectGE::KeyPressedEvent& test) {
	if (test.GetKeyCode() == GE_KEY_TAB) {
		GE_APP_TRACE("Tab key is pressed (event)");
	}

	return false;
}

class ExampleLayer : public ProjectGE::Layer {
public:

	ExampleLayer() : Layer("Example") {}

	void OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void EventSubscribe(const std::vector<ProjectGE::EventDispatcherBase*>& dispatchers, bool overlay) override {
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
	}
};

ProjectGE::Application* ProjectGE::CreateApplication() {
	return new TestZone();
}
