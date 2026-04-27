#pragma once
#include <unordered_map>
#include <string>
#include "GameSmithEngine/Utilities/ParameterContainer.h"
#include "GameSmithEngine/SerializeableFiles/GenericFileTypes/JsonFile.h"

namespace GameSmith {
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

	NLOHMANN_JSON_SERIALIZE_ENUM(ShaderDataType, {
		{ShaderDataType::None, "None"},
		{ShaderDataType::Float, "Float"},
		{ShaderDataType::Float2, "Float2"},
		{ShaderDataType::Float3, "Float3"},
		{ShaderDataType::Float4, "Float4"},
		{ShaderDataType::Int, "Int"},
		{ShaderDataType::Int2, "Int2"},
		{ShaderDataType::Int3, "Int3"},
		{ShaderDataType::Int4, "Int4"},
		{ShaderDataType::Bool, "Bool"},
		{ShaderDataType::Matrix, "Matrix"}
	})

	class ShaderMetadata : public JsonFile {
	public:
		ShaderMetadata() = default;
		void ExtractJsonData(const json& newJson) override;
	private:
		void RefreshConfig();
	private:
		std::unordered_map<std::string, ShaderDataType> m_Properties;
	};
};