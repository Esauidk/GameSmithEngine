#include "gepch.h"
#include "BlobAsset.h"

namespace GameSmith {
	Ref<char> BlobAsset::Serialize()
	{
		if (m_Size != 0 && m_Bytes != nullptr) {
			Ref<char> byteStream = Ref<char>(new char[m_Size]);
			memcpy(byteStream.get(), m_Bytes.get(), m_Size);
			return byteStream;
		}
		
		return Ref<char>();
	}

	void BlobAsset::Serialize(char* byteStream, unsigned int availableBytes)
	{
		GE_CORE_ASSERT(availableBytes >= m_Size, "Not enough space to serialize BlobAsset");
		if (m_Size != 0 && m_Bytes != nullptr) {
			memcpy(byteStream, m_Bytes.get(), m_Size);
		}

	}
	unsigned int BlobAsset::RequiredSpace() const
	{
		return m_Size;
	}

	void BlobAsset::Deserialize(char* inData, unsigned int size)
	{
		m_Bytes = Ref<char>(new char[size]);
		memcpy(m_Bytes.get(), inData, size);
		m_Size = size;
	}
};