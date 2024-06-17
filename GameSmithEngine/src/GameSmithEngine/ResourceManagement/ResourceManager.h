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

	class ResourceManager
	{
	public:
		ResourceManager();
		inline static ResourceManager* GetInstance() { return s_Instance; }

		virtual void Init(ResourceLoaderType loaderType);
		virtual void Shutdown();

		template<typename T>
		Ref<T> GetResource(ID asset) {
			if (m_ActiveResources.contains(asset)) {
				Ref<Serializeable> ptr = (*m_ActiveResources.find(asset)).second;
				return CastPtr<T>(ptr);
			}

			GE_CORE_ASSERT(m_ResourceRegistry.contains(asset), "No UUID entry for asset");
			GE_CORE_INFO("Loading file into memory!");
			UINT size;
			char* data = m_Loader->LoadResource(m_ResourceRegistry.find(asset)->second, &size);

			unsigned int metaSize;
			char* meta = m_Loader->LoadResource(m_ResourceRegistry.find(asset)->second + ".meta", &metaSize);

			GE_CORE_ASSERT(metaSize == sizeof(ResourceFileMetadata), "Meta data of resource has been manipulated and does not match the expected size");

			Ref<T> resource = Ref<T>(new T());
			resource->Deserialize(data, size);

			ResourceFileMetadata* metaPtr = (ResourceFileMetadata*)meta;
			ID metaId(metaPtr->ID);

			resource->SetId(metaId);

			m_ActiveResources.insert({ asset, resource });

			m_Loader->CleanResource(data);
			m_Loader->CleanResource(meta);

			return resource;
		}

		// Expected to be used only during testing
		template<typename T>
		Ref<T> GetResource(ID key, char* inData, UINT size) {
			if (m_ActiveResources.contains(key)) {
				Ref<Serializeable> ptr = (*m_ActiveResources.find(key)).second;
				return CastPtr<T>(ptr);
			}

			GE_CORE_INFO("Copying data into memory!");
			char* data = m_Loader->LoadResource(inData, size);

			Ref<T> resource = Ref<T>(new T());
			resource->Deserialize(data, size);

			m_ActiveResources.insert({ key, resource });

			m_Loader->CleanResource(data);

			return resource;
		}

		ID WriteResource(Ref<Serializeable> resource, std::string path);
		ID ImportResource(std::string path);
		void ScanResources();
		void CleanResources();

		void SetAssestDirectory(std::string dir) { m_AssetDirectory = dir; }

	private:
		static ResourceManager* s_Instance;

		std::unordered_map<ID, Ref<Serializeable>, ID> m_ActiveResources;
		std::unordered_map<ID, std::string, ID> m_ResourceRegistry;

		Ref<ResourceLoader> m_Loader;

		std::string m_AssetDirectory;
	};
};


