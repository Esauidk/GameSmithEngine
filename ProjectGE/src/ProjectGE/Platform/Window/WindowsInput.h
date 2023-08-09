#pragma once
#include "ProjectGE/Core/Input.h"

namespace ProjectGE {
	// Windows Implementation of Input Polling System
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


