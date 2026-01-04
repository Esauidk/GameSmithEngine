#include "gepch.h"
#include "ResourceManager.h"
#include "GameSmithEngine/Core/Log.h"

#include "ResourceLoaders/HeapResourceLoader.h"

#include <filesystem>

#include "GameSmithEngine/ResourceAssets/AssetFactory.h"

#define META_FILE_EXTENSION ".meta"
#define CONTENT_LIBRARY_FILE_EXTENSION "dll"

namespace fs = std::filesystem;
using recursive_directory_iterator = fs::recursive_directory_iterator;

namespace GameSmith {
	ResourceManager* ResourceManager::s_Instance = nullptr;

	static Ref<ResourceLoader> GetLoader(ResourceLoaderType loaderType) {
		switch (loaderType) {
		case ResourceLoaderType::Heap:
			return Ref<ResourceLoader>(new HeapResourceLoader());
		default:
			return Ref<ResourceLoader>(new HeapResourceLoader());
		}
	}

	ResourceManager::ResourceManager() : m_ResourceMaps(new ResourceMaps()), m_Loader(nullptr), m_AssetDirectory(fs::current_path().string()), m_ContentLibraryDirectory(fs::current_path().string())
	{
		if (s_Instance == nullptr) {
			s_Instance = this;
		}
	}

	void ResourceManager::Init(ResourceLoaderType loaderType)
	{
		if (s_Instance == nullptr) {
			s_Instance = new ResourceManager();
			s_Instance->m_Loader = GetLoader(loaderType);
			s_Instance->ScanResources();
			GE_CORE_INFO("Resource Manager Loaded!");
		}
		
	}

	void ResourceManager::Shutdown()
	{
		if (s_Instance != nullptr) {
			delete s_Instance;
			s_Instance = nullptr;
		}
	}

	ID ResourceManager::WriteResource(Ref<Serializeable> resource, std::string path)
	{
		std::fstream pFile(path, std::ios::out | std::ios::binary | std::ios::ate);
		GE_CORE_ASSERT(pFile.is_open(), std::format("Asset file {0} cannot be opened", path));

		std::fstream metaFile(path + ".meta", std::ios::out | std::ios::binary | std::ios::ate);
		GE_CORE_ASSERT(metaFile.is_open(), std::format("Asset file {0} cannot be opened", path));

		auto serial = resource->Serialize();
		auto id = resource->GetID();

		pFile.seekg(0, pFile.beg);
		pFile.write(serial.get(), resource->RequiredSpace());
		pFile.close();

		ResourceFileMetadata meta;
		meta.ID = id.getData();

		metaFile.seekg(0, metaFile.beg);
		metaFile.write((char*)&meta, sizeof(meta));
		metaFile.close();

		m_ResourceMaps->ResourceRegistry.insert({ id, path });
		m_ResourceMaps->ReverseResourceRegistry.insert({ path, meta.ID });

		return id;
	}

	ID ResourceManager::ImportResource(std::string path)
	{
		// TOOD: Add more to import logic, for now it's just ID assignment
		std::fstream metaFile(path + ".meta", std::ios::out | std::ios::binary | std::ios::ate);
		GE_CORE_ASSERT(metaFile.is_open(), std::format("Asset file {0} cannot be opened", path));

		auto newID = GUIDGenerator::GenerateID();

		ResourceFileMetadata meta;
		meta.ID = newID.getData();

		metaFile.seekg(0, metaFile.beg);
		metaFile.write((char*)&meta, sizeof(meta));
		metaFile.close();

		m_ResourceMaps->ResourceRegistry.insert({ meta.ID, path });
		m_ResourceMaps->ReverseResourceRegistry.insert({ path, meta.ID });

		return newID;
	}

	ID ResourceManager::GetAssetID(std::string path)
	{
		if (m_ResourceMaps->ReverseResourceRegistry.contains(path)) {
			return m_ResourceMaps->ReverseResourceRegistry.find(path)->second;
		}

		return ID();
	}

