#include "ExposedVariableWidgetConverter.h"
#include "imgui.h"

namespace GameSmithEditor {
	void GenerateVariableUI(GameSmith::Ref<GameSmith::ParameterContainer> container) {
		auto name = container->GetName();

		switch (container->GetType()) {
		case GameSmith::ContainerDataType::Float:
			ImGui::InputFloat(name.c_str(), (float*)container->GetCharData());
			break;
		case GameSmith::ContainerDataType::Float2:
			ImGui::InputFloat2(name.c_str(), (float*)container->GetCharData());
			break;
		case GameSmith::ContainerDataType::Float3:
			ImGui::InputFloat3(name.c_str(), (float*)container->GetCharData());
			break;
		case GameSmith::ContainerDataType::Float4:
			ImGui::InputFloat4(name.c_str(), (float*)container->GetCharData());
			break;
		case GameSmith::ContainerDataType::Int:
			ImGui::InputInt(name.c_str(), (int*)container->GetCharData());
			break;
		case GameSmith::ContainerDataType::Int2:
			ImGui::InputInt2(name.c_str(), (int*)container->GetCharData());
			break;
		case GameSmith::ContainerDataType::Int3:
			ImGui::InputInt3(name.c_str(), (int*)container->GetCharData());
			break;
		case GameSmith::ContainerDataType::Int4:
			ImGui::InputInt4(name.c_str(), (int*)container->GetCharData());
			break;
		}
	}
};