#pragma once
#include <string>
#include <unordered_map>
#include "GameSmithEngine/Utilities/GUIDGenerator.h"

namespace GameSmith {
	struct CacheEntry {
		Ref<const char> content;
		unsigned int size;
	};

	class GE_API ShaderIncludeCache {
	public:
		ShaderIncludeCache() = default;
		~ShaderIncludeCache() = default;
		// Adds a shader include to the cache
		void AddInclude(const std::string& includeName, const ID& includeID);
		// Retrieves a shader include from the cache and allocates space on the heap (ownership is tranferred to caller)
		const char* RetrieveShaderContent(const std::string& includeName, unsigned int* outSourceSize);

		// Clears the cache of all known shader sources
		void ClearCacheSources() { m_SourceCache.clear(); }
	private:
		std::unordered_map<std::string, ID> m_IncludeCache; // Maps include names to their content
		std::unordered_map<std::string, CacheEntry> m_SourceCache; // Maps preset include names to their content
	};
}