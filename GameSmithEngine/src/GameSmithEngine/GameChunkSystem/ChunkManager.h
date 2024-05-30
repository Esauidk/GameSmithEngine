#pragma once

#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/GameChunkSystem/GameChunk.h"
#include "GameSmithEngine/ResourceAssets/GameChunkAsset.h"

namespace GameSmith {
	class ChunkManager
	{
	public:
		inline static ChunkManager* GetInstance() { return s_Instance; }
		void Init();
		void ShutDown() {};

		void LoadChunk(ID sceneAsset);
		// Visible for testing
		void LoadChunk(ID sceneAsset, Ref<GameChunk> gameChunk);

		void AppendChunk(ID chunkAsset);
		// Visible for testing
		void AppendChunk(ID chunkName, Ref<GameChunk> gameChunk);

	private:
		static ChunkManager* s_Instance;

		std::unordered_map<ID, Ref<GameChunk>, ID> m_LoadedChunks;
	};
};


