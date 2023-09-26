#include "gepch.h"
#include "DirectX12InputLayout.h"

#include "ProjectGE/Core/Log.h"

namespace ProjectGE {
	DirectX12InputLayout::DirectX12InputLayout(const BufferLayoutBuilder& layout) {
		m_Desc = Scope<D3D12_INPUT_ELEMENT_DESC[]>(new D3D12_INPUT_ELEMENT_DESC[layout.GetElementCount()]);
		UINT index = 0;
		for (const auto& element : layout) {
			switch (element.Type) {
			case ShaderDataType::Float:
				m_Desc[index] = { element.SemanticName.c_str(), 0, DXGI_FORMAT_R32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
				break;
			case ShaderDataType::Float2:
				m_Desc[index] = { element.SemanticName.c_str(), 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
				break;
			case ShaderDataType::Float3:
				m_Desc[index] = { element.SemanticName.c_str(), 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
				break;
			case ShaderDataType::Float4:
				m_Desc[index] = { element.SemanticName.c_str(), 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
				break;
			case ShaderDataType::Int:
				m_Desc[index] = { element.SemanticName.c_str(), 0, DXGI_FORMAT_R32_SINT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
				break;
			case ShaderDataType::Int2:
				m_Desc[index] = { element.SemanticName.c_str(), 0, DXGI_FORMAT_R32G32_SINT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
				break;
			case ShaderDataType::Int3:
				m_Desc[index] = { element.SemanticName.c_str(), 0, DXGI_FORMAT_R32G32B32_SINT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
				break;
			case ShaderDataType::Int4:
				m_Desc[index] = { element.SemanticName.c_str(), 0, DXGI_FORMAT_R32G32B32_SINT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
				break;
			case ShaderDataType::Bool:
				m_Desc[index] = { element.SemanticName.c_str(), 0, DXGI_FORMAT_R8_TYPELESS, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
				break;
			default:
				GE_CORE_ASSERT(false, "DataType not supported yet");
			}
			index++;
		}

		m_InputLayout = { m_Desc.get(), layout.GetElementCount() };
	}

};
