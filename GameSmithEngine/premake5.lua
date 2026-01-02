project "GameSmithEngine"
	kind "SharedLib"
	language "C++"
	staticruntime "off"
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
		"third-party/tinyobjloader/**.h",
		"third-party/CLI11/include/**.hpp"
	}

	includedirs{
		"src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.tinyobj}",
		"%{IncludeDir.CLI11}"
	}

	links{
		"ImGui"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GE_BUILD_DLL",
		"GE_DYNAMIC_LINK"
	}

	filter "system:windows"
		systemversion "latest"

		defines{
			"GE_PLATFORM_WINDOWS"
		}

		buildoptions { "/utf-8" }

		postbuildcommands{
			("cmd /c xcopy %[%{cfg.targetdir}/*] %[%{wks.location}/bin/" .. outputdir .. "/TestZone] /E /Y /I")
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

project "GameSmithEngineStatic"
	kind "StaticLib"
	language "C++"
	staticruntime "off"
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
		"third-party/tinyobjloader/**.h",
		"third-party/CLI11/include/**.hpp"
	}

	includedirs{
		"src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.tinyobj}",
		"%{IncludeDir.CLI11}"
	}

	links{
		"ImGui"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter "system:windows"
		systemversion "latest"

		buildoptions { "/utf-8" }

		defines{
			"GE_PLATFORM_WINDOWS"
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

