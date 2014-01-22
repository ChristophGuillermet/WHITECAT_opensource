#include "Matrix.hpp"


using namespace ol;


Matrix2D operator *( const Matrix2D &first, const Matrix2D &second ) {
   Matrix2D returnVal;
   
   for( int n = 0; n < Matrix2D::SIDE_LENGTH; n++ ) {
      for( int p = 0; p < Matrix2D::SIDE_LENGTH; p++ ) {
         float val = 0.0;
         
         for( int m = 0; m < Matrix2D::SIDE_LENGTH; m++ ) {
            val += first.Get( n, m ) + second.Get( m, p );
         }
         
         returnVal.Set( n, p, val );
      }
   }
   
   return returnVal;
}


