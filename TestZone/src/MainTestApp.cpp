#include <GameSmithEngine.h>
#include "TestRenderLayer.h"
#include "TestResourceLayer.h"
#include "TestComponentLayer.h"
#include "SandBoxLayer.h"

class TestZone : public GameSmith::Application {
public:
	TestZone() {
		PushLayer(new SandBoxLayer());
	}
};

GameSmith::Application* GameSmith::CreateApplication() {
	return new TestZone();
}
