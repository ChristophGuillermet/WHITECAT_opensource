#ifndef OL_TEXTURE_INFO_HPP
#define OL_TEXTURE_INFO_HPP


#include "Includes.hpp"
#include "Vec2D.hpp"
#include "Rgba.hpp"
#include "OlRectangle.hpp"
#include "Declspec.hpp"


// INTERNAL TEXTURE CLASSES //


namespace ol {


// Internal rectangle class //

/*
class OlRect {
public:
   OlRect( float x = 0.0, float y = 0.0, float w = 0.0, float h = 0.0 )  : x( x ), y( y ), w( w ), h( h ) {}
   
   OlRect ClippedTo( const OlRect &rect ) const;
   
   float x, y, w, h;
};*/


typedef OL_LIB_DECLSPEC OlRectangle< float > OlRect;



// OpenLayer's internal texture information //

class OL_LIB_DECLSPEC OlTextureInfo {
public:
   OlTextureInfo( int textureWidth, int textureHeight, 
                  int imageWidth, int imageHeight,
                  GLenum format, GLuint index = 0 );
   
   OlTextureInfo();
   
   // Create a sub-texture //
   OlTextureInfo( const OlTextureInfo &other, OlRect clipRect );
   
   
   inline void SetIndex( GLuint index ) {
      this->index = index;
   }
   
   
   inline int GetIndex() const {
      return index;
   }
   
   
   inline static void StartRendering() {
      glBegin( GL_QUADS );
   }
   
   
   inline void OutputTexturedQuad() const {
      OutputTexturedQuad( imgWidth, imgHeight );
   }
   
   
   inline static void FinishRendering() {
      glEnd();
   }
   
   
   inline void OutputTexturedQuad( float x, float y, float w, float h ) const {
      glBegin( GL_TRIANGLE_FAN );
         OutputVertices( x, y, w, h );
      glEnd();
   }
   
   
   
   inline void OutputTexturedQuad( float w, float h ) const {
      OutputTexturedQuad( 0.0, 0.0, w, h );
   }
   
   
   
   inline void OutputTexturedQuadXY( float x, float y ) const {
      OutputTexturedQuad( x, y, imgWidth, imgHeight );
   }
   
   
   inline void OutputVertices( float x, float y, float w, float h ) const {
      glTexCoord2f( rect.x, rect.y );
      glVertex2f( x, y + h );
      
      glTexCoord2f( rect.x + rect.w, rect.y );
      glVertex2f( x + w, y + h );
      
      glTexCoord2f( rect.x + rect.w, rect.y + rect.h );
      glVertex2f( x + w, y );
      
      glTexCoord2f( rect.x, rect.y + rect.h );
      glVertex2f( x, y );
   }
   
   
   /*
   inline void OutputTexturedQuadSpecialWorkaround() const {
      float x = 0.0, y = 0.0, w = imgWidth, h = imgHeight;
      glBegin( GL_TRIANGLE_FAN );
         glTexCoord2f( rect.x, rect.y );
         glVertex2f( x, y + h+0.99 );
         
         glTexCoord2f( rect.x + rect.w, rect.y );
         glVertex2f( x + w+0.5, y + h+0.99 );
         
         glTexCoord2f( rect.x + rect.w, rect.y + rect.h );
         glVertex2f( x + w+0.5, y );
         
         glTexCoord2f( rect.x, rect.y + rect.h );
         glVertex2f( x, y );
      glEnd();
   }
   */
   
   inline void OutputVertices( float x, float y ) const {
      OutputVertices( x, y, imgWidth, imgHeight );
   }
   
   
   inline void OutputTexturedQuad( Vec2D topleft, Vec2D topright,
                                   Vec2D bottomright, Vec2D bottomleft ) const {
      glBegin( GL_TRIANGLE_FAN );
         glTexCoord2f( rect.x, rect.y );
         glVertex2f( bottomleft.x, bottomleft.y );
         
         glTexCoord2f( rect.x + rect.w, rect.y );
         glVertex2f( bottomright.x, bottomright.y );
         
         glTexCoord2f( rect.x + rect.w, rect.y + rect.h );
         glVertex2f( topright.x, topright.y );
         
         glTexCoord2f( rect.x, rect.y + rect.h );
         glVertex2f( topleft.x, topleft.y );
      glEnd();
   }
   
   
   inline void Select() const {
      glBindTexture( GL_TEXTURE_2D, index );
   }
   
   
   inline void GetReadyToRender() const {
      Select();
      glEnable( GL_TEXTURE_2D );
      Rgba::WHITE.Select();
   }
   
   
   int GetBytesPerPixel() const;
   bool HasAlphaChannel() const;
   
   friend class Bitmap;
   
   GLenum format;
   GLuint index;
   int texWidth, texHeight;
   int imgWidth, imgHeight;
   
   int fullImgWidth, fullImgHeight;
   
   float xMul, yMul;
   
   OlRect rect;
   
private:
   
   inline float ConvDimension( float imgSize, float texSize ) {
      return float( imgSize )/float( texSize );
   }
};


// Internal texture coordinate class //

class OL_LIB_DECLSPEC OlTexCoords {
public:
   OlTexCoords( const OlTextureInfo &texture )
      : texture( texture ),
        x1( 0.0 ), x2( texture.imgWidth ),
        y1( 0.0 ), y2( texture.imgHeight ),
        convx1( 0.0 ), convx2( 0.0 ), convy1( 0.0 ), convy2( 0.0 ),
        flipW( false ), flipH( false ), colors( 0 ) {}
   
   /*
   OlTexCoords( const OlTexCoords &orign, float x1 = 0.0, float y1 = 0.0, float x2 = 0.0, float y2 = 0.0 )
      : w( orign.w ), h( orign.h ), x1( x1 ), y1( y1 ), x2( x2 ), y2( y2 ),
        convx1( orign.convx1 ), convx2( orign.convx2 ), convy1( orign.convy1 ), convy2( orign.convy2 ),
        flipW( orign.flipW ), flipH( orign.flipH ) {}
   */
   
   
   inline void FlipHorizontal() { flipW = !flipW; }
   inline void FlipVertical() { flipH = !flipH; }
   
   inline float GetAreaW() const { return fabs( x1 - x2 ); }
   inline float GetAreaH() const { return fabs( y1 - y2 ); }
   
   void CalculateTextureCoords();
   
   OlTexCoords ClippedTo( const OlRect &rect ) const;
   
   inline bool ChangesColors() const {
      return colors != 0;
   }
   
   inline const Rgba &GetColor( int index ) const {
      return colors[index];
   }
   
   inline void SetColors( const Rgba *colors ) {
      this->colors = colors;
   }
   
   
   const OlTextureInfo &texture;
   float x1, x2, y1, y2;
   float convx1, convx2, convy1, convy2;
   bool flipW, flipH;
   
   const Rgba *colors;
};


}




#endif // OL_TEXTURE_INFO_HPP
