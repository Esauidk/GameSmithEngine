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
		void FlushRegistration();
		void RunGameplayInit();
		void RunGameplayUpdate(float dt);
		void Register(Connection<Component> comp) { m_PendingRegistrations.push(comp); }
	private:
		static GameplayUpdater* s_Instance;

		std::queue<Connection<Component>> m_PendingRegistrations;
		std::priority_queue<Connection<Component>, std::vector<Connection<Component>>, ComponentCompare> m_InitQueue;
		std::vector<Connection<Component>> m_RegisteredComponents;
	};
};

