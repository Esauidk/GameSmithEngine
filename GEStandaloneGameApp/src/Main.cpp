#define IMPORT_ENTRY
#include <GameSmithEngine.h>

class GEMainGame : public GameSmith::Application {
public:
	GEMainGame() : GameSmith::Application("GEMainGame") {}
};

GameSmith::Application* GameSmith::CreateApplication() {
	return new GEMainGame();
}