#pragma once
#include <gepch.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GameSmithEngine/Core/Log.h"

namespace GameSmith {
	enum class ContainerDataType {
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

	static UINT GetParameterSize(ContainerDataType type) {
		switch (type) {
		case ContainerDataType::Matrix:
			return sizeof(glm::mat4);
		case ContainerDataType::Float4:
			return sizeof(glm::vec4);
		case ContainerDataType::Float:
			return sizeof(glm::vec1);
		case ContainerDataType::Float2:
			return sizeof(glm::vec2);
		case ContainerDataType::Float3:
			return sizeof(glm::vec3);
		case ContainerDataType::Int:
			return sizeof(int);
		case ContainerDataType::Int2:
			return sizeof(int) * 2;
		case ContainerDataType::Int3:
			return sizeof(int) * 3;
		default:
			return 0;
		}
	}

	class ParameterContainer {
	public:
		inline ParameterContainer(std::string name, ContainerDataType type, unsigned int flags = 0) : m_Name(name), m_Type(type), m_Flags(flags) {}

		inline std::string GetName() { return m_Name; }
		inline ContainerDataType GetType() { return m_Type; }
		inline unsigned int GetFlags() { return m_Flags; }
		inline UINT GetSize() { return GetParameterSize(m_Type); }
		virtual char* GetCharData() = 0;
		virtual void ResetData() = 0;
		virtual Ref<ParameterContainer> MakeCopy() = 0;
		
	private:
		std::string m_Name;
		unsigned int m_Flags;
		ContainerDataType m_Type;
	};

	class IntContainer : public ParameterContainer {
	public:
		IntContainer(std::string name, unsigned int flags = 0) : ParameterContainer(name, ContainerDataType::Int, flags), m_Data(0)  {}
		IntContainer(std::string name, int data, unsigned int flags = 0) : ParameterContainer(name, ContainerDataType::Int, flags), m_Data(data) {}
		inline const int& GetData() { return m_Data; }
		inline void SetData(int updated) { m_Data = updated; }
		inline virtual char* GetCharData() override { return (char*)&m_Data; };
		inline virtual void ResetData() override {}
		inline virtual Ref<ParameterContainer> MakeCopy() override { return Ref<ParameterContainer>(new IntContainer(GetName(), m_Data, GetFlags())); }
	private:
		int m_Data;
	};

	class Int2Container : public ParameterContainer {
	public:
		Int2Container(std::string name, unsigned int flags = 0) : ParameterContainer(name, ContainerDataType::Int2, flags), m_Data() {}
		Int2Container(std::string name, int* data, unsigned int flags = 0) : ParameterContainer(name, ContainerDataType::Int2, flags) { memcpy(m_Data, data, sizeof(int) * 2); }
		virtual char* GetCharData() override { return (char*)&m_Data; };
		inline const int* GetData() { return m_Data; }
		inline void SetData(int* updated) { memcpy(m_Data, updated, GetSize()); }
		inline virtual void ResetData() override {}
		inline virtual Ref<ParameterContainer> MakeCopy() override { return Ref<ParameterContainer>(new Int2Container(GetName(), m_Data, GetFlags())); }
	private:
		int m_Data[2];
	};

	class Int3Container : public ParameterContainer {
	public:
		Int3Container(std::string name, unsigned int flags = 0) : ParameterContainer(name, ContainerDataType::Int3, flags), m_Data() {}
		Int3Container(std::string name, int* data, unsigned int flags = 0) : ParameterContainer(name, ContainerDataType::Int3, flags) { memcpy(m_Data, data, sizeof(int) * 3); }
		inline virtual char* GetCharData() override { return (char*)&m_Data; };
		inline const int* GetData() { return m_Data; }
		inline void SetData(int* updated) { memcpy(m_Data, updated, GetSize()); }
		inline virtual void ResetData() override {}
		inline virtual Ref<ParameterContainer> MakeCopy() override { return Ref<ParameterContainer>(new Int3Container(GetName(), m_Data, GetFlags())); }
	private:
		 int m_Data[3];
	};

