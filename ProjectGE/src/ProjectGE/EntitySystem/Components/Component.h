#pragma once
namespace ProjectGE {
	class GameObject;
	class Transform;

	class Component {
	public:
		Component() = default;
		Component(GameObject* gameObject, Transform* transform): m_GameObject(gameObject), m_Transform(transform) {}
		virtual void OnStart() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnDestroy() = 0;
	protected:
		GameObject* m_GameObject;
		Transform* m_Transform;
	};
};
