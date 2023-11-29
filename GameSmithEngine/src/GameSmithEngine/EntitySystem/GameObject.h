#pragma once
#include "gepch.h"
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/EntitySystem/Components/Transform.h"
#include "Components/Component.h"


namespace GameSmith {
	class GameObject {
	public:
		GameObject(std::string name = DEFAULT_GAMEOBJECT_NAME);
		inline std::string GetName() { return m_Name; }
		inline Ref<Transform> GetTransform() { return m_Transform; }

		void OnUpdate();

		template<typename T>
		Ref<T> AddComponent() {
			auto component = Ref<T>(new T(this, m_Transform.get()));
			component->OnStart();
			m_Components.emplace_back(component);

			return component;
		}

		template<typename T>
		Ref<T> GetComponent() {
			for (Ref<Component> comp : m_Components) {
				auto cast = CastPtr<T>(comp);
				if (cast != nullptr) {
					return cast;
				}
			}

			return nullptr;
		}

		template<typename T>
		void RemoveComponent(Ref<T> component) {
			auto castCheck = CastPtr<Transform>(component);
			if (castCheck == nullptr && component->GetGameObject() == this) {
				for (auto it = m_Components.begin(); it < m_Components.end(); it++) {
					if ((*it).get() == component.get()) {
						component->OnDestroy();
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
