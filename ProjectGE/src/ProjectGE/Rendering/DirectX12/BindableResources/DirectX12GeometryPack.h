#pragma once
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/GeometryPack.h"

namespace ProjectGE {
	class DirectX12GeometryPack : public GeometryPack
	{
	public:
		inline void AttachVertexBuffer(std::shared_ptr<VertexBuffer> vertex) override { m_vb = vertex; }
		inline void AttachIndexBuffer(std::shared_ptr<IndexBuffer> index) override { m_ib = index; }
		inline void AttachTopology(std::shared_ptr<Topology> top) override { m_top = top; }

		std::shared_ptr<IndexBuffer> GetIndexBuffer() const override { return m_ib; }
		void Bind() override;
	private:
		std::shared_ptr<VertexBuffer> m_vb;
		std::shared_ptr<IndexBuffer> m_ib;
		std::shared_ptr<Topology> m_top;
	};
};


