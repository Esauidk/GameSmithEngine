#include "gepch.h"
#include "DirectX12TriangleDemo.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


#include "ProjectGE/Rendering/DirectX12/DirectX12Renderer.h"

namespace ProjectGE {

	DirectX12TriangleDemo::DirectX12TriangleDemo() : TriangleDemo() {}	

	void DirectX12TriangleDemo::Draw(Renderer* renderer)
	{
		DirectX12Renderer* dRender = (DirectX12Renderer*)renderer;

		auto list = dRender->GetDrawCommandList();
		//auto list = DirectX12Renderer::GetDirectCommandList();
		m_State->Bind(list.Get());
		m_Root->Bind(list.Get());

		//TODO: Use this as refernece code when doing projection later

		/*glm::mat4 proj = glm::orthoLH(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
	
		auto eye = glm::vec3(0, 0, -10);
		auto focus = glm::vec3(0, 0, 0);
		auto up = glm::vec3(0, 1, 0);
		glm::mat4 lookAt = glm::lookAtLH(eye, focus, up);

		auto mvp = proj * lookAt;
		list->SetGraphicsRoot32BitConstants(0, sizeof(glm::mat4) / 4, &mvp, 0);*/

		m_Topo->Bind(list.Get());
		m_VertexBuffer->Bind(list.Get());
		m_IndexBuffer->Bind(list.Get());

		list->DrawIndexedInstanced(m_IndexCount, 1, 0, 0, 0);

		DirectX12Renderer::AsyncJobSubmission(list, D3D12_COMMAND_LIST_TYPE_DIRECT);
	}

	
};
