#pragma once
#include "gepch.h"
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/EntitySystem/Transform.h"
#include "Components/Component.h"
#include "GameplayUpdater.h"


namespace GameSmith {
	class GameObject {
	public:
		GameObject(std::string name = DEFAULT_GAMEOBJECT_NAME);
		inline std::string GetName() { return m_Name; }
		inline Connection<Transform> GetTransform() { return m_Transform; }

		void OnUpdate();

		template<typename T>
		Connection<T> AddComponent() {
			auto component = Ref<T>(new T(this, m_Transform.get()));
			
			auto updater = GameplayUpdater::GetInstance();

			if (updater != nullptr) {
				updater->Register(component);
			}
			else {
				GE_CORE_ERROR("Unable to get reference to gameplay updater");
			}

			m_Components.emplace_back(component);

			return component;
		}

		template<typename T>
		Connection<T> GetComponent() {
			for (Ref<Component> comp : m_Components) {
				auto cast = CastPtr<T>(comp);
				if (cast != nullptr) {
					return cast;
				}
			}

			return Ref<T>(nullptr);
		}

		template<typename T>
		void RemoveComponent(Connection<T> component) {
			auto lockComp = component.lock();
			if (lockComp->GetGameObject() == this) {
				for (auto it = m_Components.begin(); it < m_Components.end(); it++) {
					if ((*it).get() == lockComp.get()) {
						lockComp->OnDestroy();
						m_Components.erase(it);
						break;
					}
				}
			}
		}
	public:
		const static std::string DEFAULT_GAMEOBJECT_NAME;
	private:
		std::string m_Name;
		Ref<Transform> m_Transform;
		std::vector<Ref<Component>> m_Components;
	};
};
