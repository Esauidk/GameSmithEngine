#include "gepch.h"
#include "ShaderParameterGroup.h"

namespace ProjectGE {
	ShaderParameterGroupBuilder::ShaderParameterGroupBuilder()
	{
		m_Group = std::make_shared<ShaderParameterGroup>();
		std::string defaultName = "Default Group";
		GroupType defaultType = GroupType::CBUFFER;
		m_Group->SetGroupName(defaultName);
		m_Group->SetGroupType(defaultType);
		m_Group->SetSlot(0);

	}

	void ShaderParameterGroupBuilder::AddParameter(std::string& name, ParameterType& type)
	{
		ShaderParameter parm(name, type);
		m_Group->AddShaderParameter(parm);
	}
};
