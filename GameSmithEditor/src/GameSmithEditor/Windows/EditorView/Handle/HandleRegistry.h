#pragma once

#include "GameSmithEngine/Utilities/Factory/Factory.h"
#include "ViewHandle.h"

#define GE_EDITOR_REGISTERHANDLE(ClassType) \
	 static struct ClassType##RegisterAction { \
		ClassType##RegisterAction() { \
			GameSmithEditor::HandleFactory::GetInstance()->Register( \
				#ClassType, []() {return new ClassType(); } \
			); \
		} \
	}ClassType##Instance;

namespace GameSmithEditor {
	class ViewHandleRenderer;
	using HandleFactory = GameSmith::Factory<IViewHandle, ViewHandleRenderer>;
};