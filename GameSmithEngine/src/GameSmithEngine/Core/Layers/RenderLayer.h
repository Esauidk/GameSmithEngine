#pragma once
#include "GameSmithEngine/Core/Layer.h"

namespace GameSmith {
	class RenderLayer : public Layer
	{
	public:
		RenderLayer() : Layer("RenderLayer") {};
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(float dt) override;
		virtual void OnEndUpdate() override;
	};
};