	class Int4Container : public ParameterContainer {
	public:
		Int4Container(std::string name, unsigned int flags = 0) : ParameterContainer(name, ContainerDataType::Int4, flags), m_Data() {}
		Int4Container(std::string name, int* data, unsigned int flags = 0) : ParameterContainer(name, ContainerDataType::Int4, flags) { memcpy(m_Data, data, sizeof(int) * 4); }
		inline virtual char* GetCharData() override { return (char*)&m_Data; };
		inline const int* GetData() { return m_Data; }
		inline void SetData(int* updated) { memcpy(m_Data, updated, GetSize()); }
		inline virtual void ResetData() override {}
		inline virtual Ref<ParameterContainer> MakeCopy() override { return Ref<ParameterContainer>(new Int3Container(GetName(), m_Data, GetFlags())); }
	private:
		int m_Data[4];
	};

	class FloatContainer : public ParameterContainer {
	public:
		FloatContainer(std::string name, unsigned int flags = 0) : ParameterContainer(name, ContainerDataType::Float, flags), m_Data(0) {}
		FloatContainer(std::string name, glm::vec1& data, unsigned int flags = 0) : ParameterContainer(name, ContainerDataType::Float, flags), m_Data(data) {}
		inline virtual char* GetCharData() override { return (char*)&m_Data; };
		inline const glm::vec1& GetData() { return m_Data; }
		inline void SetData(glm::vec1& updated) { m_Data = updated; }
		inline virtual void ResetData() override {}
		inline virtual Ref<ParameterContainer> MakeCopy() override { return Ref<ParameterContainer>(new FloatContainer(GetName(), m_Data, GetFlags())); }
	private:
		glm::vec1 m_Data;
	};

	class Float2Container : public ParameterContainer {
	public:
		Float2Container(std::string name, unsigned int flags = 0) : ParameterContainer(name, ContainerDataType::Float2, flags), m_Data(0, 0) {}
		Float2Container(std::string name, glm::vec2& data, unsigned int flags = 0) : ParameterContainer(name, ContainerDataType::Float2, flags), m_Data(data) {}
		inline virtual char* GetCharData() override { return (char*)&m_Data; };
		inline const glm::vec2& GetData() { return m_Data; }
		inline void SetData(glm::vec2& updated) { m_Data = updated; }
		inline virtual void ResetData() override {}
		inline virtual Ref<ParameterContainer> MakeCopy() override { return Ref<ParameterContainer>(new Float2Container(GetName(), m_Data, GetFlags())); }
	private:
		glm::vec2 m_Data;
	};

	class Float3Container : public ParameterContainer {
	public:
		Float3Container(std::string name, unsigned int flags = 0) : ParameterContainer(name, ContainerDataType::Float3, flags), m_Data(0, 0, 0) {}
		Float3Container(std::string name, glm::vec3& data, unsigned int flags = 0) : ParameterContainer(name, ContainerDataType::Float3, flags), m_Data(data) {}
		inline virtual char* GetCharData() override { return (char*)&m_Data; };
		inline const glm::vec3& GetData() { return m_Data; }
		inline void SetData(glm::vec3& updated) { m_Data = updated; }
		inline virtual void ResetData() override {}
		inline virtual Ref<ParameterContainer> MakeCopy() override { return Ref<ParameterContainer>(new Float3Container(GetName(), m_Data, GetFlags())); }
	private:
		glm::vec3 m_Data;
	};

