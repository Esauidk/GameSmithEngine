#include <GameSmithEngine.h>
#include "EditorCoreLayer.h"

class GEEditor : public GameSmith::Application {
public:
	GEEditor() : GameSmith::Application("Game Smith Editor") {
		PushLayer(new GameSmithEditor::EditorCoreLayer());
	}
};

GameSmith::Application* GameSmith::CreateApplication() {
	return new GEEditor();
}