#pragma once

namespace ProjectGE {
	enum class RenderOptions {
		NONE,
		DIRECTX12
	};

	class RenderSettings
	{
	public:
		inline static RenderOptions GetOption() { return s_Option; };
	private:
		static RenderOptions s_Option;
	};
};


