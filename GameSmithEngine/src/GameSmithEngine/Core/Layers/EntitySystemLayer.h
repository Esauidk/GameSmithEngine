#pragma once
#include "GameSmithEngine/Core/Layer.h"
namespace GameSmith {
	class EntitySystemLayer : public Layer
	{
	public:
		EntitySystemLayer() : Layer("EntitySystemLayer") {};
		virtual void OnAttach(const ApplicationSpecs& specs) override;
		virtual void OnDetach() override;
		virtual void OnUpdate(float dt) override;
	};
};


