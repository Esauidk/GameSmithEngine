#pragma once
#include "Event.h"

namespace GameSmith {
	// Any event related to the a key on a keyboard
	class GE_API KeyEvent : public Event {
	public:
		inline int GetKeyCode() const { return m_KeyCode; }
	protected:
		KeyEvent(int keyCode) : m_KeyCode(keyCode) {}
		int m_KeyCode;

		CATEGORY_TYPE(EventCategory::INPUT | EventCategory::KEYBOARD)
	};

	// When the key is pressed
	class GE_API KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(int keyCode, int repeatCount) : KeyEvent(keyCode), m_RepeatCount(repeatCount) {}
		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override {
			std::stringstream oss;
			oss << "KeyPressedEvent: KeyCode" << m_KeyCode << " (" << m_RepeatCount << " repeats)";

			return oss.str();
		}

		EVENT_TYPE(KEY_PRESSED)
	private:
		int m_RepeatCount;
	};

	// When a character key is pressed
	class GE_API CharEvent : public KeyEvent {
	public:
		CharEvent(int keyCode) : KeyEvent(keyCode) {}

		std::string ToString() const override {
			std::stringstream oss;
			oss << "CharEvent: KeyCode" << m_KeyCode;

			return oss.str();
		}

		EVENT_TYPE(CHAR_TYPED)
	};

	// When a key is released
	class GE_API KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(int keyCode) : KeyEvent(keyCode) {}

		std::string ToString() const override {
			std::stringstream oss;
			oss << "KeyReleasedEvent: KeyCode " << m_KeyCode;

			return oss.str();
		}

		EVENT_TYPE(KEY_RELEASED)
	};
};