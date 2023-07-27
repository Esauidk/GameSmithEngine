#pragma once
#include "gepch.h"
#include "ProjectGE/Rendering/DirectX12/Util/d3dx12.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/PipelineDefiner.h"


/* 
This class is a encapulated version of the Root Signature (An object linking paramters to stages in the render pipeline)
Job:
	- Collects flags and parameters
	- Serializes and Build the Root Signature
	- Binds it to the pipeline
*/
namespace ProjectGE {
	class DirectX12RootSignature : public PipelineDefiner, public BindableResource
	{
	public: 
			DirectX12RootSignature(D3D12_ROOT_SIGNATURE_FLAGS flags);
			void AddParameter(const CD3DX12_ROOT_PARAMETER1& newParameter);
			void AddParameter(CD3DX12_ROOT_PARAMETER1* newParameters, UINT size);
			void BuildRootSignature();
			void Append(PipelineStateObject& pipeline) override;
			void Bind(ID3D12GraphicsCommandList6* cmdList) override;
	private:
		D3D12_FEATURE_DATA_ROOT_SIGNATURE m_RootSigFeat;
		D3D12_ROOT_SIGNATURE_FLAGS m_Flags;
		std::vector<CD3DX12_ROOT_PARAMETER1> m_Parameters;
		ComPtr<ID3D12RootSignature> m_Root;
	};
}


