#pragma once
#include "ProjectGE/Core/Core.h"
#include "ProjectGE/ResourceManagement/Resource.h"
#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/Texture.h"


namespace ProjectGE {
	class TextureAsset : public Resource
	{
	public:
		TextureAsset(char* data, UINT size) : Resource(data, size) {}

		virtual void Init() override;
		virtual void Destroy() override;

		void SetGraphicsTexture();
	private:
		Ref<Texture2D> m_GraphicTex;
	};
};


