#include "Bitmap.hpp"
#include "Transforms.hpp"
#include "Settings.hpp"
#include "Setup.hpp"
#include "Internal.hpp"
#include "GlDriver.hpp"
#include "General.hpp"
#include "Canvas.hpp"
#include "Rectangle.hpp"
#include "Framebuffer.hpp"
#include "Polygon.hpp"


#include <vector>
#include <sstream>
#include <math.h>


using namespace ol;
using namespace std;


static const int OL_BYTES_PER_TEXTURE_PIXEL = 4;
static const float MAX_POLY_SKIP_PER_AREA = 0.0015;


// HELPER FUNCTIONS //


static inline void SelectBlittingColor( float fact ) {
   #ifdef NO_COLOR_CHANNELS
      glColor4f( 1.0, 1.0, 1.0, fact );
   #else // NO_COLOR_CHANNELS
      Rgba channels = Transforms::GetColorChannels();
      glColor4f( channels.r, channels.g, channels.b, channels.a * fact );
   #endif // NO_COLOR_CHANNELS
}



static inline void SelectBlittingColor( const Rgba &col, float fact ) {
   #ifdef NO_COLOR_CHANNELS
      glColor4f( col.r, col.g, col.b, col.a * fact );
   #else // NO_COLOR_CHANNELS
      Rgba channels = Transforms::GetColorChannels();
      glColor4f( channels.r * col.r, channels.g * col.g, channels.b * col.b, channels.a * col.a * fact );
   #endif // NO_COLOR_CHANNELS
}


// GENERAL FUNCTIONS //


Bitmap::
Bitmap( int w, int h )
      : bmp( 0 ), collisionPoly( 0 ), destroyBmp( true ), pendingLoad( NULL ),
      pivot( Vec2D( 0.0, 0.0 )), useDefaultPivot( true ), isSubBitmap( false ) {
   collisionPoly = 0;

   AddToCollection();

   OlLog( string( "Creating an empty Bitmap, width: " ) + ToString( w ) +
      " height: " + ToString( h ) );

   int textureWidth = ToNextPowOfTwo(w);
   int textureHeight = ToNextPowOfTwo(h);

   float *data = new float[4 * textureWidth * textureHeight];
   Load( w, h, textureWidth, textureHeight, data );
   delete[] data;
}



Bitmap::
Bitmap( int w, int h, Rgba fillColor )
      : bmp( 0 ), collisionPoly( 0 ), destroyBmp( true ), pendingLoad( NULL ),
      pivot( Vec2D( 0.0, 0.0 )), useDefaultPivot( true ), isSubBitmap( false ) {
   collisionPoly = 0;

   AddToCollection();

   OlLog( string( "Creating an empty Bitmap, width: " ) + ToString( w ) + " height: " + ToString( h )
      + " Color: ( " + ToString( fillColor.r ) + ", " + ToString( fillColor.g ) + ", "
      + ToString( fillColor.b ) + " )" );

   int textureWidth = ToNextPowOfTwo(w);
   int textureHeight = ToNextPowOfTwo(h);

   int dataSize = 4 * textureWidth * textureHeight;

   GLfloat *data = new GLfloat[dataSize];
   GLfloat *dataEnd = data + dataSize;

   for( GLfloat *iter = data; iter != dataEnd; ) {
      *(iter++) = fillColor.r;
      *(iter++) = fillColor.g;
      *(iter++) = fillColor.b;
      *(iter++) = fillColor.a;
   }

   Load( w, h, textureWidth, textureHeight, data );
   delete[] data;
}


Bitmap::
Bitmap( const Bitmap &other, float x, float y, float width, float height )
      : bmp( 0 ), collisionPoly( 0 ), destroyBmp( false ), pendingLoad( NULL ),
      pivot( Vec2D( 0.0, 0.0 )), useDefaultPivot( true ), isSubBitmap( true ) {
   collisionPoly = 0;

   AddToCollection();

   textureInfo = other.textureInfo;
   /*printf( "TexInfo1 rect: %f %f %f %f\n", textureInfo.rect.x, textureInfo.rect.y, textureInfo.rect.w,
      textureInfo.rect.h );

   printf( "TexInfo1 dims: %d %d %d %d\n", textureInfo.texWidth, textureInfo.texHeight, textureInfo.imgWidth,
      textureInfo.imgHeight );*/

   textureInfo = OlTextureInfo( other.textureInfo, OlRect( x, y, width, height ));
   /*printf( "TexInfo2 rect: %f %f %f %f\n", textureInfo.rect.x, textureInfo.rect.y, textureInfo.rect.w,
      textureInfo.rect.h );

   printf( "TexInfo2 dims: %d %d %d %d\n", textureInfo.texWidth, textureInfo.texHeight, textureInfo.imgWidth,
      textureInfo.imgHeight );*/
}


Bitmap::
Bitmap( const Bitmap &other, const Rect &area )
      : bmp( 0 ), collisionPoly( 0 ), destroyBmp( false ), pendingLoad( NULL ),
      pivot( Vec2D( 0.0, 0.0 )), useDefaultPivot( true ), isSubBitmap( true ) {
   collisionPoly = 0;

   AddToCollection();
   textureInfo = other.textureInfo;
   textureInfo = OlTextureInfo( other.textureInfo, OlRect( area.pos.x, area.pos.y, area.size.x, area.size.y ));
}


