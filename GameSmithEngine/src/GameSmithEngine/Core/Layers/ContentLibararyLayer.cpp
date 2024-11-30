#include "gepch.h"
#include "ContentLibararyLayer.h"
#include "GameSmithEngine/ContentLibrarySystem/ContentLibraryManager.h"

namespace GameSmith {
	void ContentLibararyLayer::OnAttach()
	{
		ContentLibraryManager::Init();
	}

	void ContentLibararyLayer::OnDetach()
	{
		ContentLibraryManager::Shutdown();
	}
};
