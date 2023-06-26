#pragma once
#include "ProjectGE/Layer.h"
#include "ProjectGE/Events/ApplicationEvents.h"
#include "ProjectGE/Events/KeyboardEvents.h"
#include "ProjectGE/Events/MouseEvents.h"

namespace ProjectGE {
	class GE_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void EventSubscribe(std::vector<EventDispatcherBase*> dispatchers, bool overlay) override;
	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleaseEvent& e);
		bool OnMouseMoveEvent(MouseMoveEvent& e);
		bool OnMouseScrollEvent(MouseScrollEvent& e);

		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		//bool OnKeyTypedEvent(KeyTypedEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);
	};
};
