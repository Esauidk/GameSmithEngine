#pragma once
namespace ProjectGE {
	class Component {
	public:
		virtual void OnStart() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnDestroy() = 0;
	};
};
