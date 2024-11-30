#include "gepch.h"
#include "GameChunkAsset.h"
#include "AssetFactory.h"

namespace GameSmith {
	GE_REGISTERASSET(GameChunkAsset)

	GameChunkAsset::GameChunkAsset(Ref<GameChunk> snapShot) : m_ChunkSerialization(snapShot->Serialize()), m_ByteSize(snapShot->RequireSpace())
	{
	}

	Ref<char> GameChunkAsset::Serialize()
	{
		return m_ChunkSerialization;
	}

	void GameChunkAsset::Serialize(char* byteStream, unsigned int availableBytes)
	{
		GE_CORE_ASSERT(availableBytes >= m_ByteSize, "Not enough space to serialize the game chunk asset");
		memcpy(byteStream, m_ChunkSerialization.get(), m_ByteSize);
	}

	unsigned int GameChunkAsset::RequireSpace() const
	{
		return m_ByteSize;
	}

	void GameChunkAsset::Deserialize(char* inData, unsigned int size)
	{
		m_ChunkSerialization = Ref<char>(new char[size]);
		m_ByteSize = size;

		memcpy(m_ChunkSerialization.get(), inData, size);
	}

	Ref<GameChunk> GameChunkAsset::GenerateInstance()
	{
		Ref<GameChunk> chunk = Ref<GameChunk>(new GameChunk());
		chunk->Deserialize(m_ChunkSerialization.get(), m_ByteSize);
		return chunk;
	}
};
