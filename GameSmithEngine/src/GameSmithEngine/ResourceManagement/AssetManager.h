#pragma once
#include "gepch.h"
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/SerializeableFiles/ResourceAssets/Asset.h"
#include "ResourceLoaders/ResourceLoader.h"


namespace GameSmith {
	enum class ResourceLoaderType {
		Heap
	};

	struct ResourceFileMetadata {
		idData ID;
	};

	struct ResourceMaps {
		std::unordered_map<ID, Ref<IAsset>, IDHasher> ActiveIDResources;
		std::unordered_map<std::string, Ref<IAsset>> ActivePathResources;
		std::unordered_map<ID, std::string, IDHasher> ResourceRegistry;
		std::unordered_map<std::string, ID> ReverseResourceRegistry;
		std::unordered_map <std::string, std::string> filePathToFileName;
	};

	/// <summary>
	/// This class is intended to manage game assets throughout their lifetime
	/// 
	/// It will provide, cache, and coordinate asset's creation and deletion in memory
	/// </summary>
	class GE_API AssetManager
	{
	public:
		inline static AssetManager* GetInstance() { return s_Instance; }

		static void Init(const ResourceLoaderType loaderType);
		static void Shutdown();

		/// <summary>
		/// Loads a requested resource into memory and caches for future requests
		/// </summary>
		/// <typeparam name="T"> The Asset type to load </typeparam>
		/// <param name="asset"> The ID tied to the resource to load </param>
		/// <returns> A shared ownership reference to the loaded resource </returns>
		template<typename T>
		Ref<T> GetResource(const ID asset) {
			if (m_ResourceMaps->ActiveIDResources.contains(asset)) {
				Ref<IAsset> ptr = (*m_ResourceMaps->ActiveIDResources.find(asset)).second;
				return CastPtr<T>(ptr);
			}

			GE_CORE_ASSERT(m_ResourceMaps->ResourceRegistry.contains(asset), "No UUID entry for asset");
			GE_CORE_INFO("Loading file into memory!");
			const std::string filePath = m_ResourceMaps->ResourceRegistry.find(asset)->second;
			const std::string fileName = m_ResourceMaps->filePathToFileName.find(filePath)->second;

			UINT size;
			char* data = m_Loader->LoadResource(filePath, &size);

			unsigned int metaSize;
			char* meta = m_Loader->LoadResource(filePath + ".meta", &metaSize);

			GE_CORE_ASSERT(metaSize == sizeof(ResourceFileMetadata), "Meta data of resource has been manipulated and does not match the expected size");

			const Ref<T> resource = Ref<T>(new T(fileName));
			resource->Deserialize(data, size);

			const ResourceFileMetadata* metaPtr = (ResourceFileMetadata*)meta;
			ID metaId(metaPtr->ID);

			resource->SetID(metaId);

			m_ResourceMaps->ActiveIDResources.insert({ asset, resource });

			m_Loader->CleanResource(data);
			m_Loader->CleanResource(meta);

			return resource;
		}

		/// <summary>
		/// Loads a requested resource into memory and caches for future requests
		/// </summary>
		/// <param name="asset"> The ID tied to the resource to load </param>
		/// <returns> A shared ownership reference to the loaded resource </returns>
		Ref<IAsset> GetResource(const ID asset);

		/// <summary>
		/// Loads a requested resource into memory and caches for future requests
		/// </summary>
		/// <typeparam name="T"> The Asset type to load </typeparam>
		/// <param name="asset"> The filepath to find the resource </param>
		/// <returns> A shared ownership reference to the loaded resource </returns>
		template<typename T>
		Ref<T> GetResource(const std::string& asset) {
			if (m_ResourceMaps->ActivePathResources.contains(asset)) {
				Ref<IAsset> ptr = (*m_ResourceMaps->ActivePathResources.find(asset)).second;
				return CastPtr<T>(ptr);
			}

			// Because this is a resource that hasn't been indexed, we need to perform string manipulation during runtime
			const std::string fileNameWithExt = asset.substr(asset.find_last_of("/") + 1);
			const std::string fileName = fileNameWithExt.substr(0, asset.find_last_of("."));

			GE_CORE_INFO("Loading file into memory!");
			UINT size;
			char* data = m_Loader->LoadResource(asset, &size);

			const Ref<T> resource = Ref<T>(new T(fileName));
			resource->Deserialize(data, size);

			m_ResourceMaps->ActivePathResources.insert({ asset, resource });

			m_Loader->CleanResource(data);

			return resource;
		}

		/// <summary>
		/// Loads a requested resource into memory and caches for future requests
		/// </summary>
		/// <param name="asset"> The filepath to find the resource </param>
		/// <returns> A shared ownership reference to the loaded resource </returns>
		Ref<IAsset> GetResource(const std::string& asset);

		
		/// <summary>
		/// Ingests a requested resource into memory and caches for future requests
		/// 
		/// EXPECTED TO BE USED ONLY FOR TESTING
		/// </summary>
		/// <typeparam name="T"> The Asset type to load </typeparam>
		/// <param name="key"> The ID to associate the resource with </param>
		/// <param name="inData"> The data stream to pull the resource from </param>
		/// <param name="size"> The size of the data stream </param>
		/// <returns> A shared ownership reference to the loaded resource </returns>
		template<typename T>
		Ref<T> GetResource(const ID key, const char* inData, const UINT size) {
			if (m_ResourceMaps->ActiveIDResources.contains(key)) {
				Ref<IAsset> ptr = (*m_ResourceMaps->ActiveIDResources.find(key)).second;
				return CastPtr<T>(ptr);
			}

			GE_CORE_INFO("Copying data into memory!");
			const char* data = m_Loader->LoadResource(inData, size);

			const Ref<T> resource = Ref<T>(new T());
			resource->Deserialize(data, size);

			m_ResourceMaps->ActiveIDResources.insert({ key, resource });

			m_Loader->CleanResource(data);

			return resource;
		}

		/// <summary>
		/// Writes a new resource to the file system and associates an ID to the resource
		/// </summary>
		/// <param name="resource"> The resource to serialize </param>
		/// <param name="path"> The file system destination for the resource </param>
		/// <returns> The ID associated with the resource </returns>
		ID WriteResource(const Ref<IAsset> resource, const std::string& path);
		/// <summary>
		/// Imports a raw file into the engine asset management system
		/// </summary>
		/// <param name="path"> The file system source to import the resource from </param>
		/// <returns> The ID associated with the imported resource </returns>
		ID ImportResource(const std::string& path);
		/// <summary>
		/// Looks up the ID associated with the requested resource
		/// </summary>
		/// <param name="path"> The file system source to lookup </param>
		/// <returns> The requested resource's ID </returns>
		ID GetAssetID(const std::string& path) const;

		/// <summary>
		/// Scans all resources available under the monitored asset directory
		/// </summary>
		void ScanResources();
		/// <summary>
		/// Cleans up resources no longer referenced outside of the asset management system
		/// </summary>
		void CleanResources();

		/// <summary>
		/// Sets the asset directory path used by the asset management system
		/// </summary>
		/// <param name="dir"> The directory path to use for locating assets </param>
		void SetAssetDirectory(const std::string& dir) { m_AssetDirectory = dir; }
	private:
		AssetManager();
	private:
		static AssetManager* s_Instance;

		Scope<ResourceMaps> m_ResourceMaps;
		Ref<ResourceLoader> m_Loader;

		std::string m_AssetDirectory;
	};
};


