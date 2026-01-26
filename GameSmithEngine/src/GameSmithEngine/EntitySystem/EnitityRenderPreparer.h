#pragma once
#include "gepch.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Camera/Camera.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/MaterialSystem/Material.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/IndexBuffer.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/VertexBuffer.h"

namespace GameSmith {
	struct GE_API RenderRequest {
		Ref<VertexBuffer> vBuf;
		Ref<IndexBuffer> iBuf;
		Ref<Material> mat;
	};

	struct GE_API RenderableCamera {
		Camera* cam;
		Ref<RenderTexture> targetTex = nullptr;
	};


	// This class collects information on which entities want to be renderered and prepares to make calls to the render manager
	class GE_API EntityRenderPreparer {
	public:
		static void Init();
		static void Shutdown();
		static inline EntityRenderPreparer* GetInstance() { return s_Instance; }
		void AddRenderRequest(RenderRequest req);
		void SetMainCamera(RenderableCamera cam);
		void AddAdditionalCamera(RenderableCamera cam);
		void SendForRendering();
	private:
		EntityRenderPreparer() = default;
	private:
		static EntityRenderPreparer* s_Instance;
	private:
		std::vector<RenderRequest> m_RenderRequests;
		RenderableCamera m_MainCamera;
		std::queue<RenderableCamera> m_AdditionalCameras;
	};
};
