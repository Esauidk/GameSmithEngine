#pragma once
#include "../RenderExceptions.h"

#define RENDER_THROW(hrcall) if(FAILED((hrcall))) throw RenderExceptions::HrException(__LINE__,__FILE__, (hrcall) )

