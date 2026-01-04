#pragma once
#include "gepch.h"
#include "GameSmithEngine/ResourceAssets/Asset.h"

namespace GameSmith {
	class GE_API EngineInitAsset : public Asset {
	public:
		EngineInitAsset() = default;
		inline void SetAssetDirectory(const std::string& dir) { m_OverrideAssetDirectory = dir; }
		inline void SetContentDirectory(const std::string& dir) { m_OverrideContentDirectory = dir; }

		virtual Ref<char> Serialize() override;
		virtual void Serialize(char* byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequiredSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;
	private:
		// Directories for assets
		std::string m_OverrideAssetDirectory;
		std::string m_OverrideContentDirectory;
	};
};
