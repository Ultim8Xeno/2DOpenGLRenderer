project "Renderer"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir "%{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"
    objdir "%{wks.location}/bin-int/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"

    files
    {
        "src/**.h",
        "src/**.cpp",
        "dependencies/**.h",
        "dependencies/**.c"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE"
    }

    includedirs
    {
        "src",
        "dependencies/glad-4.5/include",
        "dependencies/glfw-3.3.8.bin.WIN64/include"
    }

    links
    {
        "opengl32.lib"
    }

    filter "system:linux"

        defines "PLATFORM_LINUX"
        linkoptions
        {
            "-lglfw"
        }
        postbuildcommands
        {
            ("cp -R Res \"%{wks.location}/" .. application .. "/Res\"")
        }

    filter "system:windows"
		systemversion "latest"

        defines "PLATFORM_WINDOWS"
        
        postbuildcommands
        {
            ("Xcopy /E /I /y Res \"%{wks.location}" .. application .. "\\Res\"")
        }

        links
        {
            "glfw3.lib"
        }
        
        libdirs
        {
            "dependencies/glfw-3.3.8.bin.WIN64/lib-vc2019"
        }

    filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

    filter "configurations:Release"
        defines "RELEASE"
        runtime "Release"
        optimize "on"