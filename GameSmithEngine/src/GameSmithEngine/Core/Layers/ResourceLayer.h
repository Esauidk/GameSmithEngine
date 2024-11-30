#pragma once
#include "GameSmithEngine/Core/Layer.h"
namespace GameSmith {
	class ResourceLayer : public Layer
	{
	public:
		ResourceLayer() : Layer("ResourceLayer") {};
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(float dt) override;
	};
};


