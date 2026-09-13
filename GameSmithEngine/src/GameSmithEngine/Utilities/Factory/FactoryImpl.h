#pragma once
#include "Factory.h"

namespace GameSmith {
	template <typename T, typename AuthCaller, typename... TArgs>
	Scope<Factory<T, AuthCaller, TArgs...>> Factory<T, AuthCaller, TArgs...>::s_Instance = nullptr;

	template <typename T, typename AuthCaller, typename... TArgs>
	Factory<T, AuthCaller, TArgs...>* Factory<T, AuthCaller, TArgs...>::GetInstance() {
		if (!s_Instance) {
			s_Instance = Scope<Factory>(new Factory());
		}
		return s_Instance.get();
	}
};
