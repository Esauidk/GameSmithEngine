#pragma once
#include "ProjectGE/Input.h"
namespace ProjectGE {
	class WindowsInput : public Input
	{
	protected:
		bool IsKeyPressedImpl(int keycode) override;
		bool IsMouseButtonPressedImpl(int button) override;
		float GetMouseXImpl() override;
		float GetMouseYImpl() override;
		std::pair<float, float> GetMousePosImpl() override;
	};
};


