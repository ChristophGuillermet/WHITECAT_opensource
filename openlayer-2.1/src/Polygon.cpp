#include "Polygon.hpp"
#include "Bitmap.hpp"
#include "Line.hpp"
#include "VertexListCollision.hpp"
#include "LineStrip.hpp"
#include "LineStripRender.hpp"

using namespace std;
using namespace ol;


ol::Poly::
Poly( const Vec2D *vertices, int numVertices, Vec2D rotationPivot )
   : outlineTexture( 0 ) {
   placement.SetRotationPivot( rotationPivot );
   for( int i = 0; i < numVertices; i++ ) {
      data.AddToEnd( vertices[i] );
   }
}


ol::Poly::
~Poly() {}


void Poly::
Add( Vec2D vec ) {
   data.AddToEnd( vec );
}


void ol::Poly::
ExecDrawOutline() const {
   data.LineStripRender( 0, 0, outlineTexture, lineWidth, placement, true );
}


void ol::Poly::
SetVertex( int index, const Vec2D &newValue ) {
	if( index < 0 || index >= (int)data.GetVertices().size()) {
		OlError( "Invalid vertex index: " + VarToString( index )
		+ " ( Number of vertices: " + VarToString( (int)data.GetVertices().size()) + " )" );
   }
   
   data.GetVertices()[index] = newValue;
}


void ol::Poly::
ExecDraw() const {
   const vector< Vec2D > &vertices = data.GetVertices();
   
	glPushMatrix();
   placement.Apply();
   glBegin( GL_POLYGON );
   
      for( std::vector< Vec2D > ::const_iterator iter = vertices.begin(); iter != vertices.end(); iter++ ) {
         glVertex2f( iter->x, iter->y );
      }
   
   glEnd();
   glPopMatrix();
}

/*
void ol::Poly::
MoveBy( const Vec2D &amount ) {
   for( std::vector< Vec2D > ::iterator iter = vertices.begin(); iter != vertices.end(); iter++ ) {
      *iter += amount;
   }
}
*/


Collision ol::Poly::
DoCollisionTest( const ol::Poly &other, const Placement &thisPlacement,
                 const Placement &otherPlacement, bool getResults ) const {
   return LineStripCollision( data.GetVertices(), other.data.GetVertices(), thisPlacement,
      otherPlacement, getResults, true, true );
}


Collision ol::Poly::
DoCollisionTest( const ol::LineStrip &other, const Placement &thisPlacement,
                 const Placement &otherPlacement, bool getResults ) const {
   return LineStripCollision( data.GetVertices(), other.GetVertices(),
                              thisPlacement, otherPlacement, getResults, true, false );
}


Collision ol::Poly::
DoCollisionTest( const ol::Line &other, const Placement &thisPlacement,
                 const Placement &otherPlacement, bool getResults ) const {
   std::vector< Vec2D > otherVertices;
   otherVertices.reserve( 2 );
   otherVertices.push_back( other.start );
   otherVertices.push_back( other.end );
   
   return LineStripCollision( data.GetVertices(), otherVertices, thisPlacement, otherPlacement, getResults,
      true, false );
}


std::string ol::Poly::
ToString() const {
    const vector< Vec2D > &vertices = data.GetVertices();
    
    std::ostringstream str;
    str << "Polygon: Placement: " << placement.ToString() << " Vertices:";
    
    for( vector< Vec2D > ::const_iterator iter = vertices.begin(); iter != vertices.end(); iter++ ) {
       str << " ( " << iter->x << ", " << iter->y << " )";
    }
    
    return str.str();
}


/*
Collision ol::Poly::
DoCollisionTest( const ol::Poly &other, const Placement &thisPlacement,
                 const Placement &otherPlacement, bool getResults ) const {
   if( vertices.size() < 2 || other.vertices.size() < 2 ) {
      OlError( "An empty Poly can't ever collide!" );
      return Collision( false );
   }
   
   Vec2D thisCo = thisPlacement.GetPosition();
   Vec2D otherCo = otherPlacement.GetPosition();
   
   Matrix2D thisTransform = thisPlacement.Get2DMatrix();
   Matrix2D otherTransform = otherPlacement.Get2DMatrix();
   
   std::vector< Vec2D > ::const_iterator thisIter = vertices.begin();
   
   Vec2D thisPrev = thisTransform.Transform( *thisIter - rotationPivot ) + thisCo + rotationPivot;
   thisIter++;
   
   // Loop through each vertex //
   while( true ) {
      bool breakNow = false;

      // Test if we've reached the last line segment //
      if( thisIter == vertices.end() ) {
         breakNow = true;
         thisIter = vertices.begin();
      }

      Vec2D thisVertex = thisTransform.Transform( *thisIter - rotationPivot ) + thisCo + rotationPivot;
      thisIter++;

      std::vector< Vec2D > ::const_iterator otherIter = other.vertices.begin();
      Vec2D otherPrev = otherTransform.Transform( *otherIter - other.rotationPivot ) + otherCo + other.rotationPivot;
      otherIter++;

      // Loop through each vertex of the other polygon //
      while( true ) {
         bool breakNow = false;

         // Test if we've reached the last line segment of the other polygon //
         if( otherIter == other.vertices.end() ) {
            breakNow = true;
            otherIter = other.vertices.begin();
         }

         Vec2D otherVertex = otherTransform.Transform( *otherIter - other.rotationPivot ) + otherCo + other.rotationPivot;
         otherIter++;

         // Test for collision //
         if( IsCounterClockwise( thisPrev, thisVertex, otherPrev )
             != IsCounterClockwise( thisPrev, thisVertex, otherVertex )
             &&
             IsCounterClockwise( otherPrev, otherVertex, thisPrev )
             != IsCounterClockwise( otherPrev, otherVertex, thisVertex )) {

            if( !getResults ) {
               return Collision( true );
            }
            else {
               Line thisLine( thisVertex, thisPrev );
               Line otherLine( otherVertex, otherPrev );

               return Collision( thisLine, otherLine );
            }
         }

         // Is last line segment of the other polygon processed? //
         if( breakNow ) {
            break;
         }

         // Advance to the next vertex of the other polygon //
         otherPrev = otherVertex;
      }

      // Is last line segment processed? //
      if( breakNow ) {
         break;
      }

      // Advance to the next vertex //
      thisPrev = thisVertex;
   }

   return Collision( false );
}

*/
