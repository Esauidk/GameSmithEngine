#pragma once
#include "gepch.h"
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/ResourceAssets/Serializable.h"
#include "ResourceLoaders/ResourceLoader.h"


namespace GameSmith {
	enum class ResourceLoaderType {
		Heap
	};

	struct ResourceFileMetadata {
		idData ID;
	};

	struct ResourceMaps {
		std::unordered_map<ID, Ref<Serializeable>, IDHasher> ActiveResources;
		std::unordered_map<ID, std::string, IDHasher> ResourceRegistry;
		std::unordered_map<std::string, ID> ReverseResourceRegistry;
	};

	class GE_API ResourceManager
	{
	public:
		ResourceManager();
		inline static ResourceManager* GetInstance() { return s_Instance; }

		virtual void Init(ResourceLoaderType loaderType);
		virtual void Shutdown();

		template<typename T>
		Ref<T> GetResource(ID asset) {
			if (m_ResourceMaps->ActiveResources.contains(asset)) {
				Ref<Serializeable> ptr = (*m_ResourceMaps->ActiveResources.find(asset)).second;
				return CastPtr<T>(ptr);
			}

			GE_CORE_ASSERT(m_ResourceMaps->ResourceRegistry.contains(asset), "No UUID entry for asset");
			GE_CORE_INFO("Loading file into memory!");
			UINT size;
			char* data = m_Loader->LoadResource(m_ResourceMaps->ResourceRegistry.find(asset)->second, &size);

			unsigned int metaSize;
			char* meta = m_Loader->LoadResource(m_ResourceMaps->ResourceRegistry.find(asset)->second + ".meta", &metaSize);

			GE_CORE_ASSERT(metaSize == sizeof(ResourceFileMetadata), "Meta data of resource has been manipulated and does not match the expected size");

			Ref<T> resource = Ref<T>(new T());
			resource->Deserialize(data, size);

			ResourceFileMetadata* metaPtr = (ResourceFileMetadata*)meta;
			ID metaId(metaPtr->ID);

			resource->SetID(metaId);

			m_ResourceMaps->ActiveResources.insert({ asset, resource });

			m_Loader->CleanResource(data);
			m_Loader->CleanResource(meta);

			return resource;
		}

		// Expected to be used only during testing
		template<typename T>
		Ref<T> GetResource(ID key, char* inData, UINT size) {
			if (m_ResourceMaps->ActiveResources.contains(key)) {
				Ref<Serializeable> ptr = (*m_ResourceMaps->ActiveResources.find(key)).second;
				return CastPtr<T>(ptr);
			}

			GE_CORE_INFO("Copying data into memory!");
			char* data = m_Loader->LoadResource(inData, size);

			Ref<T> resource = Ref<T>(new T());
			resource->Deserialize(data, size);

			m_ResourceMaps->ActiveResources.insert({ key, resource });

			m_Loader->CleanResource(data);

			return resource;
		}

		ID WriteResource(Ref<Serializeable> resource, std::string path);
		ID ImportResource(std::string path);
		ID GetAssetID(std::string path);

		void ScanResources();
		void CleanResources();

		void SetAssestDirectory(std::string dir) { m_AssetDirectory = dir; }

	private:
		static ResourceManager* s_Instance;

		Scope<ResourceMaps> m_ResourceMaps;

		Ref<ResourceLoader> m_Loader;

		std::string m_AssetDirectory;
	};
};


