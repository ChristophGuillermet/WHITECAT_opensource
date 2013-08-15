#ifndef OL_INCLUDES_HPP
#define OL_INCLUDES_HPP

#include "Declspec.hpp"

/*
#if (OL_DRIVER == SDL)
   
   #define SCREEN_W "notspecified"
   #define SCREEN_H "notspecified"
   
   #define getr32
   #define getg32
   #define getb32
   
   #define OL_MEMORY_IMG void
   
#else // (OL_DRIVER == ALLEGRO_GL)
*/
   #include <allegro.h>
   #include <alleggl.h>
   
   #define OL_MEMORY_IMG BITMAP
   
   inline OL_LIB_DECLSPEC int OlGetAlpha( OL_MEMORY_IMG *bitmap, int x, int y ) {
      return geta32( _getpixel32( bitmap, x, y ));
   }
   
//#endif // OL_DRIVER


#include <string>

#endif // OL_INCLUDES_HPP