	void ResourceManager::ScanResources()
	{
		if (m_AssetDirectory != "") {
			for (const auto& dirEntry : recursive_directory_iterator(m_AssetDirectory)) {
				if (dirEntry.is_regular_file()) {
					std::string fileName = dirEntry.path().filename().string();
					std::string path = dirEntry.path().string();

					if (std::filesystem::exists(path + META_FILE_EXTENSION)) {
						unsigned int metaSize;
						char* meta = m_Loader->LoadResource(path + META_FILE_EXTENSION, &metaSize);
						ResourceFileMetadata* metaPtr = (ResourceFileMetadata*)meta;
						ID metaId(metaPtr->ID);

						if (!m_ResourceMaps->ResourceRegistry.contains(metaId)) {
							m_ResourceMaps->ResourceRegistry.insert({ metaId, path });
							m_ResourceMaps->ReverseResourceRegistry.insert({ path, metaId });
						}
					}
					 
					GE_CORE_INFO(path);
				}
				
			}
		}
	}

	void ResourceManager::ScanContentLibraries()
	{
		if (m_ContentLibraryDirectory != "") {
			for (const auto& dirEntry : recursive_directory_iterator(m_ContentLibraryDirectory)) {
				if (dirEntry.is_regular_file()) {
					std::string fileName = dirEntry.path().filename().string();
					std::string path = dirEntry.path().string();

					std::string ext = path.substr(path.find_last_of('.') + 1);

					if (ext.compare(CONTENT_LIBRARY_FILE_EXTENSION) == 0) {
						m_ResourceMaps->ContentLibraryRegistry[fileName] = path;
					}
				}
			}
		}
	}

	void ResourceManager::CleanResources()
	{
		auto idIt = m_ResourceMaps->ActiveIDResources.begin();

		while (idIt != m_ResourceMaps->ActiveIDResources.end()) {
			if (idIt->second.use_count() == 1) {
				idIt = m_ResourceMaps->ActiveIDResources.erase(idIt);
			}
			else {
				idIt++;
			}
		}

		auto pathIt = m_ResourceMaps->ActivePathResources.begin();

		while (pathIt != m_ResourceMaps->ActivePathResources.end()) {
			if (pathIt->second.use_count() == 1) {
				pathIt = m_ResourceMaps->ActivePathResources.erase(pathIt);
			}
			else {
				pathIt++;
			}
		}
	}

	Ref<Serializeable> ResourceManager::GetResource(ID asset) {
		if (m_ResourceMaps->ActiveIDResources.contains(asset)) {
			return (*m_ResourceMaps->ActiveIDResources.find(asset)).second;
		}

		GE_CORE_ASSERT(m_ResourceMaps->ResourceRegistry.contains(asset), "No UUID entry for asset");
		GE_CORE_INFO("Loading file into memory!");

		std::string path = m_ResourceMaps->ResourceRegistry.find(asset)->second;

		UINT size;
		char* data = m_Loader->LoadResource(path, &size);

		unsigned int metaSize;
		char* meta = m_Loader->LoadResource(path + META_FILE_EXTENSION, &metaSize);

		GE_CORE_ASSERT(metaSize == sizeof(ResourceFileMetadata), "Meta data of resource has been manipulated and does not match the expected size");


		std::string ext = path.substr(path.find_last_of('.') + 1);
		Ref<Serializeable> resource = AssetFactory::GenerateAsset(ext);
		resource->Deserialize(data, size);

		ResourceFileMetadata* metaPtr = (ResourceFileMetadata*)meta;
		ID metaId(metaPtr->ID);

		resource->SetID(metaId);

		m_ResourceMaps->ActiveIDResources.insert({ asset, resource });

		m_Loader->CleanResource(data);
		m_Loader->CleanResource(meta);

		return resource;
	}

	Ref<Serializeable> ResourceManager::GetResource(std::string asset) {
		if (m_ResourceMaps->ActivePathResources.contains(asset)) {
			return (*m_ResourceMaps->ActivePathResources.find(asset)).second;
		}

		GE_CORE_INFO("Loading file into memory!");
		UINT size;
		char* data = m_Loader->LoadResource(asset, &size);

		std::string ext = asset.substr(asset.find_last_of('.') + 1);
		Ref<Serializeable> resource = AssetFactory::GenerateAsset(ext);
		resource->Deserialize(data, size);

		m_ResourceMaps->ActivePathResources.insert({ asset, resource });

		m_Loader->CleanResource(data);

		return resource;
	}
};