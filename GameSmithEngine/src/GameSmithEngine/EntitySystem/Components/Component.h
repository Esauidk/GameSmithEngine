#pragma once

#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/SerializeableFiles/ExposedMemberSerializeable.h"

namespace GameSmith {
	class GameObject;
	class Transform;

	class GE_API IComponent : public virtual ISerializeable, public virtual IExposedMembers {
	public:
		virtual ~IComponent() = default;
		virtual void Init() = 0;
		virtual void OnStart() = 0;
		virtual void OnUpdate(float dt) = 0;
		virtual void OnDestroy() = 0;
		virtual int GetPriority() const = 0;
		virtual std::string GetName() const = 0;

		virtual GameObject* GetGameObject() = 0;
		virtual Transform* GetTransform() = 0;
	};

	class GE_API Component : public ExposedMemberSerializeable, public IComponent {
	public:
		Component() = delete;
		Component(GameObject* gameObject, Transform* transform): m_Initialized(false), m_GameObject(gameObject), m_Transform(transform) {}
		void Init() override final { if (!m_Initialized) { OnStart(); m_Initialized = true; } }
		GameObject* GetGameObject() override final { return m_GameObject; }
		Transform* GetTransform() override final { return m_Transform; }
	private:
		bool m_Initialized;
		GameObject* m_GameObject;
		Transform* m_Transform;
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
