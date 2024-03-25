#pragma once
#include <gepch.h>
#include "GameSmithEngine/Utilities/ParameterContainer.h"

namespace GameSmith {
	class SLabMetadata {
	public:
		SLabMetadata() : m_ByteSize(0) {}
		inline void AddParameter(Ref<ParameterContainer> parm) { m_ByteSize += parm->GetSize();  m_Params.emplace_back(parm); }
		inline Ref<ParameterContainer> GetParameter(UINT index) { return (*(m_Params.begin() + index)); }
		inline UINT GetByteSize() { return m_ByteSize; }
	private:
		std::vector<Ref<ParameterContainer>> m_Params;
		UINT m_ByteSize;
	};

	class SLabInterpeter { 
	};
};
