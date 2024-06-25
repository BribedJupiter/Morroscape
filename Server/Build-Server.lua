project "Server"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { "**.h", "**.cpp", "**.hpp", "**.c" }

   includedirs
   {
        "Source",
        "../Vendor/Binaries/Raylib/include",
        "../Vendor/Binaries/Raylib/lib",
	    "../Core/Source"
   }

   links
   {
      "Core",
      "../Vendor/Binaries/Raylib/lib/raylib"
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"
       defines { "WINDOWS", "GRAPHICS_API_OPENGL_33", "PLATFORM_DESKTOP"}
       links {"winmm", "opengl32", "user32", "kernel32", "gdi32"}

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"