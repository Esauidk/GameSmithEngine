#include "gepch.h"
#include "EnitityRenderPreparer.h"
#include "GameSmithEngine/Rendering/RenderingManager.h"


namespace GameSmith {
	EntityRenderPreparer* EntityRenderPreparer::s_Instance = nullptr;

	void EntityRenderPreparer::Init()
	{
		if (s_Instance == nullptr) {
			s_Instance = new EntityRenderPreparer();
		}
	}

	void EntityRenderPreparer::Shutdown()
	{
		if (s_Instance != nullptr) {
			delete s_Instance;
			s_Instance = nullptr;
		}
	}

	void EntityRenderPreparer::AddRenderRequest(RenderRequest req)
	{
		m_RenderRequests.push_back(req);
	}

	void EntityRenderPreparer::SetMainCamera(RenderableCamera cam)
	{
		m_MainCamera = cam;
	}

	void EntityRenderPreparer::AddAdditionalCamera(RenderableCamera cam)
	{
		GE_CORE_ASSERT(cam.targetTex != nullptr, "Additional Cameras need a target render texture");
		m_AdditionalCameras.push(cam);
	}

	void EntityRenderPreparer::SendForRendering()
	{
		auto renderMang = RenderingManager::GetInstance();
		auto frameTex = renderMang->GetTextureForFrame();
		renderMang->GetRenderAPI()->SetRenderTexture(frameTex, 0);

		renderMang->BeginScene(m_MainCamera.cam, nullptr);
		for (auto req : m_RenderRequests) {
			renderMang->Submit(req.vBuf, req.iBuf, req.mat);
		}
		renderMang->EndScene();


		while (!m_AdditionalCameras.empty()) {
			auto& cam = m_AdditionalCameras.front();
			m_AdditionalCameras.pop();

			auto renderMang = RenderingManager::GetInstance();

			renderMang->GetRenderAPI()->SetRenderTexture(cam.targetTex, 0);

			renderMang->BeginScene(cam.cam, nullptr);
			for (auto req : m_RenderRequests) {
				renderMang->Submit(req.vBuf, req.iBuf, req.mat);
			}
			renderMang->EndScene();
		}

		m_RenderRequests.clear();
		renderMang->GetRenderAPI()->SetRenderTexture(frameTex, 0);
	}
};

