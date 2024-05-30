#include "gepch.h"
#include "ResourceManager.h"
#include "GameSmithEngine/Core/Log.h"

#include "ResourceLoaders/HeapResourceLoader.h"

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

	ResourceManager::ResourceManager()
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

	void ResourceManager::WriteResource(Ref<Serializeable> resource, std::string path)
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

		m_ResourceRegistry.insert({ id, path });
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

		m_ResourceRegistry.insert({ meta.ID, path });

		return newID;
	}

	void ResourceManager::ScanResources()
	{
		//TODO: Add actual logic to scan resources available
		m_ResourceRegistry.insert({ {0, 0, 0, 0}, "C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\TestMat.mat" });
		m_ResourceRegistry.insert({ {1,1,1,1}, "C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\SampleVertexShader.cso" });
		m_ResourceRegistry.insert({ {2,2,2,2}, "C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\RandomColorPS.cso" });
	}

	void ResourceManager::CleanResources()
	{
		auto it = m_ActiveResources.begin();

		while (it != m_ActiveResources.end()) {
			if (it->second.use_count() == 1) {
				it = m_ActiveResources.erase(it);
			}
			else {
				it++;
			}
		}
	}
};