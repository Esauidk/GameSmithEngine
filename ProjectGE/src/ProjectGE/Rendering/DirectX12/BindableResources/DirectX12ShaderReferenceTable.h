#pragma once
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/ShaderArguement.h"

namespace ProjectGE {
	class DirectX12ShaderReferenceTable : public ShaderArguement
	{
		DirectX12ShaderReferenceTable(void* data);
		void Bind(ID3D12GraphicsCommandList6* cmdList) override;
	};
};


