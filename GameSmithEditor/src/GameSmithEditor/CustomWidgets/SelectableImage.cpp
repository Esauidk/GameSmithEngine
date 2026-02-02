#include "SelectableImage.h"

namespace GameSmithEditor {
	bool SelectableImage(const std::string& label, const bool selected, const ImTextureRef image, const ImVec2& imageSize)
	{
		ImGui::Image(image, imageSize);
		ImGui::SetNextItemAllowOverlap();
		ImGui::SetCursorScreenPos(ImGui::GetItemRectMin());
		std::string uniqueLabel = "##SelectableImage_" + label;
		bool result = selected;

		const ImVec2 min = ImGui::GetItemRectMin();
		const ImVec2 max = ImGui::GetItemRectMax();

		if (ImGui::InvisibleButton(uniqueLabel.c_str(), imageSize)) {
			result = true;
		}

		ImDrawList* dl = ImGui::GetWindowDrawList();
		if (ImGui::IsItemHovered() || result  ) {
			dl->AddRectFilled(min, max, IM_COL32(255, 255, 255, 40), 3.0f);
		}

		return result;
	}
};