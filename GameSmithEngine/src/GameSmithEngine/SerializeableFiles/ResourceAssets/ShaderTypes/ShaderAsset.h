#pragma once
#include "GameSmithEngine/SerializeableFiles/ResourceAssets/Asset.h"
#include "GameSmithEngine/SerializeableFiles/ResourceAssets/ShaderTypes/HLSLAsset.h"
#include "GameSmithEngine/SerializeableFiles/ResourceAssets/ShaderTypes/ShaderConfigAsset.h"
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/Shader.h"

namespace GameSmith {
	struct HLSLSourceSet {
		Ref<HLSLAsset> sources[STAGE_NUM];
	};

	class GE_API ShaderAsset : public Asset
	{
	public:
		ShaderAsset(std::string name) : Asset(name) {
			for (int i = 0; i < STAGE_NUM; i++) {
				Stages stage = (Stages)i;
				m_Registry.AddExposedAsset<HLSLAsset>(
					ConvertShaderStageToString(stage),
					(Ref<GameSmith::IAsset>*) & (m_HLSLSource.sources[i]),
					CLASS_TO_STRING(HLSLAsset)
				);
				m_Shaders[i] = nullptr;
			}

			m_Registry.AddExposedAsset<ShaderConfigAsset>(
				"ShaderConfig",
				(Ref<GameSmith::IAsset>*)&m_ShaderConfig,
				CLASS_TO_STRING(ShaderConfigAsset)
			);
		};

		ShaderAsset(std::string name, HLSLSourceSet source) : Asset(name), m_HLSLSource(source) {
			for (int i = 0; i < STAGE_NUM; i++) {
				Stages stage = (Stages)i;
				m_Registry.AddExposedAsset<HLSLAsset>(
					ConvertShaderStageToString(stage),
					(Ref<GameSmith::IAsset>*) & (m_HLSLSource.sources[i]),
					CLASS_TO_STRING(HLSLAsset)
				);
				m_Shaders[i] = nullptr;
			}

			m_Registry.AddExposedAsset<ShaderConfigAsset>(
				"ShaderConfig",
				(Ref<GameSmith::IAsset>*) & m_ShaderConfig,
				CLASS_TO_STRING(ShaderConfigAsset)
			);
		};

		virtual void Serialize(char* byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequiredSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;

		bool HasShader(const Stages stage) const;
		Ref<Shader> GetShader(const Stages stage);
		Ref<const ShaderConfigAsset> GetShaderConfig() const { return m_ShaderConfig; }

		SERIAL_FILE(GMShader, sr)

	private:
		Ref<Shader> m_Shaders[STAGE_NUM];
		HLSLSourceSet m_HLSLSource;
		Ref<ShaderConfigAsset> m_ShaderConfig;
	};
};


