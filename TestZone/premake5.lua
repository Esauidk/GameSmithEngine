project "TestZone"
	kind "ConsoleApp"

	language "C++"
	cppdialect "C++20"

	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

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