#pragma once
#include "ProjectGE/ResourceManagement/ResourceManager.h"
#include "ProjectGE/Rendering/RenderingManager.h"

namespace ProjectGE {
	class SubsystemManager
	{
	public:
		SubsystemManager();
		~SubsystemManager();
		void Update();
	private:
		Scope<ResourceManager> m_ResourceManager;
		Scope<RenderingManager> m_RenderingManager;
	};
};

