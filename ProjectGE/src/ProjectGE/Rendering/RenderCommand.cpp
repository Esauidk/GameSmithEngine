#include "gepch.h"
#include "RenderCommand.h"

#include "ProjectGE/Rendering/DirectX12/DirectX12RendererAPI.h"

namespace ProjectGE {
	RendererAPI* RenderCommand::s_RendererAPI = new DirectX12RendererAPI;
};