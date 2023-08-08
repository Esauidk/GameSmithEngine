#pragma once
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/ShaderArguement.h"
#include "ProjectGE/Rendering/DirectX12/Util/d3dx12.h"

namespace ProjectGE {
	class DirectX12ShaderConstant : public ShaderArguement
	{
	public:
		DirectX12ShaderConstant(UINT registerSlot, UINT size);
		inline CD3DX12_ROOT_PARAMETER1& GetDefinition() { return m_Parameter; }
		void SetData(void* rawData) override;
		void Bind() override;
	private:
		CD3DX12_ROOT_PARAMETER1 m_Parameter;
		void* m_RawData;
		bool m_DataSet;
		UINT m_RegSlot;
		UINT m_Size;
	};
};


