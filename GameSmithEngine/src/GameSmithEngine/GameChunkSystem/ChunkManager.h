#pragma once
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/GameChunkSystem/GameChunk.h"
#include "GameSmithEngine/ResourceAssets/GameChunkAsset.h"

namespace GameSmith {

	struct ChunkMap
	{
		std::unordered_map<ID, Ref<GameChunk>, IDHasher> chunks;
	};

	class GE_API ChunkManager
	{
	public:
		inline static ChunkManager* GetInstance() { return s_Instance; }

		static void Init();
		static void Shutdown();

		void LoadChunk(ID sceneAsset);
		// Visible for testing
		void LoadChunk(ID sceneAsset, Ref<GameChunk> gameChunk);

		void AppendChunk(ID chunkAsset);
		// Visible for testing
		void AppendChunk(ID chunkName, Ref<GameChunk> gameChunk);

		inline Connection<GameChunk> GetCurrentMainChunk() { return m_CurrentMainChunk; }
	private:
		ChunkManager();
	private:
		static ChunkManager* s_Instance;

		Scope<ChunkMap> m_LoadedChunks;
		Ref<GameChunk> m_CurrentMainChunk;
	};

	
};


