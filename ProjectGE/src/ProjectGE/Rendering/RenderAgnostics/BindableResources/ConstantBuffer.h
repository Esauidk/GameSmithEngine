#pragma once

namespace ProjectGE {
	class ConstantBuffer {
	public:
		virtual void UpdateData(BYTE* data) = 0;
		virtual void* GetGPUReference() = 0;

		static ConstantBuffer* Create(void* initialData, UINT size);
		static ConstantBuffer* Create(UINT size);
	};
}
