#pragma once
#include "GameSmithEngine/ResourceManagement/Resource.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/MaterialSystem/Material.h"
namespace GameSmith {
	struct MaterialAssetMetadata {
		unsigned int ParamterCount;
		unsigned int TetureCount;
		struct ShaderMetadata {
			bool UsedShader = false;
			unsigned int stringLength = 0;
		}Shaders[STAGE_NUM];
	};

	class MaterialAsset : public Resource
	{
	public:
		static Ref<Material> ReadAsset(char* assetData, unsigned int assetSize);

		virtual void Init() override;
		virtual void Destroy() override;

		Ref<Material> CreateInstance();
	private:
		Ref<Material> m_GlobalVer;
	};
};


