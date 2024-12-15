project "Game"
	kind "ConsoleApp"
	language "C"
	cdialect "C17"

	objdir "Binaries/Intermediate/%{cfg.buildcfg}"
	targetdir "Binaries/Output/%{cfg.buildcfg}"

	files {
		"Source/**.cpp"
	}

	includedirs {
		"Source",
		"../Engine/Source"
	}

	links {
		"gdi32",
		"opengl32",
		"Engine"
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