#pragma once
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/MaterialSystem/Material.h"
#include "Serializable.h"

namespace GameSmith {
	struct MaterialInfo {
		std::vector<std::pair<std::string, ID>> TextureIds;
	};

	class GE_API MaterialAsset : public Serializeable
	{
	public:
		MaterialAsset();
		MaterialAsset(
			std::vector<std::pair<ID, Stages>>& shaderIds,
			std::vector<std::pair<std::string, ID>>& textureIds,
			std::vector<std::pair<std::string, ContainerDataType>>& variables
		);

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
		ID m_ShaderIds[STAGE_NUM];

		Scope<MaterialInfo> m_MatInfo;
	};
};


