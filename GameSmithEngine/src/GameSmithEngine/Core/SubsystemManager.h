#pragma once
#include "GameSmithEngine/ResourceManagement/ResourceManager.h"
#include "GameSmithEngine/Rendering/RenderingManager.h"

namespace GameSmith {
	class SubsystemManager
	{
	public:
		SubsystemManager();
		~SubsystemManager();
		void Update();
		void EndUpdate();
	private:
		Scope<ResourceManager> m_ResourceManager;
		Scope<RenderingManager> m_RenderingManager;
	};
};

