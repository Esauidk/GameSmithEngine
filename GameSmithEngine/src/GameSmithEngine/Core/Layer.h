#pragma once

#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Events/Event.h"

namespace GameSmith {
	// ABSTRACT CLASS
	// The layer class is a abstract representation of a submodule that can be inserted in and out of active execution 
	class GE_API Layer {
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
		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}