void Bitmap::
Load( int w, int h, int textureWidth, int textureHeight, GLfloat *data, GLenum format ) {
   OlAssert( w > 0 && h > 0 && textureWidth > 0 && textureHeight > 0 && data );

   OlTextureInfo info( textureWidth, textureHeight, w, h, format );
   textureInfo = GlDriver::Get()->UploadTexture( data, info );

   Select();
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
   
   if( Settings::useAntiAlias ) {
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
   }
   else {
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   }
}



void Bitmap::
CopyFromCanvas( int x, int y ) {
   int w = Width();
   int h = Height();

   if( x < 0 ) {
      w += x;
      x = 0;
   }

   if( y < 0 ) {
      h += y;
      y = 0;
   }

   if( x + w > Canvas::Width() ) {
      w = Canvas::Width() - x;
   }

   if( y + h > Canvas::Height() ) {
      h = Canvas::Height() - y;
   }

   Select();

   glFlush();
   glFinish();

   FrameBuffer::GetInstance().CopyTexSubImage( x, y, w, h, textureInfo.texHeight - Height() );
}



enum {
  TOPLEFT_COL_INDEX = 0,
  TOPRIGHT_COL_INDEX = 1,
  BOTTOMRIGHT_COL_INDEX = 2,
  BOTTOMLEFT_COL_INDEX = 3
};



void Bitmap::
TexturedQuad( float w, float h, float fact, const RenderMode &mode ) const {
   GetReadyToRender( fact );

   OlRect renderArea = mode.GetRenderRect( OlRect( 0.0, 0.0, Width(), Height() ));
   
   renderArea.w *= w/Width();
   renderArea.h *= h/Height();
   
   OlTexCoords texCoordArea = mode.GetTexCoords( OlTexCoords( textureInfo ));

   texCoordArea.CalculateTextureCoords();

   bool setsTexCoords = mode.SetsTexCoords();

   glBegin( GL_QUADS );
      if( texCoordArea.ChangesColors())
         SelectBlittingColor( texCoordArea.GetColor(BOTTOMLEFT_COL_INDEX), fact );

      if( setsTexCoords )
         mode.SetTexCoord( OL_TC_BOTTOMLEFT, renderArea, texCoordArea, w, h );
      else
         RenderMode::PrimarySetTexCoord( OL_TC_BOTTOMLEFT, renderArea, texCoordArea, w, h );

      glVertex2f( renderArea.x, renderArea.y + renderArea.h );

      if( texCoordArea.ChangesColors())
         SelectBlittingColor( texCoordArea.GetColor(BOTTOMRIGHT_COL_INDEX), fact );

      if( setsTexCoords )
         mode.SetTexCoord( OL_TC_BOTTOMRIGHT, renderArea, texCoordArea, w, h );
      else
         RenderMode::PrimarySetTexCoord( OL_TC_BOTTOMRIGHT, renderArea, texCoordArea, w, h );

      glVertex2f( renderArea.x + renderArea.w, renderArea.y + renderArea.h );


      if( texCoordArea.ChangesColors())
         SelectBlittingColor( texCoordArea.GetColor(TOPRIGHT_COL_INDEX), fact );

      if( setsTexCoords )
         mode.SetTexCoord( OL_TC_TOPRIGHT, renderArea, texCoordArea, w, h );
      else
         RenderMode::PrimarySetTexCoord( OL_TC_TOPRIGHT, renderArea, texCoordArea, w, h );

      glVertex2f( renderArea.x + renderArea.w, renderArea.y );


      if( texCoordArea.ChangesColors())
         SelectBlittingColor( texCoordArea.GetColor(TOPLEFT_COL_INDEX), fact );

      if( setsTexCoords )
         mode.SetTexCoord( OL_TC_TOPLEFT, renderArea, texCoordArea, w, h );
      else
         RenderMode::PrimarySetTexCoord( OL_TC_TOPLEFT, renderArea, texCoordArea, w, h );

      glVertex2f( renderArea.x, renderArea.y );
   glEnd();
}




void Bitmap::
TexturedQuad( float w, float h, float fact ) const {
   GetReadyToRender( fact );

   textureInfo.OutputTexturedQuad( w, h );

   if( !Settings::TextureMappingUsed() )
      glDisable( GL_TEXTURE_2D );
}



void Bitmap::
StartFastBlitting( float fact ) {
   SelectBlittingColor( fact );
   glEnable( GL_TEXTURE_2D );
   //OlTextureInfo::StartRendering();
}



void Bitmap::
FinishFastBlitting() {
   //OlTextureInfo::FinishRendering();

   if( !Settings::TextureMappingUsed() )
      glDisable( GL_TEXTURE_2D );
}



