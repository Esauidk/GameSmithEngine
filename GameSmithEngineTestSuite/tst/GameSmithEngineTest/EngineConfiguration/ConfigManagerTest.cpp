#include "gtest/gtest.h"
#include "GameSmithEngine/EngineConfiguration/ConfigManager.h"

TEST(ConfigManagerTest, InitAndShutdown) {
	EXPECT_NO_THROW(GameSmith::ConfigManager::Init());
	auto* inst = GameSmith::ConfigManager::GetInstance();
	EXPECT_NE(inst, nullptr);
	GameSmith::ConfigManager::Shutdown();
}

TEST(ConfigManagerTest, DefaultConfigsMatchClassDefaults) {
	GameSmith::ConfigManager::Init();
	auto* inst = GameSmith::ConfigManager::GetInstance();

	GameSmith::AssetManagementConfig defaultAssetConfig;
	const auto& assetConfig = inst->GetAssetManagementConfig();
	EXPECT_EQ(assetConfig.GetAssetDirectory(), defaultAssetConfig.GetAssetDirectory());

	GameSmith::ContentLibraryConfig defaultLibConfig;
	const auto& libConfig = inst->GetContentLibraryConfig();
	EXPECT_EQ(libConfig.GetContentLibraryDirectory(), defaultLibConfig.GetContentLibraryDirectory());

	GameSmith::RenderingConfig defaultRenderConfig;
	const auto& renderConfig = inst->GetRenderingConfig();
	EXPECT_EQ(renderConfig.IsVSyncEnabled(), defaultRenderConfig.IsVSyncEnabled());
	EXPECT_EQ(renderConfig.IsFullscreenMode(), defaultRenderConfig.IsFullscreenMode());
	EXPECT_EQ(renderConfig.GetResolutionWidth(), defaultRenderConfig.GetResolutionWidth());
	EXPECT_EQ(renderConfig.GetResolutionHeight(), defaultRenderConfig.GetResolutionHeight());

	GameSmith::ConfigManager::Shutdown();
}
