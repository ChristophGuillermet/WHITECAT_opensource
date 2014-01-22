#ifndef OL_POLYGON_HPP
#define OL_POLYGON_HPP

#include "RawLineStrip.hpp"
#include "Vec2D.hpp"
#include "Rgba.hpp"
#include "Internal.hpp"
#include "General.hpp"
#include "Shape.hpp"
#include "Placement.hpp"
#include "Collisions.hpp"
#include "LineStrip.hpp"
#include "Line.hpp"
#include "Declspec.hpp"

#include <vector>

namespace ol {

class Bitmap;
class LineStrip;


class OL_LIB_DECLSPEC Poly : public Shape {
public:
   Poly( Vec2D rotationPivot = Vec2D( 0.0, 0.0 ))
      : outlineTexture( 0 ) { placement.SetRotationPivot( rotationPivot ); }

   // Construct the polygon from a list of vertices //
   template< class std_container >
   Poly( const std_container &vertices, Vec2D rotationPivot = Vec2D( 0.0, 0.0 ));
   
   // Construct the polygon from a list of vertices //
   Poly( const Vec2D *vertices, int numVertices, Vec2D rotationPivot = Vec2D( 0.0, 0.0 ));
   
   virtual ~Poly();
   
   // Adds a vertex to the polygon //
   virtual void Add( Vec2D vec );
   
   virtual void SetVertex( int index, const Vec2D &newValue );
   
   // Returns a reference to the vertex with the given index //
   inline const Vec2D &GetVertex( unsigned int index ) const {
      static Vec2D dummyValue;

      if( index < 0 || index >= data.GetVertices().size()) {
         OlError( "Invalid vertex index: " + VarToString( index )
            + " ( Number of vertices: " + VarToString( data.GetVertices().size()) + " )" );
         return dummyValue;
      }
      
      return data.GetVertices()[index];
   }
   
   
   inline Vec2D &GetVertex( unsigned int index ) {
      static Vec2D dummyValue;

      if( index < 0 || index >= data.GetVertices().size()) {
         OlError( "Invalid vertex index: " + VarToString( index )
            + " ( Number of vertices: " + VarToString( data.GetVertices().size()) + " )" );
         return dummyValue;
      }
      
      return data.GetVertices()[index];
   }
   
   inline int GetNumberOfVertices() {
      return data.GetVertices().size();
   }
   
   inline void Draw( const Rgba &color ) const {
   #ifdef OL_NO_STATE_CHANGE
      GLboolean texturesEnabled;
      glGetBooleanv( GL_TEXTURE_2D, &texturesEnabled );
   #endif
      glDisable( GL_TEXTURE_2D );
      color.Select();

      ExecDraw();

   #ifdef OL_NO_STATE_CHANGE
      if( texturesEnabled )
         glEnable( GL_TEXTURE_2D );
   #endif
   }

   void DrawOutline( const Rgba &color ) const {
   #ifdef OL_NO_STATE_CHANGE
      GLboolean texturesEnabled;
      glGetBooleanv( GL_TEXTURE_2D, &texturesEnabled );
   #endif
      glDisable( GL_TEXTURE_2D );
      color.Select();

      ExecDrawOutline();

   #ifdef OL_NO_STATE_CHANGE
      if( texturesEnabled )
         glEnable( GL_TEXTURE_2D );
   #endif
   }

   // Tests if two polygons collide //
   
   inline bool Collides( const Poly &other, const Placement &thisPlacement,
                  const Placement &otherPlacement ) const {
      return DoCollisionTest( other, thisPlacement, otherPlacement, false ).IsCollision();
   }

	inline bool Collides( const Poly& other ) const {
		return DoCollisionTest( other, placement, other.placement, false ).IsCollision();
	}
   
   // Tests if two polygons collide and gives detailed information about the collision //
   
   inline Collision GetCollision( const Poly &other, const Placement &thisPlacement,
                  const Placement &otherPlacement ) const {
      return DoCollisionTest( other, thisPlacement, otherPlacement );
   }

	inline Collision GetCollision( const Poly &other ) const {
		return DoCollisionTest( other, placement, other.placement );
	}

   // Returns a constant reference to the list of the vertices //
   inline const std::vector< Vec2D > &GetVertices() const { return data.GetVertices(); }
   
