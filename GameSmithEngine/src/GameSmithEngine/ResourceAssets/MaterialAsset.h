#pragma once
#include "GameSmithEngine/Rendering/RenderAgnostics/MaterialSystem/Material.h"
#include "Serializable.h"

namespace GameSmith {
	class MaterialAsset : public Serializeable
	{
	public:
		MaterialAsset() = default;
		MaterialAsset(
			std::vector<std::pair<std::string, Stages>>& shaderPaths,
			std::vector<std::pair<std::string, std::string>>& texturePaths,
			std::vector<std::pair<std::string, ContainerDataType>>& variables
		);

		static Ref<Material> ReadAsset(char* assetData, unsigned int assetSize);

		virtual Ref<char> Serialize() override;
		virtual void Serialize(char* byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequireSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;

		Ref<Material> CreateInstance();
	private:
		struct MaterialAssetMetadata {
			unsigned int ParamterCount;
			unsigned int TetureCount;
			struct ShaderMetadata {
				bool UsedShader = false;
			}Shaders[STAGE_NUM];
		};

		Ref<Material> m_GlobalVer;

		MaterialAssetMetadata m_Metadata;
		std::string m_ShaderPaths[STAGE_NUM];

		std::vector<std::string> m_TexturePaths;
	};
};


