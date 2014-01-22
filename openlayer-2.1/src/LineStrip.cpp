#define DEBUGMODE
#include "LineStrip.hpp"
#include "Bitmap.hpp"
#include "VertexListCollision.hpp"
#include "LineStripRender.hpp"

#include <iostream>

using namespace std;
using namespace ol;


Collision LineStrip::
DoCollisionTest( const ol::LineStrip &other, const Placement &thisPlacement,
                 const Placement &otherPlacement, bool getResults ) const {
   return LineStripCollision( GetVertices(), other.GetVertices(), thisPlacement,
                              otherPlacement, getResults, false, false );
}


Collision LineStrip::
DoCollisionTest( const ol::Line &other, const Placement &thisPlacement,
                 const Placement &otherPlacement, bool getResults ) const {
   std::vector< Vec2D > otherVertices;
   otherVertices.reserve( 2 );
   otherVertices.push_back( other.start );
   otherVertices.push_back( other.end );
   
   return LineStripCollision( GetVertices(), otherVertices, thisPlacement, otherPlacement, getResults, true, false );
}



void LineStrip::
ExecDraw() const {
   Render( 0, 0 );
}


void LineStrip::
Draw( const Rgba &startColor, const Rgba &endColor ) const {
   Render( &startColor, &endColor );
}


void LineStrip::
Render( const Rgba *color1, const Rgba *color2 ) const {
   data.LineStripRender( color1, color2, texture, lineWidth, placement, false );
}


Vec2D LineStrip::
GetVertex( unsigned int index ) const {
   const list< Vec2D > &vertices = GetVertices();
   
   if( index >= vertices.size()) {
      OlError( "Tried to retrieve vertex #" + VarToString( index ) + " from a LineStrip with only "
               + VarToString( vertices.size() ) + " vertices!" );
      
      return Vec2D( 0.0, 0.0 );
   }
   
   list< Vec2D > ::const_iterator iter = vertices.begin();
   
   for( unsigned int i = 0; i < index && iter != vertices.end(); i++ ) {
      iter++;
   }
   
   return *iter;
}


std::string LineStrip::
ToString() const {
    std::ostringstream str;
    str << "LineStrip: Placement: " << placement.ToString() << " Vertices:";
    
    const list< Vec2D > &vertices = GetVertices();
    
    for( list< Vec2D > ::const_iterator iter = vertices.begin(); iter != vertices.end(); iter++ ) {
       str << " ( " << iter->x << ", " << iter->y << " )";
    }
    
    return str.str();
}

