#pragma once
#include "gepch.h"
#include "ProjectGE/Core/Core.h"
#include "Components/Component.h"

namespace ProjectGE {
	class GameObject {
	public:
		GameObject();

		template<typename T>
		Ref<T> AddComponent() {
			auto component = Ref<T>(new T());
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
	private:
		std::vector<Ref<Component>> m_Components;
	};
};
