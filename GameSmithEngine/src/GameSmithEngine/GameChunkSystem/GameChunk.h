#pragma once
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/EntitySystem/GameObject.h"
#include "GameSmithEngine/ResourceAssets/Serializable.h"

namespace GameSmith {
	class GameChunk : public Serializeable
	{
	public:
		GameChunk() = default;
		GameChunk(std::vector<Connection<GameObject>> objects) : m_GameObjects(objects) {};
		~GameChunk();

		virtual Ref<char> Serialize() override;
		virtual void Serialize(char* byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequireSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;
	private:
		std::vector<Connection<GameObject>> m_GameObjects;
	private:
		struct ChunkMetadata {
			unsigned int gmCount;
		};
	};
};


