#pragma once
#include <gepch.h>
#include <glm/glm.hpp>

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
		default:
			return 0;
		}
	}

	class ShaderParameter {
	public:
		inline ShaderParameter(std::string name, ShaderDataType type) : m_Name(name), m_Type(type) {}

		inline std::string GetName() { return m_Name; }
		inline ShaderDataType GetType() { return m_Type; }
		inline UINT GetSize() { return GetParameterSize(m_Type); }
		
		virtual void* GetData() = 0;
		
	private:
		std::string m_Name;
		ShaderDataType m_Type;
	};

	class ShaderParameterInt : public ShaderParameter {
	public:
		ShaderParameterInt(std::string name) : ShaderParameter(name, ShaderDataType::Int), m_Data(0)  {}
		inline virtual void* GetData() override { return &m_Data; }
	private:
		int m_Data;
	};

	class ShaderParameterInt2 : public ShaderParameter {
	public:
		ShaderParameterInt2(std::string name) : ShaderParameter(name, ShaderDataType::Int2), m_Data() {}
		inline virtual void* GetData() override { return &m_Data; }
	private:
		int m_Data[2];
	};

	class ShaderParameterInt3 : public ShaderParameter {
	public:
		ShaderParameterInt3(std::string name) : ShaderParameter(name, ShaderDataType::Int3), m_Data() {}
		inline virtual void* GetData() override { return &m_Data; }
	private:
		 int m_Data[3];
	};

	class ShaderParameterFloat : public ShaderParameter {
	public:
		ShaderParameterFloat(std::string name) : ShaderParameter(name, ShaderDataType::Float), m_Data(0) {}
		inline virtual void* GetData() override { return &m_Data; }
	private:
		glm::vec1 m_Data;
	};

	class ShaderParameterFloat2 : public ShaderParameter {
	public:
		ShaderParameterFloat2(std::string name) : ShaderParameter(name, ShaderDataType::Float2), m_Data(0, 0) {}
		inline virtual void* GetData() override { return &m_Data; }
	private:
		glm::vec2 m_Data;
	};

	class ShaderParameterFloat3 : public ShaderParameter {
	public:
		ShaderParameterFloat3(std::string name) : ShaderParameter(name, ShaderDataType::Float3), m_Data(0, 0, 0) {}
		inline virtual void* GetData() override { return &m_Data; }
	private:
		glm::vec3 m_Data;
	};

	class ShaderParameterFloat4 : public ShaderParameter {
	public:
		ShaderParameterFloat4(std::string name) : ShaderParameter(name, ShaderDataType::Float4), m_Data(0, 0, 0, 0) {}
		inline virtual void* GetData() override { return &m_Data; }
	private:
		glm::vec4 m_Data;
	};

	class ShaderParameterMatrix : public ShaderParameter {
	public:
		ShaderParameterMatrix(std::string name) : ShaderParameter(name, ShaderDataType::Matrix), m_Data(0) {}
		inline virtual void* GetData() override { return &m_Data; }
	private:
		glm::mat4 m_Data;
	};
};


