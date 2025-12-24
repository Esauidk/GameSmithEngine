#pragma once
#include "gepch.h"
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/EntitySystem/Components/Component.h"

#include "GameSmithEngine/Core/JobSystem/JobManager.h"

#define COMPONENT_GROUP_SIZE 8

namespace GameSmith {
	class GE_API GameplayUpdater
	{
	public:
		static void Init();
		static void ShutDown();
		static GameplayUpdater* GetInstance() { return s_Instance; }
		void FlushRegistration();
		void RunGameplayInit();
		void RunGameplayUpdate(float dt);
		void Register(Connection<Component> comp) { m_PendingRegistrations.push(comp); }
	private:
		GameplayUpdater();
	private:
		static GameplayUpdater* s_Instance;

		// Component organization
		std::queue<Connection<Component>> m_PendingRegistrations;
		std::priority_queue<Connection<Component>, std::vector<Connection<Component>>, ComponentCompare> m_InitQueue;
		std::vector<Connection<Component>> m_RegisteredComponents;

		// Job System integration
		std::vector<Ref<JobBatchCounter>> m_BatchCounters;
	};

	struct ComponentUpdateGroupData {
		std::array<Connection<Component>, COMPONENT_GROUP_SIZE> components;
		Connection<JobBatchCounter> counter;
		float deltaTime;
	};
};



