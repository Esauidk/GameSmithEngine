#pragma once
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/Texture.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Shaders/ShaderUtil.h"
#include "Serializable.h"


namespace GameSmith {
	class TextureAsset : public Serializeable
	{
	public:
		virtual Ref<char> Serialize() override;
		virtual void Serialize(Ref<char> byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequireSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;

		void SetGraphicsTexture(UINT texSlot, Stages stage);

		//TODO: Remove, this is for testing
		Ref<Texture2D> GetTexture() { return m_GraphicTex; }
	private:
		Ref<Texture2D> m_GraphicTex;
	};
};


