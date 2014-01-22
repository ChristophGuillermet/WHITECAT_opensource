# -------------------------------------------------------
# OpenLayer cmake build script. Creates makefiles for unix 
# based systems and/or
# project files for a given environment like MSVC.
#
# TODO:
#	* Create CYGWIN TARGET
#	* Fix MSVC
#	* Seperate into modules?
#
# Written by: juvinious
# -------------------------------------------------------

# -------------------------------------------------------
# Ensure that we are doing an out of source build
# Prevents any mishaps
# -------------------------------------------------------
if(EXISTS ${CMAKE_SOURCE_DIR}/CMakeCache.txt)
	file(REMOVE ${CMAKE_SOURCE_DIR}/CMakeCache.txt)
	file(REMOVE_RECURSE ${CMAKE_SOURCE_DIR}/CMakeFiles)
endif(EXISTS ${CMAKE_SOURCE_DIR}/CMakeCache.txt)
if(EXISTS ${CMAKE_BINARY_DIR}/Makefile)
	file(REMOVE ${CMAKE_BINARY_DIR}/Makefile)
endif(EXISTS ${CMAKE_BINARY_DIR}/Makefile)	
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	if(UNIX AND NOT CYGWIN)
		message(FATAL_ERROR "Oops. Don't do an in-source build. Create an extra directory change into it and run cmake pointing to the base directory. IE: \nmkdir mybuild && cd mybuild && cmake ../ && make\nYou may need to remove CMakeCache.txt and the CMakeFiles directory in ${CMAKE_SOURCE_DIR} if you can't get rid of this error.")
	else(UNIX AND NOT CYGWIN)
		message(FATAL_ERROR "Oops. Don't do an in-source build. Create an extra directory change into it and run cmake pointing to the base directory. IE: \nmkdir mybuild; cd mybuild; cmakesetup ../\nYou may need to remove CMakeCache.txt and the CMakeFiles directory in  ${CMAKE_SOURCE_DIR} if you can't get rid of this error.")
	endif(UNIX AND NOT CYGWIN)
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})


# -------------------------------------------------------
# Set working directories
# -------------------------------------------------------
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)


# -------------------------------------------------------
# Directory in which extra macros can be found
# -------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/build)

# -------------------------------------------------------
# Add in uninstall target 
# -------------------------------------------------------
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/build/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

# -------------------------------------------------------
# project name
# -------------------------------------------------------
project (OpenLayer)

# -------------------------------------------------------
# Including needed macros
# -------------------------------------------------------
include(CMakeMacros)
find_package(ZLIB)
find_package(FREETYPE)
find_package(PNG)
find_package(ALLEGRO REQUIRED)
find_package(OpenGL REQUIRED)
find_package(ALLEGROGL REQUIRED)

MARK_AS_ADVANCED(
  CLEAR
  ZLIB_LIBRARY
  ZLIB_INCLUDE_DIR
  PNG_LIBRARY
  PNG_INCLUDE_DIR
  OPENGL_glu_LIBRARY
  OPENGL_gl_LIBRARY
  OPENGL_INCLUDE_DIR
  )

# -------------------------------------------------------
# User setable options
# -------------------------------------------------------
## libraries should be shared by default for Debian
if(UNIX AND NOT CYGWIN)
  option(CREATE_STATIC_LIB "Make the library static?" off)
else(UNIX AND NOT CYGWIN)
  option(CREATE_STATIC_LIB "Make the library static?" on)
endif(UNIX AND NOT CYGWIN)
dependent_option(DISABLE_TTF "Disable TTF Support" off "FREETYPE_FOUND" on)
if(NOT DISABLE_TTF)
	set(TTF_OK 1)
endif(NOT DISABLE_TTF)
dependent_option(ENABLE_INTERNAL_FONT "Use internal font renderer" on "TTF_OK" on)
# dependent_option(DISABLE_PNG "Disable PNG support" on "PNG_FOUND" on)
option(DISABLE_PNG "Disable PNG support" off)
option(DISABLE_OLD_API "Disable backwards compatibility with 2.0 and below" off)
option(DISABLE_STATE_CHANGES "Disable state changes" off)

if(NOT MSVC)
	set(CXXFLAGS "-O2 -Wall -fexpensive-optimizations")
else(NOT MSVC)
	set(CXXFLAGS "-O2")
endif(NOT MSVC)

add_definitions(${CXXFLAGS})

# -------------------------------------------------------
# Check whether to create a shared or static library
# -------------------------------------------------------
if(CREATE_STATIC_LIB)
	set(CREATE_STATIC_LIB STATIC)
else(CREATE_STATIC_LIB)
	set(CREATE_STATIC_LIB SHARED)
	set(OL_SHARED 1)
	set(OL_LIB_BUILD 1)
	if(MSVC)
		add_definitions(/DOL_SHARED)
		add_definitions(/DOL_LIB_BUILD)
	else(MSVC)
		add_definitions(-DOL_SHARED)
		add_definitions(-DOL_LIB_BUILD)
	endif(MSVC)
