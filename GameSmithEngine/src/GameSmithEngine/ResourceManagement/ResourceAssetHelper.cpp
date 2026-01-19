#include "gepch.h"
#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/SerializeableFiles/Serializable.h"
#include "ResourceAssetHelper.h"

namespace GameSmith {
	std::string BinaryStreamReader::GetString()
	{
		std::string collectedString(m_CurPtr); 
		
		m_CurPtr += collectedString.length()+1; 
		
		return collectedString;
	}

	unsigned int BinaryStreamReader::GetUInt()
	{
		unsigned int val = *((unsigned int*)m_CurPtr);

		m_CurPtr += sizeof(unsigned int);

		return val;
	}

	char* BinaryStreamReader::GetBytes(unsigned int byteCount)
	{
		char* savePtr = m_CurPtr;
		m_CurPtr += byteCount;
		return savePtr;
	}


	BinaryStreamWriter::BinaryStreamWriter(unsigned int byteSize) : m_Buffer(Ref<char>(new char[byteSize])), m_CurPtr(m_Buffer.get()), m_BufferSize(byteSize), m_outsideSrc(false)  {
	}

	BinaryStreamWriter::BinaryStreamWriter(char* ptr, unsigned int bufferSize) : m_Buffer(nullptr), m_OutSideStartPtr(ptr), m_CurPtr(ptr), m_BufferSize(bufferSize), m_outsideSrc(true)
	{
	}

	void BinaryStreamWriter::WriteString(std::string str)
	{
		size_t length = str.length() + 1;
		memcpy(m_CurPtr, str.c_str(), length);
		m_CurPtr += length;
	}

	void BinaryStreamWriter::WriteUInt(unsigned int i)
	{
		memcpy(m_CurPtr, &i, sizeof(unsigned int));
		m_CurPtr += sizeof(int);
	}

	void BinaryStreamWriter::WriteSerializeable(Serializeable* serializeable)
	{
		unsigned int requiredSize = serializeable->RequiredSpace();
		GE_CORE_ASSERT(GetRemainingSpace() >= requiredSize, "Not enough space in buffer to write serializeable");
		serializeable->Serialize(m_CurPtr, GetRemainingSpace());
		m_CurPtr += requiredSize;
	}

	void BinaryStreamWriter::WriteVector(const std::vector<Ref<Serializeable>> vector)
	{
		unsigned int requiredSize = sizeof(unsigned int);
		for (auto& item : vector) {
			requiredSize += item->RequiredSpace();
		}
		GE_CORE_ASSERT(GetRemainingSpace() >= requiredSize, "Not enough space in buffer to write serializeable vector");

		WriteUInt((unsigned int)vector.size());
		for (auto& item : vector) {
			item->Serialize(m_CurPtr, GetRemainingSpace());
			m_CurPtr += item->RequiredSpace();
		}
	}

	void BinaryStreamWriter::WriteVector(const std::vector<Connection<Serializeable>> vector) {
		unsigned int requiredSize = sizeof(unsigned int);
		for (auto& item : vector) {
			requiredSize += item.lock()->RequiredSpace();
		}

		GE_CORE_ASSERT(GetRemainingSpace() >= requiredSize, "Not enough space in buffer to write serializeable vector");

		WriteUInt((unsigned int)vector.size());
		for (auto& item : vector) {
			auto lockedItem = item.lock();
			lockedItem->Serialize(m_CurPtr, GetRemainingSpace());
			m_CurPtr += lockedItem->RequiredSpace();
		}
	}

	void BinaryStreamWriter::WriteByte(char* bytes, unsigned int byteCount)
	{
		memcpy(m_CurPtr, bytes, byteCount);
		m_CurPtr += byteCount;
	}

	void BinaryStreamWriter::CommitToFile(std::string destination)
	{
		std::fstream pFile(destination, std::ios::out | std::ios::binary | std::ios::ate);
		GE_CORE_ASSERT(pFile.is_open(), std::format("Asset file {0} cannot be opened", destination));

		pFile.seekg(0, pFile.beg);
		if (m_outsideSrc) {
			pFile.write(m_OutSideStartPtr, m_CurPtr - m_OutSideStartPtr);
		}
		else {
			pFile.write(m_Buffer.get(), m_CurPtr - m_Buffer.get());
		}
		
		pFile.close();
	}

	void BinaryStreamWriter::MoveCurPtr(unsigned int bytes)
	{
		GE_CORE_ASSERT(bytes <= GetRemainingSpace(), "Not enough space to move writer {} bytes", bytes);

		m_CurPtr += bytes;
	}

	BinaryStreamReader BinaryStreamReader::ReadDirectlyFromFile(std::string fileName)
	{
		std::fstream pFile(fileName, std::ios::in | std::ios::binary | std::ios::ate);
		GE_CORE_ASSERT(pFile.is_open(), std::format("File {0} cannot be opened", fileName));

		unsigned int size = (UINT)pFile.tellg();
		pFile.seekg(0, pFile.beg);

		char* buf = new char[size];

		pFile.read(buf, size);
		pFile.close();

		BinaryStreamReader reader(Ref<char>(buf), size);
		
		return reader;
	}
};

