#pragma once

#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Utilities/ExposedVariableRegistry.h"

namespace GameSmith {
	class GameObject;
	class Transform;

	class Component {
	public:
		Component() = default;
		Component(GameObject* gameObject, Transform* transform): m_Initialized(false), m_GameObject(gameObject), m_Transform(transform) {}
		void Init() { if (!m_Initialized) { OnStart(); m_Initialized = true; } }
		virtual void OnStart() = 0;
		virtual void OnUpdate(float dt) = 0;
		virtual void OnDestroy() = 0;
		virtual int GetPriority() const = 0;
		virtual std::string GetName() const = 0;

		void BootstrapRegistry(std::unordered_map<std::string, Ref<ParameterContainer>> variableEntries) { m_Registry.BootstrapFromValueMap(variableEntries); }
		void GenerateVariableEntries(std::unordered_map<std::string, Ref<ParameterContainer>>* outMap) { m_Registry.GenerateVariableMap(outMap); }
		GameObject* GetGameObject() { return m_GameObject; }
		Transform* GetTransform() { return m_Transform; }
	protected:
		bool m_Initialized;
		GameObject* m_GameObject;
		Transform* m_Transform;
		ExposedVariableRegistry m_Registry;
	};

	struct ComponentCompare {
		bool operator()(const Connection<Component> lhs, const Connection<Component> rhs) {
			if (lhs.expired() && rhs.expired()) {
				return false;
			}
			else if(lhs.expired()) {
				return true;
			}
			else if (rhs.expired()) {
				return false;
			}
			else {
				return lhs.lock()->GetPriority() > rhs.lock()->GetPriority();
			}
		}
	};
};
