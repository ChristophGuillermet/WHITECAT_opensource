#include "GlDriver.hpp"
#include "General.hpp"
#include "GfxRend.hpp"
#include "TextureInfo.hpp"


using namespace ol;


// CONSTANTS //

const static int FILENAME_BUF_SIZE = 512;


// STATIC CLASS VARIABLES //

static AllegroGLDriver defaultDriver;


GlDriver *GlDriver::activeDriver = &defaultDriver;
const std::string GlDriver::TEXTURE_NOT_POWER_OF_TWO_EXT = "GL_ARB_texture_non_power_of_two";


// GENERAL FUNCTIONS //


bool AllegroGLDriver::
SetupProgram( bool setupKeyboard, bool setupMouse, bool setupTimer ) {
   if( allegro_init() != 0 ) return false;

   if( install_allegro_gl() != 0 ) return false;

   if( setupKeyboard ) {
      if( install_keyboard() != 0 ) return false;
   }

   if( setupTimer || setupMouse ) {
      if( install_timer() != 0 ) return false;
   }

   if( setupMouse ) {
      if( install_mouse() == -1 ) return false;
   }

   return true;
}



bool AllegroGLDriver::
SetupScreen( int w, int h, bool fullscreen, int colorDepth, int zDepth, int refreshRate ) {
   allegro_gl_clear_settings();
	allegro_gl_set( AGL_COLOR_DEPTH, colorDepth );
	allegro_gl_set( AGL_DOUBLEBUFFER, 1 );
	allegro_gl_set( AGL_Z_DEPTH, zDepth );
	allegro_gl_set( AGL_RENDERMETHOD, 1 );
	allegro_gl_set( AGL_WINDOWED, fullscreen? FALSE : TRUE );
	allegro_gl_set( AGL_SUGGEST, AGL_COLOR_DEPTH | AGL_DOUBLEBUFFER
	                          | AGL_RENDERMETHOD | AGL_Z_DEPTH | AGL_WINDOWED );

	if( refreshRate > 0 ) {
      request_refresh_rate( refreshRate );
   }

	if( set_gfx_mode( GFX_OPENGL, w, h, 0, 0) != 0 ) {
      std::string errorText = std::string( "Couldn't up the screen! (Width: ") + ToString( w )
                         + ", Height: " + ToString( h ) + ", "
                         + ((fullscreen)? "Fullscreen" : "Windowed")
                         + ", Color depth: " + ToString( colorDepth )
                         + ", Z Depth: "+ ToString( zDepth ) + ")";

      OlError( errorText );
      return false;
   }

   return true;
}



static inline void ExtractColor( unsigned char *&pixelPtr, int color, int format ) {
   *pixelPtr++ = getr( color );
   *pixelPtr++ = getg( color );
   *pixelPtr++ = getb( color );

   if( format == GL_RGBA ) {
      *pixelPtr++ = geta( color );
   }
}

/*
glEnum GlDriver::
GetBytesPerPixel( glEnum textureFormat, int colorDepth ) {
   int returnVal = 0;

   switch( colorDepth ) {
      case 32:
         switch( textureFormat ) {
            case GL_RGB:
               returnVal = 4;
              break;
            case GL_RGBA:
               returnVal = 3;
              break;
            default:
               OlError( "Unknown texture format in GetBytesPerPixel: " + ToString((int) textureFormat ));
         }
       break;
      case 16:
         switch( textureFormat ) {
            case GL_RGB:
               returnVal = 4;
              break;
            case GL_RGBA:
               returnVal = 3;
              break;
            default:
               OlError( "Unknown texture format in GetBytesPerPixel: " + ToString((int) textureFormat ));
         }
       break;
      default:
         OlError( "Unknown color depth in GetBytesPerPixel: " + ToString( colorDepth ));
   }
}
*/


Rgba AllegroGLDriver::
GetPixel( OL_MEMORY_IMG *img, int x, int y ) {
   int pixel = getpixel( img, x, y );
   return Rgba( getr( pixel ), getg( pixel ), getb( pixel ), geta( pixel ));
}



