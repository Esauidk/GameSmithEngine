#pragma once

namespace GameSmith {
	enum DirectX12QueueType {
		Direct,
		Copy,
		NUM_QUEUES
	};

	inline std::string QueueString(DirectX12QueueType type) {
		switch (type) {
		case Direct:
			return "Direct";
		case Copy:
			return "Copy";
		default:
			return "Unknown";
		}
	}
};
