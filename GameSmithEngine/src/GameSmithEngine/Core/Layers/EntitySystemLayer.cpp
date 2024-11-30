#include "gepch.h"
#include "EntitySystemLayer.h"
#include "GameSmithEngine/EntitySystem/GameObjectManager.h"
#include "GameSmithEngine/EntitySystem/GameplayUpdater.h"
#include "GameSmithEngine/GameChunkSystem/ChunkManager.h"

namespace GameSmith {
	void EntitySystemLayer::OnAttach()
	{
		GameObjectManager::Init();
		GameplayUpdater::Init();
		ChunkManager::Init();
	}

	void EntitySystemLayer::OnDetach()
	{
		ChunkManager::Shutdown();
		GameplayUpdater::ShutDown();
		GameObjectManager::ShutDown();
	}

	void EntitySystemLayer::OnUpdate(float dt)
	{
		auto goManager = GameObjectManager::GetInstance();
		goManager->CleanGameObjects();

		auto goUpdater = GameplayUpdater::GetInstance();
		goUpdater->RunGameplayInit();
		goUpdater->RunGameplayUpdate(dt);
		goUpdater->FlushRegistration();
	}
};