endif(CREATE_STATIC_LIB)

# -------------------------------------------------------
# Paths and system setup
# -------------------------------------------------------

# -------------------------------------------------------
# UNIX BASED SYSTEMS
# -------------------------------------------------------
if(UNIX AND NOT CYGWIN)
	# -------------------------------------------------------
	# Take those user options and set the necessary compile time preprocessors
	# -------------------------------------------------------
	if(NOT DISABLE_TTF)
		if(ENABLE_INTERNAL_FONT)
			add_definitions(-DUSE_NEW_TTF)
			set(USE_NEW_TTF 1)
		endif(ENABLE_INTERNAL_FONT)
	else(NOT DISABLE_TTF)
		add_definitions(-DOL_NO_TTF)
		set(OL_NO_TTF 1)
	endif(NOT DISABLE_TTF)
	if(DISABLE_PNG)
		add_definitions(-DOL_NO_PNG)
		set(OL_NO_PNG 1)
	endif(DISABLE_PNG)
	if(DISABLE_OLD_API)
		add_definitions(-DOL_NO_OLD_API)
		set(OL_NO_OLD_API 1)
	endif(DISABLE_OLD_API)
	if(DISABLE_STATE_CHANGES)
		add_definitions(-DOL_NO_STATE_CHANGE)
		set(OL_NO_STATE_CHANGE 1)
	endif(DISABLE_STATE_CHANGES)
	
	if(NOT ENABLE_INTERNAL_FONT)
		add_subdirectory(utils/glyphkeeper)
		set(GLYPHKEEPER_LIB glyph-agl)
	endif(NOT ENABLE_INTERNAL_FONT)

endif(UNIX AND NOT CYGWIN)

# -------------------------------------------------------
# MINGW
# -------------------------------------------------------

if(MINGW)
	# -------------------------------------------------------
	# Take those user options and set the necessary compile time preprocessors
	# -------------------------------------------------------
	if(NOT DISABLE_TTF)
		if(ENABLE_INTERNAL_FONT)
			add_definitions(-DUSE_NEW_TTF)
			set(USE_NEW_TTF 1)
		endif(ENABLE_INTERNAL_FONT)
	else(NOT DISABLE_TTF)
		add_definitions(-DOL_NO_TTF)
		set(OL_NO_TTF 1)
	endif(NOT DISABLE_TTF)
	if(DISABLE_PNG)
		add_definitions(-DOL_NO_PNG)
		set(OL_NO_PNG 1)
	endif(DISABLE_PNG)
	if(DISABLE_OLD_API)
		add_definitions(-DOL_NO_OLD_API)
		set(OL_NO_OLD_API 1)
	endif(DISABLE_OLD_API)
	if(DISABLE_STATE_CHANGES)
		add_definitions(-DOL_NO_STATE_CHANGE)
		set(OL_NO_STATE_CHANGE 1)
	endif(DISABLE_STATE_CHANGES)
	
	if(NOT ENABLE_INTERNAL_FONT)
		add_subdirectory(utils/glyphkeeper)
		set(GLYPHKEEPER_LIB glyph-agl)
	endif(NOT ENABLE_INTERNAL_FONT)
	
	set(WIN_LIBS -luser32 -lgdi32) 
endif(MINGW)

# -------------------------------------------------------
# MSVC
# -------------------------------------------------------

if(MSVC)
	# -------------------------------------------------------
	# Take those user options and set the necessary compile time preprocessors
	# -------------------------------------------------------
	SET(CMAKE_CXX_FLAGS "/nologo /W3 /Gy")
    	SET(CMAKE_CXX_FLAGS_DEBUG "/MTd /Z7 /Od")
	SET(CMAKE_CXX_FLAGS_RELEASE "/MT /O2")
	SET(CMAKE_CXX_FLAGS_MINSIZEREL "/MT /O2")
	SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MTd /Z7 /Od")
	set(CMAKE_CONFIGURATION_TYPES Release)
	add_definitions("/link SUBSYSTEM:WINDOWS")
	
	if(NOT DISABLE_TTF)
		if(ENABLE_INTERNAL_FONT)
			add_definitions(/DUSE_NEW_TTF)
			set(USE_NEW_TTF 1)
		endif(ENABLE_INTERNAL_FONT)
	else(NOT DISABLE_TTF)
		add_definitions(/DOL_NO_TTF)
		set(OL_NO_TTF 1)
	endif(NOT DISABLE_TTF)
	if(DISABLE_PNG)
		add_definitions(/DOL_NO_PNG)
		set(OL_NO_PNG 1)
	endif(DISABLE_PNG)
	if(DISABLE_OLD_API)
		add_definitions(/DOL_NO_OLD_API)
		set(OL_NO_OLD_API 1)
	endif(DISABLE_OLD_API)
	if(DISABLE_STATE_CHANGES)
		add_definitions(/DOL_NO_STATE_CHANGE)
		set(OL_NO_STATE_CHANGE 1)
	endif(DISABLE_STATE_CHANGES)
	
	if(NOT ENABLE_INTERNAL_FONT)
		add_subdirectory(utils/glyphkeeper)
		set(GLYPHKEEPER_LIB glyph-agl)
	endif(NOT ENABLE_INTERNAL_FONT)
	
	set(WIN_LIBS /user32 /gdi32) 
