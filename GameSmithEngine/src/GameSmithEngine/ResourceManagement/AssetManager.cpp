#include "gepch.h"
#include "AssetManager.h"
#include "GameSmithEngine/Core/Log.h"

#include "ResourceLoaders/HeapResourceLoader.h"

#include "GameSmithEngine/SerializeableFiles/ResourceAssets/AssetFactory.h"

#define META_FILE_EXTENSION ".meta"
#define CONTENT_LIBRARY_FILE_EXTENSION "dll"

using recursive_directory_iterator = fs::recursive_directory_iterator;

namespace GameSmith {
	AssetManager* AssetManager::s_Instance = nullptr;

	static Ref<ResourceLoader> GetLoader(const ResourceLoaderType loaderType) {
		switch (loaderType) {
		case ResourceLoaderType::Heap:
			return Ref<ResourceLoader>(new HeapResourceLoader());
		default:
			return Ref<ResourceLoader>(new HeapResourceLoader());
		}
	}

	AssetManager::AssetManager() : m_ResourceMaps(new ResourceMaps()), m_Loader(nullptr), m_AssetDirectory(fs::current_path().string())
	{
		if (s_Instance == nullptr) {
			s_Instance = this;
		}
		
	}

	void AssetManager::Init(const ResourceLoaderType loaderType)
	{
		if (s_Instance == nullptr) {
			s_Instance = new AssetManager();
			s_Instance->m_Loader = GetLoader(loaderType);
			s_Instance->ScanResources();

			GE_CORE_INFO("Resource Manager Loaded!");
		}
		
	}

	void AssetManager::Shutdown()
	{
		if (s_Instance != nullptr) {
			delete s_Instance;
			s_Instance = nullptr;
		}
	}

	ID AssetManager::WriteResource(const Ref<IAsset> resource, const std::string& destDir)
	{
		const std::string filePath = std::format("{0}\\{1}{2}", destDir, resource->GetName(), resource->GetFileExtension());
		std::fstream pFile(filePath, std::ios::out | std::ios::binary | std::ios::ate);
		GE_CORE_ASSERT(pFile.is_open(), std::format("Asset file {0} cannot be opened", destDir));

		std::fstream metaFile(filePath + META_FILE_EXTENSION, std::ios::out | std::ios::binary | std::ios::ate);
		GE_CORE_ASSERT(metaFile.is_open(), std::format("Asset file {0} cannot be opened", destDir));

		const auto serial = resource->Serialize();
		const auto id = resource->GetID();

		pFile.seekg(0, pFile.beg);
		unsigned int size = resource->RequiredSpace();
		if (size > 0) {
			pFile.write(serial.get(), resource->RequiredSpace());
		}
		pFile.close();

		ResourceFileMetadata meta;
		meta.ID = id.getData();

		metaFile.seekg(0, metaFile.beg);
		metaFile.write((char*)&meta, sizeof(meta));
		metaFile.close();

		const fs::path filePathObj(filePath);
		const std::string fileName = filePathObj.stem().string();
		const std::string ext = filePathObj.extension().string();
		m_ResourceMaps->ResourceRegistry.insert({ id, filePath });
		m_ResourceMaps->ReverseResourceRegistry.insert({ filePath, id });
		if (m_ResourceMaps->ExtensionToFilePaths.contains(ext)) {
			m_ResourceMaps->ExtensionToFilePaths[ext].push_back(filePath);
		}
		else {
			m_ResourceMaps->ExtensionToFilePaths.insert({ ext, { filePath } });
		}

		return id;
	}

	ID AssetManager::CreateResource(const std::string& fileName, const std::string& destDir)
	{
		const std::string filePath = std::format("{0}\\{1}", destDir, fileName);
		GE_CORE_ASSERT(!m_ResourceMaps->ReverseResourceRegistry.contains(filePath), "Resource already exists at the desired path");

		const fs::path filePathObj(filePath);
		const std::string ext = filePathObj.extension().string();
		const std::string fileNameNoExt = filePathObj.stem().string();
		Ref<IAsset> asset = AssetFactory::GenerateAsset(ext, fileNameNoExt);

		return WriteResource(asset, destDir);
	}

	ID AssetManager::ImportResource(const std::string& path)
	{
		const fs::path filePath(path);
		const std::string fileNameWithExt = filePath.filename().string();
		const std::string ext = filePath.extension().string();
		const std::string fileName = filePath.filename().stem().string();
		std::string destPath = std::format("{0}\\{1}", m_AssetDirectory, fileNameWithExt);
		if (!fs::copy_file(filePath.string(), destPath)) {
			GE_CORE_ERROR("Failed to import resource");
			return ID();
		}

		std::fstream metaFile(destPath + META_FILE_EXTENSION, std::ios::out | std::ios::binary | std::ios::ate);
		GE_CORE_ASSERT(metaFile.is_open(), std::format("Asset file {0} cannot be opened", destPath));

		const auto newID = GUIDGenerator::GenerateID();

		ResourceFileMetadata meta;
		meta.ID = newID.getData();

		metaFile.seekg(0, metaFile.beg);
		metaFile.write((char*)&meta, sizeof(meta));
		metaFile.close();

		m_ResourceMaps->ResourceRegistry.insert({ meta.ID, destPath });
		m_ResourceMaps->ReverseResourceRegistry.insert({ destPath, meta.ID });
		if (m_ResourceMaps->ExtensionToFilePaths.contains(ext)) {
			m_ResourceMaps->ExtensionToFilePaths[ext].push_back(destPath);
		} else {
			m_ResourceMaps->ExtensionToFilePaths.insert({ ext, { destPath } });
		}
		

		return newID;
	}

