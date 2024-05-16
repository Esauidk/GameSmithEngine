#pragma once
#include "GameSmithEngine/Rendering/RenderAgnostics/MaterialSystem/Material.h"
#include "Serializable.h"

namespace GameSmith {
	struct MaterialAssetMetadata {
		unsigned int ParamterCount;
		unsigned int TetureCount;
		struct ShaderMetadata {
			bool UsedShader = false;
			unsigned int stringLength = 0;
		}Shaders[STAGE_NUM];
	};

	class MaterialAsset : public Serializeable
	{
	public:
		static Ref<Material> ReadAsset(char* assetData, unsigned int assetSize);

		virtual Ref<char> Serialize() override;
		virtual void Serialize(Ref<char> byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequireSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;

		Ref<Material> CreateInstance();
	private:
		Ref<Material> m_GlobalVer;
	};
};


