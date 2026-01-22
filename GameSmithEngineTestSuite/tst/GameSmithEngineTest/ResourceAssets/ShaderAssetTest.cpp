#include "gtest/gtest.h"

#include "GameSmithEngineTest/TestInclude.h"

static GameSmith::Ref<char> loadFile(std::string file, unsigned int* outSize) {
	std::fstream pFile(file, std::ios::in | std::ios::binary | std::ios::ate);
	GE_CORE_ASSERT(pFile.is_open(), std::format("Asset file {0} cannot be opened", file));

	*outSize = (UINT)pFile.tellg();
	pFile.seekg(0, pFile.beg);

	char* buf = new char[*outSize];

	pFile.read(buf, *outSize);
	pFile.close();

	return GameSmith::Ref<char>(buf);
}

static unsigned int testShaderSize;
static GameSmith::Ref<char> testShader = loadFile(std::format("{0}{1}", TEST_RESOURCES, "/shaders/SampleVertexShader.cso"), &testShaderSize);

TEST(ShaderAssetTest, Deserialize) {
	GameSmith::RenderingManager::Init();

	GameSmith::ShaderAsset asset("Test");
	EXPECT_NO_THROW(asset.Deserialize(testShader.get(), testShaderSize));
	GameSmith::RenderingManager::ShutDown();
}

TEST(ShaderAssetTest, SerializeNew) {
	GameSmith::RenderingManager::Init();

	GameSmith::ShaderAsset asset("Test");
	EXPECT_NO_THROW(asset.Deserialize(testShader.get(), testShaderSize));

	GameSmith::Ref<char> serial = asset.Serialize();

	for (unsigned int i = 0; i < testShaderSize; i++) {
		EXPECT_EQ(*(testShader.get() + i), *(serial.get() + i));
	}
	GameSmith::RenderingManager::ShutDown();
}

TEST(ShaderAssetTest, SerializeAppend) {
	GameSmith::RenderingManager::Init();

	GameSmith::ShaderAsset asset("Test");
	EXPECT_NO_THROW(asset.Deserialize(testShader.get(), testShaderSize));

	GameSmith::Ref<char> serial = GameSmith::Ref<char>(new char[testShaderSize + 5]);
	asset.Serialize(serial.get() + 5, testShaderSize);

	for (unsigned int i = 0; i < testShaderSize; i++) {
		EXPECT_EQ(*(testShader.get() + i), *(serial.get() + i + 5));
	}
	GameSmith::RenderingManager::ShutDown();
}

TEST(ShaderAssetTest, GetRequiredSize) {
	GameSmith::RenderingManager::Init();

	GameSmith::ShaderAsset asset("Test");
	EXPECT_NO_THROW(asset.Deserialize(testShader.get(), testShaderSize));

	EXPECT_EQ(testShaderSize, asset.RequiredSpace());
	GameSmith::RenderingManager::ShutDown();
}