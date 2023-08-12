#pragma once
#include "gepch.h"
#include "ProjectGE/Rendering/DirectX12/Util/d3dx12.h"
#include "ProjectGE/Rendering/RenderAgnostics/PipelineDefiner.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/ShaderArguementDefiner.h"


using Microsoft::WRL::ComPtr;

/* 
This class is a encapulated version of the Root Signature (An object linking paramters to stages in the render pipeline)
Job:
	- Collects flags and parameters
	- Serializes and Build the Root Signature
	- Binds it to the pipeline
*/
namespace ProjectGE {
	class DirectX12RootSignature : public ShaderArguementDefiner
	{
	public: 
			DirectX12RootSignature(D3D12_ROOT_SIGNATURE_FLAGS flags);
			ShaderArguement* AddArguement(ShaderArguementType type, UINT size = 0 ) override;
			void FinalizeSignature() override;
			void Append(PipelineStateObject& pipeline) override;
			void Bind() override;
	private:
		D3D12_FEATURE_DATA_ROOT_SIGNATURE m_RootSigFeat;
		D3D12_ROOT_SIGNATURE_FLAGS m_Flags;
		std::vector<CD3DX12_ROOT_PARAMETER1> m_Parameters;
		ComPtr<ID3D12RootSignature> m_Root;
		UINT m_AvailableSlot;
		bool m_FinalizedSignature;
	};
}


