#pragma once

#include "ProjectGE/Events/Event.h"

namespace ProjectGE {
	// ABSTRACT CLASS
	// The layer class is a abstract representation of a submodule that can be inserted in and out of active execution 
	class Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		// Logic to perform when layer is added to the system
		virtual void OnAttach() {/* To allow inheritor classes to not implement any logic */ }
		// Logic to perform when layer is removed from the system
		virtual void OnDetach() {/* To allow inheritor classes to not implement any logic */ }
		// Logic to perform on every update cycle
		virtual void OnUpdate() {/* To allow inheritor classes to not implement any logic */ }
		// Logic to perform when rendering the GUI
		virtual void OnImGuiRender() {/* To allow inheritor classes to not implement any logic */ }
		// Logic allow the layer to subscribe to events
		virtual void EventSubscribe(const std::vector<EventDispatcherBase*>& dispatchers, bool overlay) {/* To allow inheritor classes to not implement any logic */ }
		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}
