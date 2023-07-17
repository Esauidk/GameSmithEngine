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
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	};
};
