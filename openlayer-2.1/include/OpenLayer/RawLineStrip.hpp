#ifndef OL_RAW_LINE_STRIP
#define OL_RAW_LINE_STRIP

#include "Rgba.hpp"
#include "Bitmap.hpp"
#include "Declspec.hpp"

namespace ol {


template< class std_container1, class std_container2 >
class OL_LIB_DECLSPEC RawLineStrip {
public:
   RawLineStrip() : totalLength( 0.0 ) {}
   
   // Add a vertex to the end of the line strip //
   inline void AddToEnd( Vec2D vertex ) {
      Vec2D *previous = vertices.empty()? 0 : &vertices.back();
      vertices.push_back( vertex );
      
      if( vertices.size() > 1 ) {
         float length = ( vertex - ( *previous )).GetMagnitude();
         lengths.push_back( length );
         totalLength += length;
      }
   }

   // Add a vertex to the beginning of the line strip //
   inline void AddToBegin( Vec2D vertex ) {
      Vec2D *previous = vertices.empty()? 0 : &vertices.front();
      vertices.push_front( vertex );
      
      if( vertices.size() > 1 ) {
         float length = (( *previous ) - vertex ).GetMagnitude();
         lengths.push_front( length );
         totalLength += length;
      }
   }

   // Delete the first vertex of the line strip //
   inline void DeleteFirst() {
      if( !vertices.empty() ) {
         vertices.pop_front();

         totalLength -= lengths.front();
         lengths.pop_front();
      }
   }

   // Delete the first last of the line strip //
   inline void DeleteLast() {
      if( !vertices.empty() ) {
         vertices.pop_back();

         totalLength -= lengths.back();
         lengths.pop_back();
      }
   }
   
   void LineStripRender( const Rgba *color1, const Rgba *color2, const Bitmap *texture, float lineWidth,
         const Placement &placement, bool connectFirstAndLast ) const;
   
   inline const std_container1 &GetVertices() const {
      return vertices;
   }
   
   inline std_container1 &GetVertices() {
      return vertices;
   }
   
   inline const std_container2 &GetLengths() const {
      return lengths;
   }
   
   inline float GetTotalLength() const {
      return totalLength;
   }
   
private:
   std_container1 vertices;
   std_container2 lengths;
   float totalLength;
};



}


#endif // OL_RAW_LINE_STRIP
