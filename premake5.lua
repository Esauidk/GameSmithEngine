workspace "GameSmithEngineProject"
	architecture "x64"

	configurations{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["ImGui"] = "GameSmithEngine/third-party/imgui"
IncludeDir["glm"] = "GameSmithEngine/third-party/glm"
IncludeDir["stb"] = "GameSmithEngine/third-party/stb"
IncludeDir["tinyobj"] = "GameSmithEngine/third-party/tinyobjloader"
IncludeDir["googletest"] = "third-party/googletest/googletest/include"

include "GameSmithEngine/third-party/imgui"

project "GoogleTest"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"third-party/googletest/googletest/src/gtest-all.cc",
		"third-party/googletest/googlemock/src/gmock-all.cc"
	}

	includedirs{
		"third-party/googletest/googletest/include",
		"third-party/googletest/googletest",
		"third-party/googletest/googlemock/include",
		"third-party/googletest/googlemock/"
	}

	filter "system:windows"
	systemversion "latest"
	filter "configurations:Debug"
	defines "GE_DEBUG"
	runtime "Debug"
	symbols "on"

	filter "configurations:Release"
	defines "GE_Release"
	runtime "Release"
	optimize "on"

project "GameSmithEngine"
	location "GameSmithEngine"
	kind "StaticLib"
	language "C++"
	staticruntime "on"
	cppdialect "C++20"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "gepch.h"
	pchsource "GameSmithEngine/src/gepch.cpp"

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/third-party/stb/**.h",
		"%{prj.name}/third-party/stb/**.cpp",
		"%{prj.name}/third-party/glm/glm/**.hpp",
		"%{prj.name}/third-party/glm/glm/**.inl",
		"%{prj.name}/third-party/tinyobjloader/**.h"
	}

	includedirs{
		"%{prj.name}/src",
		"%{prj.name}/third-party/spdlog/include",
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

project "GameSmithEngineTestSuite"
	location "GameSmithEngineTestSuite"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"%{prj.name}/tst/**.h",
		"%{prj.name}/tst/**.cpp"
	}

	links{
		"GameSmithEngine",
		"GoogleTest"
	}
	
	includedirs{
		"%{prj.name}/tst",
		"GameSmithEngine/third-party/spdlog/include",
		"GameSmithEngine/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.googletest}",
		"third-party/googletest/googletest/src/"
	}

	filter "system:windows"
		systemversion "latest"

		defines{
			"GE_PLATFORM_WINDOWS"
		}


	filter "system:windows"
	systemversion "latest"
	filter "configurations:Debug"
	defines "GE_DEBUG"
	runtime "Debug"
	symbols "on"

	filter "configurations:Release"
	defines "GE_Release"
	runtime "Release"
	optimize "on"

project "GameSmithEditor"
	location "GameSmithEditor"
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
		"GameSmithEngine/third-party/spdlog/include",
		"GameSmithEngine/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}"
	}

	links{
		"GameSmithEngine"
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
		"GameSmithEngine/third-party/spdlog/include",
		"GameSmithEngine/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}"
	}

	links{
		"GameSmithEngine"
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