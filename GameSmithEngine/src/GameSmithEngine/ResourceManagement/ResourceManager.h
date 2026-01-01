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
		std::unordered_map<ID, Ref<Serializeable>, IDHasher> ActiveIDResources;
		std::unordered_map<std::string, Ref<Serializeable>> ActivePathResources;
		std::unordered_map<ID, std::string, IDHasher> ResourceRegistry;
		std::unordered_map<std::string, ID> ReverseResourceRegistry;
		std::unordered_map<std::string, std::string> ContentLibraryRegistry;
	};

	class GE_API ResourceManager
	{
	public:
		inline static ResourceManager* GetInstance() { return s_Instance; }

		static void Init(ResourceLoaderType loaderType);
		static void Shutdown();

		template<typename T>
		Ref<T> GetResource(ID asset) {
			if (m_ResourceMaps->ActiveIDResources.contains(asset)) {
				Ref<Serializeable> ptr = (*m_ResourceMaps->ActiveIDResources.find(asset)).second;
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

			m_ResourceMaps->ActiveIDResources.insert({ asset, resource });

			m_Loader->CleanResource(data);
			m_Loader->CleanResource(meta);

			return resource;
		}

		Ref<Serializeable> GetResource(ID asset);

		template<typename T>
		Ref<T> GetResource(std::string asset) {
			if (m_ResourceMaps->ActivePathResources.contains(asset)) {
				Ref<Serializeable> ptr = (*m_ResourceMaps->ActivePathResources.find(asset)).second;
				return CastPtr<T>(ptr);
			}

			GE_CORE_INFO("Loading file into memory!");
			UINT size;
			char* data = m_Loader->LoadResource(asset, &size);

			Ref<T> resource = Ref<T>(new T());
			resource->Deserialize(data, size);

			m_ResourceMaps->ActivePathResources.insert({ asset, resource });

			m_Loader->CleanResource(data);

			return resource;
		}

		Ref<Serializeable> GetResource(std::string asset);

		const std::unordered_map<std::string, std::string>& GetContentLibrariesFiles() { return m_ResourceMaps->ContentLibraryRegistry; };

		// Expected to be used only during testing
		template<typename T>
		Ref<T> GetResource(ID key, char* inData, UINT size) {
			if (m_ResourceMaps->ActiveIDResources.contains(key)) {
				Ref<Serializeable> ptr = (*m_ResourceMaps->ActiveIDResources.find(key)).second;
				return CastPtr<T>(ptr);
			}

			GE_CORE_INFO("Copying data into memory!");
			char* data = m_Loader->LoadResource(inData, size);

			Ref<T> resource = Ref<T>(new T());
			resource->Deserialize(data, size);

			m_ResourceMaps->ActiveIDResources.insert({ key, resource });

			m_Loader->CleanResource(data);

			return resource;
		}

		ID WriteResource(Ref<Serializeable> resource, std::string path);
		ID ImportResource(std::string path);
		ID GetAssetID(std::string path);

		void ScanResources();
		void ScanContentLibraries();
		void CleanResources();

		void SetAssestDirectory(std::string dir) { m_AssetDirectory = dir; }
		void SetContentLibraryDirectory(std::string dir) { m_ContentLibraryDirectory = dir; }
	private:
		ResourceManager();
	private:
		static ResourceManager* s_Instance;

		Scope<ResourceMaps> m_ResourceMaps;
		Ref<ResourceLoader> m_Loader;

		std::string m_AssetDirectory;
		std::string m_ContentLibraryDirectory;
	};
};


