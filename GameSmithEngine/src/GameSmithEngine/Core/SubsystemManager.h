#pragma once
#include "GameSmithEngine/ResourceManagement/ResourceManager.h"
#include "GameSmithEngine/Rendering/RenderingManager.h"
#include "GameSmithEngine/EntitySystem/GameObjectManager.h"
#include "GameSmithEngine/EntitySystem/GameplayUpdater.h"
#include "GameSmithEngine/GameChunkSystem/ChunkManager.h"

namespace GameSmith {
	class SubsystemManager
	{
	public:
		SubsystemManager();
		~SubsystemManager();
		void Update(float dt);
		void EndUpdate();
	private:
		ResourceManager m_ResourceManager;
		RenderingManager m_RenderingManager;
		ChunkManager m_SceneManager;
		GameObjectManager m_GameObjectManager;
		GameplayUpdater m_GameplayUpdater;
	};
};

