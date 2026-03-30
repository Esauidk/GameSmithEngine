#pragma once
#include "GameSmithEngine/SerializeableFiles/ResourceAssets/Asset.h"
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/Shader.h"

namespace GameSmith {
	class GE_API ShaderAsset : public Asset
	{
	public:
		ShaderAsset(std::string name) : Asset(name) {};
		inline ShaderAsset(std::string name, const char* data, unsigned int dataSize);

		virtual Ref<char> Serialize() override;
		virtual void Serialize(char* byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequiredSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;

		Ref<Shader> GetShader();

		const char* GetRawByteCode() const { return m_SerializedData.get(); }
		unsigned int GetByteCodeSize() const { return m_SerializedDataSize; }
	private:
		Ref<Shader> m_Shader;

		Ref<char> m_SerializedData;
		unsigned int m_SerializedDataSize = 0;
	};
};


