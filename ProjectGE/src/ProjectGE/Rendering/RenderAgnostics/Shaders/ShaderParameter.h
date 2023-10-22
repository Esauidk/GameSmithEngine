#pragma once
#include <gepch.h>
#include <glm/glm.hpp>
#include "ProjectGE/Core/Log.h"

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
			return sizeof(glm::mat4);
		case ShaderDataType::Float4:
			return sizeof(glm::vec4);
		case ShaderDataType::Float:
			return sizeof(glm::vec1);
		case ShaderDataType::Float2:
			return sizeof(glm::vec2);
		case ShaderDataType::Float3:
			return sizeof(glm::vec3);
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
		virtual char* GetCharData() = 0;
		virtual void ResetData() = 0;
		virtual Ref<ShaderParameter> MakeCopy() = 0;
		
	private:
		std::string m_Name;
		ShaderDataType m_Type;
	};

	class ShaderParameterInt : public ShaderParameter {
	public:
		ShaderParameterInt(std::string name) : ShaderParameter(name, ShaderDataType::Int), m_Data(0)  {}
		ShaderParameterInt(std::string name, int data) : ShaderParameter(name, ShaderDataType::Int), m_Data(data) {}
		inline const int& GetData() { return m_Data; }
		inline void SetData(int updated) { m_Data = updated; }
		inline virtual char* GetCharData() override { return (char*)&m_Data; };
		inline virtual void ResetData() override {}
		inline virtual Ref<ShaderParameter> MakeCopy() override { return Ref<ShaderParameter>(new ShaderParameterInt(GetName(), m_Data)); }
	private:
		int m_Data;
	};

	class ShaderParameterInt2 : public ShaderParameter {
	public:
		ShaderParameterInt2(std::string name) : ShaderParameter(name, ShaderDataType::Int2), m_Data() {}
		ShaderParameterInt2(std::string name, int* data) : ShaderParameter(name, ShaderDataType::Int2) { memcpy(m_Data, data, sizeof(int) * 2); }
		virtual char* GetCharData() override { return (char*)&m_Data; };
		inline const int* GetData() { return m_Data; }
		inline void SetData(int* updated) { memcpy(m_Data, updated, GetSize()); }
		inline virtual void ResetData() override {}
		inline virtual Ref<ShaderParameter> MakeCopy() override { return Ref<ShaderParameter>(new ShaderParameterInt2(GetName(), m_Data)); }
	private:
		int m_Data[2];
	};

	class ShaderParameterInt3 : public ShaderParameter {
	public:
		ShaderParameterInt3(std::string name) : ShaderParameter(name, ShaderDataType::Int3), m_Data() {}
		ShaderParameterInt3(std::string name, int* data) : ShaderParameter(name, ShaderDataType::Int3) { memcpy(m_Data, data, sizeof(int) * 3); }
		inline virtual char* GetCharData() override { return (char*)&m_Data; };
		inline const int* GetData() { return m_Data; }
		inline void SetData(int* updated) { memcpy(m_Data, updated, GetSize()); }
		inline virtual void ResetData() override {}
		inline virtual Ref<ShaderParameter> MakeCopy() override { return Ref<ShaderParameter>(new ShaderParameterInt3(GetName(), m_Data)); }
	private:
		 int m_Data[3];
	};

	class ShaderParameterFloat : public ShaderParameter {
	public:
		ShaderParameterFloat(std::string name) : ShaderParameter(name, ShaderDataType::Float), m_Data(0) {}
		ShaderParameterFloat(std::string name, glm::vec1& data) : ShaderParameter(name, ShaderDataType::Float), m_Data(data) {}
		inline virtual char* GetCharData() override { return (char*)&m_Data; };
		inline const glm::vec1& GetData() { return m_Data; }
		inline void SetData(glm::vec1& updated) { m_Data = updated; }
		inline virtual void ResetData() override {}
		inline virtual Ref<ShaderParameter> MakeCopy() override { return Ref<ShaderParameter>(new ShaderParameterFloat(GetName(), m_Data)); }
	private:
		glm::vec1 m_Data;
	};

	class ShaderParameterFloat2 : public ShaderParameter {
	public:
		ShaderParameterFloat2(std::string name) : ShaderParameter(name, ShaderDataType::Float2), m_Data(0, 0) {}
		ShaderParameterFloat2(std::string name, glm::vec2& data) : ShaderParameter(name, ShaderDataType::Float2), m_Data(data) {}
		inline virtual char* GetCharData() override { return (char*)&m_Data; };
		inline const glm::vec2& GetData() { return m_Data; }
		inline void SetData(glm::vec2& updated) { m_Data = updated; }
		inline virtual void ResetData() override {}
		inline virtual Ref<ShaderParameter> MakeCopy() override { return Ref<ShaderParameter>(new ShaderParameterFloat2(GetName(), m_Data)); }
	private:
		glm::vec2 m_Data;
	};

	class ShaderParameterFloat3 : public ShaderParameter {
	public:
		ShaderParameterFloat3(std::string name) : ShaderParameter(name, ShaderDataType::Float3), m_Data(0, 0, 0) {}
		ShaderParameterFloat3(std::string name, glm::vec3& data) : ShaderParameter(name, ShaderDataType::Float3), m_Data(data) {}
		inline virtual char* GetCharData() override { return (char*)&m_Data; };
		inline const glm::vec3& GetData() { return m_Data; }
		inline void SetData(glm::vec3& updated) { m_Data = updated; }
		inline virtual void ResetData() override {}
		inline virtual Ref<ShaderParameter> MakeCopy() override { return Ref<ShaderParameter>(new ShaderParameterFloat3(GetName(), m_Data)); }
	private:
		glm::vec3 m_Data;
	};

	class ShaderParameterFloat4 : public ShaderParameter {
	public:
		ShaderParameterFloat4(std::string name) : ShaderParameter(name, ShaderDataType::Float4), m_Data(0, 0, 0, 0) {}
		ShaderParameterFloat4(std::string name, glm::vec4& data) : ShaderParameter(name, ShaderDataType::Float4), m_Data(data) {}
		inline virtual char* GetCharData() override { return (char*)&m_Data; };
		inline const glm::vec4& GetData() { return m_Data; }
		inline void SetData(glm::vec4& updated) { m_Data = updated; }
		inline virtual void ResetData() override {}
		inline virtual Ref<ShaderParameter> MakeCopy() override { return Ref<ShaderParameter>(new ShaderParameterFloat4(GetName(), m_Data)); }
	private:
		glm::vec4 m_Data;
	};

	class ShaderParameterMatrix : public ShaderParameter {
	public:
		ShaderParameterMatrix(std::string name) : ShaderParameter(name, ShaderDataType::Matrix), m_Data(0) {}
		ShaderParameterMatrix(std::string name, glm::mat4& data) : ShaderParameter(name, ShaderDataType::Matrix), m_Data(data) {}
		inline virtual char* GetCharData() override { return (char*)&m_Data; };
		inline const glm::mat4& GetData() { return m_Data; }
		inline void SetData(glm::mat4& updated) { m_Data = updated; }
		inline virtual void ResetData() override {}
		inline virtual Ref<ShaderParameter> MakeCopy() override { return Ref<ShaderParameter>(new ShaderParameterMatrix(GetName(), m_Data)); }
	private:
		glm::mat4 m_Data;
	};

	static Ref<ShaderParameter> ConvertToParameter(std::string name, ShaderDataType type, char* data) {
		switch (type) {
		case ShaderDataType::Matrix:
			return Ref<ShaderParameter>(new ShaderParameterMatrix(name, *((glm::mat4*)data)));
		case ShaderDataType::Float4:
			return Ref<ShaderParameter>(new ShaderParameterFloat4(name, *((glm::vec4*)data)));
		case ShaderDataType::Float:
			return Ref<ShaderParameter>(new ShaderParameterFloat(name, *((glm::vec1*)data)));
		case ShaderDataType::Float2:
			return Ref<ShaderParameter>(new ShaderParameterFloat2(name, *((glm::vec2*)data)));
		case ShaderDataType::Float3:
			return Ref<ShaderParameter>(new ShaderParameterFloat3(name, *((glm::vec3*)data)));
		case ShaderDataType::Int:
			return Ref<ShaderParameter>(new ShaderParameterInt(name, *((int*)data)));
		case ShaderDataType::Int2:
			return Ref<ShaderParameter>(new ShaderParameterInt2(name, (int*)data));
		case ShaderDataType::Int3:
			return Ref<ShaderParameter>(new ShaderParameterInt3(name, (int*)data));
		default:
			return 0;
		}
	}
};


