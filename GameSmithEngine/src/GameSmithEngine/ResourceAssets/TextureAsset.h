#pragma once
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/ResourceManagement/Resource.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/Texture.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Shaders/ShaderUtil.h"


namespace GameSmith {
	class TextureAsset : public Resource
	{
	public:
		virtual void Init() override;
		virtual void Destroy() override;

		void SetGraphicsTexture(UINT texSlot, Stages stage);

		//TODO: Remove, this is for testing
		Ref<Texture2D> GetTexture() { return m_GraphicTex; }
	private:
		Ref<Texture2D> m_GraphicTex;
	};
};


