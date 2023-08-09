#pragma once
#include "gepch.h"
#include "ProjectGE/Rendering/BindableResource.h"

#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/VertexBuffer.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/IndexBuffer.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/Topology.h"

namespace ProjectGE {
	// INTERFACE
	// A wrapper that holds properies of a mesh together (Vertex, Index, Topology)
	class GeometryPack : public BindableResource {
	public:
		// Names are straight forward
		virtual void AttachVertexBuffer(std::shared_ptr<VertexBuffer> vertex) = 0;
		virtual void AttachIndexBuffer(std::shared_ptr<IndexBuffer> index) = 0; 
		virtual void AttachTopology(std::shared_ptr<Topology> top) = 0;

		virtual std::shared_ptr<IndexBuffer> GetIndexBuffer() const = 0;

		// Instantiates an implementation of the GeometryPack Interface (recommended to use this instead of instantiating a specific implementation)
		static GeometryPack* Create();
	};
};
