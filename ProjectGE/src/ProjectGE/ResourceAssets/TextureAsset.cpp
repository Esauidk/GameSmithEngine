#include "gepch.h"
#include "TextureAsset.h"
#include "ProjectGE/Core/Log.h"
#include "ProjectGE/Rendering/RenderingManager.h"


namespace ProjectGE {
	void TextureAsset::Init()
	{
		auto manager = RenderingManager::GetInstance();
		
		if (manager != nullptr) {
			auto renderAPI = manager->GetRenderAPI();
			m_GraphicTex = renderAPI->CreateTexture2D(GetResourceData(), GetResourceSize());
		}
		else {
			GE_CORE_INFO("RenderManager Not Initialized: Not Allocating GPU Resources for Texture Asset");
		}
	}

	void TextureAsset::Destroy()
	{
		m_GraphicTex = nullptr;
	}

	void TextureAsset::SetGraphicsTexture()
	{
		auto manager = RenderingManager::GetInstance();

		if (manager != nullptr) {
			auto renderAPI = manager->GetRenderAPI();
			renderAPI->SetTexture2D(m_GraphicTex, STAGE_PIXEL);
		}
		else {
			GE_CORE_INFO("RenderManager Not Initialized: Not Allocating GPU Resources for Texture Asset");
		}
	}

};

