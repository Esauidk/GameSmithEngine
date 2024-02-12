#include "gtest/gtest.h"

#include "GameSmithEngine/EntitySystem/GameObject.h"
#include "Components/TestComponent.h"

TEST(GameObjectTest, AddComponent) {
	GameSmith::GameObject gameObject("Test Object");

	GameSmith::Connection<TestComponent> test;
	EXPECT_NO_THROW(test = gameObject.AddComponent<TestComponent>());
	EXPECT_FALSE(test.expired());
}

TEST(GameObjectTest, GetComponent) {
	GameSmith::GameObject gameObject("Test Object");

	GameSmith::Connection<TestComponent> test;
	EXPECT_NO_THROW(gameObject.AddComponent<TestComponent>());
	EXPECT_NO_THROW(test = gameObject.GetComponent<TestComponent>());
	EXPECT_FALSE(test.expired());
}

TEST(GameObjectTest, GetComponentError) {
	GameSmith::GameObject gameObject("Test Object");

	GameSmith::Connection<TestComponent> test;
	EXPECT_NO_THROW(test = gameObject.GetComponent<TestComponent>());
	EXPECT_TRUE(test.expired());
}

TEST(GameObjectTest, GetName) {
	GameSmith::GameObject gameObject("Test Object");

	EXPECT_EQ(gameObject.GetName(), "Test Object");
}

TEST(GameObjectTest, GetTransform) {
	GameSmith::GameObject gameObject("Test Object");

	GameSmith::Connection<GameSmith::Transform> transform = gameObject.GetTransform();
	EXPECT_FALSE(transform.expired());
}

TEST(GameObjectTest, RemoveComponent) {
	GameSmith::GameObject gameObject("Test Object");

	GameSmith::Connection<TestComponent> test;
	EXPECT_NO_THROW(test = gameObject.AddComponent<TestComponent>());
	EXPECT_FALSE(test.expired());

	gameObject.RemoveComponent<TestComponent>(test);
	EXPECT_TRUE(gameObject.GetComponent<TestComponent>().expired());
}
