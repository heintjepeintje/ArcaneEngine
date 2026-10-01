project "Arcane.Core"
	kind "StaticLib"

	language "C++"
	cppdialect "C++23"
	
	location "./Build"
	targetdir "./Binaries/Output/%{cfg.buildcfg}"
	objdir "./Binaries/Intermediate/%{cfg.buildcfg}"

	files {
		"./Source/**.cpp"
	}

	includedirs {
		"./Source"
	}

	filter "configurations:Debug"
		symbols "On"
		defines {
			"AR_PLATFORM_BUILD_CONFIG_DEBUG=1"
		}

	filter "configurations:Release"
		optimize "Speed"
		defines {
			"AR_PLATFORM_BUILD_CONFIG_RELEASE=1"
		}
