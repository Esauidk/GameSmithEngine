#include "gepch.h"
#include "DirectX12RenderTexture.h"
#include "DirectX12Texture2D.h"

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

	DirectX12RenderTexture::DirectX12RenderTexture(unsigned int width, unsigned int height, float* clearColor)
	{
		m_Metadata.channels = 3;
		m_Metadata.height = height;
		m_Metadata.width = width;
		m_Metadata.mips = 1;
		memcpy(m_Metadata.clearColor, clearColor, sizeof(float) * 4);

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
			case RTState::COPYSRC:
				m_TextureResource->GetStateTracker().TransitionBarrier(D3D12_RESOURCE_STATE_COPY_SOURCE, context);
				break;
			case RTState::COPYDST:
				m_TextureResource->GetStateTracker().TransitionBarrier(D3D12_RESOURCE_STATE_COPY_DEST, context);
				break;
			}
			m_State = newState;
			context->FinalizeResourceBarriers();
		}
	}

	void DirectX12RenderTexture::CopyToResource(Ref<DirectX12Resource> otherResource, DirectX12CommandContextBase* context)
	{
		auto& list = context->GetCommandList();
		ChangeState(RTState::COPYSRC);
		context->FinalizeResourceBarriers();
		list->CopyResource(otherResource->GetResource(), m_TextureResource->GetResource());
		ChangeState(m_PrevState);
		context->FinalizeResourceBarriers();
	}

	void DirectX12RenderTexture::CopyToResource(Ref<DirectX12RenderTexture> otherTex, DirectX12CommandContextBase* context) {
		auto& list = context->GetCommandList();
		ChangeState(RTState::COPYSRC);
		otherTex->ChangeState(RTState::COPYDST);
		context->FinalizeResourceBarriers();
		list->CopyResource(otherTex->m_TextureResource->GetResource(), m_TextureResource->GetResource());
		ChangeState(m_PrevState);
		otherTex->ChangeState(otherTex->m_PrevState);
		context->FinalizeResourceBarriers();
	}

	void DirectX12RenderTexture::UpdateSize(unsigned int width, unsigned height) {
		m_Metadata.width = width;
		m_Metadata.height = height;
		m_TextureResource = Scope<DirectX12TextureResource>(new DirectX12TextureResource(m_Metadata, TextureType::Tex2D, TextureMisc::RT));
		GenerateViews();
	}

	void DirectX12RenderTexture::ClearTexture()
	{
		auto context = DirectX12Core::GetCore().GetDirectCommandContext();
		ChangeState(RTState::WRITE);
		context->GetCommandList()->ClearRenderTargetView(m_RTDescriptor, m_Metadata.clearColor, 0, nullptr);
	}

	void DirectX12RenderTexture::CopyTexture(Ref<Texture2D> tex)
	{
		// Need to make sure all queued draws are completed before copying
		auto context = DirectX12Core::GetCore().GetDirectCommandContext();
		context->RequestWait(GameSmith::DirectX12QueueType::Direct);
		context->SubmitCommandLists();

		auto& list = context->GetCommandList();
		ChangeState(RTState::COPYSRC);
		auto d3Tex = CastPtr<DirectX12Texture2D>(tex);
		d3Tex->m_Resource->GetStateTracker().TransitionBarrier(D3D12_RESOURCE_STATE_COPY_DEST, context);
		context->FinalizeResourceBarriers();
		list->CopyResource(d3Tex->m_Resource->GetResource(), m_TextureResource->GetResource());
		d3Tex->m_Resource->GetStateTracker().UndoTransition(context);
		ChangeState(m_PrevState);
		context->FinalizeResourceBarriers();
		context->SubmitCommandLists();
	}

	void DirectX12RenderTexture::CopyTexture(Ref<RenderTexture> tex)
	{
		// Need to make sure all queued draws are completed before copying
		auto context = DirectX12Core::GetCore().GetDirectCommandContext();
		context->RequestWait(GameSmith::DirectX12QueueType::Direct);
		context->SubmitCommandLists();

		auto& list = context->GetCommandList();
		ChangeState(RTState::COPYSRC);
		auto d3Tex = CastPtr<DirectX12RenderTexture>(tex);
		d3Tex->ChangeState(RTState::COPYDST);
		context->FinalizeResourceBarriers();
		list->CopyResource(d3Tex->m_TextureResource->GetResource(), m_TextureResource->GetResource());
		ChangeState(m_PrevState);
		d3Tex->ChangeState(d3Tex->m_PrevState);
		context->FinalizeResourceBarriers();
		context->SubmitCommandLists();
	}

	void DirectX12RenderTexture::GenerateViews()
	{
		auto& core = DirectX12Core::GetCore();
		auto device = core.GetDevice();
		device->CreateShaderResourceView(m_TextureResource->GetResource(), nullptr, m_SRVDescriptor);
		device->CreateRenderTargetView(m_TextureResource->GetResource(), nullptr, m_RTDescriptor);
	}
};

