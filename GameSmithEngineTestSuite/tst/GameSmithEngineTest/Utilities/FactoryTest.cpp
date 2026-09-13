#include "gtest/gtest.h"
#include "GameSmithEngine/Utilities/Factory/FactoryImpl.h"

using namespace GameSmith;

struct TestKey {
	PassKey<TestKey> key;
} instance;

using TestFactory = Factory<std::string, TestKey>;

class FactoryTest : public ::testing::Test {
protected:
	void TearDown() override {
		TestFactory::GetInstance()->Clear();
	}
};

TEST_F(FactoryTest, GetInstance) {
	auto* reg = TestFactory::GetInstance();
	EXPECT_NE(reg, nullptr);
}

TEST_F(FactoryTest, RegisteredItemAppears) {
	auto* reg = TestFactory::GetInstance();
	reg->Register("TestKey", []() { return new std::string("hi"); });

	std::vector<std::string> keys;
	reg->GetRegisteredKeys(&keys);
	bool found = false;
	for (auto& key : keys) {
		if (key == "TestKey") {
			found = true;
			break;
		}
	}

	EXPECT_TRUE(found);
}

TEST_F(FactoryTest, MultipleRegistrationsDeduplicate) {
	auto* reg = TestFactory::GetInstance();
	reg->Register("TestKey", []() { return new std::string("hi"); });

	std::vector<std::string> keys;
	reg->GetRegisteredKeys(&keys);
	auto before = keys.size();

	reg->Register("TestKey", []() { return new std::string("hey"); });
	keys.clear();

	reg->GetRegisteredKeys(&keys);
	auto after = keys.size();
	EXPECT_EQ(before, after);
}

TEST_F(FactoryTest, InstanceIsSingleton) {
	auto* a = TestFactory::GetInstance();
	auto* b = TestFactory::GetInstance();
	EXPECT_EQ(a, b);
}


class FactoryCreateTest : public ::testing::Test {
public:
	inline static const std::string VALUE = "hey";
protected:
	void SetUp() override {
		TestFactory::GetInstance()->Register("TestFactoryComponent",
			[]() { return new std::string(VALUE); }
		);
	}

	void TearDown() override {
		TestFactory::GetInstance()->Clear();
	}
};

TEST_F(FactoryCreateTest, CreateReturnsNonNull) {
	auto str = TestFactory::GetInstance()->Create(instance.key, "TestFactoryComponent");
	EXPECT_NE(str, nullptr);
	EXPECT_EQ(*str, VALUE);
}

TEST_F(FactoryCreateTest, CreateMultiple) {
	auto a = TestFactory::GetInstance()->Create(instance.key, "TestFactoryComponent");
	auto b = TestFactory::GetInstance()->Create(instance.key, "TestFactoryComponent");
	EXPECT_NE(a, nullptr);
	EXPECT_NE(b, nullptr);
	EXPECT_NE(a, b);
}