#pragma once
#include "GameSmithEngine/ResourceManagement/ResourceManager.h"
#include "GameSmithEngine/Rendering/RenderingManager.h"
#include "GameSmithEngine/SceneSystem/SceneManager.h"

namespace GameSmith {
	class SubsystemManager
	{
	public:
		SubsystemManager();
		~SubsystemManager();
		void Update();
		void EndUpdate();
	private:
		ResourceManager m_ResourceManager;
		RenderingManager m_RenderingManager;
		SceneManager m_SceneManager;
	};
};

