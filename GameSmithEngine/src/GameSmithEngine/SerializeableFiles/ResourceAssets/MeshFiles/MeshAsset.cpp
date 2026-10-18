#include "gepch.h"
#include "MeshAsset.h"

namespace GameSmith {

	Ref<char> MeshAsset::Serialize()
	{
		unsigned int size = 0;
		return m_Serializer->SerializeMesh(m_Mesh, &size);
	}

	void MeshAsset::Serialize(char* byteStream, unsigned int availableBytes)
	{
		unsigned int size = 0;
		Ref<char> serializedData = m_Serializer->SerializeMesh(m_Mesh, &size);
		GE_CORE_ASSERT(availableBytes >= size, "Not enough bytes to serialize MeshAsset");
		memcpy(byteStream, serializedData.get(), size);
	}

	unsigned int MeshAsset::RequiredSpace() const
	{
		unsigned int size = 0;
		m_Serializer->SerializeMesh(m_Mesh, &size);

		return size;
	}

	void MeshAsset::Deserialize(char* inData, unsigned int size)
	{
		m_Mesh = m_Serializer->DeserializeMesh(inData, size);
	}

};