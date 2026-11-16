workspace "Render"

	configurations{
		"Debug",
		"Release"
	}

	architecture "x86_64"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependences"
	include"vendor/GLFW"
	include"vendor/glad"
	include"vendor/ImGui"
group ""


project "Render"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	buildoptions { 
		--用来消除中文注释产生的字符集警告 C4828
		"/wd4828"
	}

	links{
		"GLFW",
		"glad",
		"ImGui"
	}

	libdirs{
		
	}

	includedirs{
		"src/",
		"vendor/GLFW/include",
		"vendor/glad/include",
		"vendor/ImGui",
		"vendor/glm",
		"vendor/stb_image"
	}

	files{
		"src/**.h",
		"src/**.cpp"
	}

	defines{
		
	}

	filter "system:windows"
		systemversion "latest"
		defines { "KUD_PLATFORM_WINDOWS" }

	filter "system:linux"
		defines { "KUD_PLATFORM_LINUX" }

	filter "configurations:Debug"
		symbols "On"
		runtime "Debug"
		defines { "KUD_DEBUG" }

	filter "configurations:Release"
		optimize "On"
		runtime "Release"







