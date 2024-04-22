#pragma once
#include "gepch.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/VertexBuffer.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/IndexBuffer.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/MaterialSystem/Material.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Camera/Camera.h"

namespace GameSmith {
	struct RenderRequest {
		Ref<VertexBuffer> vBuf;
		Ref<IndexBuffer> iBuf;
		Ref<Material> mat;
	};

	struct RenderableCamera {
		Camera* cam;
	};


	// This class collects information on which entities want to be renderered and prepares to make calls to the render manager
	class EntityRenderPreparer {
	public:
		EntityRenderPreparer();
		static inline EntityRenderPreparer* GetInstance() { return s_Instance; }
		void AddRenderRequest(RenderRequest req);
		void AddCamera(RenderableCamera cam);
		void SendForRendering();
	private:
		static EntityRenderPreparer* s_Instance;
	private:
		std::vector<RenderRequest> m_RenderRequests;
		std::queue<RenderableCamera> m_Cameras;
	};
};
