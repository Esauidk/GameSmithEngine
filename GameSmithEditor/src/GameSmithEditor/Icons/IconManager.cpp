#include "IconManager.h"
#include "GameSmithEditor/Core/GameProject.h"
#include "GameSmithEditor/Core/CoreMacros.h"

#include <filesystem>

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

namespace GameSmithEditor {
	IconManager* IconManager::s_Instance = nullptr;

	void IconManager::Init()
	{
		if (s_Instance == nullptr) {
			s_Instance = new IconManager();
		}
	}

	void IconManager::Shutdown()
	{
		if (s_Instance != nullptr) {
			delete s_Instance;
			s_Instance = nullptr;
		}
	}

	GameSmith::Ref<GameSmith::Texture2D> IconManager::GetIcon(std::string fileExtention)
	{
		if (m_IconMap.contains(fileExtention)) {
			return m_IconMap.find(fileExtention)->second.assetRef->GetTexture();
		}
		else {
			FindIcon(fileExtention);

			if (m_IconMap.contains(fileExtention)) {
				return m_IconMap.find(fileExtention)->second.assetRef->GetTexture();
			}

			return GameSmith::Ref<GameSmith::Texture2D>();
		}
	}

	const GameSmith::ImGuiTextureSpace* IconManager::GetImGuiIcon(std::string fileExtention)
	{
		if (m_IconMap.contains(fileExtention)) {
			return m_IconMap.find(fileExtention)->second.imguiRef;
		}
		else {
			FindIcon(fileExtention);

			if (m_IconMap.contains(fileExtention)) {
				return m_IconMap.find(fileExtention)->second.imguiRef;
			}

			return nullptr;
		}
	}

	void IconManager::FindIcon(std::string fileExtention)
	{
		auto assetManager = GameSmith::AssetManager::GetInstance();
		const auto& resourceRegistry = assetManager->GetAssetDirectories();
		
		GE_APP_ASSERT(resourceRegistry.contains(EDITOR_BUILTIN_ASSETS_FOLDER), "No EditorResource database registered in the asset manager");
		auto editorIconRoot = std::format("{0}\\{1}", resourceRegistry.at(EDITOR_BUILTIN_ASSETS_FOLDER), "Icons");
		if (std::filesystem::exists(editorIconRoot)) {
			for (const auto& dirEntry : recursive_directory_iterator(editorIconRoot)) {
				auto fileName = dirEntry.path().filename().string();
				if (fileName.starts_with(fileExtention)) {
					const auto assetID = assetManager->GetAssetID(dirEntry.path().string());
					auto icon = assetManager->GetResource<GameSmith::TextureAsset>(assetID);
					auto imGUITex = GameSmith::Application::Get().GetImGuiInstance()->GenerateTextureSpace(icon->GetTexture());
					m_IconMap.insert({ fileExtention, {imGUITex, icon } });
					break;
				}
			}
		}
	}
};