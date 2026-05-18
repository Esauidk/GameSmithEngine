#include "gtest/gtest.h"
#include "GameSmithEngine/SerializeableFiles/ResourceAssets/AssetFactory.h"

// Test-only asset type.
namespace GameSmith {
	class TestAsset : public Asset {
	public:
		TestAsset(std::string name) : Asset(name) {}
		SERIAL_FILE(TestAsset, testasset)
		Ref<char> Serialize() override { return nullptr; }
		void Serialize(char* byteStream, unsigned int availableBytes) override {}
		unsigned int RequiredSpace() const override { return 0; }
		void Deserialize(char* inData, unsigned int size) override {}
	};

	// Exposes the protected GenerateAsset for testing via inheritance.
	class TestAssetFactory : public AssetFactory {
	public:
		static Ref<IAsset> MakeAsset(std::string ext, std::string fileName) {
			return GenerateAsset(ext, fileName);
		}
	};
}

using namespace GameSmith;

// ---------------------------------------------------------------------------
// Registry tests
// ---------------------------------------------------------------------------

class AssetRegistryTest : public ::testing::Test {
protected:
	void TearDown() override {
		AssetRegistry::GetInstance()->ClearRegistrations();
	}
};

TEST_F(AssetRegistryTest, GetInstance) {
	auto* reg = AssetRegistry::GetInstance();
	EXPECT_NE(reg, nullptr);
}

TEST_F(AssetRegistryTest, RegisteredAssetAppears) {
	auto* reg = AssetRegistry::GetInstance();
	reg->RegisterAsset(".testasset", "TestAsset",
		[](std::string name) { return new TestAsset(name); }
	);

	auto& assets = reg->ListRegisteredAssets();
	bool found = false;
	for (auto& pair : assets) {
		if (pair.first == ".testasset") {
			EXPECT_EQ(pair.second, "TestAsset");
			found = true;
			break;
		}
	}
	EXPECT_TRUE(found);
}

TEST_F(AssetRegistryTest, MultipleRegistrationsDeduplicate) {
	auto* reg = AssetRegistry::GetInstance();
	reg->RegisterAsset(".testasset", "TestAsset",
		[](std::string name) { return new TestAsset(name); }
	);
	auto before = reg->ListRegisteredAssets().size();
	reg->RegisterAsset(".testasset", "TestAsset",
		[](std::string name) { return new TestAsset(name); }
	);
	auto after = reg->ListRegisteredAssets().size();
	EXPECT_EQ(before, after);
}

TEST_F(AssetRegistryTest, InstanceIsSingleton) {
	auto* a = AssetRegistry::GetInstance();
	auto* b = AssetRegistry::GetInstance();
	EXPECT_EQ(a, b);
}

// ---------------------------------------------------------------------------
// Factory tests — access GenerateAsset through TestAssetFactory subclass
// ---------------------------------------------------------------------------

class AssetFactoryTest : public ::testing::Test {
protected:
	void SetUp() override {
		AssetRegistry::GetInstance()->RegisterAsset(".testasset", "TestAsset",
			[](std::string name) { return new TestAsset(name); }
		);
	}

	void TearDown() override {
		AssetRegistry::GetInstance()->ClearRegistrations();
	}
};

TEST_F(AssetFactoryTest, GenerateAssetReturnsNonNull) {
	auto asset = TestAssetFactory::MakeAsset(".testasset", "MyAsset");
	EXPECT_NE(asset, nullptr);
}

TEST_F(AssetFactoryTest, GenerateAssetSetsName) {
	auto asset = TestAssetFactory::MakeAsset(".testasset", "MyAsset");
	EXPECT_EQ(asset->GetName(), "MyAsset");
}

TEST_F(AssetFactoryTest, GenerateAssetReturnsCorrectType) {
	auto asset = TestAssetFactory::MakeAsset(".testasset", "X");
	EXPECT_EQ(asset->GetFileExtension(), ".testasset");
	EXPECT_EQ(asset->GetFileType(), "TestAsset");
}

TEST_F(AssetFactoryTest, GenerateMultipleAssets) {
	auto a = TestAssetFactory::MakeAsset(".testasset", "First");
	auto b = TestAssetFactory::MakeAsset(".testasset", "Second");
	EXPECT_NE(a, nullptr);
	EXPECT_NE(b, nullptr);
	EXPECT_NE(a.get(), b.get());
	EXPECT_EQ(a->GetName(), "First");
	EXPECT_EQ(b->GetName(), "Second");
}
