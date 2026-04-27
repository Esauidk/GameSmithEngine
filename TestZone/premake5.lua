project "TestZone"
	kind "ConsoleApp"

	language "C++"
	cppdialect "C++20"

	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs{
		"%{wks.location}/GameSmithEngine/src",
		"%{IncludeDir.glm}",
        "%{IncludeDir.spdlog}",
		"%{IncludeDir.ImGui}"
	}

	links{
		"GameSmithEngine"
	}

	defines
	{
		"GE_DYNAMIC_LINK"
	}

	filter "system:windows"
		systemversion "latest"

		buildoptions { 
			"/utf-8",
			"/wd4250"
		}

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