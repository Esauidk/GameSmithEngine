#pragma once
namespace ProjectGE {
	class Resource {
	public:
		inline void AttachResource(char* data, UINT size) { m_Data = data; m_Size = size; };
		char* GetResourceData() { return m_Data; }
		UINT GetResourceSize() { return m_Size; }

		virtual void Init() = 0;
		virtual void Destroy() = 0;
	private:
		char* m_Data;
		UINT m_Size;
	};
};
