#pragma once
#include "GameSmithEngine.h"

#define IMGUI_PAYLOAD_TYPE_GAMEOBJECT_REF "GAMEOBJ_REF"
#define IMGUI_PAYLOAD_TYPE_ASSET_REF "ASSET_REF"

namespace GameSmithEditor {
	// This is a widget that allows you to accept GameSmith Connections as input
	// Note: Current support is for GameObjects & Components
	bool InputReference(std::string varName, GameSmith::Ref<GameSmith::ConnectionContainer> refCon);
	// This is a widget that allows you to accept GameSmith Asset Refs as input
	bool InputReference(std::string varName, GameSmith::Ref<GameSmith::AssetRefContainer> refCon);
};

