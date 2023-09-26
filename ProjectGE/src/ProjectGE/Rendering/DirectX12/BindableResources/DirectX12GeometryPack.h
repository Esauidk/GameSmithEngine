#pragma once
#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/GeometryPack.h"

#include "ProjectGE/Core/Core.h"

namespace ProjectGE {
	class DirectX12GeometryPack : public GeometryPack
	{
	public:
		inline void AttachVertexBuffer(Ref<VertexBuffer> vertex) override { m_vb = vertex; }
		inline void AttachIndexBuffer(Ref<IndexBuffer> index) override { m_ib = index; }
		inline void AttachTopology(Ref<Topology> top) override { m_top = top; }

		Ref<IndexBuffer> GetIndexBuffer() const override { return m_ib; }
		void Bind() override;
	private:
		Ref<VertexBuffer> m_vb;
		Ref<IndexBuffer> m_ib;
		Ref<Topology> m_top;
	};
};


