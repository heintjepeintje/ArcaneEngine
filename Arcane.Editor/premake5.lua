project "Arcane.Editor"
	kind "ConsoleApp"

	language "C++"
	cppdialect "C++23"
	
	location "./Build"
	targetdir "./Binaries/Output/%{cfg.buildcfg}"
	objdir "./Binaries/Intermediate/%{cfg.buildcfg}"

	files {
		"./Source/**.cpp"
	}

	includedirs {
		"%{wks.location}/../Arcane.Core/Source",
		"./Source"
	}

	libdirs {
		"%{wks.location}/../Arcane.Core/Binaries/Output/%{cfg.buildcfg}"
	}

	links {
		"Arcane.Core"
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
