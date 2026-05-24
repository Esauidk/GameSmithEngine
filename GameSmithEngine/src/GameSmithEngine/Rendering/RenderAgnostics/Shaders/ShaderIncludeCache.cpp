#include "gepch.h"
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Core/Log.h"
#include "ShaderIncludeCache.h"

#include "GameSmithEngine/ResourceManagement/AssetManager.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Shaders/ShaderPresetsFactory.h"
#include "GameSmithEngine/SerializeableFiles/ResourceAssets/ShaderTypes/HLSLAsset.h"

namespace GameSmith {
	void ShaderIncludeCache::AddInclude(const std::string& includeName, const ID& includeID)
	{
		GE_APP_ASSERT(!m_IncludeCache.contains(includeName), "Shader include already exists in cache");
		m_IncludeCache[includeName] = includeID;
	}

	const char* ShaderIncludeCache::RetrieveShaderContent(const std::string& includeName, unsigned int* outSourceSize)
	{
		Ref<const char> copiedSource = nullptr;
		bool skipCache = false;
		auto ShaderPresetFactory = ShaderPresetFactory::GetInstance();
		GE_CORE_ASSERT(ShaderPresetFactory != nullptr, "ShaderPresetFactory is not loaded, cannot retrieve shader content");

		if (m_SourceCache.contains(includeName)) {
			const auto& entry = m_SourceCache.find(includeName)->second;
			*outSourceSize = entry.size;
			copiedSource = entry.content;
			skipCache = true;
		}
		else if (ShaderPresetFactory->HasShaderPreset(includeName)) {
			const auto& shaderSource = ShaderPresetFactory->GetShaderSource(includeName);
			*outSourceSize = shaderSource.size();
			copiedSource = Ref<const char>(new char[shaderSource.size()]);
			memcpy((char*)copiedSource.get(), shaderSource.c_str(), shaderSource.size());
		}
		else {
			auto assetManager = AssetManager::GetInstance();

			GE_CORE_ASSERT(assetManager != nullptr, "AssetManager is not loaded, cannot retrieve shader content");
			GE_CORE_ASSERT(m_IncludeCache.contains(includeName), "Not shader entry for {0}", includeName);

			const ID& includeID = m_IncludeCache.find(includeName)->second;

			auto source = assetManager->GetResource<HLSLAsset>(includeID);

			copiedSource = Ref<const char>(new char[source->GetSize()]);
			memcpy((char*)copiedSource.get(), source->GetBytes(), source->GetSize());
			*outSourceSize = source->GetSize();
		}
		
		if (!skipCache) {
			CacheEntry entry = { copiedSource, *outSourceSize };
			m_SourceCache[includeName] = entry;
		}

		return copiedSource.get();
	}
};