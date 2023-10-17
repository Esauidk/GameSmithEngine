#pragma once
#include "ProjectGE/Core/Core.h"
#include "ProjectGE/ResourceManagement/Resource.h"
#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/Texture.h"
#include "ProjectGE/Rendering/RenderAgnostics/Shaders/ShaderUtil.h"


namespace ProjectGE {
	class TextureAsset : public Resource
	{
	public:
		virtual void Init() override;
		virtual void Destroy() override;

		void SetGraphicsTexture(UINT texSlot, Stages stage);
	private:
		Ref<Texture2D> m_GraphicTex;
	};
};


