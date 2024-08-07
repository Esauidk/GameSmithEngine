#include "gepch.h"
#include "SubsystemManager.h"

namespace GameSmith {
	SubsystemManager::SubsystemManager() {
		m_ResourceManager.Init(ResourceLoaderType::Heap);
		m_RenderingManager.Init();
		m_SceneManager.Init();
		m_ContentLibraryManager.DiscoveryLibaries();
	}

	SubsystemManager::~SubsystemManager()
	{
		m_SceneManager.ShutDown();
		m_RenderingManager.ShutDown();
		m_ResourceManager.Shutdown();
	}



	void SubsystemManager::Update(float dt)
	{
		m_ResourceManager.CleanResources();
		m_GameObjectManager.CleanGameObjects();

		m_GameplayUpdater.RunGameplayInit();
		m_GameplayUpdater.RunGameplayUpdate(dt);
		m_GameplayUpdater.FlushRegistration();

		m_EntityRenderPreparer.SendForRendering();
	}

	void SubsystemManager::EndUpdate()
	{
		m_RenderingManager.EndFrame();
	}
};