void Bitmap::
Blit( float x, float y, const RenderMode &mode, float fact ) const {
   mode.Select();

   glPushMatrix();

   glTranslatef( x, y, 0.0 );

   TexturedQuad( textureInfo.imgWidth, textureInfo.imgHeight, fact, mode );

   glPopMatrix();

   mode.Unselect();

   if( !Settings::TextureMappingUsed() )
      glDisable( GL_TEXTURE_2D );
}



void Bitmap::
BlitRotated( float x, float y, float angle, const RenderMode &mode, float fact ) const {
   BlitRotated( x, y, pivot.x, pivot.y, angle, mode, fact );
}



void Bitmap::
BlitRotated( float x, float y, float pivotX, float pivotY, float angle, const RenderMode &mode,
             float fact ) const {
   mode.Select();

   glPushMatrix();

   glTranslatef( x, y, 0.0 );

   RotateMatrix( angle );

   glTranslatef( -pivotX, -pivotY, 0.0 );

   TexturedQuad( textureInfo.imgWidth, textureInfo.imgHeight, fact, mode );

   glPopMatrix();

   mode.Unselect();

   if( !Settings::TextureMappingUsed() )
      glDisable( GL_TEXTURE_2D );
}


void Bitmap::
BlitStretched( float x, float y, float w, float h, const RenderMode &mode,
             float fact ) const {
   mode.Select();

   glPushMatrix();

   glTranslatef( x, y, 0.0 );

   TexturedQuad( w, h, fact, mode );

   glPopMatrix();

   mode.Unselect();

   if( !Settings::TextureMappingUsed() )
      glDisable( GL_TEXTURE_2D );
}



void Bitmap::
BlitTransformed( float x, float y, float w, float h, float angle, const RenderMode &mode,
             float fact ) const {
   BlitTransformed( x, y, w, h, pivot.x, pivot.y, angle, mode, fact );
}



void Bitmap::
BlitTransformed( float x, float y, float w, float h, float pivotX, float pivotY, float angle,
                 const RenderMode &mode, float fact ) const {
   mode.Select();

   glPushMatrix();

   glTranslatef( x, y, 0.0 );

   RotateMatrix( angle );

   glTranslatef( -pivotX * w / textureInfo.imgWidth, -pivotY * h / textureInfo.imgHeight, 0.0 );

   TexturedQuad( w, h, fact, mode );

   glPopMatrix();

   mode.Unselect();

   if( !Settings::TextureMappingUsed() )
      glDisable( GL_TEXTURE_2D );
}



void Bitmap::
BlitDistorted( float x1, float y1, float x2, float y2, float x3, float y3,
               float x4, float y4, const RenderMode &mode, float fact ) const {
   mode.Select();

   GetReadyToRender( fact );

   OlRect renderArea = mode.GetRenderRect( OlRect( 0.0, 0.0, textureInfo.imgWidth, textureInfo.imgHeight ));
   OlTexCoords texCoordArea = mode.GetTexCoords( OlTexCoords( textureInfo ));

   texCoordArea.CalculateTextureCoords();

   bool setsTexCoords = mode.SetsTexCoords();

   float w = textureInfo.imgWidth;
   float h = textureInfo.imgHeight;

   glBegin( GL_QUADS );
      if( setsTexCoords )
         mode.SetTexCoord( OL_TC_BOTTOMLEFT, renderArea, texCoordArea, w, h );
      else
         RenderMode::PrimarySetTexCoord( OL_TC_BOTTOMLEFT, renderArea, texCoordArea, w, h );

      glVertex2f( x4, y4 );

      if( setsTexCoords )
         mode.SetTexCoord( OL_TC_BOTTOMRIGHT, renderArea, texCoordArea, w, h );
      else
         RenderMode::PrimarySetTexCoord( OL_TC_BOTTOMRIGHT, renderArea, texCoordArea, w, h );

      glVertex2f( x3, y3 );


      if( setsTexCoords )
         mode.SetTexCoord( OL_TC_TOPRIGHT, renderArea, texCoordArea, w, h );
      else
         RenderMode::PrimarySetTexCoord( OL_TC_TOPRIGHT, renderArea, texCoordArea, w, h );

      glVertex2f( x2, y2 );

      if( setsTexCoords )
         mode.SetTexCoord( OL_TC_TOPLEFT, renderArea, texCoordArea, w, h );
      else
         RenderMode::PrimarySetTexCoord( OL_TC_TOPLEFT, renderArea, texCoordArea, w, h );

      glVertex2f( x1, y1 );
   glEnd();

   mode.Unselect();

   if( !Settings::TextureMappingUsed() )
      glDisable( GL_TEXTURE_2D );
}




void Bitmap::
GetReadyToRender( float fact ) const {
   SelectBlittingColor( fact );
   glEnable( GL_TEXTURE_2D );
   Select();
}



void Bitmap::
Blit( float x, float y, float fact ) const {
   glPushMatrix();

   glTranslatef( x, y, 0.0 );

   TexturedQuad( textureInfo.imgWidth, textureInfo.imgHeight, fact );

   glPopMatrix();
}



void Bitmap::
SetDefaultPivot( Vec2D point ) {
   pivot = point;
   useDefaultPivot = false;
}



