#include "EditorMenu.h"

namespace GameSmithEditor {
	GameSmith::Scope<MenuRegistry> MenuRegistry::s_Instance = nullptr;

	MenuRegistry* MenuRegistry::GetInstance()
	{
		if (s_Instance == nullptr) {
			s_Instance = GameSmith::Scope<MenuRegistry>(new MenuRegistry());
		}

		return s_Instance.get();
	}

	void MenuRegistry::RegisterMenuEntry(std::string path, std::function<void()> callBack)
	{
		m_MenuRegistry.insert_or_assign(path, callBack);
	}

	void MenuRegistry::GetMenuPaths(std::vector<std::string>* outPaths)
	{
		for (auto& entry : m_MenuRegistry) {
			outPaths->push_back(entry.first);
		}
	}

	void MenuRegistry::ExecuteCallBack(std::string path)
	{
		if (m_MenuRegistry.contains(path)) {
			m_MenuRegistry.find(path)->second();
		}
	}

	void MenuExecutor::GetMenuPaths(std::vector<std::string>* outPaths)
	{
		auto instance = MenuRegistry::GetInstance();
		instance->GetMenuPaths(outPaths);
	}

	void MenuExecutor::ExecuteCallBack(std::string path)
	{
		auto instance = MenuRegistry::GetInstance();
		instance->ExecuteCallBack(path);
	}
};

