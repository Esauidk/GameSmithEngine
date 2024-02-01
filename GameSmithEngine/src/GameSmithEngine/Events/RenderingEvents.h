#pragma once
#include "Event.h"

namespace GameSmith {
	class EndFrameEvent : public Event {
	public:
		EndFrameEvent() {};
		std::string ToString() const override {
			std::stringstream oss;

			oss << "EndFrameEvent";

			return oss.str();
		}

		EVENT_TYPE(FRAME_END)
	protected:
		CATEGORY_TYPE(EventCategory::RENDER_EVENT)
	};
};
