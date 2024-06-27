#include <GameSmithEngine.h>
#include "TestingLayer.h"

class TestZone : public GameSmith::Application {
public:
	TestZone() {
		PushLayer(new TestingLayer());
	}
};

GameSmith::Application* GameSmith::CreateApplication() {
	return new TestZone();
}
