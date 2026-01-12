#pragma once
#include "GameSmithEditor/Core/EditorMenu.h"
#include "GameSmithEditor/Core/EditorCoreLayer.h"

#define REGISTER_WINDOW_DEFAULT_CALLBACK(MENUPATH, WINDOW) REGISTER_EDITOR_MENU_OPTION(\
		MENUPATH,\
		[]() {GameSmithEditor::EditorCoreLayer::CreateEditorWindow<WINDOW>(); } \
)
