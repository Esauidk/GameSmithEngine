#include "gepch.h"
#include "TextureAsset.h"
#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/Rendering/RenderingManager.h"


namespace GameSmith {
	Ref<char> TextureAsset::Serialize()
	{
		return Ref<char>();
	}

	void TextureAsset::Serialize(char* byteStream, unsigned int availableBytes)
	{
	}

	unsigned int TextureAsset::RequireSpace() const
	{
		return 0;
	}

	void TextureAsset::Deserialize(char* inData, unsigned int size)
	{
		auto manager = RenderingManager::GetInstance();

		if (manager != nullptr) {
			auto renderAPI = manager->GetRenderAPI();
			m_GraphicTex = renderAPI->CreateTexture2D(inData, size);
		}
		else {
			GE_CORE_INFO("RenderManager Not Initialized: Not Allocating GPU Resources for Texture Asset");
		}
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

