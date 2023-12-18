#include "gtest/gtest.h"
#include "GameSmithEngine/EntitySystem/GameObjectManager.h"
#include "GameSmithEngine/GameChunkSystem/GameChunk.h"

TEST(GameChunkTest, DeleteGameObjects) {
	GameSmith::GameObjectManager manager(true);
	auto gameObject = manager.CreateGameObject(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
	std::vector<GameSmith::Connection<GameSmith::GameObject>> objects;
	objects.push_back(gameObject);
	
	{
		GameSmith::GameChunk chunk(objects);
	}

	manager.CleanGameObjects();

	EXPECT_TRUE(gameObject.expired());
}