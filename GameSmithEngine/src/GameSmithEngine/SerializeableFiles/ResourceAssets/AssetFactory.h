#pragma once

#include <gepch.h>
#include "GameSmithEngine/SerializeableFiles/ResourceAssets/Asset.h"

#define GE_REGISTERASSET(ClassType) \
	 static struct ClassType##RegisterAction { \
		ClassType##RegisterAction() { \
			GameSmith::AssetRegistry::GetInstance()->RegisterAsset( \
				ClassType::GetStaticFileExtension(), [](std::string name) {return new ClassType(name); } \
			); \
		} \
	}ClassType##Instance;

namespace GameSmith {

	class AssetRegistry {
	public:
		static AssetRegistry* GetInstance();

		void RegisterAsset(std::string ext, std::function<IAsset* (std::string)> creationFunction);
	private:
		static Scope<AssetRegistry> s_Instance;

		std::unordered_map<std::string, std::function<IAsset* (std::string)>> m_ExtToAsset;

		friend class AssetFactory;
	};

	class AssetFactory {
	private:
		static Ref<IAsset> GenerateAsset(std::string ext, std::string fileName);

		friend class AssetManager;
	};
};

