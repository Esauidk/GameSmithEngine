#pragma once
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/Texture.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Shaders/ShaderUtil.h"
#include "Asset.h"


namespace GameSmith {
	class GE_API TextureAsset : public Asset
	{
	public:
		virtual Ref<char> Serialize() override;
		virtual void Serialize(char* byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequireSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;

		Ref<Texture2D> GetTexture() { return m_GraphicTex; }
	private:
		Ref<Texture2D> m_GraphicTex;
	};
};


