#include <gepch.h>
#include "AssetFactory.h"
#include "GameSmithEngine/Utilities/Factory/FactoryImpl.h"
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

	void AssetRegistry::RegisterAsset(std::string ext, std::string assetLabel, std::function<IAsset* (std::string)> creationFunction)
	{
		if (!m_ExtToAsset.contains(ext)) {
			m_ExtToAsset.insert({ ext, creationFunction });
			m_RegisteredAssets.push_back({ ext, assetLabel });
		}
	}

	template class GE_API Factory<IAsset, AssetManager, std::string>;
};

