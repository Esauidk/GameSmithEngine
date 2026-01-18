#include "gepch.h"
#include "GameSmithEngine/ContentLibrarySystem/ContentLibraryManager.h"
#include "GameSmithEngine/ResourceManagement/AssetManager.h"
#include "ResourceLayer.h"

#include "CLI/CLI.hpp"

namespace GameSmith {
	void ResourceLayer::OnAttach(const ApplicationSpecs& specs)
	{
		AssetManager::Init(ResourceLoaderType::Heap);

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

		if (assetDir != "") {
			AssetManager::GetInstance()->SetAssetDirectory(assetDir);
		}

		ContentLibraryManager::Init();
		if (contentDir != "") {
			ContentLibraryManager::GetInstance()->SetLibrarySearchPath(contentDir);
		}
	}

	void ResourceLayer::OnDetach()
	{
		AssetManager::Shutdown();
		ContentLibraryManager::Shutdown();
	}

	void ResourceLayer::OnUpdate(float dt)
	{
		auto rManager = AssetManager::GetInstance();
		rManager->CleanResources();
	}
};

