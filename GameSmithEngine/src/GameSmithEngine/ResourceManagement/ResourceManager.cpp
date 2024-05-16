#include "gepch.h"
#include "ResourceManager.h"
#include "GameSmithEngine/Core/Log.h"

#include "ResourceLoaders/HeapResourceLoader.h"

namespace GameSmith {
	ResourceManager* ResourceManager::s_Instance = nullptr;

	static Ref<ResourceLoader> GetLoader(ResourceLoaderType loaderType) {
		switch (loaderType) {
		case ResourceLoaderType::Heap:
			return Ref<ResourceLoader>(new HeapResourceLoader());
		default:
			return Ref<ResourceLoader>(new HeapResourceLoader());
		}
	}

	ResourceManager::ResourceManager()
	{
		if (s_Instance == nullptr) {
			s_Instance = this;
		}
	}

	void ResourceManager::Init(ResourceLoaderType loaderType)
	{
		m_Loader = GetLoader(loaderType);
		GE_CORE_INFO("Resource Manager Loaded!");
	}

	void ResourceManager::Shutdown()
	{
	}

	void ResourceManager::ScanResource()
	{
		auto it = m_ResourceRegistry.begin();

		while (it != m_ResourceRegistry.end()) {
			if (it->second.use_count() == 1) {
				GE_CORE_INFO("Erasing asset: {0}", it->first);

				it = m_ResourceRegistry.erase(it);
			}
			else {
				it++;
			}
		}
	}
};