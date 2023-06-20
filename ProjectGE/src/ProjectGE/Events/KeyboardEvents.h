#pragma once
#include "Event.h"

namespace ProjectGE {
	class GE_API KeyEvent : public Event {
	public:
		inline int GetKeyCode() const { return m_KeyCode; }
	protected:
		KeyEvent(int keyCode) : m_KeyCode(keyCode) {}
		int m_KeyCode;

		CATEGORY_TYPE(EventCategory::INPUT | EventCategory::KEYBOARD)
	};

	class GE_API KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(int keyCode, int repeatCount) : KeyEvent(keyCode), m_RepeatCount(repeatCount) {}
		inline int GetRepeatCount() const { return m_RepeatCount; }

		virtual std::string ToString() const override {
			std::stringstream oss;
			oss << "KeyPressedEvent: KeyCode" << m_KeyCode << " (" << m_RepeatCount << " repeats)";

			return oss.str();
		}

		EVENT_TYPE(KEY_PRESSED)
	private:
		int m_RepeatCount;
	};

	class GE_API KeyReleasedEvent : public KeyEvent {
		KeyReleasedEvent(int keyCode) : KeyEvent(keyCode) {}

		std::string ToString() const override {
			std::stringstream oss;
			oss << "KeyReleasedEvent: KeyCode" << m_KeyCode;

			oss.str();
		}

		EVENT_TYPE(KEY_RELEASED)
	};
};