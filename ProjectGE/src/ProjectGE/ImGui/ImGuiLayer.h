#pragma once
#include "ProjectGE/Layer.h"

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
	};
};
