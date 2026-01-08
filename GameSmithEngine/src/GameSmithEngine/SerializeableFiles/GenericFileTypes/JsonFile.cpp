#include "gepch.h"
#include "JsonFile.h"

#include "GameSmithEngine/ResourceManagement/ResourceAssetHelper.h"

namespace GameSmith {
	Ref<char> JsonFile::Serialize()
	{
		BinaryStreamWriter writer(RequiredSpace());
		Serialize(writer.GetCurPtr(), writer.GetRemainingSpace());

		return writer.GetBuffer();
	}

	void JsonFile::Serialize(char* byteStream, unsigned int availableBytes)
	{
		BinaryStreamWriter writer(byteStream, availableBytes);
		writer.WriteString(m_JsonString);
	}

	unsigned int JsonFile::RequiredSpace() const
	{
		return m_JsonString.length() + 1;
	}

	void JsonFile::Deserialize(char* inData, unsigned int size)
	{
		BinaryStreamReader reader(inData, size);
		m_JsonString = reader.GetString();
		m_JsonData = json::parse(m_JsonString);

		ExtractJsonData(m_JsonData);
	}

	void JsonFile::UpdateJson(const json& newJson)
	{
		m_JsonData = newJson;
		m_JsonString = m_JsonData.dump(JSON_INDENTATION_SPACES);
	}
};