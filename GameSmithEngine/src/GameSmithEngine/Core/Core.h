#pragma once

#include <memory>

#ifdef GE_PLATFORM_WINDOWS
	#if GE_DYNAMIC_LINK
		#pragma warning(disable: 4251)
		#ifdef GE_BUILD_DLL
			#define GE_API __declspec(dllexport)
		#else
			#define GE_API __declspec(dllimport)
		#endif
	#else
		#define GE_API
	#endif
#else
	#error ONLY WINDOWS IS SUPPORTED!
#endif

#ifdef GE_DEBUG
	#define GE_ENABLE_ASSERTS
#endif

#ifdef GE_ENABLE_ASSERTS
	#define GE_APP_ASSERT(x, ...) {if(!(x)) {GE_APP_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define GE_CORE_ASSERT(x, ...) {if(!(x)) {GE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define GE_APP_ASSERT(x, ...)
	#define GE_CORE_ASSERT(x, ...)
#endif

#define MIN(x, y) (x < y) ? x : y

#define MAX(x,y) (x < y) ? y : x

#define BIT(x) (1 << x)

#define GE_BIND_EVENT_FN(function, instance) std::bind(&function, instance, std::placeholders::_1)

namespace GameSmith {
	// An object smart reference that has sole ownership over a resource
	template<typename T>
	using Scope = std::unique_ptr<T>;

	// An object smart reference that shares ownership with other instances over a resource
	template<typename T>
	using Ref = std::shared_ptr<T>;

	// An object smart reference that does not claim ownership but keeps track of resource state
	template<typename T>
	using Connection = std::weak_ptr<T>;

	template<typename T, typename U>
	Scope<T> CastPtr(Scope<U> ptr) { return std::dynamic_pointer_cast<T>(ptr); }

	template<typename T, typename U>
	Ref<T> CastPtr(Ref<U> ptr) { return std::dynamic_pointer_cast<T>(ptr); }
};
