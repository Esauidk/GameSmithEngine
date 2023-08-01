#pragma once

namespace ProjectGE {
	enum class ShaderDataType {
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool
	};

	struct BufferElement {
		std::string SemanticName;
		ShaderDataType Type;
	};

	class BufferLayoutBuilder {
	public:
		BufferLayoutBuilder(const std::initializer_list<BufferElement>& list) : m_Elements(list) {}
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		inline UINT GetElementCount() const { return (UINT)m_Elements.size(); }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		std::vector<BufferElement> m_Elements;
	};

};