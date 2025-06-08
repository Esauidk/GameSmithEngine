#define IMPORT_ENTRY
#include <GameSmithEngine.h>

class GEMainGame : public GameSmith::Application {
public:
	GEMainGame(GameSmith::ApplicationSpecs specs) : GameSmith::Application(specs) {}
};

GameSmith::Application* GameSmith::CreateApplication(GameSmith::CommandLineArgs cmdArgs) {
	GameSmith::ApplicationSpecs specs;
	specs.Name = "GEMainGame";
	specs.CommandLineArgs = cmdArgs;
	return new GEMainGame(specs);
}