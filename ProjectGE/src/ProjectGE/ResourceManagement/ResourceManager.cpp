#include "gepch.h"
#include "ResourceManager.h"
#include "ProjectGE/Core/Log.h"

#include "ResourceLoaders/HeapResourceLoader.h"

namespace ProjectGE {
	ResourceManager* ResourceManager::s_Instance = nullptr;

	ResourceManager::ResourceManager()
	{
		if (s_Instance == nullptr) {
			s_Instance = this;
		}
	}

	void ResourceManager::Init()
	{
		m_Loader = Ref<ResourceLoader>(new HeapResourceLoader());
		GE_CORE_INFO("Resource Manager Loaded!");
	}

	void ResourceManager::Shutdown()
	{
	}

	void ResourceManager::ScaneResource()
	{
		auto it = m_ResourceRegistry.begin();

		while (it != m_ResourceRegistry.end()) {
			if (it->second.use_count() == 1) {
				GE_CORE_INFO("Erasing asset: {0}", it->first);
				it->second->Destroy();
				m_Loader->CleanResource(it->second->GetResourceData());

				it = m_ResourceRegistry.erase(it);
			}
			else {
				it++;
			}
		}
	}
};