	ID AssetManager::GetAssetID(const std::string& path) const
	{
		if (m_ResourceMaps->ReverseResourceRegistry.contains(path)) {
			return m_ResourceMaps->ReverseResourceRegistry.find(path)->second;
		}

		return ID();
	}

	std::string AssetManager::GetAssetPath(const ID& id) const
	{
		if (m_ResourceMaps->ResourceRegistry.contains(id)) {
			return m_ResourceMaps->ResourceRegistry[id];
		}

		return std::string();
	}

	void AssetManager::ScanResources()
	{
		if (m_AssetDirectory != "") {
			for (const auto& dirEntry : recursive_directory_iterator(m_AssetDirectory)) {
				if (dirEntry.is_regular_file()) {
					const fs::path filePath = dirEntry.path();
					const std::string fileName = filePath.filename().stem().string();
					const std::string ext = filePath.extension().string();
					const std::string path = filePath.string();

					if (std::filesystem::exists(path + META_FILE_EXTENSION)) {
						unsigned int metaSize;

						char* meta = m_Loader->LoadResource(path + META_FILE_EXTENSION, &metaSize);
						const ResourceFileMetadata* metaPtr = (ResourceFileMetadata*)meta;
						const ID metaId(metaPtr->ID);

						if (!m_ResourceMaps->ResourceRegistry.contains(metaId)) {
							m_ResourceMaps->ResourceRegistry.insert({ metaId, path });
							m_ResourceMaps->ReverseResourceRegistry.insert({ path, metaId });

							if (m_ResourceMaps->ExtensionToFilePaths.contains(ext)) {
								m_ResourceMaps->ExtensionToFilePaths[ext].push_back(path);
							}
							else {
								m_ResourceMaps->ExtensionToFilePaths.insert({ ext, { path } });
							}
						}

						m_Loader->CleanResource(meta);
					}
				}
			}
		}
	}

	void AssetManager::CleanResources()
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

	Ref<IAsset> AssetManager::GetResource(const ID asset) {
		if (m_ResourceMaps->ActiveIDResources.contains(asset)) {
			return (*m_ResourceMaps->ActiveIDResources.find(asset)).second;
		}

		GE_CORE_ASSERT(m_ResourceMaps->ResourceRegistry.contains(asset), "No UUID entry for asset");
		GE_CORE_INFO("Loading file into memory!");

		const std::string path = m_ResourceMaps->ResourceRegistry.find(asset)->second;
		const fs::path filePath(path);
		const std::string fileName = filePath.stem().string();

		UINT size;
		char* data = m_Loader->LoadResource(path, &size);

		unsigned int metaSize;
		char* meta = m_Loader->LoadResource(path + META_FILE_EXTENSION, &metaSize);

		GE_CORE_ASSERT(metaSize == sizeof(ResourceFileMetadata), "Meta data of resource has been manipulated and does not match the expected size");


		const std::string ext = filePath.extension().string();
		const Ref<IAsset> resource = AssetFactory::GenerateAsset(ext, fileName);
		resource->Deserialize(data, size);

		const ResourceFileMetadata* metaPtr = (ResourceFileMetadata*)meta;
		ID metaId(metaPtr->ID);

		resource->SetID(metaId);

		m_ResourceMaps->ActiveIDResources.insert({ asset, resource });

		m_Loader->CleanResource(data);
		m_Loader->CleanResource(meta);

		return resource;
	}

	Ref<IAsset> AssetManager::GetResource(const std::string& asset) {
		if (m_ResourceMaps->ActivePathResources.contains(asset)) {
			return (*m_ResourceMaps->ActivePathResources.find(asset)).second;
		}

		GE_CORE_INFO("Loading file into memory!");
		UINT size;
		char* data = m_Loader->LoadResource(asset, &size);

		const fs::path filePath(asset);
		const std::string fileName = filePath.stem().string();
		const std::string ext = filePath.extension().string();
		const Ref<IAsset> resource = AssetFactory::GenerateAsset(ext, fileName);
		resource->Deserialize(data, size);

		m_ResourceMaps->ActivePathResources.insert({ asset, resource });

		m_Loader->CleanResource(data);

		return resource;
	}
};