   // Returns the specified segment //
   inline Line GetSegment( unsigned int index ) const {
      if( index >= 0 && index < data.GetVertices().size()-1 ) {
         return Line( data.GetVertices()[index], data.GetVertices()[index+1] );
      }
      return Line();
   }
   
   // Moves the polygon by the specified amount //
   virtual void MoveBy( const Vec2D &amount ) {
   	placement.MoveBy( amount );
   }
   
	// Sets the position of the polygon
	inline virtual void MoveTo( const Vec2D &position ) {
		placement.SetPosition( position );
	}
	
	virtual void RotateBy( float angle ) {
      placement.RotateBy( angle );
   }
	
	// Sets the placement of the polygon
	inline void SetPlacement( const Placement &placement ) {
      this->placement = placement;
   }
   
   virtual void TransformBy( const Placement &placement ) {
      this->placement += placement;
   }
   
	// Returns the placement of the polygon
   inline const Placement &GetPlacement() const {
      return placement;
   }
   /*
   virtual Poly ToPolygon() const {
      return *this;
   }
   */
   // Sets the outline texture of the polygon, pass zero to get rid of the outline texture //
   inline void SetOutlineTexture( ol::Bitmap *texture, OutlineTextureMode mode = OPTIMIZE ) {
      outlineTexture = texture;
   }
   
   virtual std::string ToString() const;
   
   // The following functions could be derived from Set/GetPlacement
   
	inline void SetRotationAngle( float angle ) {
		placement.SetRotation( angle );
	}
   
	inline float GetRotationAngle() const {
		return placement.GetRotation();
	}
   
   inline void SetPivot( Vec2D rotationPivot ) {
      placement.SetRotationPivot( rotationPivot );
   }

   inline Vec2D GetPivot() const {
      return placement.GetRotationPivot();
   }
   
   // Collision between other Shapes
   
   inline bool Collides( const LineStrip &other, const Placement &thisPlacement,
                  const Placement &otherPlacement ) const {
      return DoCollisionTest( other, thisPlacement, otherPlacement, false ).IsCollision();
   }
   
   
   inline bool Collides( const Line &other ) {
      return DoCollisionTest( other, placement, other.GetPlacement(), false );
   }
   
   
   inline Collision GetCollision( const LineStrip &other, const Placement &thisPlacement,
                           const Placement &otherPlacement ) const {
      return DoCollisionTest( other, thisPlacement, otherPlacement, true );
   }
   
   
   inline bool Collides( const LineStrip &other ) const {
      return DoCollisionTest( other, placement, other.GetPlacement(), false ).IsCollision();
   }
   
   
   inline Collision GetCollision( const LineStrip &other ) const {
      return DoCollisionTest( other, placement, other.GetPlacement(), true );
   }
   
   inline Collision GetCollision( const Line &other, const Placement &thisPlacement,
                  const Placement &otherPlacement ) const {
      return DoCollisionTest( other, thisPlacement, otherPlacement, true );
   }
   
   inline Collision GetCollision( const Line &other ) {
      return DoCollisionTest( other, placement, other.GetPlacement(), true );
   }
   
protected:
   Collision DoCollisionTest( const Poly &other, const Placement &thisPlacement,
                              const Placement &otherPlacement, bool getResults = true ) const;
   
   Collision DoCollisionTest( const LineStrip &other, const Placement &thisPlacement,
                              const Placement &otherPlacement, bool getResults = true ) const;
   
   Collision DoCollisionTest( const Line &other, const Placement &thisPlacement,
                              const Placement &otherPlacement, bool getResults = true ) const;
   
                     
   // Draws the polygon to the active canvas //
   virtual void ExecDraw() const;

   // Draws the outline of the polygon to the active canvas //
   virtual void ExecDrawOutline() const;
   
   RawLineStrip< std::vector< Vec2D >, std::vector< float > > data;
   
	Placement placement;
   Bitmap *outlineTexture;
   OutlineTextureMode outlineMode;
};



// TEMPLATES //

#include "Internal.hpp"


template< class std_container >
Poly::Poly( const std_container &theVertices, Vec2D rotationPivot )
   : outlineTexture( 0 ) {
   
   placement.SetRotationPivot( rotationPivot );
   data.GetVertices().reserve( theVertices.size() );
   
   for( typename std_container::const_iterator iter = theVertices.begin(); iter != theVertices.end(); iter++ ) {
      data.AddToEnd( *iter );
   }
}



}


#endif // OL_POLYGON_HPP
