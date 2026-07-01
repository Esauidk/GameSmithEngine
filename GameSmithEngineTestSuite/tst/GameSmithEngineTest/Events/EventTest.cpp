#include "gtest/gtest.h"
#include "GameSmithEngine/Events/ApplicationEvents.h"
#include "GameSmithEngine/Events/KeyboardEvents.h"
#include "GameSmithEngine/Events/MouseEvents.h"
#include "GameSmithEngine/Events/RenderingEvents.h"

TEST(EventTypeTest, WindowCloseEvent) {
	GameSmith::WindowCloseEvent e;
	EXPECT_EQ(e.GetEventType(), GameSmith::EventType::WND_CLOSE);
	EXPECT_TRUE(e.IsInCategory(GameSmith::EventCategory::APP_EVENT));
	EXPECT_STREQ(e.GetName(), "WND_CLOSE");
}

TEST(EventTypeTest, WindowResizeEvent) {
	GameSmith::WindowResizeEvent e(800, 600);
	EXPECT_EQ(e.GetEventType(), GameSmith::EventType::WND_RESIZE);
	EXPECT_EQ(e.GetWidth(), (unsigned int)800);
	EXPECT_EQ(e.GetHeight(), (unsigned int)600);
}

TEST(EventTypeTest, WindowFocusEvent) {
	GameSmith::WindowFocusEvent e;
	EXPECT_EQ(e.GetEventType(), GameSmith::EventType::WND_FOCUS);
}

TEST(EventTypeTest, WindowLostFocusEvent) {
	GameSmith::WindowLostFocusEvent e;
	EXPECT_EQ(e.GetEventType(), GameSmith::EventType::WND_LOST_FOCUS);
}

TEST(EventTypeTest, WindowMovedEvent) {
	GameSmith::WindowMovedEvent e;
	EXPECT_EQ(e.GetEventType(), GameSmith::EventType::WND_MOVED);
}

TEST(EventTypeTest, AppTickEvent) {
	GameSmith::AppTickEvent e;
	EXPECT_EQ(e.GetEventType(), GameSmith::EventType::APP_TICK);
}

TEST(EventTypeTest, AppUpdateEvent) {
	GameSmith::AppUpdateEvent e;
	EXPECT_EQ(e.GetEventType(), GameSmith::EventType::APP_UPDATE);
}

TEST(EventTypeTest, AppRenderEvent) {
	GameSmith::AppRenderEvent e;
	EXPECT_EQ(e.GetEventType(), GameSmith::EventType::APP_RENDER);
}

TEST(EventTypeTest, KeyPressedEvent) {
	GameSmith::KeyPressedEvent e(65, 1);
	EXPECT_EQ(e.GetEventType(), GameSmith::EventType::KEY_PRESSED);
	EXPECT_EQ(e.GetKeyCode(), 65);
	EXPECT_EQ(e.GetRepeatCount(), 1);
	EXPECT_TRUE(e.IsInCategory(GameSmith::EventCategory::INPUT));
	EXPECT_TRUE(e.IsInCategory(GameSmith::EventCategory::KEYBOARD));
}

TEST(EventTypeTest, KeyReleasedEvent) {
	GameSmith::KeyReleasedEvent e(66);
	EXPECT_EQ(e.GetEventType(), GameSmith::EventType::KEY_RELEASED);
	EXPECT_EQ(e.GetKeyCode(), 66);
}

TEST(EventTypeTest, CharEvent) {
	GameSmith::CharEvent e(97);
	EXPECT_EQ(e.GetEventType(), GameSmith::EventType::CHAR_TYPED);
	EXPECT_EQ(e.GetKeyCode(), 97);
}

TEST(EventTypeTest, MouseMoveEvent) {
	GameSmith::MouseMoveEvent e(100.5f, 200.3f);
	EXPECT_EQ(e.GetEventType(), GameSmith::EventType::MOUSE_MOVE);
	EXPECT_FLOAT_EQ(e.GetX(), 100.5f);
	EXPECT_FLOAT_EQ(e.GetY(), 200.3f);
	EXPECT_TRUE(e.IsInCategory(GameSmith::EventCategory::INPUT));
	EXPECT_TRUE(e.IsInCategory(GameSmith::EventCategory::MOUSE));
}

