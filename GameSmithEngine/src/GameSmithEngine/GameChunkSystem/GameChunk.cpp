#include "gepch.h"
#include "GameChunk.h"
#include "GameSmithEngine/EntitySystem/GameObjectManager.h"
#include "GameSmithEngine/ResourceManagement/ResourceAssetHelper.h"

namespace GameSmith {
	GameChunk::~GameChunk()
	{
		auto gameObjectManager = GameObjectManager::GetInstance();

		for (auto gameObject : m_GameObjects) {
			gameObjectManager->DestroyGameObject(gameObject);
		}
	}

	Ref<char> GameChunk::Serialize()
	{
		ResourceAssetWriter writer(RequireSpace());

		ChunkMetadata metadata = {};
		for (auto& gm : m_GameObjects) {
			metadata.gmCount += gm.expired() ? 0 : 1;
		}

		writer.WriteClass<ChunkMetadata>(&metadata);

		for (auto& gm : m_GameObjects) {
			if (!gm.expired()) {
				gm.lock()->Serialize(writer.GetCurPtr(), writer.GetRemainingSpace());
				writer.MoveCurPtr(gm.lock()->RequireSpace());
			}
		}


		return writer.GetBuffer();
	}

	void GameChunk::Serialize(char* byteStream, unsigned int availableBytes)
	{
	}

	unsigned int GameChunk::RequireSpace() const
	{
		unsigned int size = sizeof(ChunkMetadata);

		for (auto& gm : m_GameObjects) {
			if (!gm.expired()) {
				size += gm.lock()->RequireSpace();
			}
		}

		return size;
	}

	void GameChunk::Deserialize(char* inData, unsigned int size)
	{
		std::unordered_map<ID, Connection<IDObject>, IDHasher> collectedIDs;
		std::vector<Connection<Component>> createdComps;

		ResourceAssetReader reader(inData, size);

		auto metadata = reader.ReadClass<ChunkMetadata>();

		auto gmManager = GameObjectManager::GetInstance();
		for (unsigned int i = 0; i < metadata->gmCount; i++) {
			auto gm = gmManager->CreateGameObject();

			gm.lock()->Deserialize(reader.GetCurPtr(), reader.GetRemainingBytes());
			collectedIDs.insert({ gm.lock()->GetID(), gm });

			std::vector<Connection<Component>> comps;
			gm.lock()->GetComponents(&comps);
			for (auto comp : comps) {
				collectedIDs.insert({ comp.lock()->GetID(), comp });
				createdComps.push_back(comp);
			}

			reader.MoveForward(gm.lock()->RequireSpace());

			m_GameObjects.push_back(gm);
		}

		for (auto& comp : createdComps) {
			std::unordered_map<std::string, Ref<RefContainer>> refs;
			comp.lock()->GenerateReferenceEntries(&refs);

			for (auto& entry : refs) {
				if (collectedIDs.contains(entry.second->GetCurrentRefID())) {
					auto foundEntry = collectedIDs.find(entry.second->GetCurrentRefID());
					entry.second->AssignRef(foundEntry->second, 0);
				}
			}

			comp.lock()->BootstrapReferenceRegistry(refs);
		}
	}
};

