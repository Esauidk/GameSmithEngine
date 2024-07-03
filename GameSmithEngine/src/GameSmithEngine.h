#pragma once

// FOR USE BY GameSmithEngine CLIENTS

#include "GameSmithEngine/Core/Application.h"
#include "GameSmithEngine/Core/Input.h"
#include "GameSmithEngine/Core/KeyCodes.h"
#include "GameSmithEngine/Core/Layer.h"
#include "GameSmithEngine/Core/Log.h"

#include "GameSmithEngine/Core/Timer.h"

#include "GameSmithEngine/ImGui/ImGuiLayer.h"

// ------------ Events --------------
#include "GameSmithEngine/Events/ApplicationEvents.h"
#include "GameSmithEngine/Events/KeyboardEvents.h"
#include "GameSmithEngine/Events/MouseEvents.h"
#include "GameSmithEngine/Events/RenderingEvents.h"
#include "GameSmithEngine/Events/Event.h"

// ------------Renderer---------------
#include "GameSmithEngine/Rendering/RenderingManager.h"

#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/VertexBuffer.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/IndexBuffer.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/ConstantBuffer.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/Texture.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/Shader.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/Topology.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/PipelineStateInitializer.h"

#include "GameSmithEngine/Rendering/RenderAgnostics/BasicStructs.h"

#include "GameSmithEngine/Rendering/RenderAgnostics/Camera/Camera.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Camera/OrthoCamera.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Camera/PerspectiveCamera.h"

#include "GameSmithEngine/Rendering/RenderAgnostics/LightingSystem/LightSource.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/LightingSystem/Sources/DirectionalLight.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/LightingSystem/Sources/PointLight.h"

// Material System
#include "GameSmithEngine/Rendering/RenderAgnostics/MaterialSystem/Material.h"
// -------------------------------------

// ----------- Entity System -------------
#include "GameSmithEngine/EntitySystem/GameObject.h"

// Component System
#include "GameSmithEngine/EntitySystem/Components/Component.h"
#include "GameSmithEngine/EntitySystem/Components/ComponentFactory.h"
#include "GameSmithEngine/EntitySystem/Transform.h"
#include "GameSmithEngine/EntitySystem/Components/MeshRenderer.h"
#include "GameSmithEngine/EntitySystem/Components/TestComponent.h"
#include "GameSmithEngine/EntitySystem/Components/CameraComponent.h"
// ------------------------------------------

// ------------ Game Chunk Management -------------
#include "GameSmithEngine/GameChunkSystem/ChunkManager.h"
#include "GameSmithEngine/GameChunkSystem/GameChunk.h"
// -------------------------------------------

// -----------RescourceManagement-----------
#include "GameSmithEngine/Rendering/RenderingManager.h"
#include "GameSmithEngine/ResourceAssets/TestResource.h"
#include "GameSmithEngine/ResourceAssets/TextureAsset.h"
#include "GameSmithEngine/ResourceAssets/MaterialAsset.h"
#include "GameSmithEngine/ResourceAssets/ShaderAsset.h"
#include "GameSmithEngine/ResourceAssets/MeshAsset.h"


// -------------- Utilities ----------------
#include "GameSmithEngine/Utilities/ParameterContainer.h"

#ifndef ENTRY_USED
// ENTRY POINT TO GAME ENGINE
#include "GameSmithEngine/Core/EntryPoint.h"
#endif