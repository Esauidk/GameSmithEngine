#pragma once
#include "Event.h"

namespace ProjectGE {
	class GE_API MouseMoveEvent : public Event {
	public:
		MouseMoveEvent(float x, float y) : m_X(x), m_Y(y) {}
		inline float GetX() const { return m_X; }
		inline float GetY() const { return m_Y; }

		std::string ToString() const override {
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
		MouseScrollEvent(float delta, float pos_x, float pos_y) : m_Delta(delta), m_PosX(pos_x), m_PosY(pos_y) {}
		inline float GetDelta() const{ return m_Delta; }
		inline float GetX() const { return m_PosX; }
		inline float GetY() const { return m_PosY; }

		std::string ToString() const override {
			std::stringstream oss;

			oss << "MouseScrollEvent: " << m_Delta << "(" << m_PosX << "," << m_PosY << ")";

			return oss.str();
		}

		EVENT_TYPE(MOUSE_SCROLL)
	protected:
		CATEGORY_TYPE(EventCategory::INPUT | EventCategory::MOUSE)
	private:
		float m_Delta;
		float m_PosX;
		float m_PosY;
	};

	class GE_API MouseButtonEvent : public Event {
	public:
		enum MouseButton {
			LEFT = 0,
			RIGHT = 1
		};

		inline int GetButton() const { return m_Button; }
	protected:
		CATEGORY_TYPE(EventCategory::INPUT | EventCategory::MOUSE | EventCategory::MOUSE_BUTTON)
		MouseButtonEvent(MouseButton button) : m_Button(button) {}
		int m_Button;
	};

	class GE_API MouseButtonPressEvent : public MouseButtonEvent {
	public:
		MouseButtonPressEvent(MouseButtonEvent::MouseButton button) : MouseButtonEvent(button) {}
		std::string ToString() const override {
			std::stringstream oss;

			oss << "MouseButtonPressEvent: " << m_Button;

			return oss.str();
		}
		EVENT_TYPE(MOUSE_PRESSED)
	};

	class GE_API MouseButtonReleaseEvent : public MouseButtonEvent {
	public:
		MouseButtonReleaseEvent(MouseButtonEvent::MouseButton button) : MouseButtonEvent(button) {}
		std::string ToString() const override {
			std::stringstream oss;

			oss << "MouseButtonReleaseEvent: " << m_Button;

			return oss.str();
		}
		EVENT_TYPE(MOUSE_RELEASED)
	};

};
