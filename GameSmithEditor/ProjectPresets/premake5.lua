IncludeDir = {}
IncludeDir["ImGui"] = "%{wks.location}/GameSmithEngine/third-party/imgui"
IncludeDir["glm"] = "%{wks.location}/GameSmithEngine/third-party/glm"
IncludeDir["stb"] = "%{wks.location}/GameSmithEngine/third-party/stb"
IncludeDir["tinyobj"] = "%{wks.location}/GameSmithEngine/third-party/tinyobjloader"
IncludeDir["spdlog"] = "%{wks.location}/GameSmithEngine/third-party/spdlog/include"
IncludeDir["CLI11"] = "%{wks.location}/GameSmithEngine/third-party/CLI11/include"
IncludeDir["json"] = "%{wks.location}/GameSmithEngine/third-party/json/include"
IncludeDir["GameSmithEngine"] = "%{wks.location}/GameSmithEngine/src"

workspace "GameProject"
	architecture "x64"

	configurations{
		"Debug",
		"Release",
		"Dist"
	}

	newoption {
		trigger = "prjName",
		value = "ProjectName",
		description = "The name of the game project",
		default = "DefaultGameProject"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


libdirs { "C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\GameSmithEngine" }

project(_OPTIONS["prjName"])
	location "."
	kind "SharedLib"

	language "C++"
	cppdialect "C++20"

	staticruntime "on"

	links { "GameSmithEngine" }

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"src/**.h",
		"src/**.cpp",
		"%{IncludeDir.GameSmithEngine}/**.h"
	}

	includedirs{
		"%{IncludeDir.GameSmithEngine}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.tinyobj}"
	}

	defines{
		"GE_PLATFORM_WINDOWS",
		"GE_DYNAMIC_LINK"
	}

	filter "system:windows"
		systemversion "latest"

		buildoptions { "/utf-8" }

		defines{
			"GE_PLATFORM_WINDOWS"
		}

		postbuildcommands{
			("cmd /c robocopy %[%{cfg.targetdir}/] %[%{wks.location}/Cache] " .. _OPTIONS["prjName"] .. ".dll ^& if %ERRORLEVEL% LSS 2 exit /b 0 ^& exit /b %ERRORLEVEL%")
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