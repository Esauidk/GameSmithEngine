#include "gepch.h"
#include "ConstantBuffer.h"

#include "ProjectGE/Rendering/Renderer.h"

#ifdef GE_PLATFORM_WINDOWS
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12ConstantBuffer.h"
#endif

namespace ProjectGE {
	ConstantBuffer* ConstantBuffer::Create(void* initialData, UINT size)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::DirectX12:
			return new DirectX12ConstantBuffer((BYTE*)initialData, size);
		}
		return nullptr;
	}
};