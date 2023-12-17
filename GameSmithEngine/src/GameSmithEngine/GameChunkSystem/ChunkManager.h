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

		void LoadChunk(std::string sceneAsset);
		// Visible for testing
		void LoadChunk(std::string sceneAsset, Ref<GameChunk> gameChunk);

		void AppendChunk(std::string chunkAsset);
		// Visible for testing
		void AppendChunk(std::string chunkName, Ref<GameChunk> gameChunk);

	private:
		static ChunkManager* s_Instance;

		std::unordered_map<std::string, Ref<GameChunk>> m_LoadedChunks;
	};
};


