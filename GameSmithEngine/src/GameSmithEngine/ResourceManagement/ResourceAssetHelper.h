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

		void MoveForward(unsigned int numBytes) { m_CurPtr += MIN(numBytes, GetRemainingBytes()); }
		char* GetBytes(unsigned int byteCount);
		char* GetCurPtr() const { return m_CurPtr; }

		unsigned int GetRemainingBytes() const { return (unsigned int)(m_EndPtr - m_CurPtr); }

	private:
		char* m_CurPtr;
		char* m_EndPtr;
	};

	class ResourceAssetWriter {
	public:
		ResourceAssetWriter(unsigned int byteSize);
		ResourceAssetWriter(char* ptr, unsigned int bufferSize);
		void WriteString(std::string str);
		void WriteUInt(unsigned int i);

		template <typename T>
		void WriteClass(T* data) {
			memcpy(m_CurPtr, data, sizeof(T));
			m_CurPtr += sizeof(T);
		}

		void WriteByte(char* bytes, unsigned int byteCount);
		void CommitToFile(std::string destination);

		inline Ref<char> GetBuffer() { if (m_outsideSrc) { return nullptr; }return m_Buffer; }
		inline char* GetCurPtr() const { return m_CurPtr; }
		inline void MoveCurPtr(unsigned int bytes) { m_CurPtr += bytes; }


		inline unsigned int GetBufferSize() const { return m_BufferSize; }
		inline unsigned int GetRemainingSpace() const {
			if (m_outsideSrc) { 
				return m_BufferSize - (unsigned int) (m_CurPtr - m_OutSideStartPtr); 
			} 
			
			return m_BufferSize - (unsigned int) (m_CurPtr - m_Buffer.get());
		}
	private:
		Ref<char> m_Buffer;
		char* m_OutSideStartPtr;
		char* m_CurPtr;
		unsigned int m_BufferSize;
		bool m_outsideSrc;
	};
};
