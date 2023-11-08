#pragma once

#include "GameSmithEngine/Rendering/RenderAgnostics/Shaders/ShaderParameter.h"

namespace GameSmith {
	// A definition that can be used to define a piece or all of vertex data
	struct BufferElement {
		std::string SemanticName;
		ShaderDataType Type;
	};

	// A class that is used to define vertex data before translating into a Render API format
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
