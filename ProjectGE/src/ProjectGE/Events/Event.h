#pragma once

#include "gepch.h"
#include "ProjectGE/Core.h"
#include "ProjectGE/MixStack.h"


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

#define EVENT_TYPE(type) static EventType GetStaticType() {return EventType::##type; } \
						virtual EventType GetEventType() const override {return GetStaticType();} \
						virtual const char* GetName() const override {return #type;}

#define CATEGORY_TYPE(category) virtual int GetCategoryFlags() const override {return category;}

#define EVENT_CAST(eventclass, dispatcher, result) (result = dynamic_cast<ProjectGE::EventDispatcher<eventclass>*>(dispatcher)) != nullptr
#define BIND_EVENT_FN(classname, x) std::bind(&classname::x, this, std::placeholders::_1)

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
	protected:
		void handledEvent(Event& env) {
			env.m_Handled = true;
		}
	};


	template <typename T>
	class EventDispatcher : public EventDispatcherBase {
		using Eventfn = std::function<bool(T&)>;

	public:
		EventDispatcher() {
			GE_CORE_ASSERT(!std::is_base_of<Event, T>::value);
		}
		void AddListener(Eventfn func, bool overlay) {
			Eventfn* fn = new Eventfn;
			*fn = func;
			if (overlay) {
				m_listeners.PushSpecial(fn);
			}
			else {
				m_listeners.Push(fn);
			}
			
		}

		void Dispatch(Event& evn) override {

			if (evn.GetEventType() == T::GetStaticType()) {
				// Iterate through vector

				auto test = m_listeners.begin();
				auto iter = m_listeners.end();

				for (iter; iter != m_listeners.begin();) {
					Eventfn* fn = *(--iter);
					bool handled = (*fn)(*(T*)&evn);
					if (handled) {
						handledEvent(evn);
						break;
					}
				}
			}
			
		}

	private:
		MixStack<Eventfn> m_listeners;
	};


	template <typename EventType>
	bool RegisterEvent(EventDispatcherBase* dispatch, std::function<bool(EventType&)> fnc, bool overlay) {
		EventDispatcher<EventType>* eventDispatch;

		if (EVENT_CAST(EventType, dispatch, eventDispatch)) {
			eventDispatch->AddListener(fnc, overlay);
			return true;
		}

		return false;
	}

	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}
};


