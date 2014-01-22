# -------------------------------------------------------
# OpenLayer cmake build script to create demos called from 
# main build
# Written by: juvinious
# -------------------------------------------------------

if(NOT ${CREATE_STATIC_LIB})
	set(OL_LIB_BUILD 0)
	if(MSVC)
		remove_definitions(/DOL_LIB_BUILD)
	else(MSVC)
		remove_definitions(-DOL_LIB_BUILD)
	endif(MSVC)
endif(NOT ${CREATE_STATIC_LIB})

# -------------------------------------------------------
# Create the examples
# -------------------------------------------------------
add_executable(collisiondemo ${CMAKE_SOURCE_DIR}/demos/collisiondemo/main.cpp)
target_link_libraries(collisiondemo openlayer ${ALLEGROGL_LIBRARY} ${PNG_LIB} ${GLYPHKEEPER_LIB} ${FREETYPE_LIB} ${ZLIB_LIB} ${ALLEGRO_LIBRARY} ${OPENGL_LIBRARIES} ${WIN_LIBS})
add_dependencies(collisiondemo openlayer)

add_executable(gamedemo ${CMAKE_SOURCE_DIR}/demos/gamedemo/Demo.cpp)
target_link_libraries(gamedemo openlayer ${ALLEGROGL_LIBRARY} ${PNG_LIB} ${GLYPHKEEPER_LIB} ${FREETYPE_LIB} ${ZLIB_LIB} ${ALLEGRO_LIBRARY} ${OPENGL_LIBRARIES} ${WIN_LIBS})
add_dependencies(gamedemo openlayer)
configure_file(${CMAKE_SOURCE_DIR}/demos/gamedemo/Fonts/Neuropol.ttf ${CMAKE_BINARY_DIR}/bin/Fonts/Neuropol.ttf COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/demos/gamedemo/Gfx/Background.png ${CMAKE_BINARY_DIR}/bin/Gfx/Background.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/demos/gamedemo/Gfx/Ball.png ${CMAKE_BINARY_DIR}/bin/Gfx/Ball.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/demos/gamedemo/Gfx/Blocks/Block01.png ${CMAKE_BINARY_DIR}/bin/Gfx/Blocks/Block01.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/demos/gamedemo/Gfx/Blocks/Block02.png ${CMAKE_BINARY_DIR}/bin/Gfx/Blocks/Block02.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/demos/gamedemo/Gfx/Blocks/Block03.png ${CMAKE_BINARY_DIR}/bin/Gfx/Blocks/Block03.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/demos/gamedemo/Gfx/Blocks/Block04.png ${CMAKE_BINARY_DIR}/bin/Gfx/Blocks/Block04.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/demos/gamedemo/Gfx/Paddle.png ${CMAKE_BINARY_DIR}/bin/Gfx/Paddle.png COPYONLY)

add_executable(gameloop ${CMAKE_SOURCE_DIR}/demos/gameloop/Main.cpp)
target_link_libraries(gameloop openlayer ${ALLEGROGL_LIBRARY} ${PNG_LIB} ${GLYPHKEEPER_LIB} ${FREETYPE_LIB} ${ZLIB_LIB} ${ALLEGRO_LIBRARY} ${OPENGL_LIBRARIES} ${WIN_LIBS})
add_dependencies(gameloop openlayer)
configure_file(${CMAKE_SOURCE_DIR}/demos/gameloop/Gfx/Bitmap.png ${CMAKE_BINARY_DIR}/bin/Gfx/Bitmap.png COPYONLY)

add_executable(linestripdemo ${CMAKE_SOURCE_DIR}/demos/linestripdemo/Main.cpp)
target_link_libraries(linestripdemo openlayer ${ALLEGROGL_LIBRARY} ${PNG_LIB} ${GLYPHKEEPER_LIB} ${FREETYPE_LIB} ${ZLIB_LIB} ${ALLEGRO_LIBRARY} ${OPENGL_LIBRARIES} ${WIN_LIBS})
add_dependencies(linestripdemo openlayer)

add_executable(renderbitmap ${CMAKE_SOURCE_DIR}/demos/renderbitmap/Main.cpp)
target_link_libraries(renderbitmap openlayer ${ALLEGROGL_LIBRARY} ${PNG_LIB} ${GLYPHKEEPER_LIB} ${FREETYPE_LIB} ${ZLIB_LIB} ${ALLEGRO_LIBRARY} ${OPENGL_LIBRARIES} ${WIN_LIBS})
add_dependencies(renderbitmap openlayer)

add_executable(shapedemo ${CMAKE_SOURCE_DIR}/demos/shapedemo/Main.cpp)
target_link_libraries(shapedemo openlayer ${ALLEGROGL_LIBRARY} ${PNG_LIB} ${GLYPHKEEPER_LIB} ${FREETYPE_LIB} ${ZLIB_LIB} ${ALLEGRO_LIBRARY} ${OPENGL_LIBRARIES} ${WIN_LIBS})
add_dependencies(shapedemo openlayer)
configure_file(${CMAKE_SOURCE_DIR}/demos/shapedemo/Gfx/PointerAlpha.bmp ${CMAKE_BINARY_DIR}/bin/Gfx/PointerAlpha.bmp COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/demos/shapedemo/Gfx/PointerAlpha.png ${CMAKE_BINARY_DIR}/bin/Gfx/PointerAlpha.png COPYONLY)

add_executable(textdemo ${CMAKE_SOURCE_DIR}/demos/textdemo/Main.cpp)
target_link_libraries(textdemo openlayer ${ALLEGROGL_LIBRARY} ${PNG_LIB} ${GLYPHKEEPER_LIB} ${FREETYPE_LIB} ${ZLIB_LIB} ${ALLEGRO_LIBRARY} ${OPENGL_LIBRARIES} ${WIN_LIBS})
add_dependencies(textdemo openlayer)