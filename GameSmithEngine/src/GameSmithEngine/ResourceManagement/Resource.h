#pragma once
namespace GameSmith {
	class Resource {
	public:
		inline void AttachResource(char* data, UINT size) { m_Data = data; m_Size = size; };
		char* GetResourceData() const { return m_Data; }
		UINT GetResourceSize() const { return m_Size; }

		virtual void Init() = 0;
		virtual void Destroy() = 0;
	private:
		char* m_Data = nullptr;
		UINT m_Size = 0;
	};
};
