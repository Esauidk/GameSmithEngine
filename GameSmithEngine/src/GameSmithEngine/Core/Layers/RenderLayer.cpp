#include "gepch.h"
#include "RenderLayer.h"

#include "GameSmithEngine/EntitySystem/EnitityRenderPreparer.h"
#include "GameSmithEngine/Rendering/RenderingManager.h"

namespace GameSmith {
	void RenderLayer::OnAttach()
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
		auto renderPerparer = EntityRenderPreparer::GetInstance();
		renderPerparer->SendForRendering();
	}

	void RenderLayer::OnEndUpdate()
	{
		auto renderManager = RenderingManager::GetInstance();
		renderManager->EndFrame();
	}
};
