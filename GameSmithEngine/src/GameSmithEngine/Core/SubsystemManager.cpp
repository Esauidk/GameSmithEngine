#include "gepch.h"
#include "SubsystemManager.h"

namespace GameSmith {
	SubsystemManager::SubsystemManager() {
		m_ResourceManager = Scope<ResourceManager>(new ResourceManager());
		m_RenderingManager = Scope<RenderingManager>(new RenderingManager());

		m_ResourceManager->Init(ResourceLoaderType::Heap);
		m_RenderingManager->Init();

		
	}

	SubsystemManager::~SubsystemManager()
	{
		m_RenderingManager->ShutDown();
		m_ResourceManager->Shutdown();
	}



	void SubsystemManager::Update()
	{
		m_ResourceManager->ScanResource();
	}

	void SubsystemManager::EndUpdate()
	{
		m_RenderingManager->EndFrame();
	}
};