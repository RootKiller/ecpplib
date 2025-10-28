workspace "ECppLibTests"
	configurations { "Debug", "Release" }
	architecture ("x86_64")

project "ECppLibTests"
	files {
		"**.cpp", "**.h",
	}
	files {"../libs/**.cpp", "../libs/**.h"}
	files {"../.editorconfig"}

	vpaths { ["*"] = "../**" }

	includedirs { "../libs" }

	kind "ConsoleApp"
	language "C++"

	targetdir "bin"
	targetsuffix "_%{cfg.buildcfg}"

	objdir "temp/obj/%{cfg.platform}_%{cfg.buildcfg}"
	location "temp/build"

	defines { "DEBUG=0", "RELEASE=1" }

	filter "configurations:Debug"
		defines { "BUILD=DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "BUILD=RELEASE" }
		optimize "On"