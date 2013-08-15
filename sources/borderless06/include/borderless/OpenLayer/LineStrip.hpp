#ifndef OL_LINE_STRIP_HPP
#define OL_LINE_STRIP_HPP

#include "Shape.hpp"
#include "Vec2D.hpp"
#include "Placement.hpp"
#include "Line.hpp"
#include "RawLineStrip.hpp"
#include "Declspec.hpp"
#include <list>


namespace ol {


class Bitmap;

// A series of lines clued together //

class OL_LIB_DECLSPEC LineStrip : public Shape {
public:
   LineStrip( float lineWidth = 1.0, const Bitmap *texture = 0 )
      : Shape( lineWidth ), texture( texture ) {}

   template< class std_container >
   LineStrip( const std_container &vertices, float lineWidth = 1.0, const Bitmap *texture = 0 );

   virtual ~LineStrip() {}
   
   // Draw the line strip filled with a color //
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

      ExecDraw();

   #ifdef OL_NO_STATE_CHANGE
      if( texturesEnabled )
         glEnable( GL_TEXTURE_2D );
   #endif
   }

   // Draw the line strip with a gradient //
   virtual void Draw( const Rgba &startColor, const Rgba &endColor ) const;

   // Moves the line strip by the specified amount //
   virtual void MoveBy( const Vec2D &amount ) {
		placement.MoveBy( amount );
   }

   virtual void MoveTo( const Vec2D &position ) {
   	this->placement.SetPosition( position );
   }
   
   inline void SetPlacement( const Placement &placement ) {
      this->placement = placement;
   }
   
   virtual void TransformBy( const Placement &placement ) {
      this->placement += placement;
   }
   
   inline const Placement &GetPlacement() const {
      return placement;
   }

   // Sets the filler texture of the line strip //
   inline void SetTexture( const Bitmap &texture ) {
      this->texture = &texture;
   }

   // Disables the filler texture //
   inline void DisableTexture() {
      texture = 0;
   }
   
   // Add a vertex to the end of the line strip //
   inline void AddToEnd( Vec2D vertex ) {
      data.AddToEnd( vertex );
   }

   // Add a vertex to the beginning of the line strip //
   inline void AddToBegin( Vec2D vertex ) {
      data.AddToBegin( vertex );
   }

   // Delete the first vertex of the line strip //
   inline void DeleteFirst() {
      data.DeleteFirst();
   }

   // Delete the first last of the line strip //
   inline void DeleteLast() {
      data.DeleteLast();
   }
   
   // Returns the specified vertex //
   Vec2D GetVertex( unsigned int index ) const;
   
   // Returns the specified segment //
   inline Line GetSegment( int index ) const {
      return Line( GetVertex( index ), GetVertex( index+1 ));
   }
   
   // Returns the number of vertices //
   inline int GetNumOfVertices() const {
      return data.GetVertices().size();
   }
   
   const std::list< Vec2D > &GetVertices() const {
      return data.GetVertices();
   }
   
   virtual std::string ToString() const;
   
   // The following functions could be derived from Set/GetPlacement
   
	inline void SetRotationAngle( float angle ) {
		placement.SetRotation( angle );
	}

	inline float GetRotationAngle() {
		return placement.GetRotation();
	}

	virtual void RotateBy( float angle ) {
		placement.RotateBy( angle );
	}
	
	// Collision routines
   
   inline bool Collides( const LineStrip &other, const Placement &thisPlacement,
                  const Placement &otherPlacement ) const {
      return DoCollisionTest( other, thisPlacement, otherPlacement, false ).IsCollision();
   }
   
   
   inline Collision GetCollision( const LineStrip &other, const Placement &thisPlacement,
                           const Placement &otherPlacement ) const {
      return DoCollisionTest( other, thisPlacement, otherPlacement, true );
   }
   
   
   inline Collision GetCollision( const Line &other, const Placement &thisPlacement,
                           const Placement &otherPlacement ) const {
      return DoCollisionTest( other, thisPlacement, otherPlacement, true );
   }
   
private:
   Collision DoCollisionTest( const LineStrip &other, const Placement &thisPlacement,
                              const Placement &otherPlacement, bool getResults = true ) const;
   
   Collision DoCollisionTest( const Line &other, const Placement &thisPlacement,
                              const Placement &otherPlacement, bool getResults = true ) const;
   
   // Draws the line strip with the specified color //
   void ExecDraw() const;

   // Draws the line strip with the specified color //
   void ExecDrawOutline() const {
      ExecDraw();
   }
   
   // Raw line strip rendering function //
   void Render( const Rgba *color1, const Rgba *color2 ) const;
   
   RawLineStrip< std::list< Vec2D >, std::list< float > > data;
   
   Placement placement;

   const Bitmap *texture;
};


// TEMPLATES //

template< class std_container >
LineStrip::
LineStrip( const std_container &theVertices, float lineWidth, const Bitmap *texture )
   : Shape( lineWidth ), texture( texture ) {
   for( typename std_container::const_iterator iter = theVertices.begin(); iter != theVertices.end(); iter++ ) {
      AddToEnd( *iter );
   }
}


}

#endif // OL_LINE_STRIP_HPP
