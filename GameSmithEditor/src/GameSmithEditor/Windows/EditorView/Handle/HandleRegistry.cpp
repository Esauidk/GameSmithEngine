#include "HandleRegistry.h"
#include "GameSmithEngine/Utilities/Factory/FactoryImpl.h"

namespace GameSmithEditor {
	template class GameSmith::Factory<IViewHandle, ViewHandleRenderer>;
};