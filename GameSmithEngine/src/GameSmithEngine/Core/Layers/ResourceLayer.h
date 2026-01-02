#pragma once
#include "GameSmithEngine/Core/Layer.h"
namespace GameSmith {
	class ResourceLayer : public Layer
	{
	public:
		ResourceLayer() : Layer("ResourceLayer") {};
		virtual void OnAttach(const ApplicationSpecs& specs) override;
		virtual void OnDetach() override;
		virtual void OnUpdate(float dt) override;
	};
};


