#pragma once

#include <gepch.h>
#include "GameSmithEngine/Utilities/Factory/Factory.h"
#include "GameSmithEngine/SerializeableFiles/ResourceAssets/Asset.h"

#define GE_REGISTERASSET(ClassType) \
	 static struct ClassType##RegisterAction { \
		ClassType##RegisterAction() { \
			auto factory = GameSmith::AssetFactory::GetInstance(); \
			factory->Register( \
				ClassType::GetStaticFileExtension(), \
				[](std::string name) {return new ClassType(name); } \
			); \
			factory->AssignLabel( \
				ClassType::GetStaticFileExtension(), \
				ClassType::GetStaticFileType() \
			); \
		} \
	}ClassType##Instance;

namespace GameSmith {
	class AssetManager;
	using AssetFactory = Factory<IAsset, AssetManager, std::string>;

	class GE_API AssetRegistry {
	public:
		static AssetRegistry* GetInstance();

		void RegisterAsset(std::string ext, std::string assetLabel, std::function<IAsset* (std::string)> creationFunction);
		void ClearRegistrations() { m_ExtToAsset.clear(); m_RegisteredAssets.clear(); }
		const std::vector<std::pair<std::string, std::string>>& ListRegisteredAssets() { return m_RegisteredAssets; }
	private:
		static Scope<AssetRegistry> s_Instance;

		std::unordered_map<std::string, std::function<IAsset* (std::string)>> m_ExtToAsset;
		std::vector<std::pair<std::string, std::string>> m_RegisteredAssets;
	};
};

