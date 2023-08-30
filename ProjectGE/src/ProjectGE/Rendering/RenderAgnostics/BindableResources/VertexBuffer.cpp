#include "gepch.h"
#include "VertexBuffer.h"

#include "ProjectGE/Rendering/Renderer.h"
#include "ProjectGE/Rendering/RenderAgnostics/BasicStructs.h"

#ifdef GE_PLATFORM_WINDOWS
#include "ProjectGE/Rendering/DirectX12/Resources/DirectX12VertexBuffer.h"
#endif

namespace ProjectGE {
	Ref<VertexBuffer> ProjectGE::VertexBuffer::Create(void* verticies, UINT count)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::DirectX12:
			return Ref<VertexBuffer>(new DirectX12VertexBuffer<Vertex>((Vertex*)verticies, count));
		}

		return nullptr;
	}
};