void Bitmap::
BlitRotated( float x, float y, float angle, float fact ) const {
   BlitRotated( x, y, pivot.x, pivot.y, angle, fact );
}



void Bitmap::
BlitRotated( float x, float y, float pivotX, float pivotY, float angle, float fact ) const {
   glPushMatrix();

   glTranslatef( x, y, 0.0 );

   RotateMatrix( angle );

   glTranslatef( -pivotX, -pivotY, 0.0 );

   TexturedQuad( textureInfo.imgWidth, textureInfo.imgHeight, fact );

   glPopMatrix();
}



void Bitmap::
BlitStretched( float x, float y, float w, float h, float fact ) const {
   glPushMatrix();

   glTranslatef( x, y, 0.0 );

   TexturedQuad( w, h, fact );

   glPopMatrix();
}



void Bitmap::
BlitTransformed( float x, float y, float w, float h, float angle, float fact ) const {
   BlitTransformed( x, y, w, h, pivot.x, pivot.y, angle, fact );
}



void Bitmap::
BlitTransformed( float x, float y, float w, float h, float pivotX, float pivotY,
                 float angle, float fact ) const {

   glPushMatrix();

   glTranslatef( x, y, 0.0 );

   RotateMatrix( angle );

   glTranslatef( -pivotX * w / textureInfo.imgWidth, -pivotY * h / textureInfo.imgHeight, 0.0 );

   TexturedQuad( w, h, fact );

   glPopMatrix();
}



void Bitmap::
BlitDistorted( float x1, float y1, float x2, float y2, float x3, float y3, float x4,
               float y4, float fact ) const {

   GetReadyToRender( fact );
   textureInfo.OutputTexturedQuad( Vec2D( x1, y1 ), Vec2D( x2, y2 ), Vec2D( x3, y3 ),
                                   Vec2D( x4, y4 ));
}



GarbageCollection &OlGetCollection();


void Bitmap::
Destroy( bool eraseFromCollection ) {
   UnloadFromGPU();

   if( bmp && destroyBmp ) {
      destroy_bitmap( bmp );
      bmp = 0;
   }

   if( collisionPoly ) {
      delete collisionPoly;
      collisionPoly = 0;
   }

   if( pendingLoad ) {
      delete pendingLoad;
      pendingLoad = 0;
   }

   if( eraseFromCollection ) {
      OlGetCollection().Remove( this );
   }
}



void Bitmap::
UnloadFromGPU() {
   if( textureInfo.index != 0 && !isSubBitmap ) {
      glDeleteTextures( 1, &textureInfo.index );
      textureInfo.index = 0;
   }
}



bool Bitmap::
IsValid() const {
   return textureInfo.index != 0;
}



bool Bitmap::
HasAlphaChannel() const {
   return textureInfo.HasAlphaChannel();
}



void Bitmap::
SelectDefaultPivot( int imageWidth, int imageHeight ) {
   if( useDefaultPivot ) {
      pivot.x = imageWidth/2;
      pivot.y = imageHeight/2;
   }
}



bool Bitmap::
Load( OL_MEMORY_IMG *bmp, int conversionMode, int extraFlags ) {
   collisionPoly = 0;
   return Load( bmp, conversionMode & HAS_ALPHA_CHANNEL, conversionMode & CONVERT_MAGIC_PINK,
                extraFlags );
}



bool Bitmap::
Load( OL_MEMORY_IMG *toBmp, bool hasAlphaChannel, bool convertMagicPink, int extraFlags ) {
   collisionPoly = 0;
   Destroy();

   ASSERT( toBmp );

   isSolid = !hasAlphaChannel;

   int width = toBmp->w;
   int height = toBmp->h;

   OlLog( string("Loading bitmap from an Allegro OL_MEMORY_IMG (Alpha channel: ")
      + string( hasAlphaChannel? "Yes" : "No" ) + string(", Convert Magic Pink: ")
      + string( convertMagicPink? "Yes" : "No" ) + string(")"));

   if( convertMagicPink ) {
      isSolid = false;
      bool shouldBeConverted = bitmap_color_depth( toBmp ) != 32;

      OL_MEMORY_IMG *dst = shouldBeConverted? create_bitmap_ex( 32, width, height ) : toBmp;
      int maskColor = bitmap_mask_color( toBmp );

      int colorDepth = get_color_depth();
      set_color_depth( bitmap_color_depth( dst ));

      for( int y = 0; y < dst->h; y++ ) {
         for( int x = 0; x < dst->w; x++ ) {
            int color = getpixel( toBmp, x, y );
            int alpha = hasAlphaChannel? geta32( color ) : 255;

            bool isTransparent = color == maskColor;

            if( isTransparent ) {
               int maxY = min(y+1, dst->h-1);
               int maxX = min(x+1, dst->w-1);

               int r = 0;
               int g = 0;
               int b = 0;
               int colorMixes = 0;

               for( int subY = max(y-1, 0); subY <= maxY; subY++ ) {
                  for( int subX = max(x+1, 0); subX <= maxX; subX++ ) {
                     int subColor = getpixel( toBmp, subX, subY );
                     if( subColor != maskColor ) {
                        r += getr( subColor );
                        g += getg( subColor );
                        b += getb( subColor );
                     }
                  }
               }

               if( colorMixes > 0 ) {
                  r /= colorMixes;
                  g /= colorMixes;
                  b /= colorMixes;
               }

               color = makeacol32( r, g, b, 0 );
            }
            else {
               color = makeacol32( getr( color ), getg( color ), getb( color ), alpha );
            }
            ((unsigned int *) dst->line[y])[x] = color;
         }
      }

      set_color_depth( colorDepth );

      if( !shouldBeConverted ) {
         destroyBmp = false;
      }

      bmp = dst;
   }
   else {
      bmp = toBmp;
      destroyBmp = false;
   }

   SelectDefaultPivot( bmp->w, bmp->h );

   HandleExtraFlags( extraFlags );

   SendToGPU();

   return true;
}



