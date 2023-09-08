#pragma once
#include <gepch.h>
#include "ProjectGE/Core/Core.h"

namespace ProjectGE {
	enum class GroupType {
		CBUFFER,
		TEXTURE
	};

	enum class ParameterType {
		MATRIX,
		FLOAT,
		FLOAT2,
		FLOAT3,
		FLOAT4,
		INT
	};

	class ShaderParameter {
	public:
		friend class ShaderParameterGroupBuilder;

		inline std::string GetName() { return m_Name; }
		inline ParameterType GetType() { return m_Type; }
	private:
		inline ShaderParameter(std::string& name, ParameterType& type) : m_Name(name), m_Type(type) {}
	private:
		std::string m_Name;
		ParameterType m_Type;
	};

	class ShaderParameterGroup
	{
	public:
		friend class ShaderParameterGroupBuilder;

		inline std::string GetGroupName() { return m_GroupName; }
		inline GroupType GetGroupType() { return m_GroupType; }
		inline UINT GetRegisterSlot() { return m_Slot; }
	private:
		inline void SetGroupName(std::string& name) { m_GroupName = name; }
		inline void SetGroupType(GroupType& type) { m_GroupType = type; }
		inline void SetSlot(UINT slot) { m_Slot = slot; }
		inline void AddShaderParameter(ShaderParameter& parameter) { m_Parameters.emplace_back(parameter); }
	private:
		std::string m_GroupName;
		UINT m_Slot;
		GroupType m_GroupType;
		std::vector<ShaderParameter> m_Parameters;
	};

	class ShaderParameterGroupBuilder {
	public:
		ShaderParameterGroupBuilder();
		void AddParameter(std::string& name, ParameterType& type);
		void SetGroupName(std::string& name) { m_Group->SetGroupName(name); }
		void SetRegisterSlot(UINT slot) { m_Group->SetSlot(slot); };
		void SetGroupType(GroupType type) { m_Group->SetGroupType(type); };
		Ref<ShaderParameterGroup> GetGroup() { return m_Group; }
	private:
		Ref<ShaderParameterGroup> m_Group;
	};
};


