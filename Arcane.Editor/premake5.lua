project "Arcane.Editor"
	kind "ConsoleApp"
	
	location "./Build"
	targetdir "./Binaries/Output/%{cfg.buildcfg}"
	objdir "./Binaries/Intermediate/%{cfg.buildcfg}"

	files {
		"./Source/**.cpp"
	}

	includedirs {
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
