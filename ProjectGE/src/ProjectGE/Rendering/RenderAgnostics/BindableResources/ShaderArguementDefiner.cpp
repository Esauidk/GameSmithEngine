#include "gepch.h"
#include "ShaderArguementDefiner.h"

#include "ProjectGE/Rendering/RenderSettings.h"

#ifdef GE_PLATFORM_WINDOWS
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12RootSignature.h"
#endif

namespace ProjectGE {
	ShaderArguementDefiner* ProjectGE::ShaderArguementDefiner::Create()
	{
		switch (RenderSettings::GetOption()) {
		case RenderOptions::DIRECTX12:
			return new DirectX12RootSignature(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS);
		}
		return nullptr;
	}
};

