#pragma once
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/ConstantBuffer.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/PipelineStateObject.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/PipelineStateInitializer.h"
#include "GameSmithEngine/ResourceAssets/TextureAsset.h"
#include "GameSmithEngine/Utilities/ParameterContainer.h"
#include "GameSmithEngine/Core/Log.h"

namespace GameSmith {
	struct MaterialConfig {
	};

	class Material
	{
	public:
		Material(ShaderSet shaders, MaterialConfig config,
			std::vector<std::string> parameterOrder,
			std::vector<std::string> textureOrder,
			std::unordered_map<std::string, Ref<ParameterContainer>> paramters,
			std::unordered_map<std::string, Ref<TextureAsset>> textures);

		Material(Material& oldMat);

		template<typename T>
		Ref<T> GetParameter(std::string parameterName) {
			if (m_Paramters.contains(parameterName)) {
				return CastPtr<T>(m_Paramters.find(parameterName)->second);
			}

			if (m_Textures.contains(parameterName)) {
				return CastPtr<T>(m_Textures.find(parameterName)->second);
			}

			GE_CORE_ASSERT(false, "Material failed parameter get()");
			GE_CORE_ERROR("Material has no parameter named {0}", parameterName);

			return nullptr;
		}

		void SetTexture(std::string textureName, Ref<TextureAsset> newTexture);
		void ApplyMaterial();
	private:
		ShaderSet m_Shaders;
		MaterialConfig m_Config;
		std::vector<std::string> m_ParameterKeys;
		std::vector<std::string> m_TextureKeys;
		std::unordered_map<std::string, Ref<ParameterContainer>> m_Paramters;
		std::unordered_map<std::string, Ref<TextureAsset>> m_Textures;

		unsigned int m_ParameterByteTotal;
		Ref<ConstantBuffer> m_GPULocation;
		Ref<GraphicsPipelineStateObject> m_PSO;
		PipelineStateInitializer m_PSOSettings;
	};
};


