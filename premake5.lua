solution "tupp"
	configurations {"debug", "release"}
	defines {"GLM_FORCE_RADIANS","GLM_SWIZZLE", "GLEW_STATIC"}
	flags {"FatalWarnings"}
	vectorextensions "SSE2"
	includedirs {"include", "lib/**/include", "lib"}
	libdirs{"lib/glew/build", "lib/utils/build", "lib/glcw/build", "lib/json/build"}

	configuration "windows"
		defines {"WINDOWS"}
	configuration "linux"
		defines {"LINUX"}

	configuration "debug"
		defines { "DEBUG" }
		flags { "Symbols" }
		optimize "Off"

	configuration "release"
		defines { "NDEBUG" }
		optimize "Full"

	configuration {"gmake","windows"}
		includedirs {"lib/sdl2/mingw32-x86_64/include", "lib/sdl2/mingw32-i686/include"}
		libdirs {"lib/sdl2/mingw32-x86_64/lib", "lib/sdl2/mingw32-i686/lib"}

	configuration {"gmake", "linux"}

	project "GLEW"
		kind "StaticLib"
		language "C++"
		location "lib/glew/build"
		includedirs {"lib/glew/include"}
		files {"lib/glew/src/**.c"}

		configuration {"gmake", "windows"}
			linkoptions {"-lmingw32 -static-libgcc"}

		configuration {"gmake", "linux"}
			if _OPTIONS["cc"] == "clang" then
				toolset "clang"
			end

	project "glcw"
		kind "StaticLib"
		language "C++"
		location "lib/glcw/build"
		includedirs {"lib/glcw/include"}
		files {"lib/glcw/src/**.cpp"}
		buildoptions "-std=c++14"

		configuration {"gmake", "windows"}
			linkoptions {"-lmingw32 ../lib/glew/build/GLEW.lib ../lib/log/build/utils.lib -lopengl32 -static-libgcc -static-libstdc++"}
			links {"GLEW", "utils"}

		configuration {"gmake", "linux"}
			links {"GLEW", "GL", "utils"}
			if _OPTIONS["cc"] == "clang" then
				toolset "clang"
				buildoptions "-stdlib=libc++"
				links "c++"
			end

	project "utils"
		kind "StaticLib"
		language "C++"
		location "lib/utils/build"
		includedirs {"lib/utils/include"}
		files {"lib/utils/src/**.cpp"}
		buildoptions "-std=c++14"

		configuration {"gmake", "windows"}
			linkoptions {"-lmingw32 -static-libgcc -static-libstdc++"}

		configuration {"gmake", "linux"}
			if _OPTIONS["cc"] == "clang" then
				toolset "clang"
				buildoptions "-stdlib=libc++"
				links "c++"
			end

	project "json"
		kind "StaticLib"
		language "C++"
		location "lib/utils/build"
		includedirs {"lib/utils/include"}
		files {"lib/utils/src/**.cpp"}
		buildoptions "-std=c++14"

		configuration {"gmake", "windows"}
			linkoptions {"-lmingw32 -static-libgcc -static-libstdc++"}

		configuration {"gmake", "linux"}
			if _OPTIONS["cc"] == "clang" then
				toolset "clang"
				buildoptions "-stdlib=libc++"
				links "c++"
			end

	project "zmdl_viewer"
		kind "WindowedApp"
		language "C++"
		includedirs {"include"}
		files {"src/**.cpp"}
		location "build"
		buildoptions "-std=c++14"

		configuration {"gmake","windows"}
			postbuildcommands {
				"copy ..\\lib\\sdl2\\mingw32-x86_64\\bin\\SDL2.dll SDL2.dll",
			}
			linkoptions {"-lmingw32 ../lib/glew/build/GLEW.lib ../lib/glcw/build/glcw.lib ../lib/log/build/log.lib -lopengl32 -lSDL2main -lSDL2 -static-libgcc -static-libstdc++"}
			links {"GLEW", "utils", "glcw"}

		configuration {"gmake", "linux"}
			links {"GLEW", "GL", "glcw", "SDL2", "utils"}
			if _OPTIONS["cc"] == "clang" then
				toolset "clang"
				buildoptions "-stdlib=libc++"
				links "c++"
			end
