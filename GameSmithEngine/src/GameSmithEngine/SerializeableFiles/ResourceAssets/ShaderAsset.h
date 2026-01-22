#pragma once
#include "Asset.h"
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/Shader.h"

namespace GameSmith {
	class GE_API ShaderAsset : public Asset
	{
	public:
		ShaderAsset(std::string name) : Asset(name) {};
		inline ShaderAsset(std::string name, Ref<Shader> shader) : Asset(name), m_Shader(shader) {};

		virtual Ref<char> Serialize() override;
		virtual void Serialize(char* byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequiredSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;

		inline Ref<Shader> GetShader() const { return m_Shader != nullptr ? m_Shader : nullptr; }
	private:
		Ref<Shader> m_Shader;
	};
};


