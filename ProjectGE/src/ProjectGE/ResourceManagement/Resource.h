#pragma once
namespace ProjectGE {
	class Resource {
	public:
		Resource(char* data, size_t size);
		char* GetResourceData() { return m_Data; }
		size_t GetResourceSize() { return m_Size; }

		virtual void Init() = 0;
		virtual void Destroy() = 0;
	private:
		char* m_Data;
		size_t m_Size;
	};
};
