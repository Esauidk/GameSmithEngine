#pragma once
#include <gepch.h>
#include "ProjectGE/Core/Log.h"

namespace ProjectGE {
	class ResourceAssetReader {
	public:
		inline ResourceAssetReader(char* buffer, unsigned int byteSize) : m_CurPtr(buffer), m_EndPtr(buffer + byteSize) {}
		std::string GetString();

		template <typename T>
		T* ReadData() {
			GE_CORE_ASSERT(m_CurPtr + sizeof(T) <= m_EndPtr, "Buffer has not remain room to read structure");
			T* ret = (T*)m_CurPtr;
			m_CurPtr += sizeof(T);

			return ret;
		}

	private:
		char* m_CurPtr;
		char* m_EndPtr;
	};

	class ResourceAssetWriter {
	public:
		ResourceAssetWriter(unsigned int byteSize);
		void WriteString(std::string str);

		template <typename T>
		void WriteData(T* data) {
			memcpy(m_CurPtr, data, sizeof(T));
			m_CurPtr += sizeof(T);
		}

		inline char* GetBuffer() { return m_Buffer.get(); }
		inline unsigned int GetBufferSize() { return m_BufferSize; }
	private:
		Scope<char> m_Buffer;
		char* m_CurPtr;
		unsigned int m_BufferSize;
	};
};
