#include "gepch.h"
#include "IndexBuffer.h"

#include "ProjectGE/Rendering/RenderSettings.h"

#ifdef GE_PLATFORM_WINDOWS
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12IndexBuffer.h"
#endif

namespace ProjectGE {
	IndexBuffer* ProjectGE::IndexBuffer::Create(void* verticies, UINT count)
	{
		switch (RenderSettings::GetOption()) {
		case RenderOptions::NONE:
			return nullptr;
			break;
		case RenderOptions::DIRECTX12:
			return new DirectX12IndexBuffer((WORD*)verticies, count);
			break;
		}

		return nullptr;
	}
};

