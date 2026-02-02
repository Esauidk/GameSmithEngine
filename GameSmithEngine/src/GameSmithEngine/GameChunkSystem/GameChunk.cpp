#include "gepch.h"
#include "GameChunk.h"
#include "GameSmithEngine/EntitySystem/GameObjectManager.h"
#include "GameSmithEngine/ResourceManagement/AssetManager.h"
#include "GameSmithEngine/ResourceManagement/ResourceAssetHelper.h"
#include "GameSmithEngine/SerializeableFiles/ResourceAssets/Asset.h"

namespace GameSmith {
	GameChunk::~GameChunk()
	{
		auto gameObjectManager = GameObjectManager::GetInstance();

		for (auto gameObject : m_GameObjects) {
			gameObjectManager->DestroyGameObject(gameObject);
		}
	}

	void GameChunk::GetChunkGameObjectNames(std::vector<std::string>* nameOut)
	{
		for (auto& object : m_GameObjects) {
			if (!object.expired()) {
				nameOut->push_back(object.lock()->GetName());
			}
		}
	}

	Ref<char> GameChunk::Serialize()
	{
		BinaryStreamWriter writer(RequiredSpace());

		unsigned int gmCount = 0;
		for (auto& gm : m_GameObjects) {
			gmCount += gm.expired() ? 0 : 1;
		}

		writer.WriteUInt(gmCount);

		for (auto& gm : m_GameObjects) {
			if (!gm.expired()) {
				gm.lock()->Serialize(writer.GetCurPtr(), writer.GetRemainingSpace());
				writer.MoveCurPtr(gm.lock()->RequiredSpace());
			}
		}

		return writer.GetBuffer();
	}

	void GameChunk::Serialize(char* byteStream, unsigned int availableBytes)
	{
	}

	unsigned int GameChunk::RequiredSpace() const
	{
		unsigned int size = sizeof(unsigned int);

		for (auto& gm : m_GameObjects) {
			if (!gm.expired()) {
				size += gm.lock()->RequiredSpace();
			}
		}

		return size;
	}

	void GameChunk::Deserialize(char* inData, unsigned int size)
	{
		std::unordered_map<ID, Ref<IDObject>, IDHasher> collectedIDs;
		std::vector<Connection<Component>> createdComps;

		BinaryStreamReader reader(inData, size);

		unsigned int gmCount = reader.GetUInt();

		auto gmManager = GameObjectManager::GetInstance();
		for (unsigned int i = 0; i < gmCount; i++) {

			// Create a game object (treat it as a fresh canvas)
			auto gm = gmManager->CreateGameObject();

			// Deserialize gameobject data into this blank gameobject
			gm.lock()->Deserialize(reader.GetCurPtr(), reader.GetRemainingBytes());

			// Record the ID of this gameobject
			collectedIDs.insert({ gm.lock()->GetID(), gm.lock() });

			// Go through all deserialized component and store IDs and their references
			std::vector<Connection<Component>> comps;
			gm.lock()->GetComponents(&comps);
			for (auto comp : comps) {
				collectedIDs.insert({ comp.lock()->GetID(), comp.lock() });
				createdComps.push_back(comp);
			}

			reader.MoveForward(gm.lock()->RequiredSpace());

			m_GameObjects.push_back(gm);
		}

		// Go through each component and attempt to fill in references
		for (auto& comp : createdComps) {
			std::unordered_map<std::string, Ref<ConnectionContainer>> connnectionRefs;
			comp.lock()->GenerateConnectionEntries(&connnectionRefs);

			std::unordered_map<std::string, Ref<AssetRefContainer>> assetRefs;
			comp.lock()->GenerateAssetEntries(&assetRefs);

			for (auto& entry : connnectionRefs) {
				if (collectedIDs.contains(entry.second->GetCurrentRefID())) {
					auto foundEntry = collectedIDs.find(entry.second->GetCurrentRefID());
					entry.second->AssignRef(foundEntry->second);
				}

				// TODO Add case for serialized connection objects, i.e. GameObjects
			}

			for (auto& entry : assetRefs) {
				if (collectedIDs.contains(entry.second->GetCurrentRefID())) {
					auto foundEntry = collectedIDs.find(entry.second->GetCurrentRefID());
					entry.second->AssignRef(CastPtr<Asset>(foundEntry->second));
				}
				else {
					// Attempt to load asset
					auto resourceManager = AssetManager::GetInstance();
					auto asset = resourceManager->GetResource(entry.second->GetCurrentRefID());
					// TODO: Change resource manager to return asset class for default
					entry.second->AssignRef(CastPtr<Asset>(asset));
					collectedIDs.insert({ asset->GetID(), asset });
				}
			}

			comp.lock()->BootstrapConnectionRegistry(connnectionRefs);
			comp.lock()->BootstrapAssetRegistry(assetRefs);
		}
	}
};

