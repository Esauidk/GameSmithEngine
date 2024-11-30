#include "gtest/gtest.h"
#include "GameSmithEngine/EntitySystem/GameObjectManager.h"

TEST(GameObjectManager, CreateSingleGameObject) {
	GameSmith::GameObjectManager manager;
	glm::vec3 startPos(1, 2, 3);
	glm::vec3 startRot(4, 5, 6);

	auto gameObject = manager.CreateGameObject(startPos, startRot).lock();
	EXPECT_EQ(gameObject->GetName(), GameSmith::GameObject::DEFAULT_GAMEOBJECT_NAME);

	auto transform = gameObject->GetTransform();
	EXPECT_EQ(transform.lock()->GetPosition(), startPos);
	EXPECT_EQ(transform.lock()->GetRotation(), startRot);
}

TEST(GameObjectManager, CreateMultpleGameObjects) {
	GameSmith::GameObjectManager manager;
	glm::vec3 startPos(1, 2, 3);
	glm::vec3 startRot(4, 5, 6);

	auto gameObject = manager.CreateGameObject(startPos, startRot).lock();
	EXPECT_NE(gameObject, nullptr);
	EXPECT_EQ(gameObject->GetName(), GameSmith::GameObject::DEFAULT_GAMEOBJECT_NAME);

	auto transform = gameObject->GetTransform();
	EXPECT_EQ(transform.lock()->GetPosition(), startPos);
	EXPECT_EQ(transform.lock()->GetRotation(), startRot);

	gameObject = manager.CreateGameObject(startPos, startRot).lock();
	EXPECT_NE(gameObject, nullptr);
	EXPECT_EQ(gameObject->GetName(), std::format("{} ({})", GameSmith::GameObject::DEFAULT_GAMEOBJECT_NAME, 1));

	transform = gameObject->GetTransform();
	EXPECT_EQ(transform.lock()->GetPosition(), startPos);
	EXPECT_EQ(transform.lock()->GetRotation(), startRot);
}

TEST(GameObjectManager, FindNoGameObject) {
	GameSmith::GameObjectManager manager;
	EXPECT_EQ(manager.FindGameObject("").lock().get(), nullptr);
}

TEST(GameObjectManager, FindGameObject) {
	GameSmith::GameObjectManager manager;
	glm::vec3 startPos(1, 2, 3);
	glm::vec3 startRot(4, 5, 6);

	manager.CreateGameObject(startPos, startRot);
	EXPECT_NE(manager.FindGameObject(GameSmith::GameObject::DEFAULT_GAMEOBJECT_NAME).lock().get(), nullptr);
}

TEST(GameObjectManager, CleanNoGameObjects) {
	GameSmith::GameObjectManager manager;
	EXPECT_NO_THROW(manager.CleanGameObjects());
}

TEST(GameObjectManager, CleanGameObject) {
	GameSmith::GameObjectManager manager;
	glm::vec3 startPos(1, 2, 3);
	glm::vec3 startRot(4, 5, 6);

	manager.CreateGameObject(startPos, startRot);
	auto gameObject = manager.FindGameObject(GameSmith::GameObject::DEFAULT_GAMEOBJECT_NAME);
	EXPECT_NE(gameObject.lock().get(), nullptr);

	manager.DestroyGameObject(gameObject);
	EXPECT_EQ(manager.FindGameObject(GameSmith::GameObject::DEFAULT_GAMEOBJECT_NAME).lock().get(), nullptr);

	EXPECT_NO_THROW(manager.CleanGameObjects());
	EXPECT_TRUE(gameObject.expired());
}

TEST(GameObjectManager, DeleteGameObject) {
	GameSmith::GameObjectManager manager;
	glm::vec3 startPos(1, 2, 3);
	glm::vec3 startRot(4, 5, 6);

	manager.CreateGameObject(startPos, startRot);
	auto gameObject = manager.FindGameObject(GameSmith::GameObject::DEFAULT_GAMEOBJECT_NAME);
	EXPECT_NE(gameObject.lock().get(), nullptr);

	manager.DestroyGameObject(gameObject);
	EXPECT_EQ(manager.FindGameObject(GameSmith::GameObject::DEFAULT_GAMEOBJECT_NAME).lock().get(), nullptr);
}

TEST(GameObjectManager, DeleteUntrackedGameObject) {
	GameSmith::Ref<GameSmith::GameObject> obj = GameSmith::Ref<GameSmith::GameObject>(new GameSmith::GameObject());

	GameSmith::GameObjectManager manager;
	EXPECT_NO_THROW(manager.DestroyGameObject(obj));
}

TEST(GameObjectManager, CleanOnShutDown) {
	GameSmith::GameObjectManager::Init();
	GameSmith::GameObjectManager* manager = GameSmith::GameObjectManager::GetInstance();
	glm::vec3 startPos(1, 2, 3);
	glm::vec3 startRot(4, 5, 6);

	manager->CreateGameObject(startPos, startRot);
	auto gameObject = manager->FindGameObject(GameSmith::GameObject::DEFAULT_GAMEOBJECT_NAME);
	EXPECT_NE(gameObject.lock().get(), nullptr);

	manager->DestroyGameObject(gameObject);
	EXPECT_EQ(manager->FindGameObject(GameSmith::GameObject::DEFAULT_GAMEOBJECT_NAME).lock().get(), nullptr);

	EXPECT_NO_THROW(GameSmith::GameObjectManager::ShutDown());
	EXPECT_TRUE(gameObject.expired());
}