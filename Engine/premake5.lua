project "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"

	objdir "Binaries/Intermediate/%{cfg.buildcfg}"
	targetdir "Binaries/Output/%{cfg.buildcfg}"

	files {
		"Source/**.cpp"
	}

	includedirs {
		"Source",
		"Libraries/stb",
		"Libraries/tracy/public",
		"Libraries/rapidjson/include",
		os.getenv("VULKAN_SDK") .. "/Include"
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