OlTextureInfo AllegroGLDriver::
UploadTexture( OL_MEMORY_IMG *bmp, bool isSolid ) {
   isSolid = false;
   bool useTemp = false;

   int bmpW = bmp->w;
   int bmpH = bmp->h;

   int textureW = bmpW;
   int textureH = bmpH;

   if( !IsExtensionAlvailable( TEXTURE_NOT_POWER_OF_TWO_EXT )) {
      int bmpW = bmp->w;
      int bmpH = bmp->h;

      textureW = ToNextPowOfTwo( bmpW );
      textureH = ToNextPowOfTwo( bmpH );
   }

   bool expandW = textureW != bmpW;
   bool expandH = textureH != bmpH;

   bool useExtendedDimensions = expandH || expandW;

   int format = isSolid? GL_RGB : GL_RGBA;
   int bpp = isSolid? 3 : 4;
   int bitmapBpp = bitmap_color_depth( bmp )/8;

   /*
   unsigned char *data = new unsigned char[textureW * textureH * bpp];
   unsigned char *pixelPtr = data;
   */

   unsigned char *data = new unsigned char[textureW * textureH * bpp];
   //float *data = new float[textureW * textureH * bpp];

   //char buffer[100];

   unsigned char *imageStart = data + bpp * (textureH - bmpH) * textureW;
   unsigned char *pixelPtr = imageStart;

   for( int y = bmpH-1; y >= 0; y-- ) {
      for( int x = 0; x < bmpW; x++ ) {
         int color = getpixel( bmp, x, y );
         ExtractColor( pixelPtr, color, format );
      }

      pixelPtr += bpp * (textureW - bmpW);
   }

   if( expandW ) {
      for( int y = 0; y < bmpH; y++ ) {
         int color = getpixel( bmp, bmpW-1, y );
         pixelPtr = imageStart + bpp * (y * textureW + bmpW);
         ExtractColor( pixelPtr, color, format );
      }
   }

   if( expandH ) {
      int lastLine = bmpH - 1;
      pixelPtr = imageStart - bpp * textureW;
      for( int x = 0; x < bmpW+1; x++ ) {
         int color = getpixel( bmp, x, lastLine );
         ExtractColor( pixelPtr, color, format );
      }
   }

   OlTextureInfo textureInfo( textureW, textureH, bmp->w, bmp->h, format );

   GLuint index = 0;
   glGenTextures( 1, &index );
   glBindTexture( GL_TEXTURE_2D, index );

   glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_NEAREST );

   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

   glTexImage2D( GL_TEXTURE_2D, 0, bpp, textureW, textureH, 0, format, GL_UNSIGNED_BYTE, data );
   
   delete[] data;
   
   /*glTexImage2D( GL_TEXTURE_2D, 0, 4, textureInfo.texWidth, textureInfo.texHeight,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, data );*/
   textureInfo.SetIndex( index );

   return textureInfo;
}

/*

OlTextureInfo AllegroGLDriver::
UploadTexture( OL_MEMORY_IMG *bmp, bool isSolid ) {
   OL_MEMORY_IMG *usedBmp = bmp;
   bool useTemp = false;

   int textureH = bmp->h;
   int textureW = bmp->w;

   if( !IsExtensionAlvailable( TEXTURE_NOT_POWER_OF_TWO_EXT )) {
      int bmpW = bmp->w;
      int bmpH = bmp->h;

      textureW = ToNextPowOfTwo( bmpW );
      textureH = ToNextPowOfTwo( bmpH );

      useTemp = textureW != bmpW || textureH != bmpH;

      if( useTemp ) {
         int tempW = textureW + 1;
         int tempH = textureH + 1;

         OL_MEMORY_IMG *temp = create_bitmap_ex( bitmap_color_depth( bmp ), tempW, tempH );

         blit( bmp, temp, 0, 0, 0, 0, bmpW, bmpH );
         blit( bmp, temp, bmp->w-1, 0, bmp->w, 0, 1, bmp->h );
         blit( bmp, temp, 0, bmp->h-1, 0, bmp->h, bmp->w, 1 );

         usedBmp = temp;
      }
   }


   GLint maxTextureSize = 0;
   glGetIntegerv( GL_MAX_TEXTURE_SIZE, &maxTextureSize );

   int format = isSolid? GL_RGB : GL_RGBA;

   OlTextureInfo textureInfo( textureW, textureH, bmp->w, bmp->h, format );


   if( isSolid ) {
      textureInfo.SetIndex( allegro_gl_make_texture_ex( AGL_TEXTURE_FLIP, usedBmp, format )); // AGL_TEXTURE_FLIP
   }
   else {
      textureInfo.SetIndex( allegro_gl_make_texture_ex( AGL_TEXTURE_FLIP | AGL_TEXTURE_HAS_ALPHA,
                                                        usedBmp, format ));
   }


   if( useTemp )
      destroy_bitmap( usedBmp );

   return textureInfo;
}
*/


