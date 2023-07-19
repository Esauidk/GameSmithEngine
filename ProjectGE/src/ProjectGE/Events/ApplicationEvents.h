#pragma once
#include "Event.h"

namespace ProjectGE {
	class GE_API WindowCloseEvent : public Event {
	public:
		EVENT_TYPE(WND_CLOSE)
	protected:
		CATEGORY_TYPE(EventCategory::APP_EVENT)
	};

	class GE_API WindowFocusEvent : public Event {
	public:
		EVENT_TYPE(WND_FOCUS)
	protected:
		CATEGORY_TYPE(EventCategory::APP_EVENT)
	};

	class GE_API WindowLostFocusEvent : public Event {
	public:
		EVENT_TYPE(WND_LOST_FOCUS)
	protected:
		CATEGORY_TYPE(EventCategory::APP_EVENT)
	};

	class GE_API WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(unsigned int width, unsigned height) : m_Width(width), m_Height(height) {}
		EVENT_TYPE(WND_RESIZE)
		std::string ToString() const override{
			std::stringstream oss;
			oss << "WindowResizeEvent: (" << m_Width << "," << m_Height << ")";

			return oss.str();
		}

		inline unsigned int GetWidth() { return m_Width; }
		inline unsigned int GetHeight() { return m_Height; }
	protected:
		CATEGORY_TYPE(EventCategory::APP_EVENT)
	private:
		unsigned int m_Width;
		unsigned int m_Height;
	};

	class GE_API WindowMovedEvent : public Event {
	public:
		EVENT_TYPE(WND_MOVED)
	protected:
		CATEGORY_TYPE(EventCategory::APP_EVENT)
	};

	class GE_API AppTickEvent : public Event {
	public:
		EVENT_TYPE(APP_TICK)
	protected:
		CATEGORY_TYPE(EventCategory::APP_EVENT)
	};

	class GE_API AppUpdateEvent : public Event {
	public:
		EVENT_TYPE(APP_UPDATE)
	protected:
		CATEGORY_TYPE(EventCategory::APP_EVENT)
	};

	class GE_API AppRenderEvent : public Event {
	public:
		EVENT_TYPE(APP_RENDER)
	protected:
		CATEGORY_TYPE(EventCategory::APP_EVENT)
	};
};
