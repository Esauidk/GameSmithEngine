#pragma once
#include "GameSmithEngine/SerializeableFiles/ResourceAssets/Asset.h"
#include "GameSmithEngine/SerializeableFiles/ResourceAssets/ShaderTypes/HLSLAsset.h"
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/Shader.h"

namespace GameSmith {
	struct HLSLSourceSet {
		Ref<HLSLAsset> sources[STAGE_NUM];
	};

	class GE_API ShaderAsset : public Asset
	{
	public:
		ShaderAsset(std::string name) : Asset(name) {};
		ShaderAsset(std::string name, HLSLSourceSet source) : Asset(name), m_HLSLSource(source) {};

		virtual Ref<char> Serialize() override;
		virtual void Serialize(char* byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequiredSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;

		bool HasShader(const Stages stage) const;
		Ref<Shader> GetShader(const Stages stage);

		SERIAL_FILE(GMShader, sr)

	private:
		Ref<Shader> m_Shaders[STAGE_NUM];
		HLSLSourceSet m_HLSLSource;
	};
};


