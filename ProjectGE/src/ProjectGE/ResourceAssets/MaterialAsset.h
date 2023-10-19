#pragma once
#include "ProjectGE/ResourceManagement/Resource.h"
#include "ProjectGE/Rendering/RenderAgnostics/MaterialSystem/Material.h"
namespace ProjectGE {
	struct MaterialAssetMetadata {
		unsigned int ParamterCount;
		unsigned int TetureCount;
		struct ShaderMetadata {
			bool UsedShader;
		}shaders[STAGE_NUM];
	};

	class MaterialAssetHelper{
	public:
		static Ref<Material> ReadAsset(char* assetData, unsigned int assetSize);
	};

	class MaterialAsset : public Resource
	{
	public:
		virtual void Init() override;
		virtual void Destroy() override;

		Ref<Material> CreateInstance();
	private:
		Ref<Material> m_GlobalVer;
	};
};


