# -------------------------------------------------------
# GlyphKeeper cmake build script if not using internal
# renderer
# Written by: juvinious
# -------------------------------------------------------

# -------------------------------------------------------
# project name
# -------------------------------------------------------
project (GlyphKeeper)

# -------------------------------------------------------
# Including needed macros
# -------------------------------------------------------
find_package(FREETYPE REQUIRED)
find_package(ALLEGROGL REQUIRED)

if(NOT MSVC)
	add_definitions(-DGK_NO_LEGACY -DGLYPH_TARGET=GLYPH_TARGET_ALLEGGL)
else(NOT MSVC)
	add_definitions(/DGK_NO_LEGACY /DGLYPH_TARGET=GLYPH_TARGET_ALLEGGL)
endif(NOT MSVC)


# -------------------------------------------------------
# Source directory containing all the necessary .cpp files
# -------------------------------------------------------
set(GK_SOURCES src/glyph.c src/glyph_global_vars.c src/glyph_rend.c src/glyph_to_allegro_mono.c src/glyph_dimensions.c src/glyph_index.c src/glyph_to_allegro_aa.c src/glyph_to_opengl.c src/glyph_face.c src/glyph_main.c src/glyph_to_allegro.c src/glyph_utils.c src/glyph_memory.c src/glyph_workout.c)

# -------------------------------------------------------
# Include directory
# -------------------------------------------------------
include_directories(include src ${FREETYPE_INCLUDE_DIR})

# -------------------------------------------------------
# OpenLayer Library
# -------------------------------------------------------
add_library (glyph-agl ${CREATE_STATIC_LIBRARY} ${GK_SOURCES})

# -------------------------------------------------------
# Create the library
# -------------------------------------------------------
target_link_libraries(glyph-agl)
configure_file(include/glyph.h ${CMAKE_BINARY_DIR}/include/glyph.h COPYONLY)

# -------------------------------------------------------
# Installation
# -------------------------------------------------------
if(NOT MSVC)
	install(FILES ${CMAKE_BINARY_DIR}/include/glyph.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
	install(TARGETS glyph-agl DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
endif(NOT MSVC)
