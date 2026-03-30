#pragma once
#include <string>
#include <unordered_map>
#include "GameSmithEngine/Utilities/GUIDGenerator.h"

namespace GameSmith {
	class GE_API ShaderIncludeCache {
	public:
		ShaderIncludeCache() = default;
		~ShaderIncludeCache() = default;
		// Adds a shader include to the cache
		void AddInclude(const std::string& includeName, const ID& includeID);
		// Retrieves a shader include from the cache and allocates space on the heap (ownership is tranferred to caller)
		const char* RetrieveShaderContent(const std::string& includeName, unsigned int* outSourceSize) const;
	private:
		std::unordered_map<std::string, ID> m_IncludeCache; // Maps include names to their content
	};
}