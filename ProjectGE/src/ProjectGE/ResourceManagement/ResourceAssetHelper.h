#pragma once
#include <gepch.h>
#include "ProjectGE/Core/Log.h"

namespace ProjectGE {
	class ResourceAssetReader {
	public:
		inline ResourceAssetReader(char* buffer, unsigned int byteSize) : m_CurPtr(buffer), m_CurrentPos(0), m_BufferEnd(byteSize) {}
		inline std::string GetString() { std::string collectedString(m_CurPtr); m_CurPtr += collectedString.length(); return collectedString; }

		template <typename T>
		T* ReadData() {
			GE_CORE_ASSERT(m_CurrentPos + sizeof(T) <= m_BufferEnd, "Buffer has not remain room to read structure");
			T* ret = (T*)m_CurPtr;
			m_CurPtr += sizeof(T);
			m_CurrentPos += sizeof(T);

			return ret;
		}

	private:
		char* m_CurPtr;
		unsigned int m_CurrentPos;
		unsigned int m_BufferEnd;
	};
};