OlTextureInfo AllegroGLDriver::
UploadTexture( GLfloat *data, OlTextureInfo textureInfo ) {
   glGenTextures( 1, &textureInfo.index );
   glBindTexture( GL_TEXTURE_2D, textureInfo.index );

   glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_NEAREST );

   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

   glTexImage2D( GL_TEXTURE_2D, 0, 4, textureInfo.texWidth, textureInfo.texHeight,
                 0, GL_RGBA, GL_FLOAT, data );

   return textureInfo;
}



std::string AllegroGLDriver::
GetExecutablePath() {
   char buf[FILENAME_BUF_SIZE];
   get_executable_name( buf, FILENAME_BUF_SIZE );
   char *namePos = get_filename( buf );
   *namePos = '\0';

   return buf;
}



std::string AllegroGLDriver::
ToAbsolutePathname( std::string pathname ) {
   if( is_relative_filename( pathname.c_str())) {
      char buf[FILENAME_BUF_SIZE];
      make_absolute_filename( buf, GetExecutablePath().c_str(),
                              pathname.c_str(), FILENAME_BUF_SIZE );

      return buf;
   }
   else {
      return pathname;
   }
}



OL_MEMORY_IMG *AllegroGLDriver::
GetMemoryBitmap( int width, int height, int bpp, unsigned char *pixelData ) {
   int colorDepth = 8 * bpp;

   OL_MEMORY_IMG *img = create_bitmap_ex( colorDepth, width, height );

   int bytesPerLine = width * bpp;

   unsigned char *pixelPtr = pixelData;

   if( bpp != 3 && bpp != 4 ) {
      OlError( "Unsupported bpp in GetMemoryBitmap: " + bpp );
      return img;
   }

   // The retrieved texture is naturally upside down //

   for( int y = img->h-1; y >= 0; y-- ) {
      for( int x = 0; x < img->w; x++ ) {
         //unsigned int col = *((unsigned int *) pixelPtr );

         int r = *pixelPtr++;
         int g = *pixelPtr++;
         int b = *pixelPtr++;

         unsigned int col;

         if( bpp == 4 ) {
            int a = *pixelPtr++;
            col = makeacol_depth( colorDepth, r, g, b, a );
         }
         else {
            col = makecol_depth( colorDepth, r, g, b );
         }

         // putpixel is guranteed to work with the bitmap's color depth //
         putpixel( img, x, y, col );
      }
   }

   return img;
}



void GlDriver::
Set( GlDriver *driver ) {
   if( driver )
      activeDriver = driver;
   else
      OlError( "Tried to select a null GlDriver!" );
}



GlDriver *GlDriver::
Get() {
   return activeDriver;
}



bool GlDriver::
IsExtensionAlvailable( std::string extensionName ) {
   char *extensionsChars = (char *) glGetString( GL_EXTENSIONS );

   if( !extensionsChars ) {
      OlLog( "No OpenGL extensions were found." );
      return false;
   }


   std::string extensions = extensionsChars;

   std::string::size_type nameStart = 0;
   std::string::size_type spaceIndex = 0;

   while(( spaceIndex = extensions.find( " ", nameStart ))
            != std::string::npos) {
      std::string currentExtension = extensions.substr( nameStart, spaceIndex - nameStart );

      //TRACE( "%s\n", currentExtension.c_str() );

      if( currentExtension == extensionName ) {
         return true;
      }

      nameStart = spaceIndex + 1;
   }

   //OlLog( std::string( "OpenGL extension " ) + extensionName + " was not found.");
   return false;
}



int AllegroGLDriver::
GetImageColorDepth( OL_MEMORY_IMG *bmp ) {
   return bitmap_color_depth( bmp );
}



bool AllegroGLDriver::
SaveMemoryBitmap( OL_MEMORY_IMG *bmp, std::string filename ) {

   //save_bitmap returns non-zero on error - translate to a truth value for our
   //GLDriver function
   if( save_bitmap( filename.c_str(), bmp, 0 ))
		return false;
	return true;
}



void AllegroGLDriver::
DestroyMemoryBitmap( OL_MEMORY_IMG *bmp ) {
   if( bmp ) {
      destroy_bitmap( bmp );
   }
   else {
      OlError( "Tried to destroy a NULL BITMAP!" );
   }
}




