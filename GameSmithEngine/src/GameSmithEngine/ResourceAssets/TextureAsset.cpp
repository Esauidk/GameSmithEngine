#include "gepch.h"
#include "TextureAsset.h"
#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/Rendering/RenderingManager.h"


namespace GameSmith {
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

	void TextureAsset::SetGraphicsTexture(UINT texSlot, Stages stage)
	{
		auto manager = RenderingManager::GetInstance();

		if (manager != nullptr) {
			auto renderAPI = manager->GetRenderAPI();
			renderAPI->SetTexture2D(m_GraphicTex, texSlot, stage);
		}
		else {
			GE_CORE_INFO("RenderManager Not Initialized: Not Allocating GPU Resources for Texture Asset");
		}
	}

};

