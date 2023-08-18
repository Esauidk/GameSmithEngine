#pragma once


#include "ProjectGE/Rendering/DirectX12/Util/third-party/d3dx12.h"

namespace ProjectGE {
	class DirectX12ShaderInput {
	public:
		inline DirectX12ShaderInput(UINT registerSlot, UINT size) : m_DataSet(false), m_RegSlot(registerSlot), m_Size(size) {}
		inline CD3DX12_ROOT_PARAMETER1& GetDefinition() { return m_Parameter; }
	protected:
		CD3DX12_ROOT_PARAMETER1 m_Parameter;
		void* m_RawData;
		bool m_DataSet;
		UINT m_RegSlot;
		UINT m_Size;
	};
};
