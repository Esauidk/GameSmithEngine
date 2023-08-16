#include "gepch.h"
#include "PipelineStateObject.h"

#include "ProjectGE/Rendering/Renderer.h"

#ifdef GE_PLATFORM_WINDOWS
#include "ProjectGE/Rendering/DirectX12/DirectX12PipelineState.h"
#endif

namespace ProjectGE {
	Ref<PipelineStateObject> PipelineStateObject::Create() {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			return nullptr;
			break;
		case RendererAPI::API::DirectX12:
			return Ref<PipelineStateObject>(new DirectX12PipelineState());
			break;
		}

		return nullptr;
	}
};