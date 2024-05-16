#pragma once
#include "GameSmithEngine/GameChunkSystem/GameChunk.h"
#include "Serializable.h"

namespace GameSmith {
	class GameChunkAsset : public Serializeable
	{
	public:
		virtual Ref<char> Serialize() override;
		virtual void Serialize(Ref<char> byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequireSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;

		Ref<GameChunk> GenerateInstance();
	};
};
