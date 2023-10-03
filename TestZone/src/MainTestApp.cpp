#include <ProjectGE.h>
#include "TestRenderLayer.h"
#include "TestResourceLayer.h"

class TestZone : public ProjectGE::Application {
public:
	TestZone() {
		PushLayer(new TestResourceLayer(50));
	}
};

ProjectGE::Application* ProjectGE::CreateApplication() {
	return new TestZone();
}
