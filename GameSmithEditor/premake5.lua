project "GameSmithEditor"
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
		"src",
		"%{IncludeDir.spdlog}",
		"%{wks.location}/GameSmithEngine/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
	}

	links{
		"ImGui",
		"GameSmithEngine"
	}

	defines
	{
		"GE_DYNAMIC_LINK"
	}

	filter "system:windows"
		systemversion "latest"

		buildoptions { "/utf-8" }

		postbuildcommands{
			("cmd /c robocopy %[%{wks.location}/GameSmithEditor/Icons] %[%{wks.location}/bin/" .. outputdir .. "/GameSmithEditor/Icons] /E ^& if %ERRORLEVEL% LSS 2 exit /b 0 ^& exit /b %ERRORLEVEL%"),
			("cmd /c robocopy %[%{wks.location}/GameSmithEditor/ProjectPresets] %[%{wks.location}/bin/" .. outputdir .. "/GameSmithEditor/ProjectPresets] /E ^& if %ERRORLEVEL% LSS 2 exit /b 0 ^& exit /b %ERRORLEVEL%")
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