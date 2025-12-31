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
		ContentLibraryManager::Init();

		// TODO: Use more complex system for starting resource 
		auto specs = Application::Get().GetApplicationSpecs();
		if (specs.CommandLineArgs.Count >= 5) {
			if (std::string(specs.CommandLineArgs.Args[3]) == "--startDir") {
				auto instance = ResourceManager::GetInstance();
				instance->SetAssestDirectory(specs.CommandLineArgs.Args[4]);
				instance->ScanResources();
			}
		}

		//ContentLibraryManager::GetInstance()->LoadContentLibrary("DefaultContentLibrary", "C:/Users/esaus/Documents/Coding Projects/GameSmithSampleGame/bin/Debug-windows-x86_64/DefaultGameProject/DefaultGameProject.dll");
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

