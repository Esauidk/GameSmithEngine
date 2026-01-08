#include "gepch.h"
#include "RenderingConfig.h"

#define VSYNC_ENABLED_KEY "VSyncEnabled"
#define FULLSCREEN_MODE_KEY "FullscreenMode"

#define RESOLUTION_KEY "Resolution"
#define RESOLUTION_WIDTH_KEY "ResolutionWidth"
#define RESOLUTION_HEIGHT_KEY "ResolutionHeight"

#include "GameSmithEngine/Core/Log.h"

namespace GameSmith {
	void RenderingConfig::SetVSyncEnabled(bool enabled)
	{
		m_EnableVSync = enabled;
		RefreshConfig();
	}

	void RenderingConfig::SetFullscreenMode(bool fullscreen)
	{
		m_FullscreenMode = fullscreen;
		RefreshConfig();
	}

	void RenderingConfig::SetResolution(unsigned int width, unsigned int height)
	{
		m_ResolutionWidth = width;
		m_ResolutionHeight = height;
		RefreshConfig();
	}

	void RenderingConfig::ExtractJsonData(const json& newJson)
	{
		if (newJson.contains(VSYNC_ENABLED_KEY)) {
			m_EnableVSync = newJson[VSYNC_ENABLED_KEY].get<bool>();
		}

		if (newJson.contains(FULLSCREEN_MODE_KEY)) {
			m_FullscreenMode = newJson[FULLSCREEN_MODE_KEY].get<bool>();
		}

		if (newJson.contains(RESOLUTION_KEY)) {
			const json& resJson = newJson[RESOLUTION_KEY];
			if (!resJson.contains(RESOLUTION_WIDTH_KEY) || !resJson.contains(RESOLUTION_HEIGHT_KEY)) {
				GE_CORE_WARN("Resolution data incomplete in JSON!");
			}

			m_ResolutionWidth = resJson[RESOLUTION_WIDTH_KEY].get<unsigned int>();
			m_ResolutionHeight = resJson[RESOLUTION_HEIGHT_KEY].get<unsigned int>();
		}
	}

	void RenderingConfig::RefreshConfig()
	{
		json newJson;
		newJson[VSYNC_ENABLED_KEY] = m_EnableVSync;
		newJson[FULLSCREEN_MODE_KEY] = m_FullscreenMode;
		newJson[RESOLUTION_KEY][RESOLUTION_WIDTH_KEY] = m_ResolutionWidth;
		newJson[RESOLUTION_KEY][RESOLUTION_HEIGHT_KEY] = m_ResolutionHeight;
		UpdateJson(newJson);
	}
};

