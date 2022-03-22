workspace "LennardJones"
    architecture "x64"
    startproject "LennardJones"

    configurations {
        "Debug",
        "Release"
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

    project "LennardJones"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        staticruntime "on"
        systemversion "latest"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

        files {
            "src/**.h",
            "src/**.cpp"
        }

        includedirs {
            "%{prj.name}/src"
        }

        filter "configurations:Debug"
            symbols "on"

        filter "configurations:Release"
            optimize "on"