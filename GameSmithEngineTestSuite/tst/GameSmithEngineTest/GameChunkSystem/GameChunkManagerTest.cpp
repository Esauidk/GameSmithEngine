#include "gtest/gtest.h"
#include "GameSmithEngine/EntitySystem/GameObjectManager.h"
#include "GameSmithEngine/GameChunkSystem/ChunkManager.h"
#include "GameSmithEngine/GameChunkSystem/GameChunk.h"

TEST(GameChunkManagerTest, ChunkLoaded) {
	GameSmith::GameObjectManager::Init();
	GameSmith::GameObjectManager* manager = GameSmith::GameObjectManager::GetInstance();
	GameSmith::ChunkManager::Init();
	GameSmith::ChunkManager* chunkManager = GameSmith::ChunkManager::GetInstance();
	auto gameObject = manager->CreateGameObject(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));

	{
		std::vector<GameSmith::Connection<GameSmith::GameObject>> objects;
		objects.push_back(gameObject);
		auto chunk = GameSmith::Ref<GameSmith::GameChunk>(new GameSmith::GameChunk(objects));
		chunkManager->LoadChunk({0, 0, 0, 0}, chunk);
	}
	
	manager->CleanGameObjects();

	EXPECT_FALSE(gameObject.expired());
	GameSmith::ChunkManager::Shutdown();
	GameSmith::GameObjectManager::ShutDown();
}

TEST(GameChunkManagerTest, ChunkOverloaded) {
	GameSmith::GameObjectManager::Init();
	GameSmith::GameObjectManager* manager = GameSmith::GameObjectManager::GetInstance();
	GameSmith::ChunkManager::Init();
	GameSmith::ChunkManager* chunkManager = GameSmith::ChunkManager::GetInstance();
	auto gameObject = manager->CreateGameObject(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
	auto gameObject1 = manager->CreateGameObject(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));

	{
		std::vector<GameSmith::Connection<GameSmith::GameObject>> objects;
		objects.push_back(gameObject);
		auto chunk = GameSmith::Ref<GameSmith::GameChunk>(new GameSmith::GameChunk(objects));
		chunkManager->LoadChunk({ 0, 0, 0, 0 }, chunk);
	}

	manager->CleanGameObjects();

	EXPECT_FALSE(gameObject.expired());
	EXPECT_FALSE(gameObject1.expired());

	{
		std::vector<GameSmith::Connection<GameSmith::GameObject>> objects;
		objects.push_back(gameObject1);
		auto chunk = GameSmith::Ref<GameSmith::GameChunk>(new GameSmith::GameChunk(objects));
		chunkManager->LoadChunk({ 0, 0, 0, 0 }, chunk);
	}

	manager->CleanGameObjects();
	EXPECT_TRUE(gameObject.expired());
	EXPECT_FALSE(gameObject1.expired());
	GameSmith::ChunkManager::Shutdown();
	GameSmith::GameObjectManager::ShutDown();
}

TEST(GameChunkManagerTest, ChunkAppended) {
	GameSmith::GameObjectManager::Init();
	GameSmith::GameObjectManager* manager = GameSmith::GameObjectManager::GetInstance();
	GameSmith::ChunkManager::Init();
	GameSmith::ChunkManager* chunkManager = GameSmith::ChunkManager::GetInstance();
	auto gameObject = manager->CreateGameObject(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
	auto gameObject1 = manager->CreateGameObject(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));

	{
		std::vector<GameSmith::Connection<GameSmith::GameObject>> objects;
		objects.push_back(gameObject);
		auto chunk = GameSmith::Ref<GameSmith::GameChunk>(new GameSmith::GameChunk(objects));
		chunkManager->LoadChunk({ 0, 0, 0, 0 }, chunk);
	}

	manager->CleanGameObjects();

	EXPECT_FALSE(gameObject.expired());
	EXPECT_FALSE(gameObject1.expired());

	{
		std::vector<GameSmith::Connection<GameSmith::GameObject>> objects;
		objects.push_back(gameObject1);
		auto chunk = GameSmith::Ref<GameSmith::GameChunk>(new GameSmith::GameChunk(objects));
		chunkManager->AppendChunk({ 0, 0, 0, 1 }, chunk);
	}

	manager->CleanGameObjects();

	EXPECT_FALSE(gameObject.expired());
	EXPECT_FALSE(gameObject1.expired());
	GameSmith::ChunkManager::Shutdown();
	GameSmith::GameObjectManager::ShutDown();
}

TEST(GameChunkManagerTest, ChunkAppendedOverloaded) {
	GameSmith::GameObjectManager::Init();
	GameSmith::GameObjectManager* manager = GameSmith::GameObjectManager::GetInstance();
	GameSmith::ChunkManager::Init();
	GameSmith::ChunkManager* chunkManager = GameSmith::ChunkManager::GetInstance();
	auto gameObject = manager->CreateGameObject(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
	auto gameObject1 = manager->CreateGameObject(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
	auto gameObject2 = manager->CreateGameObject(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));

	{
		std::vector<GameSmith::Connection<GameSmith::GameObject>> objects;
		objects.push_back(gameObject);
		auto chunk = GameSmith::Ref<GameSmith::GameChunk>(new GameSmith::GameChunk(objects));
		chunkManager->LoadChunk({ 0, 0, 0, 0 }, chunk);
	}

	manager->CleanGameObjects();

	EXPECT_FALSE(gameObject.expired());
	EXPECT_FALSE(gameObject1.expired());
	EXPECT_FALSE(gameObject2.expired());

	{
		std::vector<GameSmith::Connection<GameSmith::GameObject>> objects;
		objects.push_back(gameObject1);
		auto chunk = GameSmith::Ref<GameSmith::GameChunk>(new GameSmith::GameChunk(objects));
		chunkManager->AppendChunk({ 0, 0, 0, 1 }, chunk);
	}

	manager->CleanGameObjects();

	EXPECT_FALSE(gameObject.expired());
	EXPECT_FALSE(gameObject1.expired());
	EXPECT_FALSE(gameObject2.expired());

	{
		std::vector<GameSmith::Connection<GameSmith::GameObject>> objects;
		objects.push_back(gameObject2);
		auto chunk = GameSmith::Ref<GameSmith::GameChunk>(new GameSmith::GameChunk(objects));
		chunkManager->LoadChunk({ 0, 0, 0, 2 }, chunk);
	}

	manager->CleanGameObjects();

	EXPECT_TRUE(gameObject.expired());
	EXPECT_TRUE(gameObject1.expired());
	EXPECT_FALSE(gameObject2.expired());
	GameSmith::ChunkManager::Shutdown();
	GameSmith::GameObjectManager::ShutDown();
}