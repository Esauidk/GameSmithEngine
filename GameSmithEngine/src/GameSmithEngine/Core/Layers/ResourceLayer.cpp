#include "gepch.h"
#include "ResourceLayer.h"
#include "GameSmithEngine/ResourceManagement/ResourceManager.h"

namespace GameSmith {
	void ResourceLayer::OnAttach()
	{
		ResourceManager::Init(ResourceLoaderType::Heap);
	}

	void ResourceLayer::OnDetach()
	{
		ResourceManager::Shutdown();
	}

	void ResourceLayer::OnUpdate(float dt)
	{
		auto rManager = ResourceManager::GetInstance();
		rManager->CleanResources();
	}
};