TEST(EventTypeTest, MouseScrollEvent) {
	GameSmith::MouseScrollEvent e(2.5f, 150.0f, 300.0f);
	EXPECT_EQ(e.GetEventType(), GameSmith::EventType::MOUSE_SCROLL);
	EXPECT_FLOAT_EQ(e.GetDelta(), 2.5f);
	EXPECT_FLOAT_EQ(e.GetX(), 150.0f);
	EXPECT_FLOAT_EQ(e.GetY(), 300.0f);
}

TEST(EventTypeTest, MouseButtonPressEvent) {
	GameSmith::MouseButtonPressEvent e(GameSmith::MouseButtonEvent::MouseButton::LEFT);
	EXPECT_EQ(e.GetEventType(), GameSmith::EventType::MOUSE_PRESSED);
	EXPECT_EQ(e.GetButton(), (int)GameSmith::MouseButtonEvent::MouseButton::LEFT);
	EXPECT_TRUE(e.IsInCategory(GameSmith::EventCategory::MOUSE_BUTTON));
}

TEST(EventTypeTest, MouseButtonReleaseEvent) {
	GameSmith::MouseButtonReleaseEvent e(GameSmith::MouseButtonEvent::MouseButton::RIGHT);
	EXPECT_EQ(e.GetEventType(), GameSmith::EventType::MOUSE_RELEASED);
	EXPECT_EQ(e.GetButton(), (int)GameSmith::MouseButtonEvent::MouseButton::RIGHT);
}

TEST(EventTypeTest, EndFrameEvent) {
	GameSmith::EndFrameRecordEvent e;
	EXPECT_EQ(e.GetEventType(), GameSmith::EventType::FRAME_END);
	EXPECT_TRUE(e.IsInCategory(GameSmith::EventCategory::RENDER_EVENT));
}

TEST(EventDispatcherTest, AddAndDispatch) {
	GameSmith::EventDispatcher<GameSmith::WindowResizeEvent> dispatcher;
	bool called = false;
	dispatcher.AddListener([&](GameSmith::WindowResizeEvent& e) {
		called = true;
		return false;
	}, false);
	
	GameSmith::WindowResizeEvent evt(320, 240);
	dispatcher.Dispatch(evt);
	EXPECT_TRUE(called);
}

TEST(EventDispatcherTest, OverlayListenerCalledFirst) {
	GameSmith::EventDispatcher<GameSmith::WindowResizeEvent> dispatcher;
	std::string order;
	dispatcher.AddListener([&](GameSmith::WindowResizeEvent& e) {
		order += "normal";
		return false;
	}, false);
	dispatcher.AddListener([&](GameSmith::WindowResizeEvent& e) {
		order += "overlay";
		return false;
	}, true);
	
	GameSmith::WindowResizeEvent evt(320, 240);
	dispatcher.Dispatch(evt);
	EXPECT_EQ(order, "overlaynormal");
}

TEST(EventDispatcherTest, HandledEventStopsDispatch) {
	GameSmith::EventDispatcher<GameSmith::WindowResizeEvent> dispatcher;
	int callCount = 0;
	dispatcher.AddListener([&](GameSmith::WindowResizeEvent& e) {
		callCount++;
		return true;
	}, true);
	dispatcher.AddListener([&](GameSmith::WindowResizeEvent& e) {
		callCount++;
		return false;
	}, false);
	
	GameSmith::WindowResizeEvent evt(320, 240);
	dispatcher.Dispatch(evt);
	EXPECT_EQ(callCount, 1);
}

TEST(EventDispatcherTest, WrongTypeNotDispatched) {
	GameSmith::EventDispatcher<GameSmith::WindowResizeEvent> dispatcher;
	bool called = false;
	dispatcher.AddListener([&](GameSmith::WindowResizeEvent& e) {
		called = true;
		return false;
	}, false);
	
	GameSmith::KeyPressedEvent evt(65, 1);
	dispatcher.Dispatch(evt);
	EXPECT_FALSE(called);
}

TEST(RegisterEventTest, RegisterAndDispatch) {
	GameSmith::EventDispatcher<GameSmith::WindowResizeEvent> dispatcher;
	bool called = false;
	bool registered = GameSmith::RegisterEvent<GameSmith::WindowResizeEvent>(
		&dispatcher,
		[&](GameSmith::WindowResizeEvent& e) {
			called = true;
			return false;
		},
		false
	);
	
	EXPECT_TRUE(registered);
	GameSmith::WindowResizeEvent evt(320, 240);
	dispatcher.Dispatch(evt);
	EXPECT_TRUE(called);
}
