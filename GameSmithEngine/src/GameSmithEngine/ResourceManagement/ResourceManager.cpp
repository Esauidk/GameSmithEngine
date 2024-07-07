#include "gepch.h"
#include "ResourceManager.h"
#include "GameSmithEngine/Core/Log.h"

#include "ResourceLoaders/HeapResourceLoader.h"

#include <filesystem>

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

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

	ResourceManager::ResourceManager() : m_ResourceMaps(new ResourceMaps())
	{
		if (s_Instance == nullptr) {
			s_Instance = this;
		}
	}

	void ResourceManager::Init(ResourceLoaderType loaderType)
	{
		m_Loader = GetLoader(loaderType);
		ScanResources();
		GE_CORE_INFO("Resource Manager Loaded!");
	}

	void ResourceManager::Shutdown()
	{
	}

	ID ResourceManager::WriteResource(Ref<Serializeable> resource, std::string path)
	{
		std::fstream pFile(path, std::ios::out | std::ios::binary | std::ios::ate);
		GE_CORE_ASSERT(pFile.is_open(), std::format("Asset file {0} cannot be opened", path));

		std::fstream metaFile(path + ".meta", std::ios::out | std::ios::binary | std::ios::ate);
		GE_CORE_ASSERT(metaFile.is_open(), std::format("Asset file {0} cannot be opened", path));

		auto serial = resource->Serialize();
		auto id = resource->GetId();

		pFile.seekg(0, pFile.beg);
		pFile.write(serial.get(), resource->RequireSpace());
		pFile.close();

		ResourceFileMetadata meta;
		meta.ID = id.getData();

		metaFile.seekg(0, metaFile.beg);
		metaFile.write((char*)&meta, sizeof(meta));
		metaFile.close();

		m_ResourceMaps->ResourceRegistry.insert({ id, path });

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

		return newID;
	}

	void ResourceManager::ScanResources()
	{
		if (m_AssetDirectory != "") {
			for (const auto& dirEntry : recursive_directory_iterator(m_AssetDirectory)) {
				if (dirEntry.is_regular_file()) {
					std::string fileName = dirEntry.path().filename().string();
					std::string path = dirEntry.path().string();

					if (std::filesystem::exists(path + ".meta")) {
						unsigned int metaSize;
						char* meta = m_Loader->LoadResource(path + ".meta", &metaSize);
						ResourceFileMetadata* metaPtr = (ResourceFileMetadata*)meta;
						ID metaId(metaPtr->ID);

						if (!m_ResourceMaps->ResourceRegistry.contains(metaId)) {
							m_ResourceMaps->ResourceRegistry.insert({ metaId, path });
						}
					}
					 
					std::cout << dirEntry << std::endl;
				}
				
			}
		}
	}

	void ResourceManager::CleanResources()
	{
		auto it = m_ResourceMaps->ActiveResources.begin();

		while (it != m_ResourceMaps->ActiveResources.end()) {
			if (it->second.use_count() == 1) {
				it = m_ResourceMaps->ActiveResources.erase(it);
			}
			else {
				it++;
			}
		}
	}
};