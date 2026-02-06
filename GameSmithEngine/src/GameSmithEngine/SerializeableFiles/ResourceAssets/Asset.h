#pragma once

#include "GameSmithEngine/SerializeableFiles/ExposedMemberSerializeable.h"

#define SERIAL_FILE(FILE_TYPE, FILE_EXT) static std::string GetStaticFileExtension() {return #FILE_EXT;} \
										static std::string GetStaticFileType() {return #FILE_TYPE;} \
										const std::string GetFileExtension() const override {return GetStaticFileExtension();} \
										const std::string GetFileType() const override {return GetStaticFileType();}

namespace GameSmith {
	// A serializeable object that is defined to be an 
	// 
	// 
	// 
	// 
	// (either single serializable or bundled serializables)
	class GE_API IAsset : public virtual ISerializeable, public virtual IExposedMembers {
	public:
		virtual ~IAsset() = default;
		virtual const std::string& GetName() const = 0;
		virtual const std::string GetFileExtension() const = 0;
		virtual const std::string GetFileType() const = 0;
	};

	class GE_API Asset : public ExposedMemberSerializeable, public IAsset {
	public:
		Asset(std::string fileName) : m_FileName(fileName) {}

		const std::string& GetName() const override { return m_FileName; }
		const std::string GetFileExtension() const override { return "*"; }
		const std::string GetFileType() const override { return "Any"; }
	private:
		const std::string m_FileName;
	};
};
