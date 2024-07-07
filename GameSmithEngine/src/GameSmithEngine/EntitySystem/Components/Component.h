#pragma once

#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Utilities/ExposedVariableRegistry.h"
#include "GameSmithEngine/ResourceAssets/Serializable.h"

namespace GameSmith {
	class GameObject;
	class Transform;

	class GE_API Component {
	public:
		Component() = default;
		Component(GameObject* gameObject, Transform* transform): m_Initialized(false), m_GameObject(gameObject), m_Transform(transform) {}
		void Init() { if (!m_Initialized) { OnStart(); m_Initialized = true; } }
		virtual void OnStart() = 0;
		virtual void OnUpdate(float dt) = 0;
		virtual void OnDestroy() = 0;
		virtual int GetPriority() const = 0;
		virtual std::string GetName() const = 0;

		inline void BootstrapRegistry(std::unordered_map<std::string, Ref<ParameterContainer>>& variableEntries) { 
			m_Registry.BootstrapFromValueMap(variableEntries); 
			PostRegistryBootstrap(); 
		}

		inline unsigned int RegistrySerializationSize() { return m_Registry.RequireSpace(); }
		inline void SerializeRegistry(char* byteStream, unsigned int availableBytes) { m_Registry.Serialize(byteStream, availableBytes); }
		inline void DeserializeRegistry(char* inData, unsigned int bytes) { m_Registry.Deserialize(inData, bytes); }
		

		virtual void PostRegistryBootstrap() {};
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
