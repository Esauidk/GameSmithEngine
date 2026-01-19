#pragma once
#include <gepch.h>
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/EntitySystem/GameObject.h"
#include "GameSmithEngine/SerializeableFiles/Serializable.h"

#define GAMECHUNK_DEFAULT_NAME "NewChunk"

namespace GameSmith {
	class GE_API GameChunk : public Serializeable
	{
	public:
		GameChunk() : m_ChunkName(GAMECHUNK_DEFAULT_NAME) {};
		GameChunk(std::vector<Connection<GameObject>>& objects) : m_GameObjects(objects), m_ChunkName(GAMECHUNK_DEFAULT_NAME) {};
		~GameChunk();

		inline void AddObjectToChunk(Connection<GameObject> object) { m_GameObjects.push_back(object); }
		inline void SetChunkName(std::string& name) { m_ChunkName = name; }
		inline std::string GetChunkName() const { return m_ChunkName; }
		void GetChunkGameObjectNames(std::vector<std::string>* nameOut);

		virtual Ref<char> Serialize() override;
		virtual void Serialize(char* byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequiredSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;
	private:
		std::vector<Connection<GameObject>> m_GameObjects;
	private:
		std::string m_ChunkName;
	};
};


