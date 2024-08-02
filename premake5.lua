include "Dependencies.lua"

workspace "GameSmithEngineProject"
	architecture "x64"

	configurations{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "GameSmithEngine/third-party/imgui"
group ""

group "Core"
	include "GameSmithEngine"
group ""

group "Tools"
	include "GameSmithEditor"
group ""

group "Misc"
	include "TestZone"
	include "GameSmithEngineTestSuite"
	include "GEStandaloneGameApp"
group ""
