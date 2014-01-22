#include "Placement.hpp"
#include "Internal.hpp"
#include "Setup.hpp"
#include "math.h"


using namespace ol;


Matrix2D Placement::
Get2DMatrix() const {
   Matrix2D returnVal;
   
   float sinA = sin( rotation );
   float cosA = cos( rotation );
   
   returnVal.Set( 0, 0, stretch * cosA );
   returnVal.Set( 1, 0, stretch * sinA );
   returnVal.Set( 0, 1, stretch * -sinA );
   returnVal.Set( 1, 1, stretch * cosA );
   
   return returnVal;
}


void Placement::
Apply( const Vec2D &pivot ) const {
   glTranslatef( position.x, position.y, 0.0 );
   
   if( rotation != 0.0 ) {
      glTranslatef( pivot.x, pivot.y, 0.0 );
      RotateMatrix( -rotation );
      glTranslatef( -pivot.x, -pivot.y, 0.0 );
   }
   
   if( stretch != 1.0 ) {
      float xTranslate = -(stretch - 1.0) * Setup::GetWindowWidth()/2;
      float yTranslate = -(stretch - 1.0) * Setup::GetWindowHeight()/2;
      
      glTranslatef( xTranslate, yTranslate, 0.0 );
      
      glScalef( stretch, stretch, 0.0 );
   }
}


std::string Placement::
ToString() const {
    std::ostringstream str;
    str << "Position: ( " << position.x << ", " << position.y << " ) Angle: " << rotation
        << " Stretch: " << stretch;
    return str.str();
}


