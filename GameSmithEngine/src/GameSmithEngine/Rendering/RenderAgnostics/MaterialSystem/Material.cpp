#include "gepch.h"
#include "Material.h"

#include "GameSmithEngine/Rendering/RenderingManager.h"

namespace GameSmith {
	Material::Material(ShaderSet shaders, MaterialConfig config,
		std::vector<std::string> order,
		std::vector<std::string> textureOrder,
		std::unordered_map<std::string, Ref<ParameterContainer>> paramters,
		std::unordered_map<std::string, Ref<TextureAsset>> textures) :
		m_Shaders(shaders),
		m_Config(config),
		m_ParameterKeys(order),
		m_TextureKeys(textureOrder),
		m_Paramters(paramters),
		m_Textures(textures),
		m_ParameterByteTotal(0)
	{
		for (auto& entry : m_Paramters) {
			m_ParameterByteTotal += entry.second->GetSize();
		}

		auto renderManager = RenderingManager::GetInstance();

		GE_CORE_ASSERT(renderManager != nullptr, "Render manager is not running! Required for material to be used");

		m_GPULocation = renderManager->GetRenderAPI()->CreateConstantBuffer(m_ParameterByteTotal);

		m_PSOSettings.shaderSet = m_Shaders;
		m_PSOSettings.toplopgyType = TopologyType::Triangle;
		m_PSOSettings.numRT = 2;
		m_PSOSettings.tesselation = m_Shaders.shaders[STAGE_HULL] != nullptr || m_Shaders.shaders[STAGE_DOMAIN] != nullptr;

		m_PSO = renderManager->GetPSOManager()->RetrieveOrCreateStateObject(m_PSOSettings);
	}

	Material::Material(Material& oldMat) : 
		m_Shaders(oldMat.m_Shaders), 
		m_Config(oldMat.m_Config), 
		m_ParameterByteTotal(oldMat.m_ParameterByteTotal), 
		m_ParameterKeys(oldMat.m_ParameterKeys),
		m_TextureKeys(oldMat.m_TextureKeys),
		m_Textures(oldMat.m_Textures)
	{
		std::unordered_map<std::string, Ref<ParameterContainer>> copyMap;
		for (auto entry : oldMat.m_Paramters) {
			copyMap.insert({ entry.first, entry.second->MakeCopy() });
		}

		m_Paramters = copyMap;

		auto renderManager = RenderingManager::GetInstance();

		GE_CORE_ASSERT(renderManager != nullptr, "Render manager is not running! Required for material to be used");

		m_GPULocation = renderManager->GetRenderAPI()->CreateConstantBuffer(m_ParameterByteTotal, "Material Copy");

		m_PSOSettings.shaderSet = m_Shaders;
		m_PSOSettings.numRT = 2;
		m_PSOSettings.toplopgyType = TopologyType::Triangle;
		m_PSOSettings.tesselation = m_Shaders.shaders[STAGE_HULL] != nullptr || m_Shaders.shaders[STAGE_DOMAIN] != nullptr;

		m_PSO = renderManager->GetPSOManager()->RetrieveOrCreateStateObject(m_PSOSettings);
	}

	void Material::SetTexture(std::string textureName, Ref<TextureAsset> newTexture)
	{
		GE_CORE_ASSERT(m_Textures.find(textureName) != m_Textures.end(), "Not a valid texutre in this material");
		m_Textures.insert({ textureName, newTexture });
	}

	void Material::ApplyMaterial() {
		char* temp = new char[m_GPULocation->GetBufferSize()];
		char* cur = temp;
		for (auto& entry : m_ParameterKeys) {
			auto pair = m_Paramters.find(entry);
			char* ptr = pair->second->GetCharData();
			unsigned int bytes = pair->second->GetSize();
			memcpy(cur, ptr, bytes);
			cur += bytes;
		}

		m_GPULocation->UpdateData((BYTE*)temp, m_ParameterByteTotal);
		
		delete[] temp;

		auto renderManager = RenderingManager::GetInstance();

		GE_CORE_ASSERT(renderManager != nullptr, "Render manager is not running! Required for material to be used");

		renderManager->GetRenderAPI()->SetTopology(m_PSOSettings.toplopgyType, m_PSOSettings.tesselation);

		renderManager->GetRenderAPI()->SetGraphicsPipelineState(m_PSO);


		renderManager->GetRenderAPI()->SetConstantBuffer(m_GPULocation, STAGE_VERTEX, ShaderConstantType::Instance);
		renderManager->GetRenderAPI()->SetConstantBuffer(m_GPULocation, STAGE_PIXEL, ShaderConstantType::Instance);

		if (m_PSOSettings.tesselation) {
			renderManager->GetRenderAPI()->SetConstantBuffer(m_GPULocation, STAGE_HULL, ShaderConstantType::Instance);
			renderManager->GetRenderAPI()->SetConstantBuffer(m_GPULocation, STAGE_DOMAIN, ShaderConstantType::Instance);
		}

		UINT slot = 0;
		for (auto& entry : m_TextureKeys) {
			auto pair = m_Textures.find(entry);
			pair->second->SetGraphicsTexture(slot, STAGE_VERTEX);
			pair->second->SetGraphicsTexture(slot, STAGE_PIXEL);

			if (m_PSOSettings.tesselation) {
				pair->second->SetGraphicsTexture(slot, STAGE_HULL);
				pair->second->SetGraphicsTexture(slot, STAGE_DOMAIN);
			}
			slot++;
		}

		
	}
};
