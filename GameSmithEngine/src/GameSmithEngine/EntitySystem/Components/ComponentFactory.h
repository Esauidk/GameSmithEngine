#pragma once
#include "gepch.h"
#include "Component.h"

#define GE_REGISTERCOMPONENT(ClassType) \
	 static struct ClassType##RegisterAction { \
		ClassType##RegisterAction() { \
			GameSmith::ComponentRegistry::GetInstance()->RegisterComponent( \
				#ClassType, [](GameSmith::GameObject* g, GameSmith::Transform* t) {return new ClassType(g, t); } \
			); \
		} \
	}ClassType##Instance;

namespace GameSmith {
	class ComponentRegistry {
	public:
		ComponentRegistry();
		inline static ComponentRegistry* GetInstance() { 
			if (s_Instance == nullptr) {
				s_Instance = Scope<ComponentRegistry>(new ComponentRegistry());
			}

			return s_Instance.get(); 
		}

		inline void RegisterComponent(
			std::string className, 
			std::function<Component* (GameObject*, Transform*)> generator) 
		{ m_Generators.insert({ className, generator }); }

		void ListRegisteredComponents(std::vector<std::string>* outVec);
	private:
		static Scope<ComponentRegistry> s_Instance;

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
