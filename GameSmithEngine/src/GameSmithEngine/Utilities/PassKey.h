#pragma once

namespace GameSmith {
	template <typename T>
	class PassKey {
		friend T;
		explicit PassKey() = default;
	};
};