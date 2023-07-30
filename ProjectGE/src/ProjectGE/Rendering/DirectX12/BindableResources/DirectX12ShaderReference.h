#pragma once
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/ShaderArguement.h"
namespace ProjectGE {
	class DirectX12ShaderReference : public ShaderArguement
	{
	public:
		DirectX12ShaderReference(void* data);
		void Bind(ID3D12GraphicsCommandList6* cmdList) override;
	};
};


