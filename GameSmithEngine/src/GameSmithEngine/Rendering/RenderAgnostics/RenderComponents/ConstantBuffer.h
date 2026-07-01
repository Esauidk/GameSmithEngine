#pragma once

namespace GameSmith
{
	enum class UpdateFrequency {
		Never,
		OncePerFrame,
		OncePerDrawCall
	};
	class ConstantBuffer {
	public:
		virtual ~ConstantBuffer() = default;
		virtual void UpdateData(BYTE* data, UINT byteSize) = 0;
		virtual unsigned int GetBufferSize() = 0;
	};
}
