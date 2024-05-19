#include "gtest/gtest.h"

#include "GameSmithEngineTest/TestInclude.h"

static char* loadFile(std::string file, unsigned int* outSize) {
	std::fstream pFile(file, std::ios::in | std::ios::binary | std::ios::ate);
	GE_CORE_ASSERT(pFile.is_open(), std::format("Asset file {0} cannot be opened", file));

	*outSize = (UINT)pFile.tellg();
	pFile.seekg(0, pFile.beg);

	char* buf = new char[*outSize];

	pFile.read(buf, *outSize);
	pFile.close();

	return buf;
}

TEST(ShaderAssetTest, Deserialize) {
	GameSmith::RenderingManager manager(true);
	manager.Init();

	std::string shaderPath = TEST_RESOURCES;
	shaderPath += "/shaders/SampleVertexShader.cso";
	
	unsigned size;
	char* file = loadFile(shaderPath, &size);

	GameSmith::ShaderAsset asset;
	EXPECT_NO_THROW(asset.Deserialize(file, size));

	delete[] file;
}

TEST(ShaderAssetTest, SerializeNew) {
	GameSmith::RenderingManager manager(true);
	manager.Init();

	std::string shaderPath = TEST_RESOURCES;
	shaderPath += "/shaders/SampleVertexShader.cso";

	unsigned size;
	char* file = loadFile(shaderPath, &size);

	GameSmith::ShaderAsset asset;
	EXPECT_NO_THROW(asset.Deserialize(file, size));

	GameSmith::Ref<char> serial = asset.Serialize();

	for (unsigned int i = 0; i < size; i++) {
		EXPECT_EQ(*(file + i), *(serial.get() + i));
	}

	delete[] file;
}

TEST(ShaderAssetTest, SerializeAppend) {
	GameSmith::RenderingManager manager(true);
	manager.Init();

	std::string shaderPath = TEST_RESOURCES;
	shaderPath += "/shaders/SampleVertexShader.cso";

	unsigned size;
	char* file = loadFile(shaderPath, &size);

	GameSmith::ShaderAsset asset;
	EXPECT_NO_THROW(asset.Deserialize(file, size));

	GameSmith::Ref<char> serial = GameSmith::Ref<char>(new char[size + 5]);
	asset.Serialize(serial.get() + 5, size);

	for (unsigned int i = 0; i < size; i++) {
		EXPECT_EQ(*(file + i), *(serial.get() + i + 5));
	}

	delete[] file;
}

TEST(ShaderAssetTest, GetRequiredSize) {
	GameSmith::RenderingManager manager(true);
	manager.Init();

	std::string shaderPath = TEST_RESOURCES;
	shaderPath += "/shaders/SampleVertexShader.cso";

	unsigned size;
	char* file = loadFile(shaderPath, &size);

	GameSmith::ShaderAsset asset;
	EXPECT_NO_THROW(asset.Deserialize(file, size));

	EXPECT_EQ(size, asset.RequireSpace());

	delete[] file;
}