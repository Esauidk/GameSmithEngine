#pragma once
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/ConstantBuffer.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/PipelineStateObject.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/PipelineStateInitializer.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/Texture.h"
#include "GameSmithEngine/Utilities/ParameterContainer.h"
#include "GameSmithEngine/Core/Log.h"

namespace GameSmith {
	struct MaterialConfig {
	};

	class Material
	{
		friend class MaterialAsset;
	public:
		Material(ShaderSet& shaders, MaterialConfig& config,
			std::vector<std::string>& parameterOrder,
			std::vector<std::string>& textureOrder,
			std::unordered_map<std::string, Ref<ParameterContainer>>& paramters,
			std::unordered_map<std::string, Ref<Texture2D>>& textures);

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

		void SetTexture(std::string textureName, Ref<Texture2D> newTexture);
		void ApplyMaterial();

		inline const ShaderSet& GetShaderSet() { return m_Shaders; }
	private:
		const std::unordered_map<std::string, Ref<ParameterContainer>>& DumpCurrentParameterMap() { return m_Paramters; }
		const std::vector<std::string>& DumpParameterOrder() { return m_ParameterKeys; }
	private:
		ShaderSet m_Shaders;
		MaterialConfig m_Config;
		std::vector<std::string> m_ParameterKeys;
		std::vector<std::string> m_TextureKeys;
		std::unordered_map<std::string, Ref<ParameterContainer>> m_Paramters;
		std::unordered_map<std::string, Ref<Texture2D>> m_Textures;

		unsigned int m_ParameterByteTotal;
		Ref<ConstantBuffer> m_GPULocation;
		Ref<GraphicsPipelineStateObject> m_PSO;
		PipelineStateInitializer m_PSOSettings;
	};
};


