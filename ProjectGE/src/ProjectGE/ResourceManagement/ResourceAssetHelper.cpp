#include "gepch.h"
#include "ResourceAssetHelper.h"

namespace ProjectGE {
	std::string ResourceAssetReader::GetString()
	{
		std::string collectedString(m_CurPtr); 
		
		m_CurPtr += collectedString.length()+1; 
		
		return collectedString;
	}

	char* ResourceAssetReader::GetBytes(unsigned int byteCount)
	{
		char* savePtr = m_CurPtr;
		m_CurPtr += byteCount;
		return savePtr;
	}


	ResourceAssetWriter::ResourceAssetWriter(unsigned int byteSize) {
		m_Buffer = Scope<char>(new char[byteSize]);
		m_CurPtr = m_Buffer.get();
		m_BufferSize = byteSize;
	}

	void ResourceAssetWriter::WriteString(std::string str)
	{
		size_t length = str.length() + 1;
		memcpy(m_CurPtr, str.c_str(), length);
		m_CurPtr += length;
	}

	void ResourceAssetWriter::WriteByte(char* bytes, unsigned int byteCount)
	{
		memcpy(m_CurPtr, bytes, byteCount);
	}
};

