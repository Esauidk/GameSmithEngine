#pragma once

#include "GameSmithEngine/SerializeableFiles/GenericFileTypes/JsonFile.h"

#define RENDERING_CONFIG_FILENAME "RenderingConfig.json"
namespace GameSmith {
	class RenderingConfig : public JsonFile {
	public:
		RenderingConfig() : JsonFile(), m_EnableVSync(true), m_FullscreenMode(false), m_ResolutionWidth(1920), m_ResolutionHeight(1080) {}
		virtual ~RenderingConfig() = default;
		inline bool IsVSyncEnabled() const { return m_EnableVSync; }
		inline bool IsFullscreenMode() const { return m_FullscreenMode; }
		inline unsigned int GetResolutionWidth() const { return m_ResolutionWidth; }
		inline unsigned int GetResolutionHeight() const { return m_ResolutionHeight; }
		void SetVSyncEnabled(bool enabled);
		void SetFullscreenMode(bool fullscreen);
		void SetResolution(unsigned int width, unsigned int height);
	protected:
		void ExtractJsonData(const json& newJson) override;
	private:
		void RefreshConfig();
	private:
		bool m_EnableVSync;
		bool m_FullscreenMode;
		unsigned int m_ResolutionWidth;
		unsigned int m_ResolutionHeight;
	};
};
