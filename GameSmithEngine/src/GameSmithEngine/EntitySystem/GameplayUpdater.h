#pragma once
#include "gepch.h"
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/EntitySystem/Components/Component.h"

namespace GameSmith {
	class GameplayUpdater
	{
	public:
		GameplayUpdater();
		static GameplayUpdater* GetInstance() { return s_Instance; }
		void RunGameplayInit();
		void RunGameplayUpdate();
		void Register(Connection<Component> comp) { m_InitQueue.push(comp);  m_RegisteredComponents.push_back(comp); }
	private:
		static GameplayUpdater* s_Instance;

		std::priority_queue<Connection<Component>, std::vector<Connection<Component>>, ComponentCompare> m_InitQueue;
		std::vector<Connection<Component>> m_RegisteredComponents;
	};
};

