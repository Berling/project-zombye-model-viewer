solution "tupp"
	configurations {"debug", "release"}
	defines {"GLM_FORCE_RADIANS","GLM_SWIZZLE", "GLEW_STATIC"}
	flags {"FatalWarnings"}
	vectorextensions "SSE2"
	includedirs {"include", "lib/**/include", "lib", "mesh_converter/include/", "animation_converter/include/"}
	libdirs{"lib/glew/build", "lib/log/build", "lib/glcw/build"}

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

	project "animation_converter"
		kind "ConsoleApp"

		location "animation_converter"

		buildoptions "-std=c++1y"

		files "animation_converter/src/**.cpp"

		defines "GLM_FORCE_RADIANS"

		links "jsoncpp"

		configuration {"gmake", "linux"}
			if _OPTIONS["cc"] == "clang" then
				toolset "clang"
				buildoptions "-stdlib=libc++"
				links "c++"
			end

	project "mesh_converter"
		kind "ConsoleApp"

		location "mesh_converter"

		buildoptions "-std=c++1y"

		files "mesh_converter/src/**.cpp"

		defines "GLM_FORCE_RADIANS"

		links "jsoncpp"

		configuration {"gmake", "linux"}
			if _OPTIONS["cc"] == "clang" then
				toolset "clang"
				buildoptions "-stdlib=libc++"
				links "c++"
			end

	project "GLEW"
		kind "StaticLib"
		language "C++"
		location "lib/glew/build"
		includedirs {"lib/glew/include"}
		files { "lib/glew/include/**.h", "lib/glew/src/**.c"}

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
		files {"lib/glcw/include/**.hpp", "lib/glcw/src/**.cpp"}
		buildoptions "-std=gnu++1y"

		configuration {"gmake", "windows"}
			linkoptions {"-lmingw32 ../lib/glew/build/GLEW.lib ../lib/log/build/log.lib -lopengl32 -static-libgcc -static-libstdc++"}
			links {"GLEW", "log"}

		configuration {"gmake", "linux"}
			links {"GLEW", "GL", "log"}
			if _OPTIONS["cc"] == "clang" then
				toolset "clang"
				buildoptions "-stdlib=libc++"
				links "c++"
			end

	project "log"
		kind "StaticLib"
		language "C++"
		location "lib/log/build"
		includedirs {"lib/log/include"}
		files {"lib/log/include/**.hpp", "lib/log/src/**.cpp"}
		buildoptions "-std=gnu++1y"

		configuration {"gmake", "windows"}
			linkoptions {"-lmingw32 -static-libgcc -static-libstdc++"}

		configuration {"gmake", "linux"}
			if _OPTIONS["cc"] == "clang" then
				toolset "clang"
				buildoptions "-stdlib=libc++"
				links "c++"
			end

	project "jsoncpp"
		kind "StaticLib"
		language "C++"
		location "lib/jsoncpp/build"
		includedirs {"lib/jsoncpp/include"}
		files {"lib/jsoncpp/include/**.hpp", "lib/jsoncpp/src/**.cpp"}
		buildoptions "-std=gnu++1y"

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
		files {"inlude/**.h", "include/**.hpp", "src/**.cpp"}
		location "build"
		buildoptions "-std=gnu++1y"

		configuration {"gmake","windows"}
			postbuildcommands {
				"copy ..\\lib\\sdl2\\mingw32-x86_64\\bin\\SDL2.dll SDL2.dll",
			}
			linkoptions {"-lmingw32 ../lib/glew/build/GLEW.lib ../lib/glcw/build/glcw.lib ../lib/log/build/log.lib -lopengl32 -lSDL2main -lSDL2 -static-libgcc -static-libstdc++"}
			links {"GLEW", "log", "glcw", "jsoncpp"}

		configuration {"gmake", "linux"}
			links {"GLEW", "GL", "glcw", "SDL2", "log", "jsoncpp"}
			libdirs {os.findlib("SDL2")}
			if _OPTIONS["cc"] == "clang" then
				toolset "clang"
				buildoptions "-stdlib=libc++"
				links "c++"
			end
