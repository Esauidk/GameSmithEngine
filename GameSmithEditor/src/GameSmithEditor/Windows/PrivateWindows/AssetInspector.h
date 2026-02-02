#pragma once
#include "GameSmithEditor/Windows/EditorWindow.h"
#include "GameSmithEngine.h"

namespace GameSmithEditor {
	class AssetInspector : public EditorWindow {
	public:
		AssetInspector(GameSmith::Ref<GameSmith::Asset> asset) : EditorWindow("AssetInspector"), m_InspectedAsset(asset) {}
		void OnImGuiRender() override;
		void OnUpdate(float dt) override;
	private:
		GameSmith::Ref<GameSmith::Asset> m_InspectedAsset;
		bool m_Open = true;
	};
};
