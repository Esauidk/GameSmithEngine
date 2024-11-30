#include "gepch.h"
#include "ChunkManager.h"
#include "GameSmithEngine/ResourceManagement/ResourceManager.h"

namespace GameSmith {
	ChunkManager* ChunkManager::s_Instance = nullptr;

	ChunkManager::ChunkManager() : m_LoadedChunks(new ChunkMap())
	{
		auto chunk = GameSmith::Ref<GameSmith::GameChunk>(new GameSmith::GameChunk());
		LoadChunk(chunk->GetID(), chunk);
	}

	void ChunkManager::Init()
	{
		if (s_Instance == nullptr) {
			s_Instance = new ChunkManager();
			GE_CORE_INFO("SceneManager Loaded");
		}
	}

	void ChunkManager::Shutdown()
	{
		if (s_Instance != nullptr) {
			delete s_Instance;
			s_Instance = nullptr;
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
