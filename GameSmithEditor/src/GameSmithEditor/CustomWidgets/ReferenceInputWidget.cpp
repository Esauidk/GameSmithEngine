#include "ReferenceInputWidget.h"
#include "imgui.h"

namespace GameSmithEditor {
	bool InputReference(std::string varName, GameSmith::Ref<GameSmith::ConnectionContainer> refCon)
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
			unsigned int flags = refCon->GetFlags();
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_GAMEOBJECT_REF)) {
				GameSmith::Connection<GameSmith::GameObject> obj = *((GameSmith::Connection<GameSmith::GameObject>*)(payload->Data));

				if (refCon->TypeCheck(obj)) {
					refCon->AssignRef(obj);
					refCon->AssignID(obj.lock()->GetID());
					found = true;
				}
				else {
					std::vector<GameSmith::Connection<GameSmith::Component>> comps;
					obj.lock()->GetComponents<GameSmith::Component>(&comps);

					for (auto& comp : comps) {
						if (refCon->TypeCheck(comp)) {
							refCon->AssignRef(comp);
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

	bool InputReference(std::string varName, GameSmith::Ref<GameSmith::AssetRefContainer> refCon)
	{
		bool found = false;
		ImGui::Text(varName.c_str());
		ImGui::SameLine();
		std::string refInfo = std::format("{0}:{1}", "Empty", refCon->GetTypeName());
		auto curRef = refCon->GetCurrentRef();
		if (curRef != nullptr) {
			const GameSmith::ID& id = curRef->GetID();
			refInfo = std::format(
				"{0}:{1},{2},{3},{4}",
				refCon->GetTypeName(),
				id.getData().ID1,
				id.getData().ID2,
				id.getData().ID3,
				id.getData().ID4
			);
		}

		ImGui::Button(refInfo.c_str());

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_ASSET_REF)) {
				GameSmith::ID* assetID = (GameSmith::ID*)(payload->Data);

				auto resourceManager = GameSmith::ResourceManager::GetInstance();
				GameSmith::Ref<GameSmith::Serializeable> asset = resourceManager->GetResource(*assetID);
				// TODO: Update resource manager to return asset classes
				GameSmith::Ref<GameSmith::Asset> tmpUpgrade = CastPtr<GameSmith::Asset>(asset);

				if (refCon->TypeCheck(tmpUpgrade)) {
					refCon->AssignRef(tmpUpgrade);
					refCon->AssignID(tmpUpgrade->GetID());
					found = true;
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
