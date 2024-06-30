project "GameSmithEngine"
	kind "StaticLib"
	language "C++"
	staticruntime "on"
	cppdialect "C++20"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "gepch.h"
	pchsource "src/gepch.cpp"

	files{
		"src/**.h",
		"src/**.cpp",
		"third-party/stb/**.h",
		"third-party/stb/**.cpp",
		"third-party/glm/glm/**.hpp",
		"third-party/glm/glm/**.inl",
		"third-party/tinyobjloader/**.h"
	}

	includedirs{
		"src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.tinyobj}"
	}

	links{
		"ImGui"
	}

	filter "system:windows"
		systemversion "latest"

		defines{
			"GE_PLATFORM_WINDOWS",
			"GE_BUILD_DLL"
		}

		postbuildcommands{
			("{COPY} %{cfg.buildtarget.relpath}/../**.cso ../bin/" ..outputdir .. "/TestZone")
		}
		

	filter "configurations:Debug"
		defines "GE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "GE_Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "GE_Dist"
		runtime "Release"
		optimize "on"
