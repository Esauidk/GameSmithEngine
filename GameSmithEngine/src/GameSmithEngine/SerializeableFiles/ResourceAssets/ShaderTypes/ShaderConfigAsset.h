#pragma once
#include "GameSmithEngine/SerializeableFiles/ResourceAssets/Asset.h"
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Utilities/ParameterContainer.h"

namespace GameSmith {
	struct PrimitivePropertyDetails {
		char name[CONTAINER_BUFFER_DEFAULT_SIZE] = {'\0'};
		ContainerDataType type;
	};

	class GE_API ShaderConfigAsset : public Asset {
	public:
		ShaderConfigAsset(std::string name);

		const std::vector<Ref<PrimitivePropertyDetails>>& GetPrimitiveProperties() const { return m_PrimitiveProperties; }
		void Serialize(char* byteStream, unsigned int availableBytes) override;
		unsigned int RequiredSpace() const override { return (unsigned int)(sizeof(unsigned int) + sizeof(PrimitivePropertyDetails) * m_PrimitiveProperties.size()); }
		void Deserialize(char* inData, unsigned int size) override;

		void PostRegistryBootstrap() override;

		SERIAL_FILE(ShaderConfig, GMsrcfg)
	private:
		void RegisterProperty(unsigned int index, Ref<PrimitivePropertyDetails> propertyDetails);
	private:
		int m_PropertyCount = 0;
		std::vector<Ref<PrimitivePropertyDetails>> m_PrimitiveProperties;
	};
};
