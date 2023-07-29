#pragma once
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/ShaderArguement.h"

namespace ProjectGE {
	class DirectX12ShaderConstant : public ShaderArguement
	{
		DirectX12ShaderConstant(void* data);
		void Bind(ID3D12GraphicsCommandList6* cmdList) override;
	};
};


