#pragma once
namespace GameSmith {
	enum class FilterType {
		Point,
		Linear,
		Anisotropic
	};

	enum class PaddingMethod {
		Wrap,
		Clamp,
		Mirror
	};

	class Sampler {
	public:
		virtual FilterType GetFilterType() const = 0;
		virtual PaddingMethod GetPaddingMethod() const = 0;
	};
};