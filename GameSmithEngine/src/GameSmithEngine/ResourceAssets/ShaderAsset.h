#pragma once
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/Shader.h"
#include "Serializable.h"

namespace GameSmith {
	class ShaderAsset : public Serializeable
	{
	public:
		ShaderAsset() = default;
		inline ShaderAsset(Ref<Shader> shader) : m_Shader(shader) {};

		virtual Ref<char> Serialize() override;
		virtual void Serialize(char* byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequireSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;

		inline Ref<Shader> GetShader() const { return m_Shader != nullptr ? m_Shader : nullptr; }
	private:
		Ref<Shader> m_Shader;
	};
};


