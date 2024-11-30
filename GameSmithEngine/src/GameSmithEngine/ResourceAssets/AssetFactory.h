#pragma once

#include <gepch.h>
#include "GameSmithEngine/ResourceAssets/Serializable.h"

#define GE_REGISTERASSET(ClassType) \
	 static struct ClassType##RegisterAction { \
		ClassType##RegisterAction() { \
			GameSmith::AssetRegistry::GetInstance()->RegisterAsset( \
				ClassType::GetStaticFileExtension(), []() {return new ClassType(); } \
			); \
		} \
	}ClassType##Instance;

namespace GameSmith {

	class AssetRegistry {
	public:
		static AssetRegistry* GetInstance();

		void RegisterAsset(std::string ext, std::function<Serializeable* ()> creationFunction);
	private:
		static Scope<AssetRegistry> s_Instance;

		std::unordered_map<std::string, std::function<Serializeable* ()>> m_ExtToAsset;

		friend class AssetFactory;
	};

	class AssetFactory {
	private:
		static Ref<Serializeable> GenerateAsset(std::string ext);

		friend class ResourceManager;
	};
};

