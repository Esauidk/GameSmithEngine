#include "ReferenceInputWidget.h"
#include "imgui.h"

namespace GameSmithEditor {
	bool InputReference(std::string varName, GameSmith::Ref<GameSmith::RefContainer> refCon)
	{
		bool found = false;
		ImGui::Text(varName.c_str());
		ImGui::SameLine();
		std::string refInfo = std::format("{0}:{1}", "Empty", refCon->GetTypeName());
		auto curRef = refCon->GetCurrentRef();
		if (CastPtr<GameSmith::GameObject>(curRef.lock()) != nullptr) {
			auto ptr = CastPtr<GameSmith::GameObject>(curRef.lock());
			refInfo = std::format("{0}:{1}", ptr->GetName(), "GameObject");
		}
		else if (CastPtr<GameSmith::Component>(curRef.lock()) != nullptr) {
			auto ptr = CastPtr<GameSmith::Component>(curRef.lock());
			refInfo = std::format("{0}:{1}", ptr->GetGameObject()->GetName(), ptr->GetName());
		}

		ImGui::Button(refInfo.c_str());

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_GAMEOBJECT_REF)) {
				GameSmith::Connection<GameSmith::GameObject> obj = *((GameSmith::Connection<GameSmith::GameObject>*)(payload->Data));
				
				if (refCon->TypeCheck(obj)) {
					refCon->AssignRef(obj, 0);
					refCon->AssignID(obj.lock()->GetID());
					found = true;
				}
				else {
					std::vector<GameSmith::Connection<GameSmith::Component>> comps;
					obj.lock()->GetComponents<GameSmith::Component>(&comps);

					for (auto& comp : comps) {
						if (refCon->TypeCheck(comp)) {
							refCon->AssignRef(comp, 0);
							refCon->AssignID(comp.lock()->GetID());
							found = true;
							break;
						}
					}
				}
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::SameLine();

		if (ImGui::SmallButton("Select")) {
		}

		return found;
	}
};
