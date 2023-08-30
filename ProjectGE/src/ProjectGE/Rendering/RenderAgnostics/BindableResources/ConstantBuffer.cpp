#include "gepch.h"
#include "ConstantBuffer.h"

#include "ProjectGE/Rendering/Renderer.h"

#ifdef GE_PLATFORM_WINDOWS
#include "ProjectGE/Rendering/DirectX12/Resources/DirectX12ConstantBuffer.h"
#endif

namespace ProjectGE {
	Ref<ConstantBuffer> ConstantBuffer::Create(void* initialData, UINT size)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::DirectX12:
			return Ref<ConstantBuffer>(new DirectX12ConstantBuffer((BYTE*)initialData, size));
		}
		return nullptr;
	}

	Ref<ConstantBuffer> ConstantBuffer::Create(UINT size)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::DirectX12:
			return Ref<ConstantBuffer>(new DirectX12ConstantBuffer(size));
		}
		return nullptr;
	}
};