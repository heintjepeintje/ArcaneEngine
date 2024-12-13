project "glad"
	kind "StaticLib"
	language "C"
	cdialect "C11"

	objdir "Binaries/Intermediate/%{cfg.buildcfg}"
	targetdir "Binaries/Output/%{cfg.buildcfg}"

	files {
		"src/**.c"
	}

	includedirs {
		"include"
	}

	filter "configurations:Debug"
		symbols "On"
		defines {
			"_DEBUG"
		}

	filter "configurations:Release"
		symbols "On"
		defines {
			"NDEBUG"
		}