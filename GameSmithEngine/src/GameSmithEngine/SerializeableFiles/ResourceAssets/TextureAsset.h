#pragma once
#include "Asset.h"
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/Texture.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Shaders/ShaderUtil.h"


namespace GameSmith {
	class GE_API TextureAsset : public Asset
	{
	public:
		TextureAsset(std::string name) : Asset(name) {};

		virtual Ref<char> Serialize() override;
		virtual void Serialize(char* byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequiredSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;

		Ref<Texture2D> GetTexture() { return m_GraphicTex; }
	private:
		Ref<Texture2D> m_GraphicTex;
	};
};


