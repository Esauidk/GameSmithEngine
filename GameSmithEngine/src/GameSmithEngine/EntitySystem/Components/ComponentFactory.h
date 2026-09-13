#pragma once
#include "gepch.h"
#include "Component.h"
#include "GameSmithEngine/Utilities/Factory/Factory.h"

#define GE_REGISTERCOMPONENT(ClassType) \
	 static struct ClassType##RegisterAction { \
		ClassType##RegisterAction() { \
			GameSmith::ComponentFactory::GetInstance()->Register( \
				#ClassType, [](GameSmith::GameObject* g, GameSmith::Transform* t) {return new ClassType(g, t); } \
			); \
		} \
	}ClassType##Instance;

namespace GameSmith {
	using ComponentFactory = Factory<Component, GameObject, GameObject*, Transform*>;
	class GE_API ComponentRegistry {
	public:
		ComponentRegistry();
		static ComponentRegistry* GetInstance();

		inline void RegisterComponent(
			std::string className, 
			std::function<Component* (GameObject*, Transform*)> generator) 
		{ 
			int i = 0;
			m_Generators.insert({ className, generator }); 
		}

		void ListRegisteredComponents(std::vector<std::string>* outVec);
		void ClearRegistrations() { m_Generators.clear(); }
	private:
		static Scope<ComponentRegistry> s_Instance;

		std::unordered_map<std::string, std::function<Component* (GameObject*, Transform*)>> m_Generators;
	};
};
