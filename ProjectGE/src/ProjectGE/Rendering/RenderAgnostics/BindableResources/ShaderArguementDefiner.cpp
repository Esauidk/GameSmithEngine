#include "gepch.h"
#include "ShaderArguementDefiner.h"

#include "ProjectGE/Rendering/Renderer.h"

#ifdef GE_PLATFORM_WINDOWS
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12RootSignature.h"
#endif

namespace ProjectGE {
	Ref<ShaderArguementDefiner> ProjectGE::ShaderArguementDefiner::Create()
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::DirectX12:
			return Ref<ShaderArguementDefiner>(new DirectX12RootSignature(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS));
		}
		return nullptr;
	}
};

