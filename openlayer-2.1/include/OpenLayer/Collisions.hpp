#ifndef COLLISIONS_HPP
#define COLLISIONS_HPP


#include "Vec2D.hpp"
#include "Declspec.hpp"
#include <utility>
#include <vector>

namespace ol {


class Line;


enum CollidingObject {
   OBJ_A = 0,
   OBJ_B = 1,
   NUM_OBJS = 2
};


typedef std::pair< const Line&, const Line& > LinePair;


class OL_LIB_DECLSPEC Collision {
public:
   Collision( bool isCollision = false )
      : isCollision( isCollision ) {
      normals[(int) OBJ_A] = 0;
      normals[(int) OBJ_B] = 0;
	}
   
   Collision( const Line &aSegment, const Line &bSegment );
   
// GRRRRR erreur !
// les occurences de std::vector< std::pair< Line, Line > *> posent de gros
// problèmes ilfaut changer ça !

// On pourrait essayer des std::vector< std::pair< const Line&, const Line& > *>
// à la place.

   Collision( const std::vector< LinePair * > &segmentLists );


   Collision( const Vec2D &aNormal, const Vec2D &bNormal, const Vec2D &collisionPoint )
      : isCollision( true ) {
      normals[(int) OBJ_A] = new Vec2D( aNormal );
      normals[(int) OBJ_B] = new Vec2D( bNormal );
      points.push_back( collisionPoint );
   }
   

   Collision( const Collision& c );
   ~Collision();


   // Returns true if a collision occured //
   inline bool IsCollision() {
      return isCollision;
   }


   // Same as above using a conversion to bool //
   inline operator bool() {
      return IsCollision();
   }


   // Returns the exact point of collision //
   inline Vec2D GetPoint() {
      if( !points.empty() ) {
         return points[0];
      }
      else {
         return Vec2D( 0.0f, 0.0f );
      }
   }
   

   // Returns the all collision points //
   inline const std::vector< Vec2D > &GetAllPoints() const {
      return points;
   }
   
   // Returns a colliding line segment for OBJ_A or OBJ_B //
   const Line GetSegment( CollidingObject objectID );
   
   // Returns all colliding segments //
   inline const std::vector< LinePair * > &GetAllSegments() {
      return segmentLists;
   }
   
   // Returns the normal of the collision point for OBJ_A or OBJ_B //
   Vec2D GetNormal( CollidingObject objectID );

   Collision& operator =( const Collision& c );

private:
   Line CreateVirtualSegment( const Vec2D &normal );
   
   bool isCollision;
   
   std::vector< Vec2D > points;
   
   std::vector< LinePair * > segmentLists;
   Vec2D *normals[(int) NUM_OBJS];

   const static Line DEFAULT_SEGMENT;
   const static Vec2D DEFAULT_NORMAL;
};



}





#endif // COLLISIONS_HPP
