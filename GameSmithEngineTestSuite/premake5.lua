project "GoogleTest"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"%{wks.location}/third-party/googletest/googletest/src/gtest-all.cc",
		"%{wks.location}/third-party/googletest/googlemock/src/gmock-all.cc"
	}

	includedirs{
		"%{wks.location}/third-party/googletest/googletest/include",
		"%{wks.location}/third-party/googletest/googletest",
		"%{wks.location}/third-party/googletest/googlemock/include",
		"%{wks.location}/third-party/googletest/googlemock/"
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


project "GameSmithEngineTestSuite"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"tst/**.h",
		"tst/**.cpp"
	}

	links{
		"GameSmithEngine",
		"GoogleTest"
	}
	
	includedirs{
		"tst",
		"%{wks.location}/GameSmithEngine/src",
        "%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.googletest}",
		"%{wks.location}/third-party/googletest/googletest/src/"
	}

	filter "system:windows"
		systemversion "latest"

		defines{
			"GE_PLATFORM_WINDOWS"
		}

		postbuildcommands{
			("{COPYDIR} \"%{prj.location}resources\" \"%{cfg.buildtarget.directory}resources\"")
		}


	filter "system:windows"
	systemversion "latest"
	filter "configurations:Debug"
	defines "GE_DEBUG"
	defines "TEST_RESOURCES=\"resources\""
	runtime "Debug"
	symbols "on"

	filter "configurations:Release"
	defines "GE_Release"
	defines "TEST_RESOURCES=\"resources\""
	runtime "Release"
	optimize "on"