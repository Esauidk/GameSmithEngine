#include "gepch.h"
#include "Factory.h"

namespace GameSmith {
	template <typename T, typename AuthCaller, typename... TArgs>
	Scope<Factory<T, AuthCaller, TArgs...>> Factory<T, AuthCaller, TArgs...>::s_Instance = nullptr;
}