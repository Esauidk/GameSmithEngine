#pragma once
#include "ProjectGE/Rendering/RenderAgnostics/Shaders/ShaderParameter.h"
#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/ConstantBuffer.h"
#include "ProjectGE/ResourceAssets/TextureAsset.h"
#include "ProjectGE/Core/Log.h"

namespace ProjectGE {
	struct MaterialConfig {
	};

	class Material
	{
	public:
		Material(ShaderSet shaders,
			std::vector<std::string> parameterOrder,
			std::vector<std::string> textureOrder,
			std::unordered_map<std::string, Ref<ShaderParameter>> paramters,
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
		std::unordered_map<std::string, Ref<ShaderParameter>> m_Paramters;
		std::unordered_map<std::string, Ref<TextureAsset>> m_Textures;

		unsigned int m_ParameterByteTotal;
		Ref<ConstantBuffer> m_GPULocation;
	};
};


