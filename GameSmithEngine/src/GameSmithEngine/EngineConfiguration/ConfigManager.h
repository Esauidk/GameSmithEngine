#pragma once

#include "Configs/AssetManagementConfig.h"
#include "Configs/ContentLibraryConfig.h"
#include "Configs/RenderingConfig.h"

#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/ResourceManagement/ResourceLoaders/ResourceLoader.h"

#define CONFIG_FOLDER "Configs/"
namespace GameSmith {
	class ConfigManager {
	public:
		static void Init();
		static void Shutdown();

		static ConfigManager* GetInstance() { return s_Instance; }
	public:
		inline const AssetManagementConfig& GetAssetManagementConfig() { return m_AssetManagementConfig; }
		inline const ContentLibraryConfig& GetContentLibraryConfig() { return m_ContentLibraryConfig; }
		inline const RenderingConfig& GetRenderingConfig() { return m_RenderingConfig; }
	private:
		ConfigManager();
	private:
		static ConfigManager* s_Instance;
	private:
		Ref<ResourceLoader> m_FileLoader;

		AssetManagementConfig m_AssetManagementConfig;
		ContentLibraryConfig m_ContentLibraryConfig;
		RenderingConfig m_RenderingConfig;
	};
};
