#include "gepch.h"
#include "ResourceLayer.h"
#include "GameSmithEngine/ResourceManagement/ResourceManager.h"
#include "GameSmithEngine/ContentLibrarySystem/ContentLibraryManager.h"

// Temporary
#include "GameSmithEngine/Core/Application.h"

namespace GameSmith {
	void ResourceLayer::OnAttach()
	{
		ResourceManager::Init(ResourceLoaderType::Heap);

		// TODO: Use more complex system for starting resource 
		auto specs = Application::Get().GetApplicationSpecs();
		if (specs.CommandLineArgs.Count >= 3) {
			if (std::string(specs.CommandLineArgs.Args[1]) == "--contentDir") {
				auto instance = ResourceManager::GetInstance();
				instance->SetContentLibraryDirectory(specs.CommandLineArgs.Args[2]);
				instance->ScanContentLibraries();
			}
		}

		auto& contentLibs = ResourceManager::GetInstance()->GetContentLibrariesFiles();
		ContentLibraryManager::Init(contentLibs);
	}

	void ResourceLayer::OnDetach()
	{
		ResourceManager::Shutdown();
		ContentLibraryManager::Shutdown();
	}

	void ResourceLayer::OnUpdate(float dt)
	{
		auto rManager = ResourceManager::GetInstance();
		rManager->CleanResources();
	}
};

