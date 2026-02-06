#include <gepch.h>
#include "AssetFactory.h"
#include "GameSmithEngine/Core/Log.h"

namespace GameSmith {
	Scope<AssetRegistry> AssetRegistry::s_Instance = nullptr;

	AssetRegistry* AssetRegistry::GetInstance()
	{
		if (s_Instance == nullptr) {
			s_Instance = Scope<AssetRegistry>(new AssetRegistry());
		}

		return s_Instance.get();
	}
	void AssetRegistry::RegisterAsset(std::string ext, std::function<IAsset* (std::string)> creationFunction)
	{
		if (!m_ExtToAsset.contains(ext)) {
			m_ExtToAsset.insert({ ext, creationFunction });
		}
	}

	Ref<IAsset> AssetFactory::GenerateAsset(std::string ext, std::string fileName)
	{
		auto registry = AssetRegistry::GetInstance();

		GE_CORE_ASSERT(registry->m_ExtToAsset.contains(ext), std::format("No definition for requested extention: {0}", ext));

		auto entry = registry->m_ExtToAsset.find(ext);
		return Ref<IAsset>(entry->second(fileName));
	}
};

