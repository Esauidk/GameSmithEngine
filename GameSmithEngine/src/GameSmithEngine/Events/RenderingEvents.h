#pragma once
#include "Event.h"

namespace GameSmith {
	class GE_API EndFrameRecordEvent : public Event {
	public:
		EndFrameRecordEvent() {};
		std::string ToString() const override {
			std::stringstream oss;

			oss << "EndFrameRecordEvent";

			return oss.str();
		}

		EVENT_TYPE(FRAME_END)
	protected:
		CATEGORY_TYPE(EventCategory::RENDER_EVENT)
	};
};
