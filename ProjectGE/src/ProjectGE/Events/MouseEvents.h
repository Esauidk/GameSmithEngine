#pragma once
#include "Event.h"

namespace ProjectGE {
	class GE_API MouseMoveEvent : public Event {
	public:
		MouseMoveEvent(float x, float y) : m_X(x), m_Y(y) {}
		inline float GetX() const { return m_X; }
		inline float GetY() const { return m_Y; }

		virtual std::string ToString() const override {
			std::stringstream oss;

			oss << "MouseMoveEvent: " << "(" << m_X << "," << m_Y << ")";

			return oss.str();
		}

		EVENT_TYPE(MOUSE_MOVE)
	protected:
		CATEGORY_TYPE(EventCategory::INPUT | EventCategory::MOUSE)
	private:
		float m_X;
		float m_Y;
	};

	class GE_API MouseScrollEvent : public Event {
	public:
		MouseScrollEvent(float delta_x, float delta_y) : m_DeltaX(delta_x), m_DeltaY(delta_y) {}
		inline float GetDeltaX() const { return m_DeltaX; }
		inline float GetDeltaY() const { return m_DeltaY; }

		virtual std::string ToString() const override {
			std::stringstream oss;

			oss << "MouseScrollEvent: " << "(" << m_DeltaX << "," << m_DeltaY << ")";

			return oss.str();
		}

		EVENT_TYPE(MOUSE_SCROLL)
	protected:
		CATEGORY_TYPE(EventCategory::INPUT | EventCategory::MOUSE)
	private:
		float m_DeltaX;
		float m_DeltaY;
	};

	class GE_API MouseButtonEvent : public Event {
	public:
		inline int GetButton() const { return m_Button; }
	protected:
		CATEGORY_TYPE(EventCategory::INPUT | EventCategory::MOUSE | EventCategory::MOUSE_BUTTON)
			MouseButtonEvent(int button) : m_Button(button) {}
		int m_Button;
	};

	class GE_API MouseButtonPressEvent : public MouseButtonEvent {
	public:
		MouseButtonPressEvent(int button) : MouseButtonEvent(button) {}
		virtual std::string ToString() const override {
			std::stringstream oss;

			oss << "MouseButtonPressEvent: " << m_Button;

			return oss.str();
		}
		EVENT_TYPE(MOUSE_PRESSED)
	};

	class GE_API MouseButtonReleaseEvent : public MouseButtonEvent {
	public:
		MouseButtonReleaseEvent(int button) : MouseButtonEvent(button) {}
		virtual std::string ToString() const override {
			std::stringstream oss;

			oss << "MouseButtonReleaseEvent: " << m_Button;

			return oss.str();
		}
		EVENT_TYPE(MOUSE_RELEASED)
	};

};
