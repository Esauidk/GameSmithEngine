#pragma once

// FOR USE BY PROJECTGE CLIENTS

#include "ProjectGE/Application.h"
#include "ProjectGE/Input.h"
#include "ProjectGE/KeyCodes.h"
#include "ProjectGE/Layer.h"
#include "ProjectGE/Log.h"

#include "ProjectGE/ImGui/ImGuiLayer.h"

// Renderer
#include "ProjectGE/Rendering/Renderer.h"
#include "ProjectGE/Rendering/RenderCommand.h"

#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/VertexBuffer.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/IndexBuffer.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/ShaderArguementDefiner.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/Shader.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/PipelineStateObject.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/Topology.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/GeometryPack.h"

#include "ProjectGE/Rendering/RenderAgnostics/BasicStructs.h"

#include "ProjectGE/Camera/OrthoCamera.h"

// ENTRY POINT TO GAME ENGINE
#include "ProjectGE/EntryPoint.h"