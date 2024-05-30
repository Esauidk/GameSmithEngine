#pragma once
#include <gepch.h>
#include "GameSmithEngine/Core/Log.h"

namespace GameSmith {
	class ResourceAssetReader {
	public:
		inline ResourceAssetReader(char* buffer, unsigned int byteSize) : m_CurPtr(buffer), m_EndPtr(buffer + byteSize) {}
		std::string GetString();
		unsigned int GetUInt();

		template <typename T>
		T* ReadClass() {
			GE_CORE_ASSERT(m_CurPtr + sizeof(T) <= m_EndPtr, "Buffer has not remain room to read structure");
			T* ret = (T*)m_CurPtr;
			m_CurPtr += sizeof(T);

			return ret;
		}

		char* GetBytes(unsigned int byteCount);

	private:
		char* m_CurPtr;
		char* m_EndPtr;
	};

	class ResourceAssetWriter {
	public:
		ResourceAssetWriter(unsigned int byteSize);
		void WriteString(std::string str);
		void WriteUInt(unsigned int i);

		template <typename T>
		void WriteClass(T* data) {
			memcpy(m_CurPtr, data, sizeof(T));
			m_CurPtr += sizeof(T);
		}

		void WriteByte(char* bytes, unsigned int byteCount);
		void CommitToFile(std::string destination);

		inline Ref<char> GetBuffer() { return m_Buffer; }
		inline unsigned int GetBufferSize() { return m_BufferSize; }
	private:
		Ref<char> m_Buffer;
		char* m_CurPtr;
		unsigned int m_BufferSize;
	};
};
