#pragma once
#include "gepch.h"
#include "ProjectGE/Core/Core.h"
#include "Resource.h"
#include "ResourceLoaders/ResourceLoader.h"

namespace ProjectGE {
	enum class ResourceLoaderType {
		Heap
	};

	class ResourceManager
	{
	public:
		ResourceManager();
		inline static ResourceManager* GetInstance() { return s_Instance; }

		virtual void Init();
		virtual void Shutdown();
		// Need to figure out how to store multiple resource types while being able to initialize them during loading
		template<typename T>
		Ref<T> GetResource(std::string asset) {
			if (m_ResourceRegistry.contains(asset)) {
				Ref<Resource> ptr = (*m_ResourceRegistry.find(asset)).second;
				return CastPtr<T>(ptr);
			}

			UINT size;
			char* data = m_Loader->LoadResource(asset, &size);

			Ref<T> resource = Ref<T>(new T(data, size));
			resource->Init();

			m_ResourceRegistry.insert({ asset, resource });

			return resource;
		}

		void ScaneResource();

	private:
		static ResourceManager* s_Instance;

		std::unordered_map<std::string, Ref<Resource>> m_ResourceRegistry;
		Ref<ResourceLoader> m_Loader;
	};
};


