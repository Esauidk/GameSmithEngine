#pragma once
#include <gepch.h>

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
		Bool,
		Matrix
	};

	static UINT GetParameterSize(ShaderDataType type) {
		switch (type) {
		case ShaderDataType::Matrix:
		case ShaderDataType::Float4:
			return sizeof(float) * 4;
		case ShaderDataType::Float:
			return sizeof(float);
		case ShaderDataType::Float2:
			return sizeof(float) * 2;
		case ShaderDataType::Float3:
			return sizeof(float) * 3;
		case ShaderDataType::Int:
			return sizeof(int);
		case ShaderDataType::Int2:
			return sizeof(int) * 2;
		case ShaderDataType::Int3:
			return sizeof(int) * 3;
		}
	}

	class ShaderParameter {
	public:
		inline ShaderParameter(std::string name, ShaderDataType type) : m_Name(name), m_Type(type) {}

		inline std::string GetName() { return m_Name; }
		inline ShaderDataType GetType() { return m_Type; }
		inline UINT GetSize() { return GetParameterSize(m_Type); }
		
	private:
		std::string m_Name;
		ShaderDataType m_Type;
	};
};


