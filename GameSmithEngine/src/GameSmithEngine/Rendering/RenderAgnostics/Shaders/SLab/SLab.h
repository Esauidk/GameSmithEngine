#pragma once
#include <gepch.h>
#include "GameSmithEngine/Rendering/RenderAgnostics/Shaders/ShaderParameter.h"

namespace GameSmith {
	class SLabMetadata {
	public:
		SLabMetadata() : m_ByteSize(0) {}
		inline void AddParameter(Ref<ShaderParameter> parm) { m_ByteSize += parm->GetSize();  m_Params.emplace_back(parm); }
		inline Ref<ShaderParameter> GetParameter(UINT index) { return (*(m_Params.begin() + index)); }
		inline UINT GetByteSize() { return m_ByteSize; }
	private:
		std::vector<Ref<ShaderParameter>> m_Params;
		UINT m_ByteSize;
	};

	class SLabInterpeter { 
	};
};
