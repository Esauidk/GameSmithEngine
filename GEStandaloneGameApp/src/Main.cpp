#include "GameSmithEngine/Core/Application.h"
#include "GameSmithEngine/Core/EntryPoint.h"

class GEMainGame : public GameSmith::Application {
public:
	GEMainGame() : GameSmith::Application("GEMainGame") {}
};

GameSmith::Application* GameSmith::CreateApplication() {
	return new GEMainGame();
}