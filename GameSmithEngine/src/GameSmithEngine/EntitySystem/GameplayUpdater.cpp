#include "gepch.h"
#include "GameplayUpdater.h"

#include "GameSmithEngine/Core/JobSystem/JobUtils.h"

namespace GameSmith {

	DEFINE_JOB_FN(
		ComponentUpdateJob,
		{
			ComponentUpdateGroupData* groupData = static_cast<ComponentUpdateGroupData*>(ptr);
			bool canRun = false;

			if (!groupData->counter.expired()) {
				auto lockedCounter = groupData->counter.lock();
				if (lockedCounter->GetCounter() == 0) {
					canRun = true;
				}
				else {
					WorkerPauseCurrentJob(lockedCounter);
				}
			}

			auto componentConnection = groupData->components[jobParams.batchIndex];
			if (!componentConnection.expired()) {
				componentConnection.lock()->OnUpdate(groupData->deltaTime);
			}
		}
	)

	GameplayUpdater* GameplayUpdater::s_Instance = nullptr;

	GameplayUpdater::GameplayUpdater()
	{
	}

	void GameplayUpdater::Init()
	{
		if (s_Instance == nullptr) {
			s_Instance = new GameplayUpdater();
		}
	}

	void GameplayUpdater::ShutDown()
	{
		if (s_Instance != nullptr) {
			delete s_Instance;
			s_Instance = nullptr;
		}
	}

	void GameplayUpdater::FlushRegistration()
	{
		while (!m_PendingRegistrations.empty()) {
			auto comp = m_PendingRegistrations.front();

			if (!comp.expired()) {
				m_InitQueue.push(comp);
				m_RegisteredComponents.push_back(comp);
			}

			m_PendingRegistrations.pop();
		}
	}

	void GameplayUpdater::RunGameplayInit()
	{
		while (!m_InitQueue.empty()) {
			auto comp = m_InitQueue.top();

			if (!comp.expired()) {
				comp.lock()->Init();
			}

			m_InitQueue.pop();
		}
	}

	void GameplayUpdater::RunGameplayUpdate(float dt)
	{
		if (std::is_heap(m_RegisteredComponents.begin(), m_RegisteredComponents.end(), ComponentCompare())) {
			std::make_heap(m_RegisteredComponents.begin(), m_RegisteredComponents.end(), ComponentCompare());
		}

		auto jobManager = JobManager::GetInstance();
		
		int currentPriority = 0;
		ComponentUpdateGroupData currentGroup = {};
		currentGroup.deltaTime = dt;
		int currentGroupIndex = 0;
		bool pendingGroup = false;

		Ref<JobBatchCounter> currentCounter = nullptr;
		for (auto it = m_RegisteredComponents.begin(); it != m_RegisteredComponents.end();) {
			if (it->expired()) {
				it = m_RegisteredComponents.erase(it);
			}
			else {
				if (!pendingGroup) {
					pendingGroup = true;
				}

				auto lockedComponent = it->lock();
				int componentPriority = lockedComponent->GetPriority();
				if (componentPriority <= currentPriority && currentGroupIndex < COMPONENT_GROUP_SIZE) {
					// Continue with current group
					currentGroup.components[currentGroupIndex] = *it;
					currentGroupIndex++;

					if (componentPriority < currentPriority) {
						GE_CORE_WARN("{0}. {1}. {2}",
							"A component with a priority lower than our current queueing priority was found",
							"Added to the update group",
							"However, this indicates something is wrong with the system"
						);
					}
				}
				else {
					// Submit current group
					auto newCounter = jobManager->StartJobs(
						ComponentUpdateJob,
						&currentGroup,
						sizeof(ComponentUpdateGroupData),
						COMPONENT_GROUP_SIZE,
						JobPriority::Medium
					);

					// Save for tracking
					m_BatchCounters.push_back(newCounter);

					// Only update the batch counter info if we are making a new group because of priority change
					if (componentPriority > currentPriority) {
						currentCounter = newCounter;
						currentPriority = componentPriority;
					}

					// Start new group
					currentGroup = {};
					currentGroupIndex = 0;

					currentGroup.counter = currentCounter;
					currentGroup.components[currentGroupIndex] = *it;
					currentGroup.deltaTime = dt;
					currentGroupIndex++;
				}

				// Move to next component
				it++;
			}
		}

		if (pendingGroup) {
			// Submit last group
			auto newCounter = jobManager->StartJobs(
				ComponentUpdateJob,
				&currentGroup,
				sizeof(ComponentUpdateGroupData),
				currentGroupIndex,
				JobPriority::Medium
			);

			// Save for tracking
			m_BatchCounters.push_back(newCounter);
		}

		// Check that all groups have been completed
		while (m_BatchCounters.size() > 0) {
			for (auto it = m_BatchCounters.begin(); it != m_BatchCounters.end();) {
				if (it->get()->GetCounter() == 0) {
					it = m_BatchCounters.erase(it);
				}
				else {
					it++;
				}
			}
		}
	}
};
