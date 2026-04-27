#pragma once
#include "Asset.h"
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/MaterialSystem/Material.h"
#include "GameSmithEngine/SerializeableFiles/ResourceAssets/TextureAsset.h"
#include "GameSmithEngine/SerializeableFiles/ResourceAssets/ShaderTypes/ShaderAsset.h"

namespace GameSmith {
	class GE_API MaterialAsset : public Asset
	{
	public:
		MaterialAsset(std::string name);
		/*MaterialAsset(
			const std::string name,
			const std::unordered_map<Stages, ID>& shaderIds,
			const std::vector<std::string>& textureOrder,
			const std::unordered_map<std::string, ID>& textureIds,
			const std::vector<std::string>& parameterOrder,
			const std::unordered_map<std::string, ContainerDataType>& variables
		);*/

		void Serialize(char* byteStream, unsigned int availableBytes) override;
		unsigned int RequiredSpace() const override;
		void Deserialize(char* inData, unsigned int size) override;

		void PostRegistryBootstrap() override;

		SERIAL_FILE(Material, mat)

		Ref<Material> CreateInstance();
	private:
		struct MaterialAssetMetadata {
			unsigned int ParamterCount = 0;
			unsigned int TetureCount = 0;
		};

		// We need a layer of indirection because we need to expose textures to the exposed member system
		// This expects the location of the pointer to manipulate to be consistent,
		// so we can't just store the Ref<TextureAsset> directly in the map
		struct TextureRefContainer {
			Ref<TextureAsset> asset = nullptr;
		};

		MaterialAssetMetadata m_Metadata;
		Ref<ShaderAsset> m_Shader;

		// Order is important for materials
		std::vector<std::string> m_ParameterKeys;
		std::vector<std::string> m_TextureKeys;

		std::unordered_map<std::string, Ref<TextureRefContainer>> m_Textures;
		std::unordered_map<std::string, Ref<ParameterContainer>> m_Parameters;
	};
};


