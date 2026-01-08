#include "IconManager.h"
#include "GameSmithEditor/Core/GameProject.h"

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
		if (GameProject::IsLoaded()) {
			if (m_IconMap.contains(fileExtention)) {
				return m_IconMap.find(fileExtention)->second.assetRef->GetTexture();
			}
			else {
				LoadIcon(fileExtention);
				
				if (m_IconMap.contains(fileExtention)) {
					return m_IconMap.find(fileExtention)->second.assetRef->GetTexture();
				}

				return GameSmith::Ref<GameSmith::Texture2D>();
			}
		}
		else {
			return GameSmith::Ref<GameSmith::Texture2D>();
		}
	}

	const GameSmith::ImGuiTextureSpace* IconManager::GetImGuiIcon(std::string fileExtention)
	{
		if (GameProject::IsLoaded()) {
			if (m_IconMap.contains(fileExtention)) {
				return m_IconMap.find(fileExtention)->second.imguiRef;
			}
			else {
				LoadIcon(fileExtention);

				if (m_IconMap.contains(fileExtention)) {
					return m_IconMap.find(fileExtention)->second.imguiRef;
				}

				return nullptr;
			}
		}
		else {
			return nullptr;
		}
	}

	void IconManager::LoadIcon(std::string fileExtention)
	{
		auto editorIconRoot = std::format("{0}/{1}", GameProject::GetEditorResourceFolder(), "/Icons");
		if (std::filesystem::exists(editorIconRoot)) {
			for (const auto& dirEntry : recursive_directory_iterator(editorIconRoot)) {
				auto fileName = dirEntry.path().filename().string();
				if (fileName.starts_with(fileExtention)) {
					auto resourceManager = GameSmith::AssetManager::GetInstance();
					auto icon = resourceManager->GetResource<GameSmith::TextureAsset>(dirEntry.path().string());
					auto imGUITex = GameSmith::Application::Get().GetImGuiInstance()->GenerateTextureSpace(icon->GetTexture());
					m_IconMap.insert({ fileExtention, {imGUITex, icon } });
					break;
				}
			}
		}
	}
};