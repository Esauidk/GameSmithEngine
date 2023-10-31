#include <ProjectGE.h>
#include "TestRenderLayer.h"
#include "TestResourceLayer.h"
#include "TestComponentLayer.h"

class TestZone : public ProjectGE::Application {
public:
	TestZone() {
		PushLayer(new TestRenderLayer());
		PushLayer(new TestResourceLayer());
		PushLayer(new TestComponentLayer());
	}
};

ProjectGE::Application* ProjectGE::CreateApplication() {
	return new TestZone();
}
