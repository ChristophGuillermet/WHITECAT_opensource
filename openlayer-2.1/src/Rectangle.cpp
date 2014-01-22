#include "Rectangle.hpp"
#include "VertexListCollision.hpp"
#include <vector>


using namespace ol;

static const float OL_NEAR_ZERO = 0.00001;

void Rect::
ExecDraw() const {
#ifdef OL_NO_STATE_CHANGE
   GLboolean texturesEnabled;
   glGetBooleanv( GL_TEXTURE_2D, &texturesEnabled );
#endif

   bool applyRotation = fabs( rotationAngle ) > OL_NEAR_ZERO;
   
   if( roundness <= OL_NEAR_ZERO ) {
      if( applyRotation ) {
         glPushMatrix();
         RotateMatrix( rotationAngle );
      }
      
      glBegin( GL_TRIANGLE_FAN );
         
         glVertex2f( pos.x, pos.y+size.y );
         glVertex2f( pos.x+size.x, pos.y+size.y );
         glVertex2f( pos.x+size.x, pos.y );
         glVertex2f( pos.x, pos.y );
         
      glEnd();
      
      if( applyRotation ) {
         glPopMatrix();
      }
   }
   else {
      glPushMatrix();
      
      if( applyRotation ) {
         RotateMatrix( rotationAngle );
      }
      
      float topY = pos.y + roundness;
      float bottomY = pos.y + size.y - roundness;
      
      float leftX = pos.x + roundness;
      float rightX = pos.x + size.x - roundness;
      
      glBegin( GL_QUADS );
         // Center //
         glVertex2f( pos.x, bottomY );
         glVertex2f( pos.x+size.x, bottomY );
         glVertex2f( pos.x+size.x, topY );
         glVertex2f( pos.x, topY );
         
         // Top //
         glVertex2f( leftX, topY );
         glVertex2f( rightX, topY );
         glVertex2f( rightX, pos.y );
         glVertex2f( leftX, pos.y );
         
         // Bottom //
         glVertex2f( leftX, pos.y + size.y );
         glVertex2f( rightX, pos.y + size.y );
         glVertex2f( rightX, bottomY );
         glVertex2f( leftX, bottomY );
      glEnd();
      
      glTranslatef( pos.x + roundness, pos.y + roundness, GLfloat( 0.0 ));
      RenderCircleQuarter( AL_PI );
      
      glTranslatef( size.x - 2 * roundness, 0.0, GLfloat( 0.0 ));
      RenderCircleQuarter( -AL_PI/2 );
      
      glTranslatef( 0.0, size.y - 2 * roundness, GLfloat( 0.0 ));
      RenderCircleQuarter( 0.0 );
      
      glTranslatef( -size.x + 2 * roundness, 0.0, GLfloat( 0.0 ));
      RenderCircleQuarter( AL_PI/2 );
      
      glPopMatrix();
   }
#ifdef OL_NO_STATE_CHANGE
   if( texturesEnabled )
      glEnable( GL_TEXTURE_2D );
#endif
}



void Rect::
RenderCircleQuarter( float angleStart ) const {
   float endAngle = angleStart + AL_PI/2;
   
   glBegin( GL_TRIANGLE_FAN );
      glVertex2f( 0, 0 );
      
      for( float a = angleStart; a < endAngle; a += angleIncrement ) {
         glVertex2f( cos(a) * roundness, sin(a) * roundness );
      }
      
      glVertex2f( cos( endAngle ) * roundness, sin( endAngle ) * roundness );
   glEnd();
}



void Rect::
RenderThickCircleOutlineQuarter( float startAngle ) const {
   float endAngle = startAngle + AL_PI/2;
   float outerRad = roundness + lineWidth;
   float innerRad = roundness;
   
   glBegin( GL_QUADS );
      float prevIX = cos(startAngle) * innerRad;
      float prevIY = sin(startAngle) * innerRad;
      
      float prevOX = cos(startAngle) * outerRad;
      float prevOY = sin(startAngle) * outerRad;
      
      for( float a = startAngle; a < endAngle; a += angleIncrement ) {
         float cosa = cos(a);
         float sina = sin(a);
         
         float newIX = cosa * innerRad;
         float newIY = sina * innerRad;
         
         float newOX = cosa * outerRad;
         float newOY = sina * outerRad;
         
         glVertex2f( newIX, newIY );
         glVertex2f( prevIX, prevIY );
         
         glVertex2f( prevOX, prevOY );
         glVertex2f( newOX, newOY );
         
         prevIX = newIX;
         prevIY = newIY;
         
         prevOX = newOX;
         prevOY = newOY;
      }
      
      glVertex2f( cos(endAngle) * innerRad, sin(endAngle) * innerRad);
      glVertex2f( prevIX, prevIY );
      
      glVertex2f( prevOX, prevOY );
      glVertex2f( cos(endAngle) * outerRad, sin(endAngle) * outerRad); 
   glEnd();
}



