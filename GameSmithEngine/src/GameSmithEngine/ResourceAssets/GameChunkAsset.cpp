#include "gepch.h"
#include "GameChunkAsset.h"

namespace GameSmith {
	Ref<char> GameChunkAsset::Serialize()
	{
		// TODO: Implement
		return Ref<char>();
	}

	void GameChunkAsset::Serialize(char* byteStream, unsigned int availableBytes)
	{
		// TODO: Implement
	}

	unsigned int GameChunkAsset::RequireSpace() const
	{
		// TODO: Implement
		return 0;
	}

	void GameChunkAsset::Deserialize(char* inData, unsigned int size)
	{
		// TODO: Implement
	}

	Ref<GameChunk> GameChunkAsset::GenerateInstance()
	{
		return Ref<GameChunk>();
	}
};
