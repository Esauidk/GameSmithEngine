#pragma once

#include "ProjectGE/Core.h"
#include "ProjectGE/Events/Event.h"

namespace ProjectGE {
	class GE_API Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void EventSubscribe(std::vector<EventDispatcherBase*> dispatchers, bool overlay) {}
		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}
