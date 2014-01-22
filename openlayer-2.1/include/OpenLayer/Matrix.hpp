#ifndef OL_MATRIX_HPP
#define OL_MATRIX_HPP


#include "Vec2D.hpp"
#include "Declspec.hpp"


namespace ol {


class OL_LIB_DECLSPEC Matrix2D {
public:
   
   enum {
      SIDE_LENGTH = 2,
      NUM_VALUES = SIDE_LENGTH * SIDE_LENGTH
   };
   
   
   inline float Get( int x, int y ) const {
      return values[SIDE_LENGTH * y + x];
   }
   
   
   inline void Set( int x, int y, float val ) {
      values[SIDE_LENGTH * y + x] = val;
   }
   
   
   inline Vec2D Transform( const Vec2D &vec ) {
      return Vec2D( Get( 0, 0 ) * vec.x + Get( 1, 0 ) * vec.y,
                    Get( 0, 1 ) * vec.x + Get( 1, 1 ) * vec.y );
   }
   
   
   float values[NUM_VALUES];
};



Matrix2D operator*( const Matrix2D &first, const Matrix2D &second );



}


#endif // OL_MATRIX_HPP
