#pragma once
#include <gepch.h>
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/Utilities/PassKey.h"

namespace GameSmith {
	template <typename T, typename AuthCaller, typename... TArgs>
	class Factory {
	public:
		static Factory* GetInstance();

		void Register(const std::string& name, std::function<T* (TArgs...)> func) {
			if (!m_registry.contains(name)) {
				m_registry[name] = func;
			}
		}

		void AssignLabel(const std::string& name, const std::string& label) {
			GE_CORE_ASSERT(m_registry.contains(name), std::format("No definition for requested key: {0}", name));
			if (!m_labels.contains(name)) {
				m_labels[name] = label;
			}
		}

		std::unordered_map<std::string, std::string> GetLabels() const {
			return m_labels;
		}

		void GetRegisteredKeys(std::vector<std::string>* outVec) const {
			for (const auto& entry : m_registry) {
				outVec->push_back(entry.first);
			}
		}

		Ref<T> Create(PassKey<AuthCaller> key, std::string name, TArgs... args) {
			GE_CORE_ASSERT(m_registry.contains(name), std::format("No definition for requested key: {0}", name));
			return Ref<T>(m_registry[name](args...));
		}

		void Clear() {
			m_registry.clear();
			m_labels.clear();
		}
	private:
		static Scope<Factory> s_Instance;
		std::unordered_map<std::string, std::function<T* (TArgs...)>> m_registry;
		std::unordered_map<std::string, std::string> m_labels;
	};
};