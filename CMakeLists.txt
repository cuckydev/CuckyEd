cmake_minimum_required(VERSION 3.8)

###########
# Options #
###########
option(PKG_CONFIG_STATIC_LIBS "On platforms with pkg-config, static-link the dependencies (good for Windows builds, so you don't need to bundle DLL files)" OFF)

#########
# Setup #
#########

# Define project, source, and includes
project(CuckyEd LANGUAGES CXX)

add_executable(CuckyEd WIN32
	# CuckyEd
	"src/Main.cpp"
	"src/Error.cpp"
	"src/Error.h"
	"src/Project.cpp"
	"src/Project.h"
	
	# Libraries
	# mdcomp
	"lib/mdcomp/artc42.cc"
	"lib/mdcomp/artc42.hh"
	"lib/mdcomp/basic_decoder.hh"
	"lib/mdcomp/bigendian_io.hh"
	"lib/mdcomp/bitstream.hh"
	"lib/mdcomp/comper.cc"
	"lib/mdcomp/comper.hh"
	"lib/mdcomp/enigma.cc"
	"lib/mdcomp/enigma.hh"
	"lib/mdcomp/ignore_unused_variable_warning.hh"
	"lib/mdcomp/kosinski.cc"
	"lib/mdcomp/kosinski.hh"
	"lib/mdcomp/kosplus.cc"
	"lib/mdcomp/kosplus.hh"
	"lib/mdcomp/lzkn1.cc"
	"lib/mdcomp/lzkn1.hh"
	"lib/mdcomp/lzss.hh"
	"lib/mdcomp/moduled_adaptor.hh"
	"lib/mdcomp/nemesis.cc"
	"lib/mdcomp/nemesis.hh"
	"lib/mdcomp/rocket.cc"
	"lib/mdcomp/rocket.hh"
	"lib/mdcomp/saxman.cc"
	"lib/mdcomp/saxman.hh"
	"lib/mdcomp/snkrle.cc"
	"lib/mdcomp/snkrle.hh"
)

# Include library directory
target_include_directories(CuckyEd PRIVATE "lib")

# Some other CMake setup stuff
list(APPEND CMAKE_MODULE_PATH
	"${CMAKE_SOURCE_DIR}/cmake"
)

set_target_properties(CuckyEd PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
	#CXX_STANDARD 17
	#CXX_STANDARD_REQUIRED ON
	#CXX_EXTENSIONS OFF
)

################
# Dependencies #
################
find_package(PkgConfig QUIET)

# Find SDL2
find_package(SDL2)
if(PKG_CONFIG_FOUND)
	pkg_check_modules(sdl2 QUIET IMPORTED_TARGET sdl2)
endif()

#Link SDL2
if(TARGET PkgConfig::sdl2)
	if(PKG_CONFIG_STATIC_LIBS)
		# Pkg-Config static
		message(STATUS "Using system SDL2 (pkg-config, static)")
		list(REMOVE_ITEM sdl2_STATIC_CFLAGS "-Dmain=SDL_main")
		list(REMOVE_ITEM sdl2_STATIC_LDFLAGS "-lSDL2main")
		target_compile_options(CuckyEd PRIVATE ${sdl2_STATIC_CFLAGS})
		target_link_libraries(CuckyEd PRIVATE ${sdl2_STATIC_LDFLAGS})
	else()
		# Pkg-Config dynamic
		message(STATUS "Using system SDL2 (pkg-config, dynamic)")
		list(REMOVE_ITEM sdl2_CFLAGS "-Dmain=SDL_main")
		list(REMOVE_ITEM sdl2_LDFLAGS "-lSDL2main")
		target_compile_options(CuckyEd PRIVATE ${sdl2_CFLAGS})
		target_link_libraries(CuckyEd PRIVATE ${sdl2_LDFLAGS})
	endif()
elseif(TARGET SDL2::SDL2)
	# CMake-generated config (Arch, vcpkg, Raspbian)
	message(STATUS "Using system SDL2 (CMake, dynamic)")
	target_link_libraries(CuckyEd PRIVATE SDL2::SDL2)
elseif(TARGET SDL2::SDL2-static)
	# CMake-generated config (Arch, vcpkg, Raspbian)
	message(STATUS "Using system SDL2 (CMake, static)")
	target_link_libraries(CuckyEd PRIVATE SDL2::SDL2-static)
elseif(SDL2_FOUND)
	# Autotools-generated config (MSYS2)
	message(STATUS "Using system SDL2 (Autotools)")
	target_include_directories(CuckyEd PRIVATE ${SDL2_INCLUDE_DIRS})
	target_link_libraries(CuckyEd PRIVATE ${SDL2_LIBRARIES})
endif()

##########
# Tweaks #
##########
# This is messy as hell, and has been replaced by CMAKE_MSVC_RUNTIME_LIBRARY,
# but that's a very recent CMake addition, so we're still doing it this way for now
if(MSVC AND MSVC_LINK_STATIC_RUNTIME)
	# Statically-link the CRT (vcpkg static libs do this)
	foreach(flag_var CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
		if(${flag_var} MATCHES "/MD")
			string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
		endif()
	endforeach()
endif()

# Do some other MSVC fixes
if(MSVC)
	# Disable warnings that normally fire up on MSVC when using "unsafe" functions instead of using MSVC's "safe" _s functions
	target_compile_definitions(CuckyEd PRIVATE _CRT_SECURE_NO_WARNINGS)
	
	# Make it so source files are recognized as UTF-8 by MSVC
	target_compile_options(CuckyEd PRIVATE "/utf-8")
	
	# Use `main` instead of `WinMain`
	set_target_properties(CuckyEd PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()
