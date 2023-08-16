#pragma once

#include "ProjectGE/Core/Core.h"
namespace ProjectGE {
	class ConstantBuffer {
	public:
		virtual ~ConstantBuffer() = default;
		virtual void UpdateData(BYTE* data) = 0;
		virtual void* GetGPUReference() = 0;

		static Ref<ConstantBuffer> Create(void* initialData, UINT size);
		static Ref<ConstantBuffer> Create(UINT size);
	};
}
