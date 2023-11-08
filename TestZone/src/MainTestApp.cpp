#include <GameSmithEngine.h>
#include "TestRenderLayer.h"
#include "TestResourceLayer.h"
#include "TestComponentLayer.h"

class TestZone : public GameSmith::Application {
public:
	TestZone() {
		PushLayer(new TestRenderLayer());
		PushLayer(new TestResourceLayer());
		PushLayer(new TestComponentLayer());
	}
};

GameSmith::Application* GameSmith::CreateApplication() {
	return new TestZone();
}
