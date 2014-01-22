#include "Shape.hpp"
#include "Collisions.hpp"
#include "Line.hpp"
#include "Declspec.hpp"
#include <utility>


template< class std_container1, class std_container2 >
ol::Collision ol::Shape::
LineStripCollision( const std_container1 &vertices, const std_container2 &otherVertices,
                    const Placement &thisPlacement, const Placement &otherPlacement,
                    bool getResults, bool thisConnectFirstAndLast, bool otherConnectFirstAndLast ) const {
   
   if( vertices.size() < 2 || otherVertices.size() < 2 ) {
      OlError( "An empty shape can't ever collide!" );
      return Collision( false );
   }
   
   Vec2D thisCo = thisPlacement.GetPosition();
   Vec2D otherCo = otherPlacement.GetPosition();
   
   Matrix2D thisTransform = thisPlacement.Get2DMatrix();
   Matrix2D otherTransform = otherPlacement.Get2DMatrix();
   
   typename std_container1::const_iterator thisIter = vertices.begin();
   
   const Vec2D rotationPivot = thisPlacement.GetRotationPivot();
   const Vec2D otherRotationPivot = otherPlacement.GetRotationPivot();
   
   Vec2D thisPrev = thisTransform.Transform( *thisIter - rotationPivot ) + thisCo + rotationPivot;
   
   thisIter++;
   
   std::vector< LinePair * > segmentLists;
   
   // Loop through each vertex //
   while( true ) {
      bool breakNow = false;
      
      // Test if we've reached the last line segment //
      if( thisIter == vertices.end() ) {
         if( !thisConnectFirstAndLast ) {
            break;
         }
         
         breakNow = true;
         thisIter = vertices.begin();
      }
      
      Vec2D thisVertex = thisTransform.Transform( *thisIter - rotationPivot ) + thisCo + rotationPivot;
      thisIter++;

      typename std_container2::const_iterator otherIter = otherVertices.begin();
      Vec2D otherPrev = otherTransform.Transform( *otherIter - otherRotationPivot ) + otherCo + otherRotationPivot;
      otherIter++;
      
      // Loop through each vertex of the other polygon //
      while( true ) {
         bool breakNow = false;
         
         // Test if we've reached the last line segment of the other polygon //
         if( otherIter == otherVertices.end() ) {
            if( !otherConnectFirstAndLast ) {
               break;
            }
            
            breakNow = true;
            otherIter = otherVertices.begin();
         }

         Vec2D otherVertex = otherTransform.Transform( *otherIter - otherRotationPivot )
                             + otherCo + otherRotationPivot;
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
               
               segmentLists.push_back( new LinePair( thisLine, otherLine ));
               
               /*
               return Collision( thisLine, otherLine );*/
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
   
   if( !segmentLists.empty() ) {
      return Collision( segmentLists );
   }
   
   return Collision( false );
}


