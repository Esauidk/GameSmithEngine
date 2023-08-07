#include "gepch.h"
#include "Renderer.h"


namespace ProjectGE {
	void Renderer::BeginScene()
	{
	}
	void Renderer::EndScene()
	{
	}
	void Renderer::Submit(VertexBuffer* vBuffer, IndexBuffer* iBuffer)
	{
		vBuffer->Bind();
		iBuffer->Bind();
		RenderCommand::DrawIndexed(vBuffer, iBuffer);
	}
};
