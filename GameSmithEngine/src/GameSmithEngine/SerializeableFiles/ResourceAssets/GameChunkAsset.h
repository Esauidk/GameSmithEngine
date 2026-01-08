#pragma once
#include "Asset.h"
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/GameChunkSystem/GameChunk.h"

namespace GameSmith {
	class GE_API GameChunkAsset : public Asset
	{
	public:
		GameChunkAsset() = default;
		GameChunkAsset(Ref<GameChunk> snapShot);
		virtual Ref<char> Serialize() override;
		virtual void Serialize(char* byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequiredSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;

		SERIAL_FILE(Game Chunk, chunk)

		Ref<GameChunk> GenerateInstance();
	private:
		Ref<char> m_ChunkSerialization;
		unsigned int m_ByteSize = 0;
	};
};
