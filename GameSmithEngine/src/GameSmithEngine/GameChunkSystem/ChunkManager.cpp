#include "gepch.h"
#include "ChunkManager.h"
#include "GameSmithEngine/ResourceManagement/ResourceManager.h"

namespace GameSmith {
	ChunkManager* ChunkManager::s_Instance = nullptr;

	ChunkManager::ChunkManager() : m_LoadedChunks(new ChunkMap())
	{
	}

	void ChunkManager::Init()
	{
		GE_CORE_INFO("SceneManager Loaded");

		if (s_Instance == nullptr) {
			s_Instance = this;
		}
	}

	void ChunkManager::LoadChunk(ID chunkAsset)
	{
		m_LoadedChunks->chunks.clear();
		AppendChunk(chunkAsset);
		m_CurrentMainChunk = m_LoadedChunks->chunks[chunkAsset];
	}

	void ChunkManager::LoadChunk(ID chunkAsset, Ref<GameChunk> gameChunk)
	{
		m_LoadedChunks->chunks.clear();
		AppendChunk(chunkAsset, gameChunk);
		m_CurrentMainChunk = m_LoadedChunks->chunks[chunkAsset];
	}

	void ChunkManager::AppendChunk(ID chunkAsset)
	{
		if (!m_LoadedChunks->chunks.contains(chunkAsset)) {
			auto resourceManager = ResourceManager::GetInstance();

			auto loadedAsset = resourceManager->GetResource<GameChunkAsset>(chunkAsset);

			m_LoadedChunks->chunks.insert({ chunkAsset, loadedAsset->GenerateInstance() });
		}
	}

	void ChunkManager::AppendChunk(ID chunkName, Ref<GameChunk> gameChunk)
	{
		if (!m_LoadedChunks->chunks.contains(chunkName)){
			m_LoadedChunks->chunks.insert({ chunkName, gameChunk });
		}
	}

};
