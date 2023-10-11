#pragma once
#include "ProjectGE/Rendering/RenderAgnostics/Shaders/ShaderParameter.h"
#include "ProjectGE/ResourceAssets/TextureAsset.h"
#include "ProjectGE/Core/Log.h"

namespace ProjectGE {
	class Material
	{
	public:
		Material(std::unordered_map<std::string , Ref<ShaderParameter>> paramters, 
			std::unordered_map<std::string, Ref<TextureAsset>> textures) : 
			m_Paramters(paramters), 
			m_Textures(textures) 
		{};

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
		void ApplyMaterial();
	private:
		std::unordered_map<std::string, Ref<ShaderParameter>> m_Paramters;
		std::unordered_map<std::string, Ref<TextureAsset>> m_Textures;
	};
};


