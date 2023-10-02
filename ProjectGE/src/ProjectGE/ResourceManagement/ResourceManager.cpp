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
};