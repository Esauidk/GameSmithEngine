#pragma once
#include "gepch.h"
#include "ProjectGE/Core/Core.h"
#include "ProjectGE/EntitySystem/Components/Transform.h"
#include "Components/Component.h"


namespace ProjectGE {
	class GameObject {
	public:
		GameObject();
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

		Ref<Transform> GetTransform() { return m_Transform; }
	private:
		Ref<Transform> m_Transform;
		std::vector<Ref<Component>> m_Components;
	};
};
