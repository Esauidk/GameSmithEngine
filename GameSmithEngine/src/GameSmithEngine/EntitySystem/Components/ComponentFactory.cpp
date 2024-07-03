#include "gepch.h"
#include "ComponentFactory.h"


#include "MeshRenderer.h"
#include "TestComponent.h"


namespace GameSmith {
	Scope<ComponentRegistry> ComponentRegistry::s_Instance = nullptr;

	ComponentRegistry::ComponentRegistry()
	{
	}

	void ComponentRegistry::ListRegisteredComponents(std::vector<std::string>* outVec)
	{
		for (auto entry : m_Generators) {
			outVec->push_back(entry.first);
		}
	}
};

