#include "gtest/gtest.h"

#include "GameSmithEngine/EntitySystem/GameObject.h"
#include "Components/TestComponent.h"

TEST(GameObjectTest, AddComponent) {
	GameSmith::GameObject gameObject("Test Object");
	
	GameSmith::Ref<TestComponent> test;
	EXPECT_NO_THROW(test = gameObject.AddComponent<TestComponent>());
	EXPECT_NE(test.get(), nullptr);
}

TEST(GameObjectTest, GetComponent) {
	GameSmith::GameObject gameObject("Test Object");

	GameSmith::Ref<TestComponent> test;
	EXPECT_NO_THROW(gameObject.AddComponent<TestComponent>());
	EXPECT_NO_THROW(test = gameObject.GetComponent<TestComponent>());
	EXPECT_NE(test.get(), nullptr);
}

TEST(GameObjectTest, GetName) {
	GameSmith::GameObject gameObject("Test Object");

	EXPECT_EQ(gameObject.GetName(), "Test Object");
}

TEST(GameObjectTest, GetTransform) {
	GameSmith::GameObject gameObject("Test Object");

	GameSmith::Ref<GameSmith::Transform> transform = gameObject.GetTransform();
	EXPECT_NE(transform.get(), nullptr);
}

TEST(GameObjectTest, RemoveComponent) {
	GameSmith::GameObject gameObject("Test Object");

	GameSmith::Ref<TestComponent> test;
	EXPECT_NO_THROW(test = gameObject.AddComponent<TestComponent>());
	EXPECT_NE(test.get(), nullptr);

	gameObject.RemoveComponent<TestComponent>(test);
	EXPECT_EQ(gameObject.GetComponent<TestComponent>(), nullptr);
}

TEST(GameObjectTest, UpdateComponent) {
	GameSmith::GameObject gameObject("Test Object");

	GameSmith::Ref<TestComponent> test;
	EXPECT_NO_THROW(test = gameObject.AddComponent<TestComponent>());
	EXPECT_NO_THROW(gameObject.OnUpdate());
	EXPECT_EQ(test->GetUpdateCheck(), true);
}