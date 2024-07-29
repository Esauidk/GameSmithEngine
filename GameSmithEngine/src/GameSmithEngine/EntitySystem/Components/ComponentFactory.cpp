#include "gepch.h"
#include "ComponentFactory.h"


#include "MeshRenderer.h"
#include "TestComponent.h"


namespace GameSmith {
	Scope<ComponentRegistry> ComponentRegistry::s_Instance = nullptr;

	ComponentRegistry::ComponentRegistry()
	{
	}

	ComponentRegistry* ComponentRegistry::GetInstance() {
		if (s_Instance == nullptr) {
			s_Instance = Scope<ComponentRegistry>(new ComponentRegistry());
		}

		return s_Instance.get();
	}
	

	void ComponentRegistry::ListRegisteredComponents(std::vector<std::string>* outVec)
	{
		for (auto entry : m_Generators) {
			outVec->push_back(entry.first);
		}
	}
};

