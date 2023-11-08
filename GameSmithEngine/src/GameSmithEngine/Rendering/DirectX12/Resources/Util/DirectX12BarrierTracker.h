#pragma once
enum D3D12_RESOURCE_STATES;
struct ID3D12Resource;

namespace GameSmith {
	

	class DirectX12BarrierTracker
	{
	public:
		DirectX12BarrierTracker() = default;
		void Init(D3D12_RESOURCE_STATES initialState, ID3D12Resource* resource);

		void TransitionBarrier(D3D12_RESOURCE_STATES nextState);
		void UndoTransition(ID3D12Resource* resource);
		inline D3D12_RESOURCE_STATES GetState() const { return m_CurrentState; };
	private:
		D3D12_RESOURCE_STATES m_CurrentState;
		D3D12_RESOURCE_STATES m_LastState;
		ID3D12Resource* m_Resource;
		bool m_IsInit;
	};
};


