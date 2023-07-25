#include "gepch.h"
#include "VertexBuffer.h"

#include "ProjectGE/Rendering/RenderSettings.h"
#include "ProjectGE/Rendering/RenderAgnostics/BasicStructs.h"

#ifdef GE_PLATFORM_WINDOWS
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12VertexBuffer.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12Renderer.h"
#endif

namespace ProjectGE {
	VertexBuffer* ProjectGE::VertexBuffer::Create(void* verticies, UINT count)
	{
		switch (RenderSettings::GetOption()) {
		case RenderOptions::NONE:
			return nullptr;
			break;
		case RenderOptions::DIRECTX12:
			//return new DirectX12VertexBuffer<Vertex>(DirectX12Renderer::GetDevice(), (Vertex*)verticies, count)
			break;
		}

		return nullptr;
	}
};

