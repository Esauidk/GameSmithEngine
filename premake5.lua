workspace "GameEngineProject"
	architecture "x64"

	configurations{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["ImGui"] = "ProjectGE/third-party/imgui"
IncludeDir["glm"] = "ProjectGE/third-party/glm"
IncludeDir["stb"] = "ProjectGE/third-party/stb"

include "ProjectGE/third-party/imgui"

project "ProjectGE"
	location "ProjectGE"
	kind "StaticLib"
	language "C++"
	staticruntime "on"
	cppdialect "C++20"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "gepch.h"
	pchsource "ProjectGE/src/gepch.cpp"

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/third-party/stb/**.h",
		"%{prj.name}/third-party/stb/**.cpp",
		"%{prj.name}/third-party/glm/glm/**.hpp",
		"%{prj.name}/third-party/glm/glm/**.inl"
	}

	includedirs{
		"%{prj.name}/src",
		"%{prj.name}/third-party/spdlog/include",
		"%{IncludeDir.stb}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
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

project "TestZone"
	location "TestZone"
	kind "ConsoleApp"

	language "C++"
	cppdialect "C++20"

	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"ProjectGE/third-party/spdlog/include",
		"ProjectGE/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}"
	}

	links{
		"ProjectGE"
	}

	filter "system:windows"
		systemversion "latest"

		defines{
			"GE_PLATFORM_WINDOWS"
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