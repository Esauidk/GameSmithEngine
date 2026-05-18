#include "gtest/gtest.h"
#include "GameSmithEngine/EntitySystem/Components/ComponentFactory.h"

// Test-only component type.
namespace GameSmith {
	class TestFactoryComponent : public Component {
	public:
		TestFactoryComponent(GameObject* go, Transform* t) : Component(go, t) {}
		void OnStart() override {}
		void OnUpdate(float dt) override {}
		void OnDestroy() override {}
		int GetPriority() const override { return 0; }
		std::string GetName() const override { return "TestFactoryComponent"; }
		Ref<char> Serialize() override { return nullptr; }
		void Serialize(char* byteStream, unsigned int availableBytes) override {}
		unsigned int RequiredSpace() const override { return 0; }
		void Deserialize(char* inData, unsigned int size) override {}
	};

	// Exposes the protected GenerateComponent for testing via inheritance.
	class TestComponentFactory : public ComponentFactory {
	public:
		static Ref<Component> MakeComponent(std::string className, GameObject* go, Transform* t) {
			return GenerateComponent(className, go, t);
		}
	};
}

using namespace GameSmith;

// ---------------------------------------------------------------------------
// Registry tests
// ---------------------------------------------------------------------------

class ComponentRegistryTest : public ::testing::Test {
protected:
	void SetUp() override {
		ComponentRegistry::GetInstance()->RegisterComponent("TestFactoryComponent",
			[](GameObject* g, Transform* t) { return new TestFactoryComponent(g, t); }
		);
	}

	void TearDown() override {
		ComponentRegistry::GetInstance()->ClearRegistrations();
	}
};

TEST_F(ComponentRegistryTest, GetInstance) {
	auto* reg = ComponentRegistry::GetInstance();
	EXPECT_NE(reg, nullptr);
}

TEST_F(ComponentRegistryTest, RegisteredComponentAppears) {
	std::vector<std::string> registered;
	ComponentRegistry::GetInstance()->ListRegisteredComponents(&registered);
	bool found = false;
	for (auto& name : registered) {
		if (name == "TestFactoryComponent") {
			found = true;
			break;
		}
	}
	EXPECT_TRUE(found);
}

TEST_F(ComponentRegistryTest, MultipleRegistrationsDeduplicate) {
	auto* reg = ComponentRegistry::GetInstance();
	std::vector<std::string> v;
	reg->ListRegisteredComponents(&v);

	auto before = v.size();
	reg->RegisterComponent("TestFactoryComponent",
		[](GameObject* g, Transform* t) { return new TestFactoryComponent(g, t); }
	);

	v.clear();
	reg->ListRegisteredComponents(&v);
	auto after = v.size();

	EXPECT_EQ(before, after);
}

// ---------------------------------------------------------------------------
// Factory tests
// ---------------------------------------------------------------------------

class ComponentFactoryTest : public ::testing::Test {
protected:
	void SetUp() override {
		ComponentRegistry::GetInstance()->RegisterComponent("TestFactoryComponent",
			[](GameObject* g, Transform* t) { return new TestFactoryComponent(g, t); }
		);
	}

	void TearDown() override {
		ComponentRegistry::GetInstance()->ClearRegistrations();
	}
};

TEST_F(ComponentFactoryTest, GenerateComponentReturnsNonNull) {
	auto comp = TestComponentFactory::MakeComponent("TestFactoryComponent", nullptr, nullptr);
	EXPECT_NE(comp, nullptr);
	EXPECT_EQ(comp->GetName(), "TestFactoryComponent");
}

TEST_F(ComponentFactoryTest, GenerateMultipleComponents) {
	auto a = TestComponentFactory::MakeComponent("TestFactoryComponent", nullptr, nullptr);
	auto b = TestComponentFactory::MakeComponent("TestFactoryComponent", nullptr, nullptr);
	EXPECT_NE(a.get(), b.get());
}
