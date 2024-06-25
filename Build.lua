--Thanks to TheCherno for help with the build setup: https://github.com/TheCherno/ProjectTemplate/tree/master
workspace "Morroscape"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Client"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Core"
	include "Core/Build-Core.lua"
group ""

include "Client/Build-Client.lua"
include "Server/Build-Server.lua"
include "Editor/Build-Editor.lua"
--includedirs {"Vendor/Binaries/Raylib/include"}
--includedirs {"Vendor/Binaries/Raylib/lib"}