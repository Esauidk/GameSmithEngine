#pragma once
#include <gepch.h>
#include "ProjectGE/Rendering/RenderAgnostics/Shaders/ShaderParameter.h"

namespace ProjectGE {
	class SLabMetadata {
	public:
		SLabMetadata() : m_ByteSize(0) {}
		inline void AddParameter(ShaderParameter parm) { m_ByteSize += parm.GetSize();  m_Params.emplace_back(parm); }
		inline ShaderParameter& GetParameter(UINT index) { (*(m_Params.begin() + index)); }
		inline UINT GetByteSize() { return m_ByteSize; }
	private:
		std::vector<ShaderParameter> m_Params;
		UINT m_ByteSize;
	};

	class SLabInterpeter {
	
	};
};
