#pragma once
#include "Asset.h"

namespace GameSmith {
	class GE_API BlobAsset : public Asset {
	public:
		BlobAsset(std::string fileName) : Asset(fileName) {}
		virtual ~BlobAsset() = default;
		virtual Ref<char> Serialize() override;
		virtual void Serialize(char* byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequiredSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;

		const char* GetBytes() const { return m_Bytes.get(); }
		const unsigned int GetSize() const { return m_Size; }

		SERIAL_FILE(Blob, blob)
	private:
		Ref<char> m_Bytes;
		unsigned int m_Size;
	};
};