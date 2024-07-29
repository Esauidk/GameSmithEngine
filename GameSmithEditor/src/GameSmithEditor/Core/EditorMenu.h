#pragma once
#include "GameSmithEngine.h"


#define REGISTER_EDITOR_MENU_OPTION(PATH, FUNCTION) \
	static struct PATH##RegisterAction { \
		PATH##RegisterAction() { \
			GameSmithEditor::MenuRegistry::GetInstance()->RegisterMenuEntry(#PATH, FUNCTION); \
		} \
	}PATH##Instance

namespace GameSmithEditor {
	class MenuRegistry
	{
	public:
		static MenuRegistry* GetInstance();
		void RegisterMenuEntry(std::string path, std::function<void()> callBack);
	private:
		void GetMenuPaths(std::vector<std::string>* outPaths);
		void ExecuteCallBack(std::string path);
	private:
		static GameSmith::Scope<MenuRegistry> s_Instance;
		std::unordered_map<std::string, std::function<void()>> m_MenuRegistry;

		friend class MenuExecutor;
	};

	class MenuExecutor {
	private:
		static void GetMenuPaths(std::vector<std::string>* outPaths);
		static void ExecuteCallBack(std::string path);

		friend class EditorCoreLayer;
	};
};

