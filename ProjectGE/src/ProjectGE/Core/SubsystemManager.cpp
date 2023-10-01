#include "gepch.h"
#include "SubsystemManager.h"

namespace ProjectGE {
	SubsystemManager::SubsystemManager() {
		m_ResourceManager = Scope<ResourceManager>(new ResourceManager());
		m_RenderingManager = Scope<RenderingManager>(new RenderingManager());

		m_ResourceManager->Init();
		m_RenderingManager->Init();

		m_RenderingManager->ShutDown();
		m_ResourceManager->Shutdown();
	}
};