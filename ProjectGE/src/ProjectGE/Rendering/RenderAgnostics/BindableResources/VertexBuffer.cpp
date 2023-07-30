#include "gepch.h"
#include "VertexBuffer.h"

#include "ProjectGE/Rendering/RenderSettings.h"
#include "ProjectGE/Rendering/RenderAgnostics/BasicStructs.h"

#ifdef GE_PLATFORM_WINDOWS
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12VertexBuffer.h"
#endif

namespace ProjectGE {
	VertexBuffer* ProjectGE::VertexBuffer::Create(void* verticies, UINT count)
	{
		switch (RenderSettings::GetOption()) {
		case RenderOptions::NONE:
			return nullptr;
		case RenderOptions::DIRECTX12:
			return new DirectX12VertexBuffer<Vertex>((Vertex*)verticies, count);
		}

		return nullptr;
	}
};