static int olColorConversion = COLORCONV_16_TO_32 | COLORCONV_24_TO_32 | COLORCONV_15_TO_32 | COLORCONV_8_TO_32;


bool Bitmap::
Load( const char *rgbFilename, const char *alphaFilename, int extraFlags ) {
   collisionPoly = 0;
   Destroy();

   if( !Setup::IsScreenSetUp() ) {
      OlLog( string( "Loading queued for bitmap: " ) + rgbFilename +
             " with an alpha channel from: " + alphaFilename );

      pendingLoad = new PendingFileAlphaLoad( rgbFilename, alphaFilename );
      return true;
   }

   OlLog( string( "Loading bitmap: " ) + rgbFilename + " with an alpha channel from: " + alphaFilename );

   set_color_conversion( olColorConversion );
   OL_MEMORY_IMG *colordata = load_bitmap( Setup::ToAbsolutePathname( rgbFilename ).c_str(), 0 );

   set_color_conversion( COLORCONV_MOST );
   OL_MEMORY_IMG *alpha = load_bitmap( Setup::ToAbsolutePathname( alphaFilename ).c_str(), 0 );


   if( !colordata ) {
      OlError( "No such bitmap!" );
      return false;
   }

   if( !alpha ) {
      OlError( "No such alpha channel bitmap!" );
      return false;
   }


   if( bitmap_color_depth( alpha ) != 8 ) {
      OlError( string( "Wrong color depth for an alpha bitmap: " )
               + ToString( bitmap_color_depth( alpha )) + " (Should be 8)");
      return false;
   }

   isSolid = false;

   bool useTemp = bitmap_color_depth( colordata ) != 32;

   bmp = useTemp? create_bitmap_ex( 32, colordata->w, colordata->h ) : colordata;

   for( int y = 0; y < bmp->h; y++ ) {
      for( int x = 0; x < bmp->w; x++ ) {
         int color = getpixel( colordata, x, y );
         int a = alpha->line[y][x];
         color = makeacol32( getr( color ), getg( color ), getb( color ), a );
         ((unsigned int *) bmp->line[y])[x] = color;
      }
   }

   if( useTemp ) {
      destroy_bitmap( colordata );
   }

   destroy_bitmap( alpha );

   SelectDefaultPivot( bmp->w, bmp->h );

   HandleExtraFlags( extraFlags );

   SendToGPU();

   return true;
}



bool Bitmap::
Load( const char *filename, int extraFlags ) {
   collisionPoly = 0;

   if( !Setup::IsScreenSetUp() ) {
      allegro_message("Loading queued for bitmap: %s\n", filename);
   
      pendingLoad = new PendingFileLoad( filename );
      return true;
   }


   string absoluteFilename = Setup::ToAbsolutePathname( filename );

   OlLog( string( "Loading bitmap: " ) + filename );

   set_color_conversion( olColorConversion );

   bmp = load_bitmap( absoluteFilename.c_str(), 0 );

   if( !bmp ) {
      OlError( "No such bitmap!" );
      return false;
   }

   int width = bmp->w;
   int height = bmp->h;

   isSolid = true;

   if( bitmap_color_depth( bmp ) == 32 ) {
      for( int y = 0; y < bmp->h; y++ ) {
         for( int x = 0; x < bmp->w; x++ ) {
            int color = ((unsigned int *) bmp->line[y])[x];
            if( geta32( color ) != 0 ) {
               isSolid = false;
               break;
            }
         }
         if( !isSolid ) {
            break;
         }
      }
   }

   if( isSolid ) {
      bool useTemp = bitmap_color_depth( bmp ) != 32;
      OL_MEMORY_IMG *dst = useTemp? create_bitmap_ex( 32, width, height ) : bmp;

      for( int y = 0; y < dst->h; y++ ) {
         for( int x = 0; x < dst->w; x++ ) {
            int color = getpixel( bmp, x, y );
            color = makeacol32( getr( color ), getg( color ), getb( color ), 255 );
            ((unsigned int *) dst->line[y])[x] = color;
         }
      }

      if( useTemp ) {
         destroy_bitmap( bmp );
         bmp = dst;
      }
   }

   SelectDefaultPivot( bmp->w, bmp->h );

   HandleExtraFlags( extraFlags );

   SendToGPU();

   return true;
}



