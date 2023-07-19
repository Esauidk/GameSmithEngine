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
		void OnImGuiRender() override;

		void Begin() const;
		void End() const;
	};
};
