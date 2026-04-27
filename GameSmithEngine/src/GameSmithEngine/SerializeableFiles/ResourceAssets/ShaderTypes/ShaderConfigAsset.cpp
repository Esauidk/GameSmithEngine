#include "gepch.h"
#include "ShaderConfigAsset.h"
#include "GameSmithEngine/SerializeableFiles/ResourceAssets/AssetFactory.h"

#define PROPERTIES_KEY "Property"

namespace GameSmith {
	GE_REGISTERASSET(ShaderConfigAsset);

	ShaderConfigAsset::ShaderConfigAsset(std::string name) : Asset(name)
	{
		m_Registry.AddExposedVariable(
			"PropertyCount",
			ContainerDataType::Int,
			&m_PropertyCount
		);
	}

	void ShaderConfigAsset::Serialize(char* byteStream, unsigned int availableBytes)
	{
		GE_CORE_ASSERT(availableBytes >= RequiredSpace(), "There's not enough space to serialize ShaderConfigAsset");
		BinaryStreamWriter writer(byteStream, availableBytes);
		writer.WriteUInt((unsigned int)m_PropertyCount);
		for (auto& property : m_PrimitiveProperties) {
			writer.WriteClass<PrimitivePropertyDetails>(property.get());
		}
	}

	void ShaderConfigAsset::Deserialize(char* inData, unsigned int size)
	{
		BinaryStreamReader reader(inData, size);
		m_PropertyCount = reader.GetUInt();
		m_PrimitiveProperties.resize(m_PropertyCount);
		for (int i = 0; i < m_PrimitiveProperties.size(); i++) {
			auto* details = reader.ReadClass<PrimitivePropertyDetails>();
			auto propertyDetails = Ref<PrimitivePropertyDetails>(new PrimitivePropertyDetails(*details));
			m_PrimitiveProperties[i] = propertyDetails;

			RegisterProperty(i, propertyDetails);
		}
	}

	void ShaderConfigAsset::PostRegistryBootstrap()
	{
		if (m_PropertyCount != m_PrimitiveProperties.size())
		{
			for (int i = 0; i < m_PrimitiveProperties.size(); i++) {
				const std::string nameKey = std::format("{0}_{1}: {2}", PROPERTIES_KEY, i, "Name");
				m_Registry.RemoveExposedVariable(nameKey);

				const std::string dataTypeKey = std::format("{0}_{1}: {2}", PROPERTIES_KEY, i, "DataType");
				m_Registry.RemoveExposedVariable(dataTypeKey);
			}


			m_PrimitiveProperties.resize(m_PropertyCount);
			for (int i = 0; i < m_PrimitiveProperties.size(); i++) {
				auto propertyDetails = m_PrimitiveProperties[i];
				if (propertyDetails == nullptr) {
					propertyDetails = Ref<PrimitivePropertyDetails>(new PrimitivePropertyDetails{});
					m_PrimitiveProperties[i] = propertyDetails;
				}

				RegisterProperty(i, propertyDetails);
			}
		}
	}

	void ShaderConfigAsset::RegisterProperty(unsigned int index, Ref<PrimitivePropertyDetails> propertyDetails)
	{
		const std::string parameterName = std::format("{0}_{1}", PROPERTIES_KEY, index);
		const std::string nameKey = std::format("{0}: {1}", parameterName, "Name");
		char* namePtr = propertyDetails->name;
		m_Registry.AddExposedVariable(
			nameKey,
			ContainerDataType::CBuffer,
			namePtr,
			CONTAINER_BUFFER_DEFAULT_SIZE,
			parameterName
		);

		const std::string dataTypeKey = std::format("{0}: {1}", parameterName, "DataType");
		m_Registry.AddExposedVariable(
			dataTypeKey,
			ContainerDataType::Int,
			&(propertyDetails->type),
			0,
			parameterName
		);
	}
}