Poly *Bitmap::
GetCollisionPoly() const {
   return collisionPoly;
}



void Bitmap::
HandleExtraFlags( int flags ) {
   if( flags & CREATE_COLLISION_POLY ) {
      int numSkips = int((1.0 - Settings::GetCollisionPolyAccuracy())
                     * bmp->w * bmp->h * MAX_POLY_SKIP_PER_AREA);
      collisionPoly = Bitmap::GetCollisionPolygon( bmp, Settings::collisionPolyAlphaLimit,
                                                   numSkips, pivot );
   }
}



void Bitmap::
SendToGPU() {
   if( !bmp ) {
      OlError( "Cannot send the Bitmap to the graphics card without any memory bitmap content!" );
      return;
   }

   UnloadFromGPU();

   textureInfo = GlDriver::Get()->UploadTexture( bmp, isSolid );

   Select();
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

   if( Settings::useAntiAlias ) {
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
   }
   else {
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   }


   if( destroyBmp && !Settings::MemoryBitmapsStored() ) {
      destroy_bitmap( bmp );
      bmp = 0;
   }
}



vector< Bitmap *> Bitmap::
LoadListOfBitmaps( string filenameBegin, string extension, unsigned int numNumbers, int extraFlags ) {
   vector< Bitmap *> bitmapList;

   for( int index = 1;; index++ ) {
      stringstream stream;
      stream << index;
      string numbers = stream.str();

      while( numbers.size() < numNumbers ) {
         numbers = string("0") + numbers;
      }

      string filename = filenameBegin + numbers + string(".") + extension;


      bool notFound = !exists( filename.c_str() );

      if( notFound && index == 1 ) {
         if( index == 1 ) {
            OlError( "LoadListOfBitmaps: No image files found! ( First filename to try was: "
                     + filename + " )" );
         }

         break;
      }

      Bitmap *frame = new Bitmap;

      if( !frame->Load( filename.c_str(), extraFlags )) {
         delete frame;
         break;
      }

      bitmapList.push_back( frame );
   }

   return bitmapList;
}



bool Bitmap::
Save( const char *filename ) {
   OL_MEMORY_IMG *image = GetMemoryBitmap();

   bool result = GlDriver::Get()->SaveMemoryBitmap( image, Setup::ToAbsolutePathname( filename ).c_str() );

   destroy_bitmap( image );
   
   return result;
}



OL_MEMORY_IMG *Bitmap::
GetMemoryBitmap() const {
   return GetMemoryBitmap( 0, 0, Width(), Height() );
}


OL_MEMORY_IMG *Bitmap::
GetMemoryBitmap( int x, int y, int width, int height ) const {
   if( x < 0 ) {
      width += x;
      x = 0;
   }

   if( y < 0 ) {
      height += y;
      y = 0;
   }

   if( x + width > Width() ) {
      width = Width() - x;
   }

   if( y + height > Height() ) {
      height = Height() - y;
   }


   unsigned int bpp = textureInfo.GetBytesPerPixel();
   unsigned char *pixels = new unsigned char[width * height * bpp];

   Canvas::Push();
   Canvas::SetTo( *this );
   FrameBuffer::GetInstance().ReadPixels( x, y, width, height, textureInfo.format, pixels );
   Canvas::Pop();

   OL_MEMORY_IMG *img = GlDriver::Get()->GetMemoryBitmap( width, height, bpp, pixels );

   delete[] pixels;

   return img;
}



void Bitmap::
SaveIfSurface( std::string filename ) const {
   int width = Canvas::Width();
   int height = Canvas::Height();
   int bpp = Canvas::GetScreenshotBPP();
   int format = Canvas::GetScreenshotFormat();

   unsigned char *pixels = new unsigned char[width * height * bpp];
   FrameBuffer::GetInstance().ReadPixels( 0, 0, width, height, format, pixels );

   OL_MEMORY_IMG *image = GlDriver::Get()->GetMemoryBitmap( width, height, bpp, pixels );
   delete[] pixels;

   GlDriver::Get()->SaveMemoryBitmap( image, Setup::ToAbsolutePathname( filename ).c_str() );
   GlDriver::Get()->DestroyMemoryBitmap( image );
}



// GARBAGE COLLECTION / AUTOLOADER ROUTINES //

void Bitmap::
AddToCollection() {
   OlGetCollection().Add( this );
}



bool Bitmap::
IsLoadingQueued() const {
   return pendingLoad != 0;
}



OlLoadResult Bitmap::
ExecuteQueuedCommands() {
   if( IsLoadingQueued() ) {
      OlLoadResult result = pendingLoad->ExecuteLoading( *this );
      delete pendingLoad;
      pendingLoad = 0;
      return result;
   }

   return OL_LR_SUCCESS;
}



static inline int GetAlpha( int x, int y, OL_MEMORY_IMG *bitmap ) {
   return OlGetAlpha( bitmap, x, y );
}



static inline bool TestAlpha( int x, int y, int moreThan, OL_MEMORY_IMG *bitmap ) {
   return GetAlpha( x, y, bitmap ) > moreThan;
}


