#include "gepch.h"
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Core/Log.h"
#include "ShaderIncludeCache.h"

#include "GameSmithEngine/ResourceManagement/AssetManager.h"
#include "GameSmithEngine/SerializeableFiles/ResourceAssets/ShaderTypes/ShaderAsset.h"

namespace GameSmith {
	void ShaderIncludeCache::AddInclude(const std::string& includeName, const ID& includeID)
	{
		GE_APP_ASSERT(!m_IncludeCache.contains(includeName), "Shader include already exists in cache");
		m_IncludeCache[includeName] = includeID;
	}

	const char* ShaderIncludeCache::RetrieveShaderContent(const std::string& includeName, unsigned int* outSourceSize) const
	{
		auto assetManager = AssetManager::GetInstance();
		GE_CORE_ASSERT(assetManager != nullptr, "AssetManager is not loaded, cannot retrieve shader content");
		GE_CORE_ASSERT(m_IncludeCache.contains(includeName), "Not shader entry for {0}", includeName);

		const ID& includeID = m_IncludeCache.find(includeName)->second;

		auto shader = assetManager->GetResource<ShaderAsset>(includeID);

		*outSourceSize = shader->GetByteCodeSize();
		return shader->GetRawByteCode();
	}
};