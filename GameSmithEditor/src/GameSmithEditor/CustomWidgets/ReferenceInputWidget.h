#pragma once
#include "GameSmithEngine.h"

#define IMGUI_PAYLOAD_TYPE_GAMEOBJECT_REF "REF"

namespace GameSmithEditor {
	bool InputReference(std::string varName, GameSmith::Ref<GameSmith::RefContainer> refCon);
};

