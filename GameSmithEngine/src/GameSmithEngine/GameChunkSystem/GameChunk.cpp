#include "gepch.h"
#include "GameChunk.h"
#include "GameSmithEngine/EntitySystem/GameObjectManager.h"

namespace GameSmith {
	GameChunk::~GameChunk()
	{
		auto gameObjectManager = GameObjectManager::GetInstance();

		for (auto gameObject : m_GameObjects) {
			gameObjectManager->DestroyGameObject(gameObject);
		}
	}
};