void Rect::
ExecDrawOutline() const {
   bool applyRotation = fabs( rotationAngle ) > OL_NEAR_ZERO;
   
   if( roundness <= OL_NEAR_ZERO ) {
      if( applyRotation ) {
         glPushMatrix();
         RotateMatrix( rotationAngle );
      }
      
      glBegin( GL_QUADS );
         // Top //
         glVertex2f( pos.x -lineWidth, pos.y -lineWidth );
         glVertex2f( pos.x+size.x +lineWidth, pos.y -lineWidth );
         glVertex2f( pos.x+size.x +lineWidth, pos.y );
         glVertex2f( pos.x -lineWidth, pos.y );
         // Bottom //
         glVertex2f( pos.x -lineWidth, pos.y+size.y );
         glVertex2f( pos.x+size.x +lineWidth, pos.y+size.y );
         glVertex2f( pos.x+size.x +lineWidth, pos.y+size.y +lineWidth );
         glVertex2f( pos.x -lineWidth, pos.y+size.y +lineWidth );
         // Left //
         glVertex2f( pos.x -lineWidth, pos.y );
         glVertex2f( pos.x, pos.y );
         glVertex2f( pos.x, pos.y+size.y );
         glVertex2f( pos.x -lineWidth, pos.y+size.y );
         // Right //
         glVertex2f( pos.x+size.x, pos.y );
         glVertex2f( pos.x+size.x +lineWidth, pos.y );
         glVertex2f( pos.x+size.x +lineWidth, pos.y+size.y );
         glVertex2f( pos.x+size.x, pos.y+size.y );
      glEnd();
      
      if( applyRotation ) {
         glPopMatrix();
      }
   }
   else {
      glPushMatrix();
      
      if( applyRotation ) {
         RotateMatrix( rotationAngle );
      }
      
      glBegin( GL_QUADS );
         // Top //
         glVertex2f( pos.x +roundness, pos.y -lineWidth );
         glVertex2f( pos.x+size.x -roundness, pos.y -lineWidth );
         glVertex2f( pos.x+size.x -roundness, pos.y );
         glVertex2f( pos.x +roundness, pos.y );
         // Bottom //
         glVertex2f( pos.x +roundness, pos.y+size.y );
         glVertex2f( pos.x+size.x -roundness, pos.y+size.y );
         glVertex2f( pos.x+size.x -roundness, pos.y+size.y +lineWidth );
         glVertex2f( pos.x +roundness, pos.y+size.y +lineWidth );
         // Left //
         glVertex2f( pos.x -lineWidth, pos.y +roundness );
         glVertex2f( pos.x, pos.y +roundness );
         glVertex2f( pos.x, pos.y+size.y -roundness );
         glVertex2f( pos.x -lineWidth, pos.y+size.y -roundness );
         // Right //
         glVertex2f( pos.x+size.x, pos.y +roundness );
         glVertex2f( pos.x+size.x +lineWidth, pos.y +roundness );
         glVertex2f( pos.x+size.x +lineWidth, pos.y+size.y -roundness );
         glVertex2f( pos.x+size.x, pos.y+size.y -roundness );
      glEnd();
      
      
      glTranslatef( pos.x + roundness, pos.y + roundness, GLfloat( 0.0 ));
      RenderThickCircleOutlineQuarter( AL_PI );
      
      glTranslatef( size.x - 2 * roundness, 0.0, GLfloat( 0.0 ));
      RenderThickCircleOutlineQuarter( -AL_PI/2 );
      
      glTranslatef( 0.0, size.y - 2 * roundness, GLfloat( 0.0 ));
      RenderThickCircleOutlineQuarter( 0.0 );
      
      glTranslatef( -size.x + 2 * roundness, 0.0, GLfloat( 0.0 ));
      RenderThickCircleOutlineQuarter( AL_PI/2 );
      
      glPopMatrix();
   }
   
#ifdef OL_NO_STATE_CHANGE
   if( texturesEnabled )
      glEnable( GL_TEXTURE_2D );
#endif
}



void Rect::
Draw( const Rgba *colors ) const {
#ifdef OL_NO_STATE_CHANGE
   GLboolean texturesEnabled;
   glGetBooleanv( GL_TEXTURE_2D, &texturesEnabled );
#endif
   glDisable( GL_TEXTURE_2D );
   
   glBegin( GL_QUADS );
      colors[3].Select();
      glVertex2f( pos.x, pos.y+size.y );
      colors[2].Select();
      glVertex2f( pos.x+size.x, pos.y+size.y );
      colors[1].Select();
      glVertex2f( pos.x+size.x, pos.y );
      colors[0].Select();
      glVertex2f( pos.x, pos.y );
      
   glEnd();
   
#ifdef OL_NO_STATE_CHANGE
   if( texturesEnabled )
      glEnable( GL_TEXTURE_2D );
#endif
}


Rect Rect::
ClippedTo( const Rect &other ) const {
   Rect returnVal;
   
   if( other.pos.x < 0 ) {
      returnVal.pos.x = 0;
      returnVal.size.x = std::max( std::min( size.x, other.pos.x + other.size.x ), 0.0f );
   }
   else {
      returnVal.pos.x = pos.x + other.pos.x;
      returnVal.size.x = std::max( std::min( size.x - other.pos.x, other.size.x ), 0.0f );
   }
   
   
   if( other.pos.y < 0 ) {
      returnVal.pos.y = 0;
      returnVal.size.y = std::max( std::min( size.y, other.pos.y + other.size.y ), 0.0f );
   }
   else {
      returnVal.pos.y = pos.y + other.pos.y;
      returnVal.size.y = std::max( std::min( size.y - other.pos.y, other.size.y ), 0.0f );
   }
   
   return returnVal;
}


std::string Rect::
ToString() const {
    std::ostringstream str;
    str << "Rect: Pos: " << pos.ToString() << " Size: " << size.ToString() << " Roundness: " << roundness
        << " Rotation angle: " << rotationAngle;
    return str.str();
}


Collision Rect::
DoCollisionTest( const ol::Poly &other, const Placement &thisPlacement,
                 const Placement &otherPlacement, bool getResults ) const {
   return LineStripCollision( GetVertices(), other.GetVertices(), thisPlacement, otherPlacement,
                              getResults, true, true );
}

