#include <GameSmithEngine.h>
#include "TestRenderLayer.h"
#include "TestResourceLayer.h"
#include "TestComponentLayer.h"
#include "SandBoxLayer.h"
#include "DemoLayer.h"
#include "TestingLayer.h"

class TestZone : public GameSmith::Application {
public:
	TestZone() {
		PushLayer(new TestingLayer());
		//PushLayer(new DemoLayer());
		//PushLayer(new SandBoxLayer());
		//PushLayer(new TestRenderLayer());
	}
};

GameSmith::Application* GameSmith::CreateApplication() {
	return new TestZone();
}
