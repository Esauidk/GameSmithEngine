#pragma once
#include "GameSmithEditor/Windows/EditorWindow.h"
#include "GameSmithEngine.h"

namespace GameSmithEditor {
	class AssetInspector : public EditorWindow {
	public:
		AssetInspector(GameSmith::Ref<GameSmith::IAsset> asset);
		void OnImGuiRender() override;
		void OnUpdate(float dt) override;
	private:
		GameSmith::Ref<GameSmith::IAsset> m_InspectedAsset;
		std::unordered_map<std::string, GameSmith::Ref<GameSmith::ParameterContainer>> m_ExposedVariables;
		std::unordered_map<std::string, GameSmith::Ref<GameSmith::ConnectionContainer>> m_ExposedRefs;
		std::unordered_map<std::string, GameSmith::Ref<GameSmith::AssetRefContainer>> m_ExposedAssets;

		bool m_Open = true;
	};
};
