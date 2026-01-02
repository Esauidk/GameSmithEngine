#include "gepch.h"
#include "ResourceLayer.h"
#include "GameSmithEngine/ResourceManagement/ResourceManager.h"
#include "GameSmithEngine/ContentLibrarySystem/ContentLibraryManager.h"

#include "CLI/CLI.hpp"

namespace GameSmith {
	void ResourceLayer::OnAttach(const ApplicationSpecs& specs)
	{
		ResourceManager::Init(ResourceLoaderType::Heap);

		CLI::App app{ "Resource Layer Arguments" };

		std::string assetDir = "";
		std::string contentDir = "";
		app.add_option("--assetDir", assetDir, "The directory to pull assets from");
		app.add_option("--contentDir", contentDir, "The directory to pull content libraries from");

		try {
			app.parse(specs.CommandLineArgs.Count, specs.CommandLineArgs.Args);
		}
		catch (const CLI::ParseError &e) {
			GE_CORE_WARN("Issue parsing resource layer arguments, {}", app.exit(e));
		}

		if (contentDir != "") {
			ResourceManager::GetInstance()->SetContentLibraryDirectory(contentDir);
			ResourceManager::GetInstance()->ScanContentLibraries();
		}

		if (assetDir != "") {
			ResourceManager::GetInstance()->SetAssetDirectory(assetDir);
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