/*
static inline bool IsSurrounded( int x, int y, int moreThan, OL_MEMORY_IMG *bitmap ) {
   return TestAlpha( x-1, y )
       && TestAlpha( x+1, y )
       && TestAlpha( x, y-1 )
       && TestAlpha( x, y+1 );
}



static inline bool IsSurroundedInv( int x, int y, int lessThan, OL_MEMORY_IMG *bitmap ) {
   return !TestAlpha( x-1, y )
       && !TestAlpha( x+1, y )
       && !TestAlpha( x, y-1 )
       && !TestAlpha( x, y+1 );
}
*/


//extern BITMAP *tempbuf;


/*
enum {
   CB_EMPTY = 0,
   CB_TRACED = 1,
   CB_FILLED = 2
};
*/


// COLLISION ROUTINES //

ol::Poly *Bitmap::
GetCollisionPolygon( OL_MEMORY_IMG *bitmap, int alphaLimit, int numSkips, Vec2D rotationPivot ) {
   OlLog( "Creating collision polygon (Number of skips between vertices: "
          + ToString( numSkips ) + ")" );

   int bmpColorDepth = GlDriver::Get()->GetImageColorDepth( bitmap );

   if( bmpColorDepth != 32 ) {
      OlError( "Illegal Bitmap color depth for GetCollisionPoly: " + ToString( bmpColorDepth )
               + " ( Expected 32 )" );
      return 0;
   }

   Poly *poly = new Poly( rotationPivot );

   int lastCol = bitmap->w-1;
   int lastRow = bitmap->h-1;

   int x = bitmap->w/2;
   int y = bitmap->h/2;

   int xMov = 1;
   int yMov = 1;

   int lastXMove = x, lastYMove = y;

   int startX = 0;
   int startY = 0;

   bool found = false;

   for( int iy = 0; iy < bitmap->h; iy++ ) {
      for( int ix = 0; ix < bitmap->w; ix++ ) {
         if( TestAlpha( ix, iy, alphaLimit, bitmap )) {
            startX = ix;
            startY = iy;

            found = true;
            break;
         }
      }

      if( found ) {
         break;
      }
   }

   x = startX;
   y = startY;

   int skipCounter = 0;
   
   int i = 0;
   
   int width = bitmap->w;
   int height = bitmap->h;
   
   char *collisionBuffer = new char[width * height];
   
   for( int ii = 0; ii < width * height; ii++ ) {
      collisionBuffer[ii] = 0;
   }
   
   do {
      //printf( "x %d y %d xMov %d yMov %d\n", x, y, lastXMove, lastYMove );
      //putpixel( tempbuf, x, y, makeacol( 4*i, 4*i, (255 - 4*i), 255 ));
      
      bool polyAllowed = true;
      
      int storedX = x;
      int storedY = y;
      
      char &collisionBufPoint = collisionBuffer[ y * width + x];
      
      bool movedAlready = false;
      
      if( collisionBufPoint != 0 ) {
         //cout << "Going back" << endl;
         // We're going back in our tracks //
         if( lastXMove == 0 ) {
            if( x > 0 && TestAlpha( x-1, y, alphaLimit, bitmap )
               && collisionBuffer[ y * width + (x-1) ] == 0 ) {
               
               xMov = -1;
               x += xMov;
               yMov = 0;
               movedAlready = true;
            }
            else if( x < lastCol && TestAlpha( x+1, y, alphaLimit, bitmap )
               && collisionBuffer[ y * width + (x+1) ] == 0 ) {
               
               xMov = 1;
               x += xMov;
               yMov = 0;
               movedAlready = true;
            }
         }
         
         if( lastYMove == 0 || !movedAlready ) {
            if( lastYMove <= 0 ) {
               if( y > 0 && TestAlpha( x, y-1, alphaLimit, bitmap )
                  && collisionBuffer[ (y-1) * width + x ] == 0 ) {
                  
                  yMov = -1;
                  y += yMov;
                  xMov = 0;
                  movedAlready = true;
               }
            }
            
            if( lastYMove >= 0 && !movedAlready ) {
               if( y < lastRow && TestAlpha( x, y+1, alphaLimit, bitmap )
                  && collisionBuffer[ (y+1) * width + x ] == 0 ) {
                  
                  yMov = 1;
                  y += yMov;
                  xMov = 0;
                  movedAlready = true;
               }
            }
         }
         
         if( !movedAlready && lastYMove == 0 ) {
            if((( lastXMove < 0 && x > 0 ) || ( lastXMove > 0 && x < lastCol ))
                && TestAlpha( x+lastXMove, y, alphaLimit, bitmap )
               && collisionBuffer[ y * width + (x+lastXMove) ] == 0 ) {
               
               xMov = lastXMove;
               x += xMov;
               yMov = 0;
               movedAlready = true;
            }
         }
         
         if( !movedAlready ) {
            movedAlready = true;
            
            xMov = -(( collisionBufPoint >> 4 )-1);
            yMov = -(( collisionBufPoint & 0xf )-1);
            
            //printf( "Moves: %d %d\n", xMov, yMov );
            x += xMov;
            y += yMov;
         }
      }
      
      if( movedAlready ) {
         lastYMove = yMov;
         lastXMove = xMov;
            
         //printf( "movedAlready: %d %d\n", (int) lastXMove, (int) lastYMove );
      }
      else {
         collisionBufPoint = ((lastXMove+1) << 4) | (lastYMove+1);
         
         i++;
   
         if( y == 0 || y == lastRow ) {
            if( lastYMove != 0 ) {
               // Entering the edge //
               xMov = ( x != 0 && TestAlpha( x-1, y, alphaLimit, bitmap ))? -1 : 1;
            }
            else {
               polyAllowed = false;
            }
   
   
            if(( x != 0 || xMov > 0 ) && ( x != lastCol || xMov < 0 )) {
               if( !TestAlpha( x + xMov, y, alphaLimit, bitmap )) {
                  yMov = (y == 0)? 1 : -1;
                  y += yMov;
               }
               else {
                  x += xMov;
               }
            }
         }
         else if( x == 0 || x == lastCol ) {
            if( lastXMove != 0 ) {
               // Entering the edge //
               yMov = ( y != 0 && TestAlpha( x, y-1, alphaLimit, bitmap ))? -1 : 1;
            }
            else {
               polyAllowed = false;
            }
            
            if(( y != 0 || yMov > 0 ) && ( y != lastRow || yMov < 0 )) {
               if( !TestAlpha( x, y + yMov, alphaLimit, bitmap )) {
                  xMov = (x == 0)? 1 : -1;
                  x += xMov;
               }
               else {
                  y += yMov;
               }
            }
         }
         else {
            bool left = TestAlpha( x-1, y, alphaLimit, bitmap );
            bool right = TestAlpha( x+1, y, alphaLimit, bitmap );
            bool top = TestAlpha( x, y-1, alphaLimit, bitmap );
            bool bottom = TestAlpha( x, y+1, alphaLimit, bitmap );
            
            //putpixel( bitmap, x, y, 0xffff009f );
            //printf( "%d %d %d %d\n", (int) left, (int) right, (int) top, (int) bottom );
            
            if( left ) {
               if( top ) {
                  if( right ) {
                     if( bottom ) {
                        if( lastXMove == 0 ) {
                           bool lastLeft = TestAlpha( x-1, y - lastYMove, alphaLimit, bitmap );
                           xMov = lastLeft? 1 : -1;
                           x += xMov;
                        }
                        else {
                           bool lastTop = TestAlpha( x - lastXMove, y-1, alphaLimit, bitmap );
                           yMov = lastTop? 1 : -1;
                           y += yMov;
                        }
                     }
                     else {
                        x += xMov;
                        polyAllowed = false;
                     }
                  }
                  else {
                     if( bottom ) {
                        y += yMov;
                        polyAllowed = false;
                     }
                     else {
                        if( lastXMove == 0 ) {
                           xMov = -1;
                           x += xMov;
                        }
                        else {
                           yMov = -1;
                           y += yMov;
                        }
                     }
                  }
               }
               else {
                  if( right ) {
                     // Bottom doesn't matter //
                     x += xMov;
                     polyAllowed = false;
                  }
                  else if( bottom ) {
                     if( lastXMove == 0 ) {
                        xMov = -1;
                        x += xMov;
                     }
                     else {
                        yMov = 1;
                        y += yMov;
                     }
                  }
                  else {
                     // End of a line, go back left //
                     xMov = -1;
                     x += xMov;
                  }
               }
            }
            else {
               if( top ) {
                  if( bottom ) {
                     // Right doesn't matter //
                     y += yMov;
                     polyAllowed = false;
                  }
                  else if( right ) {
                     if( lastXMove == 0 ) {
                        xMov = 1;
                        x += xMov;
                     }
                     else {
                        yMov = -1;
                        y += yMov;
                     }
                  }
                  else {
                     // End of a line, go back up //
                     yMov = -1;
                     y += yMov;
                  }
               }
               else {
                  if( right ) {
                     if( bottom ) {
                        if( lastXMove == 0 ) {
                           xMov = 1;
                           x += xMov;
                        }
                        else {
                           yMov = 1;
                           y += yMov;
                        }
                     }
                     else {
                        // End of a line, go back right //
                        xMov = 1;
                        x += xMov;
                     }
                  }
                  else if( bottom ) {
                     // End of a line, go back down //
                     yMov = 1;
                     y += yMov;
                  }
                  else {
                     printf( "GetCollisionPoly confused!" );
                     OlError( "GetCollisionPoly confused!" );
                     break;
                  }
               }
            }
         }
   
         lastXMove = x - storedX;
         lastYMove = y - storedY;
      }
      
      skipCounter++;
      
      if( polyAllowed && skipCounter > numSkips ) {
         poly->Add( Vec2D( storedX, storedY ));
         skipCounter = 0;
      }
   }
   while( x != startX || y != startY );
   
   delete[] collisionBuffer;

   return poly;
}



void Bitmap::
UnloadToMemory() {
   bmp = GetMemoryBitmap();
   destroyBmp = true;
   UnloadFromGPU();
}


