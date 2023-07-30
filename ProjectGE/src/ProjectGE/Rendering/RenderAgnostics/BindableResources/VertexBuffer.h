#pragma once
#include "ProjectGE/Rendering/RenderAgnostics/BufferLayout.h"
#include "ProjectGE/Rendering/RenderAgnostics/PipelineDefiner.h"
#include "ProjectGE/Rendering/BindableResource.h"


namespace ProjectGE {
	class VertexBuffer : public BindableResource
	{
	public:
		virtual ~VertexBuffer() {}
		virtual void AttachLayout(const BufferLayoutBuilder& layout) = 0;
		virtual PipelineDefiner* GetLayout() = 0;
		static VertexBuffer* Create(void* verticies, UINT count);
	};
};


