#pragma once

#include "ProjectGE/Core.h"
#include "ProjectGE/Events/Event.h"

namespace ProjectGE {
	class GE_API Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {/* To allow inheritor classes to not implement any logic */ }
		virtual void OnDetach() {/* To allow inheritor classes to not implement any logic */ }
		virtual void OnUpdate() {/* To allow inheritor classes to not implement any logic */ }
		virtual void OnImGuiRender() {/* To allow inheritor classes to not implement any logic */ }
		virtual void EventSubscribe(const std::vector<EventDispatcherBase*>& dispatchers, bool overlay) {/* To allow inheritor classes to not implement any logic */ }
		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}
