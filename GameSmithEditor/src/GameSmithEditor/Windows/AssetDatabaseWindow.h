#pragma once
#include "GameSmithEngine.h"
#include "EditorWindow.h"

namespace GameSmithEditor {
	class AssetDatabaseWindow : public EditorWindow
	{
	public:
		AssetDatabaseWindow() : EditorWindow("AssetDatabaseWindow") {}
		void OnImGuiRender() override;
		void OnUpdate() override;
	};
}


