#ifndef OL_TEXTURED_POLY_HPP
#define OL_TEXTURED_POLY_HPP

#include "Polygon.hpp"
#include "Vec2D.hpp"
#include "Declspec.hpp"
#include <vector>
#include <utility>

//#define OL_DEBUG_TEXTURED_POLY


namespace ol {

class Bitmap;


class OL_LIB_DECLSPEC TexturedPoly : public Poly {
public:
   TexturedPoly( Vec2D rotationPivot = Vec2D( 0.0, 0.0 ))
      : Poly( rotationPivot ), texture( 0 ), dirty( true ) {}
      
   TexturedPoly( const Bitmap &texture, Vec2D rotationPivot = Vec2D( 0.0, 0.0 ))
      : Poly( rotationPivot ), texture( &texture ), dirty( true ) {}
   
   template< class std_container >
   TexturedPoly( const Bitmap &texture, const std_container &vertices, Vec2D rotationPivot = Vec2D( 0.0, 0.0 ))
      : Poly( vertices, rotationPivot ), texture( &texture ), dirty( true ) {}
   
   
   TexturedPoly( const Bitmap &texture, const Vec2D *vertices, int numVertices,
                 Vec2D rotationPivot = Vec2D( 0.0, 0.0 ))
      : Poly( vertices, numVertices, rotationPivot ), texture( &texture ), dirty( true ) {}
   
   virtual void Draw( float opacity );
   
   virtual void Add( Vec2D vec );
   
   virtual void SetVertex( int index, const Vec2D &newValue );
   
   virtual void SetTexture( const Bitmap &texture );
   
   void Construct();
   
protected:
   void AddEdgePoints( std::vector< Vec2D > &vecs, const Line &segment, Line **textureEdges, bool lowestFirst );
   
   bool LineCollides( const Line &line, std::vector< Vec2D > ::const_iterator start,
                      std::vector< Vec2D > ::const_iterator end );
   
   inline bool isInside( const Vec2D &vec, float x, float y, float w, float h ) const {
      return vec.x >= x && vec.y >= y && vec.x < x + w && vec.y < y + h;
   }
   
   class SideProcessed {
   public:
      SideProcessed() : isEmpty( false ) {}
      
      std::vector< Vec2D > vertices;
      bool isEmpty;
   };
   
   SideProcessed ProcessSide( std::vector< Vec2D > ::const_iterator start,
                              std::vector< Vec2D > ::const_iterator end,
                              float x, float y, bool isUpper, bool leftToRight );
   
   class SlicePart {
   public:
      SlicePart( float top, float bottom, float x, float textureStartX, float textureStartY, const Bitmap *bmp );
      
      inline void RenderStripPart() {
         glTexCoord2f( textureX, textureTop );
         glVertex2f( x, top );
         glTexCoord2f( textureX, textureBottom );
         glVertex2f( x, bottom );
      }
      
      inline bool IsHeightZero() {
         return bottom - top < 2.0;
      }

#ifndef OL_DEBUG_TEXTURED_POLY
   protected:
#endif // OL_DEBUG_TEXTURED_POLY
      float top, bottom;
      float textureTop, textureBottom;
      float x, textureX;
   };
   
   class HSlice {
   public:
      ~HSlice();
      
      inline void AddPart( SlicePart *part ) {
         parts.push_back( part );
      }
      
      void Render();
      
   private:
      std::vector< SlicePart *> parts;
   };
   
   std::vector< HSlice *> slices;
   const Bitmap *texture;
   bool dirty; // Indicates the need of reconstruction
};



}



#endif // OL_TEXTURED_POLY_HPP




