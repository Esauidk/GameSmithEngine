#include "gepch.h"
#include "ComponentFactory.h"


#include "MeshRenderer.h"
#include "TestComponent.h"


namespace GameSmith {
	ComponentRegistry* ComponentRegistry::s_Instance = nullptr;

	ComponentRegistry::ComponentRegistry()
	{
		if (s_Instance == nullptr) {
			s_Instance = this;
			RegisterEngineComponents();
		}
	}

	void ComponentRegistry::ListRegisteredComponents(std::vector<std::string>* outVec)
	{
		for (auto entry : m_Generators) {
			outVec->push_back(entry.first);
		}
	}

	void ComponentRegistry::RegisterEngineComponents()
	{
		RegisterComponent("MeshRenderer", [](GameObject* g, Transform* t) {return new MeshRenderer(g, t); });
		RegisterComponent("TestComponent", [](GameObject* g, Transform* t) {return new TestComponent(g, t); });
	}
};

