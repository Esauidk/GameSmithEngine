#pragma once

#include "GameSmithEngine/SerializeableFiles/Serializable.h"
#include "nlohmann/json.hpp"

#define JSON_INDENTATION_SPACES 4

using json = nlohmann::json;
namespace GameSmith {
	class JsonFile : public AbstractBaseSerializeable {
	public:
		JsonFile() : m_JsonData({}) {
			m_JsonString = m_JsonData.dump(JSON_INDENTATION_SPACES);
		};

		virtual ~JsonFile() = default;

		virtual Ref<char> Serialize() override;
		virtual void Serialize(char* byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequiredSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;
	protected:
		void UpdateJson(const json& newJson);
		virtual void ExtractJsonData(const json& newJson) = 0;
	private:
		json m_JsonData;
		std::string m_JsonString;
	};
};
