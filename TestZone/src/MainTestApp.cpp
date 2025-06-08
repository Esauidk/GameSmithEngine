#define IMPORT_ENTRY
#include <GameSmithEngine.h>
#include "TestingLayer.h"

class TestZone : public GameSmith::Application {
public:
	TestZone(GameSmith::ApplicationSpecs specs) : GameSmith::Application(specs) {
		PushLayer(new TestingLayer());
	}
};

GameSmith::Application* GameSmith::CreateApplication(GameSmith::CommandLineArgs cmdArgs) {
	GameSmith::ApplicationSpecs specs;
	specs.CommandLineArgs = cmdArgs;

	return new TestZone(specs);
}
