#pragma once
#include "gepch.h"
#include "GameSmithEngine.h"

namespace GameSmithEditor {
	class IconManager
	{
	public:
		static void Init();
		static void Shutdown();
		inline static IconManager* GetInstance() { return s_Instance; }
	public:
		GameSmith::Ref<GameSmith::Texture2D> GetIcon(std::string fileExtention);
		const GameSmith::ImGuiTextureSpace* GetImGuiIcon(std::string fileExtention);
	private:
		IconManager() = default;
		void LoadIcon(std::string fileExtention);
	private:
		struct IconEntry {
			const GameSmith::ImGuiTextureSpace* imguiRef;
			GameSmith::Ref<GameSmith::TextureAsset> assetRef;
		};
	private:
		static IconManager* s_Instance;
		std::unordered_map<std::string, IconEntry> m_IconMap;
	};
};


