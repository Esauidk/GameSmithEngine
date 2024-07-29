#pragma once
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/GameChunkSystem/GameChunk.h"
#include "Serializable.h"

namespace GameSmith {
	class GE_API GameChunkAsset : public Serializeable
	{
	public:
		GameChunkAsset() = default;
		GameChunkAsset(Ref<GameChunk> snapShot);
		virtual Ref<char> Serialize() override;
		virtual void Serialize(char* byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequireSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;

		SERIAL_FILE(Game Chunk, chunk)

		Ref<GameChunk> GenerateInstance();
	private:
		Ref<char> m_ChunkSerialization;
		unsigned int m_ByteSize;
	};
};
