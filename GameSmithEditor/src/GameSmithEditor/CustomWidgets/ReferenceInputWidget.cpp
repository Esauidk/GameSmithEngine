#include "ReferenceInputWidget.h"
#include "imgui.h"

namespace GameSmithEditor {
	void InputReference() {
		char test[256];
		ImGui::InputText("Test", test, 256);
		ImGui::SameLine();
		ImGui::Button("Load");
	};
};
