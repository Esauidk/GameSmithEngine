#define IMPORT_ENTRY
#include <GameSmithEngine.h>
#include "EditorCoreLayer.h"

class GEEditor : public GameSmith::Application {
public:
	GEEditor(GameSmith::ApplicationSpecs& specs) : GameSmith::Application(specs) {
		PushLayer(new GameSmithEditor::EditorCoreLayer());
	}
};

GameSmith::Application* GameSmith::CreateApplication(GameSmith::CommandLineArgs cmdArgs) {
	GameSmith::ApplicationSpecs specs;
	specs.CommandLineArgs = cmdArgs;
	specs.Name = EDITOR_WINDOW_TITLE;
	return new GEEditor(specs);
}