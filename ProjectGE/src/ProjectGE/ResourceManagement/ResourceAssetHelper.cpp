#include "gepch.h"
#include "ResourceAssetHelper.h"

namespace ProjectGE {
	std::string ResourceAssetReader::GetString()
	{
		std::string collectedString(m_CurPtr); m_CurPtr += collectedString.length()+1; return collectedString;
	}


	ResourceAssetWriter::ResourceAssetWriter(unsigned int byteSize) {
		m_Buffer = Scope<char>(new char[byteSize]);
		m_CurPtr = m_Buffer.get();
		m_BufferSize = byteSize;
	}

	void ResourceAssetWriter::WriteString(std::string str)
	{
		memcpy(m_CurPtr, str.c_str(), str.length()+1);
		m_CurPtr += str.length() + 1;
	}
};

