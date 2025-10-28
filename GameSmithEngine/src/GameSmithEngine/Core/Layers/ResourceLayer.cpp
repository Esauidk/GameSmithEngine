#include "gepch.h"
#include "ResourceLayer.h"
#include "GameSmithEngine/ResourceManagement/ResourceManager.h"

// Temporary
#include "GameSmithEngine/Core/Application.h"

namespace GameSmith {
	void ResourceLayer::OnAttach()
	{
		ResourceManager::Init(ResourceLoaderType::Heap);

		// TODO: Use more complex system for starting resource 
		auto specs = Application::Get().GetApplicationSpecs();
		if (specs.CommandLineArgs.Count >= 5) {
			if (std::string(specs.CommandLineArgs.Args[3]) == "--startDir") {
				auto instance = ResourceManager::GetInstance();
				instance->SetAssestDirectory(specs.CommandLineArgs.Args[4]);
				instance->ScanResources();
			}
		}
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

