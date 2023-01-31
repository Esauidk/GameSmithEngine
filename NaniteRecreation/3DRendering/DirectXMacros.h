#pragma once

#define RENDER_THROW(hrcall) if(FAILED((hrcall))) throw DirectXRenderer::HrException(__LINE__,__FILE__, (hrcall) )

