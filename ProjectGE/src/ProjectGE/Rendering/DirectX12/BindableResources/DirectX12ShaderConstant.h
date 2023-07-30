#pragma once
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/ShaderArguement.h"
#include "ProjectGE/Rendering/DirectX12/Util/d3dx12.h"

namespace ProjectGE {
	class DirectX12ShaderConstant : public ShaderArguement
	{
	public:
		DirectX12ShaderConstant(UINT registerSLot, void* data, UINT size);
		~DirectX12ShaderConstant();
		inline CD3DX12_ROOT_PARAMETER1& GetDefinition() { return m_Parameter; }
		void Bind(ID3D12GraphicsCommandList6* cmdList) override;
	private:
		CD3DX12_ROOT_PARAMETER1 m_Parameter;
		void* m_RawData;
		UINT m_RegSlot;
		UINT m_Size;
	};
};


