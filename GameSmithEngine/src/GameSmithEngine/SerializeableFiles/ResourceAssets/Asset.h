#pragma once

#include "GameSmithEngine/SerializeableFiles/Serializable.h"

namespace GameSmith {
	// A serializeable object that is defined to be an 
	// 
	// 
	// 
	// 
	// (either single serializable or bundled serializables)
	class GE_API Asset : public Serializeable {
	public:
		Asset(std::string fileName) : m_FileName(fileName) {}
		virtual ~Asset() = default;

		const std::string& GetName() const { return m_FileName; }
	private:
		const std::string m_FileName;
	};
};
