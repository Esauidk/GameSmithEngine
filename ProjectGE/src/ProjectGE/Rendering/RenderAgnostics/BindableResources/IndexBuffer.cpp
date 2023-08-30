#include "gepch.h"
#include "IndexBuffer.h"

#include "ProjectGE/Rendering/Renderer.h"

#ifdef GE_PLATFORM_WINDOWS
#include "ProjectGE/Rendering/DirectX12/Resources/DirectX12IndexBuffer.h"
#endif

namespace ProjectGE {
	Ref<IndexBuffer> IndexBuffer::Create(void* verticies, UINT count)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			return nullptr;
			break;
		case RendererAPI::API::DirectX12:
			return Ref<IndexBuffer>(new DirectX12IndexBuffer((WORD*)verticies, count));
			break;
		}

		return nullptr;
	}
};

