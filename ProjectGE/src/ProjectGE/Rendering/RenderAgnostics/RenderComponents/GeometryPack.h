#pragma once
#include "ProjectGE/Core/Core.h"
#include "ProjectGE/Rendering/BindableResource.h"

#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/VertexBuffer.h"
#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/IndexBuffer.h"
#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/Topology.h"

namespace ProjectGE {
	// INTERFACE
	// A wrapper that holds properies of a mesh together (Vertex, Index, Topology)
	class GeometryPack : public BindableResource {
	public:
		// Names are straight forward
		virtual void AttachVertexBuffer(Ref<VertexBuffer> vertex) = 0;
		virtual void AttachIndexBuffer(Ref<IndexBuffer> index) = 0; 
		virtual void AttachTopology(Ref<Topology> top) = 0;

		virtual Ref<IndexBuffer> GetIndexBuffer() const = 0;

		// Instantiates an implementation of the GeometryPack Interface (recommended to use this instead of instantiating a specific implementation)
		static Ref<GeometryPack> Create();
	};
};
