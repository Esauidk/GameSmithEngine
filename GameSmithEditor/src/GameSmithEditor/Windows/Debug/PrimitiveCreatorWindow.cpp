#ifdef GE_EDITOR_DEBUG
#include "PrimitiveCreatorWindow.h"
#include "GameSmithEditor/Utils/SystemCallUtils.h"
#include <imgui.h>
#include <filesystem>

#define DEFAULT_CUBE_SIZE 1

namespace fs = std::filesystem;

namespace GameSmithEditor {
	REGISTER_WINDOW_DEFAULT_CALLBACK(Debug_PrimitiveCreator, PrimitiveCreatorWindow);

	enum class PrimitiveType {
		Cube = 0,
		Sphere = 1,
		Cylinder = 2,
		NumPrimitives = 3
	};

	std::string PrimitiveTypeToString(PrimitiveType type) {
		switch (type) {
		case PrimitiveType::Cube:
			return "Cube";
		case PrimitiveType::Sphere:
			return "Sphere";
		case PrimitiveType::Cylinder:
			return "Cylinder";
		default:
			return "Unknown";
		}
	}

	GameSmith::Ref<GameSmith::Mesh> CreatePrimitive(PrimitiveType type) {
		switch (type) {
		case PrimitiveType::Cube:
			return GameSmith::CreateCube(DEFAULT_CUBE_SIZE);
		default:
			return nullptr;
		}
	}

	void PrimitiveCreatorWindow::OnImGuiRender()
	{
		ImGui::Begin("Primitive Creator", &m_Open);
		ImGui::Text("Select a primitive to create:");
		for (int i = 0; i < static_cast<int>(PrimitiveType::NumPrimitives); ++i) {
			const PrimitiveType type = static_cast<PrimitiveType>(i);
			const std::string typeString = PrimitiveTypeToString(type);
			if (ImGui::Button(typeString.c_str())) {
				std::string fileType = GameSmith::ObjMeshAsset::GetStaticFileExtension();
				if (fileType.find('.') != std::string::npos) {
					fileType = fileType.substr(fileType.find('.')+1);
				}
				// Handle primitive creation logic here
				FileSearchCriteria criteria;
				criteria.filePath = std::filesystem::current_path().string();
				criteria.fileType = GameSmith::ObjMeshAsset::GetStaticFileType();
				criteria.filePrefix = fileType;

				std::string outPath;
				if (CreateFileDialog(criteria, &outPath)) {
					const fs::path filePathObj(outPath);
					std::string fileName = filePathObj.stem().string();
					std::string destinationDir = filePathObj.parent_path().string();
					auto mesh = CreatePrimitive(type);
					auto meshAsset = GameSmith::Ref<GameSmith::ObjMeshAsset>(new GameSmith::ObjMeshAsset(fileName, mesh));

					auto assetManager = GameSmith::AssetManager::GetInstance();
					assetManager->WriteResource(meshAsset, destinationDir);
					GE_APP_INFO("Selected file: {0}", outPath);
				}
			}

			if (i < static_cast<int>(PrimitiveType::NumPrimitives) - 1) {
				ImGui::SameLine();
			}
		}
		ImGui::End();
	}

	void PrimitiveCreatorWindow::OnUpdate(float dt)
	{
		if (!m_Open) {
			CloseWindow();
		}
	}
}
#endif