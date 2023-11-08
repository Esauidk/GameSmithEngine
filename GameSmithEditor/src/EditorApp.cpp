#include <GameSmithEngine.h>

class GEEditor : public GameSmith::Application {
public:
	GEEditor() {}
};

GameSmith::Application* GameSmith::CreateApplication() {
	return new GEEditor();
}