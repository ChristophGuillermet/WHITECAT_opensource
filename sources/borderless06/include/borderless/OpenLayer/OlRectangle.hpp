#ifndef OL_INTERNAL_RECT
#define OL_INTERNAL_RECT

#include <cmath>
#include "Declspec.hpp"

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif


namespace ol {

// Internal  rectangle class //

template< typename Type >
   class OL_LIB_DECLSPEC OlRectangle {
   public:
      OlRectangle( Type x = 0.0, Type y = 0.0, Type w = 0.0, Type h = 0.0 )  : x( x ), y( y ), w( w ), h( h ) {}
      
      OlRectangle ClippedTo( const OlRectangle &rect ) const {
         OlRectangle returnVal;
         
         if( rect.x < 0 ) {
            returnVal.x = 0;
            returnVal.w = std::max( std::min( w, rect.x + rect.w ), Type( 0 ));
         }
         else {
            returnVal.x = x + rect.x;
            returnVal.w = std::max( std::min( w - rect.x, rect.w ), Type( 0 ));
         }
         
         
         if( rect.y < 0 ) {
            returnVal.y = 0;
            returnVal.h = std::max( std::min( h, rect.y + rect.h ), Type( 0 ));
         }
         else {
            returnVal.y = y + rect.y;
            returnVal.h = std::max( std::min( h - rect.y, rect.h ), Type( 0 ));
         }
         
         return returnVal;
      }
      
      Type x, y, w, h;
   };


}


#endif // OL_INTERNAL_RECT
