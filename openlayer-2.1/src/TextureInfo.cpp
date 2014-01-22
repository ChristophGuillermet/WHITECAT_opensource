#include "TextureInfo.hpp"
#include "Internal.hpp"

#include <cmath>

using namespace ol;
using namespace std;


void OlTexCoords::
CalculateTextureCoords() {
   convx1 = texture.rect.x + x1 / texture.imgWidth * texture.rect.w;
   convx2 = texture.rect.x + x2 / texture.imgWidth * texture.rect.w;

   if( flipW ) {
      float temp = convx1;
      convx1 = convx2;
      convx2 = temp;
   }

   convy1 = texture.rect.y + texture.rect.h - y1 / texture.imgHeight * texture.rect.h;
   convy2 = texture.rect.y + texture.rect.h - y2 / texture.imgHeight * texture.rect.h;

   if( flipH ) {
      float temp = convy1;
      convy1 = convy2;
      convy2 = temp;
   }
}




OlTexCoords OlTexCoords::
ClippedTo( const OlRect &rect ) const {
   OlTexCoords returnVal = *this;

   if( rect.x < 0.0 ) {
      returnVal.x1 = x1;
      returnVal.x2 = min( x2 + rect.x, returnVal.x1 + rect.x + rect.w );
   }
   else {
      returnVal.x1 = x1 + rect.x;
      returnVal.x2 = min( returnVal.x1 + x2 - rect.x, returnVal.x1 + rect.w );
   }


   if( rect.y < 0.0 ) {
      returnVal.y1 = y1;
      returnVal.y2 = min( y2 + rect.y, returnVal.y1 + rect.y + rect.h );
   }
   else {
      returnVal.y1 = y1 + rect.y;
      returnVal.y2 = min( returnVal.y1 + y2 - rect.y, returnVal.y1 + rect.h );
   }

   return returnVal;
}




OlTextureInfo::
OlTextureInfo( int textureWidth, int textureHeight,
               int imageWidth, int imageHeight, GLenum format, GLuint index )
   : format( format ), index( index ),
     texWidth( textureWidth ), texHeight( textureHeight ),
     imgWidth( imageWidth ), imgHeight( imageHeight ),
     fullImgWidth( imageWidth ), fullImgHeight( imageHeight ),
     xMul( ConvDimension( imageWidth, textureWidth )),
     yMul( ConvDimension( imageHeight, textureHeight )) {

   rect = OlRect( 0.0, 1.0 - yMul, xMul, yMul );
}



OlTextureInfo::
OlTextureInfo( const OlTextureInfo &other, OlRect clipRect )
   : format( other.format ), index( other.index ),
     texWidth( other.texWidth ), texHeight( other.texHeight ),
     fullImgWidth( other.fullImgWidth ), fullImgHeight( other.fullImgHeight ),
     xMul( other.xMul ), yMul( other.yMul ) {

   clipRect.y = other.imgHeight - clipRect.y - clipRect.h;
   /*
   printf( "Other rect: %f %f %f %f\n", clipRect.x, clipRect.y,
      clipRect.w, clipRect.h );*/

   float xRectMul = other.xMul / other.fullImgWidth;
   float yRectMul = other.yMul / other.fullImgHeight;

   //printf( "Mul: %f, %f\n", xRectMul, yRectMul );

   OlRect convClipRect( xRectMul * clipRect.x, yRectMul * clipRect.y,
                        xRectMul * clipRect.w, yRectMul * clipRect.h );
   /*
   printf( "Converted rect: %f %f %f %f\n", convClipRect.x, convClipRect.y,
      convClipRect.w, convClipRect.h );

   printf( "Rect: %f %f %f %f\n", other.rect.x, other.rect.y,
      other.rect.w, other.rect.h );*/

   rect = other.rect.ClippedTo( convClipRect );
   /*
   printf( "Clipped rect: %f %f %f %f\n", rect.x, rect.y,
      rect.w, rect.h );
   */
   
   float sizeXMul = rect.w / other.rect.w;
   float sizeYMul = rect.h / other.rect.h;

   //printf( "SizeMul: %f, %f\n", sizeXMul, sizeYMul );

   imgWidth = int(sizeXMul * other.imgWidth);
   imgHeight = int(sizeYMul * other.imgHeight);
   /*
   texWidth = int(sizeXMul * other.texWidth);
   texHeight = int(sizeYMul * other.texHeight);*/
}



OlTextureInfo::
OlTextureInfo()
   : format( 0 ), index( 0 ), texWidth( 0 ), texHeight( 0 ), imgWidth( 0 ),
     imgHeight( 0 ), fullImgWidth( 0 ), fullImgHeight( 0 ) {}



int OlTextureInfo::
GetBytesPerPixel() const {
   switch( format ) {
      case GL_RGBA:
         return 4;
      case GL_RGB:
         return 3;
      case GL_ALPHA:
         return 1;
   }

   OlError( "Unknown pixel format" );

   return 0;
}


bool OlTextureInfo::
HasAlphaChannel() const {
   switch( format ) {
      case GL_RGBA:
         return true;
      case GL_RGB:
         return false;
      case GL_ALPHA:
         return true;
   }

   OlError( "Unknown pixel format" );

   return false;
}


