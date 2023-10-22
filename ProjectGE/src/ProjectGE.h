#pragma once

// FOR USE BY PROJECTGE CLIENTS

#include "ProjectGE/Core/Application.h"
#include "ProjectGE/Core/Input.h"
#include "ProjectGE/Core/KeyCodes.h"
#include "ProjectGE/Core/Layer.h"
#include "ProjectGE/Core/Log.h"

#include "ProjectGE/Core/Timer.h"

#include "ProjectGE/ImGui/ImGuiLayer.h"

// ------------Renderer---------------
#include "ProjectGE/Rendering/RenderingManager.h"

#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/VertexBuffer.h"
#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/IndexBuffer.h"
#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/ConstantBuffer.h"
#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/Shader.h"
#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/Topology.h"
#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/GeometryPack.h"
#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/PipelineStateInitializer.h"

#include "ProjectGE/Rendering/RenderAgnostics/BasicStructs.h"

// Material System
#include "ProjectGE/Rendering/RenderAgnostics/MaterialSystem/Material.h"
#include "ProjectGE/Rendering/RenderAgnostics/Shaders/ShaderParameter.h"
// -------------------------------------

// -----------RescourceManagement-----------
#include "ProjectGE/Rendering/RenderingManager.h"
#include "ProjectGE/ResourceAssets/TestResource.h"
#include "ProjectGE/ResourceAssets/TextureAsset.h"
#include "ProjectGE/ResourceAssets/MaterialAsset.h"
#include "ProjectGE/ResourceAssets/ShaderAsset.h"
#include "ProjectGE/ResourceAssets/MeshAsset.h"

// ---------Components-----------------
#include "ProjectGE/Components/Transform.h"
//-------------------------------------
#include "ProjectGE/Camera/OrthoCamera.h"

#ifndef ENTRY_USED
// ENTRY POINT TO GAME ENGINE
#include "ProjectGE/Core/EntryPoint.h"
#endif