#include "gepch.h"
#include "EntitySystemLayer.h"
#include "GameSmithEngine/EntitySystem/GameObjectManager.h"
#include "GameSmithEngine/EntitySystem/GameplayUpdater.h"
#include "GameSmithEngine/GameChunkSystem/ChunkManager.h"

// Temporary for starting game chunk
#include "GameSmithEngine/Core/Application.h"
#include "GameSmithEngine/ResourceManagement/ResourceManager.h"

namespace GameSmith {
	void EntitySystemLayer::OnAttach()
	{
		GameObjectManager::Init();
		GameplayUpdater::Init();
		ChunkManager::Init();

		// TODO: Use more complex system for starting game chunk
		auto specs = Application::Get().GetApplicationSpecs();
		if (specs.CommandLineArgs.Count >= 3) {
			if (std::string(specs.CommandLineArgs.Args[1]) ==  "--startChunk") {
				auto chunkId = ResourceManager::GetInstance()->GetAssetID(std::string(specs.CommandLineArgs.Args[2]));
				ChunkManager::GetInstance()->LoadChunk(chunkId);
			}
		}
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

