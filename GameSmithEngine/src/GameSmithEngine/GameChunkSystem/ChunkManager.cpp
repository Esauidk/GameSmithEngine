#include "gepch.h"
#include "ChunkManager.h"

#include "GameSmithEngine/ResourceManagement/ResourceManager.h"

namespace GameSmith {
	ChunkManager* ChunkManager::s_Instance = nullptr;

	void ChunkManager::Init()
	{
		GE_CORE_INFO("SceneManager Loaded");

		if (s_Instance == nullptr) {
			s_Instance = this;
		}
	}

	void ChunkManager::LoadChunk(ID chunkAsset)
	{
		m_LoadedChunks.clear();
		AppendChunk(chunkAsset);
		m_CurrentMainChunk = m_LoadedChunks[chunkAsset];
	}

	void ChunkManager::LoadChunk(ID chunkAsset, Ref<GameChunk> gameChunk)
	{
		m_LoadedChunks.clear();
		AppendChunk(chunkAsset, gameChunk);
		m_CurrentMainChunk = m_LoadedChunks[chunkAsset];
	}

	void ChunkManager::AppendChunk(ID chunkAsset)
	{
		if (!m_LoadedChunks.contains(chunkAsset)) {
			auto resourceManager = ResourceManager::GetInstance();

			auto loadedAsset = resourceManager->GetResource<GameChunkAsset>(chunkAsset);

			m_LoadedChunks.insert({ chunkAsset, loadedAsset->GenerateInstance() });
		}
	}

	void ChunkManager::AppendChunk(ID chunkName, Ref<GameChunk> gameChunk)
	{
		if (!m_LoadedChunks.contains(chunkName)){
			m_LoadedChunks.insert({ chunkName, gameChunk });
		}
	}

};
