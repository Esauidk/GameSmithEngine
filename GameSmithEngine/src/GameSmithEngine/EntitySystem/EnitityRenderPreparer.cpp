#include "gepch.h"
#include "EnitityRenderPreparer.h"
#include "GameSmithEngine/Rendering/RenderingManager.h"


namespace GameSmith {
	EntityRenderPreparer* EntityRenderPreparer::s_Instance = nullptr;

	EntityRenderPreparer::EntityRenderPreparer()
	{
		if (s_Instance == nullptr) {
			s_Instance = this;
		}
	}

	void EntityRenderPreparer::AddRenderRequest(RenderRequest req)
	{
		m_RenderRequests.push_back(req);
	}

	void EntityRenderPreparer::AddCamera(RenderableCamera cam)
	{
		m_Cameras.push(cam);
	}

	void EntityRenderPreparer::SendForRendering()
	{
		while (!m_Cameras.empty()) {
			auto cam = m_Cameras.front();
			// TODO: Send for rendering
			m_Cameras.pop();

			auto renderMang = RenderingManager::GetInstance();

			renderMang->BeginScene(cam.cam, nullptr);
			for (auto req : m_RenderRequests) {
				renderMang->Submit(req.vBuf, req.iBuf, req.mat);
			}
			renderMang->EndScene();
		}

		m_RenderRequests.clear();
	}
};

