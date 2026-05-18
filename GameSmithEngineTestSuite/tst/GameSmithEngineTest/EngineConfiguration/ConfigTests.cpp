#include "gtest/gtest.h"
#include "GameSmithEngine/EngineConfiguration/Configs/AssetManagementConfig.h"
#include "GameSmithEngine/EngineConfiguration/Configs/ContentLibraryConfig.h"
#include "GameSmithEngine/EngineConfiguration/Configs/RenderingConfig.h"

TEST(AssetManagementConfigTest, DefaultValues) {
	GameSmith::AssetManagementConfig config;
	EXPECT_EQ(config.GetAssetDirectory(), "");
}

TEST(AssetManagementConfigTest, SetAssetDirectory) {
	GameSmith::AssetManagementConfig config;
	config.SetAssetDirectory("./my_assets");
	EXPECT_EQ(config.GetAssetDirectory(), "./my_assets");
}

TEST(ContentLibraryConfigTest, DefaultValues) {
	GameSmith::ContentLibraryConfig config;
	EXPECT_EQ(config.GetContentLibraryDirectory(), "");
}

TEST(ContentLibraryConfigTest, SetContentLibraryDirectory) {
	GameSmith::ContentLibraryConfig config;
	config.SetContentLibraryDirectory("./libs");
	EXPECT_EQ(config.GetContentLibraryDirectory(), "./libs");
}

TEST(RenderingConfigTest, DefaultValues) {
	GameSmith::RenderingConfig config;
	EXPECT_TRUE(config.IsVSyncEnabled());
	EXPECT_FALSE(config.IsFullscreenMode());
	EXPECT_EQ(config.GetResolutionWidth(), (unsigned int)1920);
	EXPECT_EQ(config.GetResolutionHeight(), (unsigned int)1080);
}

TEST(RenderingConfigTest, SetVSync) {
	GameSmith::RenderingConfig config;
	config.SetVSyncEnabled(false);
	EXPECT_FALSE(config.IsVSyncEnabled());
	config.SetVSyncEnabled(true);
	EXPECT_TRUE(config.IsVSyncEnabled());
}

TEST(RenderingConfigTest, SetFullscreen) {
	GameSmith::RenderingConfig config;
	config.SetFullscreenMode(true);
	EXPECT_TRUE(config.IsFullscreenMode());
	config.SetFullscreenMode(false);
	EXPECT_FALSE(config.IsFullscreenMode());
}

TEST(RenderingConfigTest, SetResolution) {
	GameSmith::RenderingConfig config;
	config.SetResolution(2560, 1440);
	EXPECT_EQ(config.GetResolutionWidth(), (unsigned int)2560);
	EXPECT_EQ(config.GetResolutionHeight(), (unsigned int)1440);
}
