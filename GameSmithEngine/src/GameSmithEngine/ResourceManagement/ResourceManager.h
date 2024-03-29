#pragma once
#include "gepch.h"
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Core/Log.h"
#include "Resource.h"
#include "ResourceLoaders/ResourceLoader.h"

namespace GameSmith {
	enum class ResourceLoaderType {
		Heap
	};

	class ResourceManager
	{
	public:
		ResourceManager();
		inline static ResourceManager* GetInstance() { return s_Instance; }

		virtual void Init(ResourceLoaderType loaderType);
		virtual void Shutdown();
		// Need to figure out how to store multiple resource types while being able to initialize them during loading
		template<typename T>
		Ref<T> GetResource(std::string asset) {
			if (m_ResourceRegistry.contains(asset)) {
				Ref<Resource> ptr = (*m_ResourceRegistry.find(asset)).second;
				return CastPtr<T>(ptr);
			}

			GE_CORE_INFO("Loading file: {0} into memory!", asset);
			UINT size;
			char* data = m_Loader->LoadResource(asset, &size);

			Ref<T> resource = Ref<T>(new T());
			resource->AttachResource(data, size);
			resource->Init();

			m_ResourceRegistry.insert({ asset, resource });

			return resource;
		}

		// Expected to be used only during testing
		template<typename T>
		Ref<T> GetResource(std::string key, char* inData, UINT size) {
			if (m_ResourceRegistry.contains(key)) {
				Ref<Resource> ptr = (*m_ResourceRegistry.find(key)).second;
				return CastPtr<T>(ptr);
			}

			GE_CORE_INFO("Copying data with key: {0} into memory!", key);
			char* data = m_Loader->LoadResource(inData, size);

			Ref<T> resource = Ref<T>(new T());
			resource->AttachResource(data, size);
			resource->Init();

			m_ResourceRegistry.insert({ key, resource });

			return resource;
		}

		void ScanResource();

	private:
		static ResourceManager* s_Instance;

		std::unordered_map<std::string, Ref<Resource>> m_ResourceRegistry;
		Ref<ResourceLoader> m_Loader;
	};
};


