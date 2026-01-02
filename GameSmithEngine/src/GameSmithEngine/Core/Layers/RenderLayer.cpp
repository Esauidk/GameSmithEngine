#include "gepch.h"
#include "RenderLayer.h"

#include "GameSmithEngine/EntitySystem/EnitityRenderPreparer.h"
#include "GameSmithEngine/Rendering/RenderingManager.h"

namespace GameSmith {
	void RenderLayer::OnAttach(const ApplicationSpecs& specs)
	{
		RenderingManager::Init();
		EntityRenderPreparer::Init();
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
	}

	void RenderLayer::OnEndUpdate()
	{
		auto renderManager = RenderingManager::GetInstance();
		renderManager->EndFrame();
	}
};
