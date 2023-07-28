#include "gepch.h"
#include "PipelineStateObject.h"

#include "ProjectGE/Rendering/RenderSettings.h"

#ifdef GE_PLATFORM_WINDOWS
#include "ProjectGE/Rendering/DirectX12/DirectX12PipelineState.h"
#endif

namespace ProjectGE {
	PipelineStateObject* PipelineStateObject::Create() {
		switch (RenderSettings::GetOption()) {
		case RenderOptions::NONE:
			return nullptr;
			break;
		case RenderOptions::DIRECTX12:
			return new DirectX12PipelineState();
			break;
		}

		return nullptr;
	}
};