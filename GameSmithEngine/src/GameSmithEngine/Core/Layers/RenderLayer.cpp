#include "gepch.h"
#include "RenderLayer.h"

#include "GameSmithEngine/EntitySystem/EnitityRenderPreparer.h"
#include "GameSmithEngine/Rendering/RenderingManager.h"
#include "GameSmithEngine/ResourceManagement/AssetManager.h"

namespace GameSmith {
	void RenderLayer::OnAttach(const ApplicationSpecs& specs)
	{
		RenderingManager::Init();
		EntityRenderPreparer::Init();

		auto assetManager = AssetManager::GetInstance();
		if (assetManager) {
			std::vector<std::pair<std::string, std::string>> shaderFiles;
			assetManager->GetAssetPathsOfType<GameSmith::HLSLAsset>(&shaderFiles);

			std::unordered_map<std::string, GameSmith::ID> shaderFileCache;
			for (auto& entry : shaderFiles) {
				auto id = assetManager->GetAssetID(entry.second);
				shaderFileCache[entry.first] = id;
			}

			auto rm = GameSmith::RenderingManager::GetInstance();
			rm->SetShaderSourceCache(shaderFileCache);
		}
	}

	void RenderLayer::OnDetach()
	{
		EntityRenderPreparer::Shutdown();
		RenderingManager::ShutDown();
	}

	void RenderLayer::OnUpdate(float dt)
	{
		auto renderPreparer = EntityRenderPreparer::GetInstance();
		renderPreparer->SendForRendering();

		auto renderManager = RenderingManager::GetInstance();
		renderManager->SyncDataTransfer();
	}

	void RenderLayer::OnEndUpdate()
	{
		auto renderManager = RenderingManager::GetInstance();
		renderManager->EndFrame();
	}
};