	class Float4Containor : public ParameterContainer {
	public:
		Float4Containor(std::string name, unsigned int flags = 0) : ParameterContainer(name, ContainerDataType::Float4, flags), m_Data(0, 0, 0, 0) {}
		Float4Containor(std::string name, glm::vec4& data, unsigned int flags = 0) : ParameterContainer(name, ContainerDataType::Float4, flags), m_Data(data) {}
		inline virtual char* GetCharData() override { return (char*)&m_Data; };
		inline const glm::vec4& GetData() { return m_Data; }
		inline void SetData(glm::vec4& updated) { m_Data = updated; }
		inline virtual void ResetData() override {}
		inline virtual Ref<ParameterContainer> MakeCopy() override { return Ref<ParameterContainer>(new Float4Containor(GetName(), m_Data, GetFlags())); }
	private:
		glm::vec4 m_Data;
	};

	class MatrixContainor : public ParameterContainer {
	public:
		MatrixContainor(std::string name, unsigned int flags = 0) : ParameterContainer(name, ContainerDataType::Matrix, flags), m_Data(1) {}
		MatrixContainor(std::string name, glm::mat4& data, unsigned int flags = 0) : ParameterContainer(name, ContainerDataType::Matrix), m_Data(data) {}
		inline virtual char* GetCharData() override { return (char*)glm::value_ptr(m_Data); };
		inline const glm::mat4& GetData() { return m_Data; }
		inline void SetData(glm::mat4& updated) { m_Data = updated; }
		inline virtual void ResetData() override {}
		inline virtual Ref<ParameterContainer> MakeCopy() override { return Ref<ParameterContainer>(new MatrixContainor(GetName(), m_Data, GetFlags())); }
	private:
		glm::mat4 m_Data;
	};

	static Ref<ParameterContainer> ConvertToParameter(std::string name, ContainerDataType type, char* data, unsigned int flags = 0) {
		switch (type) {
		case ContainerDataType::Matrix:
			return Ref<ParameterContainer>(new MatrixContainor(name, *((glm::mat4*)data), flags));
		case ContainerDataType::Float4:
			return Ref<ParameterContainer>(new Float4Containor(name, *((glm::vec4*)data), flags));
		case ContainerDataType::Float:
			return Ref<ParameterContainer>(new FloatContainer(name, *((glm::vec1*)data), flags));
		case ContainerDataType::Float2:
			return Ref<ParameterContainer>(new Float2Container(name, *((glm::vec2*)data), flags));
		case ContainerDataType::Float3:
			return Ref<ParameterContainer>(new Float3Container(name, *((glm::vec3*)data), flags));
		case ContainerDataType::Int:
			return Ref<ParameterContainer>(new IntContainer(name, *((int*)data), flags));
		case ContainerDataType::Int2:
			return Ref<ParameterContainer>(new Int2Container(name, (int*)data, flags));
		case ContainerDataType::Int3:
			return Ref<ParameterContainer>(new Int3Container(name, (int*)data, flags));
		case ContainerDataType::Int4:
			return Ref<ParameterContainer>(new Int4Container(name, (int*)data, flags));
		default:
			return 0;
		}
	}

	static Ref<ParameterContainer> CreateContainer(std::string name, ContainerDataType type, unsigned int flags = 0) {
		switch (type) {
		case ContainerDataType::Matrix:
			return Ref<ParameterContainer>(new MatrixContainor(name, flags));
		case ContainerDataType::Float4:
			return Ref<ParameterContainer>(new Float4Containor(name, flags));
		case ContainerDataType::Float:
			return Ref<ParameterContainer>(new FloatContainer(name, flags));
		case ContainerDataType::Float2:
			return Ref<ParameterContainer>(new Float2Container(name, flags));
		case ContainerDataType::Float3:
			return Ref<ParameterContainer>(new Float3Container(name, flags));
		case ContainerDataType::Int:
			return Ref<ParameterContainer>(new IntContainer(name, flags));
		case ContainerDataType::Int2:
			return Ref<ParameterContainer>(new Int2Container(name, flags));
		case ContainerDataType::Int3:
			return Ref<ParameterContainer>(new Int3Container(name, flags));
		case ContainerDataType::Int4:
			return Ref<ParameterContainer>(new Int4Container(name, flags));
		default:
			return 0;
		}
	}
};


