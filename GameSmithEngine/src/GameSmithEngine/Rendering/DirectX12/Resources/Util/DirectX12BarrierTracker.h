#pragma once
enum D3D12_RESOURCE_STATES;
struct ID3D12Resource;

namespace GameSmith {
	class DirectX12CommandContextBase;


	class DirectX12BarrierTracker
	{
	public:
		DirectX12BarrierTracker() = default;
		void Init(D3D12_RESOURCE_STATES initialState, ID3D12Resource* resource);
		void AssumeState(D3D12_RESOURCE_STATES newCurrentState) { m_LastState = m_CurrentState;  m_CurrentState = newCurrentState; }

		void TransitionBarrier(D3D12_RESOURCE_STATES nextState, DirectX12CommandContextBase* context);
		void UndoTransition(DirectX12CommandContextBase* context);
		inline D3D12_RESOURCE_STATES GetState() const { return m_CurrentState; };
	private:
		D3D12_RESOURCE_STATES m_CurrentState;
		D3D12_RESOURCE_STATES m_LastState;
		ID3D12Resource* m_Resource;
		bool m_IsInit;
	};
};


