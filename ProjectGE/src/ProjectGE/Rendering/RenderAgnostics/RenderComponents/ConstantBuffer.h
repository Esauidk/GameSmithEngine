#pragma once

namespace ProjectGE {
	class ConstantBuffer {
	public:
		virtual ~ConstantBuffer() = default;
		virtual void UpdateData(BYTE* data, UINT byteSize) = 0;
	};
}
