project "GEStandaloneGameApp"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   staticruntime "off"

   links { "GameSmithEngine" }

   files{ 
      "src/**.h", 
      "src/**.cpp" 
   }

   includedirs{
		"%{wks.location}\\GameSmithEngine\\src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}"
	}

   defines{
		"GE_DYNAMIC_LINK"
	}

   targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
   objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"
      buildoptions { 
			"/utf-8",
			"/wd4250"
		}
      defines { "GE_PLATFORM_WINDOWS" }

   filter "configurations:Debug"
      defines { "GE_DEBUG" }
      runtime "Debug"
      symbols "On"

   filter "configurations:Release"
      defines { "GE_RELEASE" }
      runtime "Release"
      optimize "On"
      symbols "On"

   filter "configurations:Dist"
      kind "WindowedApp"
      defines { "GE_DIST" }
      runtime "Release"
      optimize "On"
      symbols "Off"