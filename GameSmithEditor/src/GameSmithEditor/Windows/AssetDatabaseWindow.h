#pragma once
#include "GameSmithEngine.h"
#include "EditorWindow.h"

#include <filesystem>

namespace GameSmithEditor {
	class AssetDatabaseWindow : public EditorWindow
	{
	public:
		AssetDatabaseWindow();
		void OnAttach() override;
		void OnDetach() override;
		void OnImGuiRender() override;
	private:
		static int s_Count;
		std::filesystem::path m_CurrentPath;
	};
}


