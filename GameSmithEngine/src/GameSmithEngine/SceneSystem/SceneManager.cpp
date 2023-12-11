#include "gepch.h"
#include "SceneManager.h"

#include "GameSmithEngine/ResourceManagement/ResourceManager.h"

namespace GameSmith {
	void SceneManager::LoadScene(std::string sceneAsset)
	{
		if (m_LoadedSceneAssets.contains(sceneAsset)) {
			m_LoadedScenes.insert(
				{
					sceneAsset,
					m_LoadedSceneAssets.find(sceneAsset)->second->GenerateInstance()
				}
			);
		}
		else {
			auto resourceManager = ResourceManager::GetInstance();

			auto loadedAsset = resourceManager->GetResource<SceneAsset>(sceneAsset);
			
			m_LoadedSceneAssets.insert({ sceneAsset, loadedAsset });
			m_LoadedScenes.insert({ sceneAsset, loadedAsset->GenerateInstance() });
		}
		
	}

	void SceneManager::LoadScene(std::string sceneAsset, Ref<SceneAsset> asset)
	{
		m_LoadedScenes.insert({ sceneAsset, asset->GenerateInstance() });
	}
};
