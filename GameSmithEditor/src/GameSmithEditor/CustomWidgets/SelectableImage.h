#pragma once

#include <string>
#include "imgui.h"

namespace GameSmithEditor {
	bool SelectableImage(const std::string& label, const bool selected, const ImTextureRef image, const ImVec2& imageSize);
};
