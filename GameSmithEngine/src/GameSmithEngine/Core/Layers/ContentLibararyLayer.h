#pragma once
#include "GameSmithEngine/Core/Layer.h"

namespace GameSmith {
	class ContentLibararyLayer : public Layer
	{
	public:
		ContentLibararyLayer() : Layer("ContentLibraryLayer") {}
		virtual void OnAttach() override;
		virtual void OnDetach() override;
	};
};


