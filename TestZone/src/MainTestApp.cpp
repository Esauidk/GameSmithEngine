#include <ProjectGE.h>

bool printTest(ProjectGE::Event& test) {
	GE_APP_ERROR(test);

	return false;
}

class ExampleLayer : public ProjectGE::Layer {
public:

	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override {
		GE_APP_INFO("ExampleLayer::Update");
	}

	void EventSubscribe(std::vector<ProjectGE::EventDispatcherBase*> dispatchers, bool overlay) override {
		for (auto dispatcher : dispatchers) {
			ProjectGE::EventDispatcher<ProjectGE::WindowCloseEvent>* close_event;
			ProjectGE::EventDispatcher<ProjectGE::WindowMovedEvent>* move_event;
			ProjectGE::EventDispatcher<ProjectGE::WindowResizeEvent>* resize_event;
			ProjectGE::EventDispatcher<ProjectGE::WindowFocusEvent>* focus_event;

			if (EVENT_CAST(WindowCloseEvent, dispatcher, close_event)) {
				close_event->AddListener(printTest);
				continue;
			}

			if (EVENT_CAST(WindowMovedEvent, dispatcher, move_event)) {
				move_event->AddListener(printTest);
				continue;
			}

			if (EVENT_CAST(WindowResizeEvent, dispatcher, resize_event)) {
				resize_event->AddListener(printTest);
				continue;
			}
			if (EVENT_CAST(WindowFocusEvent, dispatcher, focus_event)) {
				focus_event->AddListener(printTest);
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
	~TestZone() {

	}
};

ProjectGE::Application* ProjectGE::CreateApplication() {
	return new TestZone();
}
