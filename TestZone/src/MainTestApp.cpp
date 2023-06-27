#include <ProjectGE.h>

bool printTest(ProjectGE::Event& test) {
	GE_APP_ERROR(test);

	return false;
}

class ExampleLayer : public ProjectGE::Layer {
public:

	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override {
		//GE_APP_INFO("ExampleLayer::Update");
	}

	void EventSubscribe(std::vector<ProjectGE::EventDispatcherBase*> dispatchers, bool overlay) override {
	
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