endif(MSVC)

# -------------------------------------------------------
# Added stuff for osx
# -------------------------------------------------------
if(APPLE)
	set(WIN_LIBS "-framework Carbon")
endif(APPLE)

# -------------------------------------------------------
# Setup some vars that might cause dep issues
# -------------------------------------------------------
if(DISABLE_PNG)
	set(PNG_LIB "")
else(DISABLE_PNG)
	set(PNG_LIB ${PNG_LIBRARY})
endif(DISABLE_PNG)
if(DISABLE_TTF)
	set(FREETYPE_LIB "")
	set(FREETYPE_INCLUDE "")
	set(ZLIB_LIB "")
else(DISABLE_TTF)
	set(FREETYPE_LIB ${FREETYPE_LIBRARY})
	set(FREETYPE_INCLUDE ${FREETYPE_INCLUDE_DIR})
	set(ZLIB_LIB ${ZLIB_LIBRARY})
endif(DISABLE_TTF)	

# -------------------------------------------------------
# Create the necessary header file for the library
# -------------------------------------------------------
configure_file(build/OpenLayer.hpp.in ${CMAKE_BINARY_DIR}/include/OpenLayer.hpp)

# -------------------------------------------------------
# openlayer-config for *nix based systems
# -------------------------------------------------------
if(UNIX AND NOT CYGWIN)
	if(NOT DISABLE_TTF)
		if(NOT ENABLE_INTERNAL_FONT)
			set(R_GK -lglyph-agl)
		endif(NOT ENABLE_INTERNAL_FONT)
		set(R_FT -lfreetype)
	endif(NOT DISABLE_TTF)
	if(NOT DISABLE_PNG)
		set(R_PNG -lpng)
	endif(NOT DISABLE_PNG)
	set(REQUIRED_LIBS "-lagl ${R_PNG} ${R_GK} ${R_FT} -lz `allegro-config --libs` -lGL -lGLU")
	configure_file(build/openlayer-config.in ${CMAKE_BINARY_DIR}/bin/openlayer-config ESCAPE_QUOTES)
endif(UNIX AND NOT CYGWIN)

# -------------------------------------------------------
# Source directory containing all the necessary .cpp files
# -------------------------------------------------------
aux_source_directory(src SOURCES)

# -------------------------------------------------------
# Include directory
# -------------------------------------------------------
if(UNIX AND NOT CYGWIN)
  set(FREETYPE_INCLUDE "/usr/include/freetype2")
endif(UNIX AND NOT CYGWIN)

include_directories(include ${CMAKE_BINARY_DIR}/include include/OpenLayer ${FREETYPE_INCLUDE})

# -------------------------------------------------------
# OpenLayer Library
# -------------------------------------------------------
add_library (openlayer ${CREATE_STATIC_LIB} ${SOURCES})
if(UNIX AND NOT CYGWIN)
  SET_TARGET_PROPERTIES(
    openlayer
    PROPERTIES
    SOVERSION 2
    VERSION 2.1.0
    )
endif(UNIX AND NOT CYGWIN)

# -------------------------------------------------------
# Create the library
# -------------------------------------------------------
target_link_libraries(openlayer ${ALLEGROGL_LIBRARY} ${PNG_LIB} ${GLYPHKEEPER_LIB} ${FREETYPE_LIB} ${ZLIB_LIB} ${ALLEGRO_LIBRARY} ${OPENGL_LIBRARIES} ${WIN_LIBS})

# -------------------------------------------------------
# Demos
# -------------------------------------------------------
add_subdirectory(${CMAKE_SOURCE_DIR}/demos)

# -------------------------------------------------------
# Installation
# -------------------------------------------------------
if(NOT MSVC)
	if(NOT ${CMAKE_INSTALL_PREFIX} AND ${CMAKE_INSTALL_PREFIX} STREQUAL "")
		message(FATAL_ERROR "You need to set the base location where to install OpenLayer (ie /usr/local)")
	endif(NOT ${CMAKE_INSTALL_PREFIX} AND ${CMAKE_INSTALL_PREFIX} STREQUAL "")
	install(FILES ${CMAKE_BINARY_DIR}/include/OpenLayer.hpp DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
	install(DIRECTORY include/OpenLayer DESTINATION ${CMAKE_INSTALL_PREFIX}/include PATTERN ".svn" EXCLUDE PATTERN "*~" EXCLUDE)
	install(TARGETS openlayer DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
	if(UNIX AND NOT CYGWIN)
		install(PROGRAMS ${CMAKE_BINARY_DIR}/bin/openlayer-config DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
	endif(UNIX AND NOT CYGWIN)
endif(NOT MSVC)
