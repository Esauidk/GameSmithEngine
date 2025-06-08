#include "gepch.h"
#include "ForwardRender.h"

namespace GameSmith {
	void ForwardRender::Submit(Ref<VertexBuffer> vBuff, Ref<IndexBuffer> iBuff, Ref<Material> mat) const
	{
		m_APIInstance->SetVertexBuffer(vBuff);
		m_APIInstance->SetIndexBuffer(iBuff);
		mat->ApplyMaterial();
		m_APIInstance->DrawIndexed(iBuff->GetCount(), 1);

		// TODO: Remove by queuing command lists to be processed on a seperate thread
		//m_APIInstance->SubmitRecording();
	}
};