#pragma once

#include "ProjectGE/Core.h"
#include <string>
#include <functional>
#include <queue>
#include <unordered_map>
#include <type_traits>

namespace ProjectGE {
/*
* ----------------------
* Engine's Event System
* ----------------------
*
* Events: Momentary activities happening in the engine (ex: Mouse clicked)
* Dispatcher: Notifier of events to listeners
* A dispatcher will keep track of listeners for different events
* Once events are fired the subset of listeners will be notifed
*/

	enum class EventType {
		None = 0,
		WND_CLOSE, WND_FOCUS, WND_LOST_FOCUS, WND_RESIZE, WND_MOVED,
		APP_TICK, APP_UPDATE, APP_RENDER,
		MOUSE_PRESSED, MOUSE_RELEASED, MOUSE_MOVE, MOUSE_SCROLL,
		KEY_PRESSED, KEY_RELEASED

	};

	enum EventCategory {
		None = 0,
		APP_EVENT = BIT(0),
		INPUT = BIT(1),
		KEYBOARD = BIT(2),
		MOUSE = BIT(3),
		MOUSE_BUTTON = BIT(4)
	};

	class GE_API EventResult {

	};

	class GE_API Event {
		friend class EventDispatcherBase;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual std::string ToString() const { return GetName(); }
		inline bool IsInCategory(EventCategory category) {
			return GetCategoryFlags() & category;
		}

		inline EventResult& GetEventResult() {
			return m_Result;
		}

	protected:
		virtual int GetCategoryFlags() const = 0;
		bool m_Handled = false;
		EventResult m_Result;
	};


	class GE_API EventDispatcherBase {
	public:
		virtual void Dispatch(Event& env) = 0;
		void handledEvent(Event& env) {
			env.m_Handled = true;
		}
	};


	template <typename T>
	class GE_API EventDispatcher : public EventDispatcherBase {
		using Eventfn = std::function<bool(T&)>;

	public:
		EventDispatcher() {
			if (!std::is_base_of<Event, T>::value) {
				// Yell here
			}
		}
		void AddListener(Eventfn func) {
			m_listeners.push(func);
		}

		void Dispatch(Event& evn) override {
			// Copy of the listener queue
			std::queue<Eventfn> copyQueue = m_listeners;

			while (!copyQueue.empty()) {
				Eventfn fn = copyQueue.front();
				bool handled = fn(*(T*)&evn);
				if (handled) {
					handledEvent(evn);
					break;
				}
				copyQueue.pop();
			}
		}

		void operator+=(Eventfn func) {
			AddListener(func);
		}

	private:
		std::queue<Eventfn> m_listeners;
	};

	

#define EVENT_TYPE(type) static EventType GetStaticType() {return EventType::##type; } \
						virtual EventType GetEventType() const override {return GetStaticType();} \
						virtual const char* GetName() const override {return #type;}

#define CATEGORY_TYPE(category) virtual int GetCategoryFlags() const override {return category;}

	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}
};

