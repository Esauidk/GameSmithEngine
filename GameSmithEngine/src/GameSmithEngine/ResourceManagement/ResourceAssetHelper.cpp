#include "gepch.h"
#include "ResourceAssetHelper.h"
#include "GameSmithEngine/Core/Log.h"

namespace GameSmith {
	std::string ResourceAssetReader::GetString()
	{
		std::string collectedString(m_CurPtr); 
		
		m_CurPtr += collectedString.length()+1; 
		
		return collectedString;
	}

	unsigned int ResourceAssetReader::GetUInt()
	{
		unsigned int val = *((unsigned int*)m_CurPtr);

		m_CurPtr += sizeof(unsigned int);

		return val;
	}

	char* ResourceAssetReader::GetBytes(unsigned int byteCount)
	{
		char* savePtr = m_CurPtr;
		m_CurPtr += byteCount;
		return savePtr;
	}


	ResourceAssetWriter::ResourceAssetWriter(unsigned int byteSize) : m_Buffer(Ref<char>(new char[byteSize])), m_CurPtr(m_Buffer.get()), m_BufferSize(byteSize), m_outsideSrc(false)  {
	}

	ResourceAssetWriter::ResourceAssetWriter(char* ptr, unsigned int bufferSize) : m_Buffer(nullptr), m_OutSideStartPtr(ptr), m_CurPtr(ptr), m_BufferSize(bufferSize), m_outsideSrc(true)
	{
	}

	void ResourceAssetWriter::WriteString(std::string str)
	{
		size_t length = str.length() + 1;
		memcpy(m_CurPtr, str.c_str(), length);
		m_CurPtr += length;
	}

	void ResourceAssetWriter::WriteUInt(unsigned int i)
	{
		memcpy(m_CurPtr, &i, sizeof(unsigned int));
		m_CurPtr += sizeof(int);
	}

	void ResourceAssetWriter::WriteByte(char* bytes, unsigned int byteCount)
	{
		memcpy(m_CurPtr, bytes, byteCount);
		m_CurPtr += byteCount;
	}

	void ResourceAssetWriter::CommitToFile(std::string destination)
	{
		std::fstream pFile(destination, std::ios::out | std::ios::binary | std::ios::ate);
		GE_CORE_ASSERT(pFile.is_open(), std::format("Asset file {0} cannot be opened", destination));

		pFile.seekg(0, pFile.beg);
		pFile.write(m_Buffer.get(), m_CurPtr - m_Buffer.get());
		pFile.close();
	}

	ResourceAssetReader ResourceAssetReader::ReadDirectlyFromFile(std::string fileName)
	{
		std::fstream pFile(fileName, std::ios::in | std::ios::binary | std::ios::ate);
		GE_CORE_ASSERT(pFile.is_open(), std::format("File {0} cannot be opened", fileName));

		unsigned int size = (UINT)pFile.tellg();
		pFile.seekg(0, pFile.beg);

		char* buf = new char[size];

		pFile.read(buf, size);
		pFile.close();

		ResourceAssetReader reader(Ref<char>(buf), size);
		
		return reader;
	}
};

