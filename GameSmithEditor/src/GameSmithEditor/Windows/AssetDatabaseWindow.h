#pragma once
#include "GameSmithEngine.h"
#include "EditorWindow.h"

#include <filesystem>

namespace GameSmithEditor {
	class AssetDatabaseWindow : public EditorWindow
	{
	public:
		AssetDatabaseWindow();
		void OnAttach(const GameSmith::ApplicationSpecs& specs) override;
		void OnDetach() override;
		void OnImGuiRender() override;
		void OnUpdate(float dt) override;
	private:
		static int s_Count;
		std::filesystem::path m_CurrentPath;
		std::string m_SelectedAsset = "";
		bool m_Open = true;
	};
}


