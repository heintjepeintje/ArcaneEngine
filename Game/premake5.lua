project "Game"
	kind "ConsoleApp"
	language "C++"
	cdialect "C++20"

	objdir "Binaries/Intermediate/%{cfg.buildcfg}"
	targetdir "Binaries/Output/%{cfg.buildcfg}"

	files {
		"Source/**.cpp"
	}

	includedirs {
		"Source",
		"../Engine/Source",
		"../Engine/Libraries/glm",
		"../Engine/Libraries/tracy/public"
	}

	links {
		"gdi32",
		"opengl32",
		"Engine",
		"tracy",
		"ImGui",
		"ws2_32",
		"winmm",
		"dbghelp",
		"shlwapi"
	}

	filter "configurations:Debug"
		symbols "On"
		defines {
			"_DEBUG",
			"TRACY_ENABLE"
		}

	filter "configurations:Release"
		symbols "On"
		defines {
			"NDEBUG"
		}