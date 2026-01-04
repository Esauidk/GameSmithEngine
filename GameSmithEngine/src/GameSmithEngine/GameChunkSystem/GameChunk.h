#pragma once
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/EntitySystem/GameObject.h"
#include "GameSmithEngine/ResourceAssets/Serializable.h"

namespace GameSmith {
	class GE_API GameChunk : public Serializeable
	{
	public:
		GameChunk() = default;
		GameChunk(std::vector<Connection<GameObject>>& objects) : m_GameObjects(objects) {};
		inline void AddObjectToChunk(Connection<GameObject> object) { m_GameObjects.push_back(object); }
		~GameChunk();

		virtual Ref<char> Serialize() override;
		virtual void Serialize(char* byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequiredSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;
	private:
		std::vector<Connection<GameObject>> m_GameObjects;
	private:
		struct ChunkMetadata {
			unsigned int gmCount;
		};
	};
};


