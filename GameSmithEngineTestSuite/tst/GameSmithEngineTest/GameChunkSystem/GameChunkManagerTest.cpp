#include "gtest/gtest.h"
#include "GameSmithEngine/EntitySystem/GameObjectManager.h"
#include "GameSmithEngine/GameChunkSystem/ChunkManager.h"
#include "GameSmithEngine/GameChunkSystem/GameChunk.h"

TEST(GameChunkManagerTest, ChunkLoaded) {
	GameSmith::GameObjectManager manager(true);
	GameSmith::ChunkManager chunkManager;
	auto gameObject = manager.CreateGameObject(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));

	{
		std::vector<GameSmith::Connection<GameSmith::GameObject>> objects;
		objects.push_back(gameObject);
		auto chunk = GameSmith::Ref<GameSmith::GameChunk>(new GameSmith::GameChunk(objects));
		chunkManager.LoadChunk("Test Chunk", chunk);
	}
	
	manager.CleanGameObjects();

	EXPECT_FALSE(gameObject.expired());
}

TEST(GameChunkManagerTest, ChunkOverloaded) {
	GameSmith::GameObjectManager manager(true);
	GameSmith::ChunkManager chunkManager;
	auto gameObject = manager.CreateGameObject(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
	auto gameObject1 = manager.CreateGameObject(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));

	{
		std::vector<GameSmith::Connection<GameSmith::GameObject>> objects;
		objects.push_back(gameObject);
		auto chunk = GameSmith::Ref<GameSmith::GameChunk>(new GameSmith::GameChunk(objects));
		chunkManager.LoadChunk("Test Chunk", chunk);
	}

	manager.CleanGameObjects();

	EXPECT_FALSE(gameObject.expired());
	EXPECT_FALSE(gameObject1.expired());

	{
		std::vector<GameSmith::Connection<GameSmith::GameObject>> objects;
		objects.push_back(gameObject1);
		auto chunk = GameSmith::Ref<GameSmith::GameChunk>(new GameSmith::GameChunk(objects));
		chunkManager.LoadChunk("Test Chunk", chunk);
	}

	manager.CleanGameObjects();
	EXPECT_TRUE(gameObject.expired());
	EXPECT_FALSE(gameObject1.expired());
}

TEST(GameChunkManagerTest, ChunkAppended) {
	GameSmith::GameObjectManager manager(true);
	GameSmith::ChunkManager chunkManager;
	auto gameObject = manager.CreateGameObject(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
	auto gameObject1 = manager.CreateGameObject(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));

	{
		std::vector<GameSmith::Connection<GameSmith::GameObject>> objects;
		objects.push_back(gameObject);
		auto chunk = GameSmith::Ref<GameSmith::GameChunk>(new GameSmith::GameChunk(objects));
		chunkManager.LoadChunk("Test Chunk", chunk);
	}

	manager.CleanGameObjects();

	EXPECT_FALSE(gameObject.expired());
	EXPECT_FALSE(gameObject1.expired());

	{
		std::vector<GameSmith::Connection<GameSmith::GameObject>> objects;
		objects.push_back(gameObject1);
		auto chunk = GameSmith::Ref<GameSmith::GameChunk>(new GameSmith::GameChunk(objects));
		chunkManager.AppendChunk("Test Chunk1", chunk);
	}

	manager.CleanGameObjects();

	EXPECT_FALSE(gameObject.expired());
	EXPECT_FALSE(gameObject1.expired());
}

TEST(GameChunkManagerTest, ChunkAppendedOverloaded) {
	GameSmith::GameObjectManager manager(true);
	GameSmith::ChunkManager chunkManager;
	auto gameObject = manager.CreateGameObject(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
	auto gameObject1 = manager.CreateGameObject(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
	auto gameObject2 = manager.CreateGameObject(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));

	{
		std::vector<GameSmith::Connection<GameSmith::GameObject>> objects;
		objects.push_back(gameObject);
		auto chunk = GameSmith::Ref<GameSmith::GameChunk>(new GameSmith::GameChunk(objects));
		chunkManager.LoadChunk("Test Chunk", chunk);
	}

	manager.CleanGameObjects();

	EXPECT_FALSE(gameObject.expired());
	EXPECT_FALSE(gameObject1.expired());
	EXPECT_FALSE(gameObject2.expired());

	{
		std::vector<GameSmith::Connection<GameSmith::GameObject>> objects;
		objects.push_back(gameObject1);
		auto chunk = GameSmith::Ref<GameSmith::GameChunk>(new GameSmith::GameChunk(objects));
		chunkManager.AppendChunk("Test Chunk1", chunk);
	}

	manager.CleanGameObjects();

	EXPECT_FALSE(gameObject.expired());
	EXPECT_FALSE(gameObject1.expired());
	EXPECT_FALSE(gameObject2.expired());

	{
		std::vector<GameSmith::Connection<GameSmith::GameObject>> objects;
		objects.push_back(gameObject2);
		auto chunk = GameSmith::Ref<GameSmith::GameChunk>(new GameSmith::GameChunk(objects));
		chunkManager.LoadChunk("Test Chunk2", chunk);
	}

	manager.CleanGameObjects();

	EXPECT_TRUE(gameObject.expired());
	EXPECT_TRUE(gameObject1.expired());
	EXPECT_FALSE(gameObject2.expired());
}