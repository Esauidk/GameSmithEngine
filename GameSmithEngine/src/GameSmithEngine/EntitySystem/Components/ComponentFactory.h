#pragma once
#include "gepch.h"
#include "Component.h"

namespace GameSmith {
	class ComponentRegistry {
	public:
		ComponentRegistry();
		inline static ComponentRegistry* GetInstance() { return s_Instance; }

		inline void RegisterComponent(
			std::string className, 
			std::function<Component* (GameObject*, Transform*)> generator) 
		{ m_Generators.insert({ className, generator }); }

		void ListRegisteredComponents(std::vector<std::string>* outVec);
	private:
		void RegisterEngineComponents();
	private:
		static ComponentRegistry* s_Instance;

		std::unordered_map<std::string, std::function<Component* (GameObject*, Transform*)>> m_Generators;

		friend class ComponentFactory;
	};

	class ComponentFactory {
	private:
		static inline Ref<Component> GenerateComponent(std::string className, GameObject* go, Transform* t) {
			auto generator = ComponentRegistry::s_Instance->m_Generators.find(className);
			return Ref<Component>(generator->second(go, t));
		}

		friend class GameObject;
	};
};
