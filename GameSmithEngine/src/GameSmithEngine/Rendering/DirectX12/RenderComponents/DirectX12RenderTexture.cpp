#include "gepch.h"
#include "DirectX12RenderTexture.h"

#include "GameSmithEngine/Rendering/DirectX12/DirectX12Core.h"


namespace GameSmith {
	DirectX12RenderTexture::DirectX12RenderTexture(unsigned int width, unsigned int height) : m_State(RTState::READ), m_PrevState(RTState::READ)
	{
		m_Metadata.channels = 3;
		m_Metadata.height = height;
		m_Metadata.width = width;
		m_Metadata.mips = 1;

		m_TextureResource = Scope<DirectX12TextureResource>(new DirectX12TextureResource(m_Metadata, TextureType::Tex2D, TextureMisc::RT));
		auto& core = DirectX12Core::GetCore();
		m_SRVDescriptor = core.GetDescriptorLoader(CBVSRVUAV).AllocateSlot();
		m_RTDescriptor = core.GetDescriptorLoader(RT).AllocateSlot();

		GenerateViews();
	}

	void DirectX12RenderTexture::ChangeState(RTState newState, bool force)
	{
		if (force || newState != m_State) {
			m_PrevState = m_State;
			auto context = DirectX12Core::GetCore().GetDirectCommandContext();
			switch (newState) {
			case RTState::WRITE:
				m_TextureResource->GetStateTracker().TransitionBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET, context);
				break;
			case RTState::READ:
				m_TextureResource->GetStateTracker().TransitionBarrier(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, context);
				break;
			case RTState::COPY:
				m_TextureResource->GetStateTracker().TransitionBarrier(D3D12_RESOURCE_STATE_COPY_SOURCE, context);
				break;
			}
		}
	}

	void DirectX12RenderTexture::CopyToResource(Ref<DirectX12Resource> otherResource, DirectX12CommandContextBase* context)
	{
		auto& list = context->GetCommandList();
		otherResource->GetResourceStateTracker().TransitionBarrier(D3D12_RESOURCE_STATE_COPY_DEST, context);
		ChangeState(RTState::COPY);
		context->FinalizeResourceBarriers();
		list->CopyResource(otherResource->GetResource(), m_TextureResource->GetResource());
		ChangeState(m_PrevState);
		context->FinalizeResourceBarriers();
	}

	void DirectX12RenderTexture::UpdateSize(unsigned int width, unsigned height) {
		m_Metadata.width = width;
		m_Metadata.height = height;
		m_TextureResource = Scope<DirectX12TextureResource>(new DirectX12TextureResource(m_Metadata, TextureType::Tex2D, TextureMisc::RT));
		GenerateViews();
	}

	void DirectX12RenderTexture::GenerateViews()
	{
		auto& core = DirectX12Core::GetCore();
		auto device = core.GetDevice();
		device->CreateShaderResourceView(m_TextureResource->GetResource(), nullptr, m_SRVDescriptor);
		device->CreateRenderTargetView(m_TextureResource->GetResource(), nullptr, m_RTDescriptor);
	}
};

