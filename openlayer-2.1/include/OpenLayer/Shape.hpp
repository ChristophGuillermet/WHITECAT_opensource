#ifndef OL_SHAPE_HPP
#define OL_SHAPE_HPP


#include "Includes.hpp"
#include "Rgba.hpp"
#include "Vec2D.hpp"
#include "Collisions.hpp"
#include "Declspec.hpp"


#ifdef OL_NO_STATE_CHANGE
   #define OL_SHAPE_START_RENDERING() \
      GLboolean texturesEnabled; \
      glGetBooleanv( GL_TEXTURE_2D, &texturesEnabled ); \
      glDisable( GL_TEXTURE_2D );

#else // OL_NO_STATE_CHANGE

   #define OL_SHAPE_START_RENDERING() glDisable( GL_TEXTURE_2D );

#endif // OL_NO_STATE_CHANGE


#ifdef OL_NO_STATE_CHANGE
   #define OL_SHAPE_FINISH_RENDERING() \
      if( texturesEnabled ) \
         glEnable( GL_TEXTURE_2D );

#else // OL_NO_STATE_CHANGE

   #define OL_SHAPE_FINISH_RENDERING() (void) 0;

#endif // OL_NO_STATE_CHANGE

namespace ol {

class Poly;


// The base class of all Shapes //

class OL_LIB_DECLSPEC Shape {
public:
   Shape( float lineWidth = 1.0 )
      : lineWidth( lineWidth ), displayList( 0 ) {}

   virtual ~Shape();
   
   void Draw( const Rgba &color ) const {
      OL_SHAPE_START_RENDERING()

      color.Select();
      ExecDraw();

      OL_SHAPE_FINISH_RENDERING()
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


   // Returns true if this and the other Shape collide //
   bool Collides( const Shape &other, const Placement &thisPlacement, const Placement &otherPlacement );


   // Returns true if this and the other Shape collide //
   Collision GetCollision( const Shape &other, const Placement &thisPlacement,
                           const Placement &otherPlacement );


   // Records the results of the drawing function instead of actually drawing the primitive //
   inline void RecordDraw() {
      StartRecording();
      ExecDraw();
      FinishRecording();
   }

   // Same as above but with outlines //
   inline void RecordDrawOutline() {
      StartRecording();
      ExecDrawOutline();
      FinishRecording();
   }

   inline void DrawRecord( const Rgba &color ) const {
   #ifdef OL_NO_STATE_CHANGE
      GLboolean texturesEnabled;
      glGetBooleanv( GL_TEXTURE_2D, &texturesEnabled );
   #endif
      glDisable( GL_TEXTURE_2D );
      color.Select();

      glCallList( displayList );

   #ifdef OL_NO_STATE_CHANGE
      if( texturesEnabled )
         glEnable( GL_TEXTURE_2D );
   #endif
   }

   inline void DrawRecord( const Rgba &color, const Vec2D &displacement ) const {
      glPushMatrix();
      glTranslatef( displacement.x, displacement.y, 0.0 );
      DrawRecord( color );
      glPopMatrix();
   }

   // Moves the shape by the specified amount //
   virtual void MoveBy( const Vec2D &amount ) = 0;

   // Moves the shape to the specified position //
   virtual void MoveTo( const Vec2D &position ) = 0;

   // Rotates the shape by the specified angle //
   virtual void RotateBy( float angle );
   
   // Transforms the shape by a Placement //
   virtual void TransformBy( const Placement &placement );
   
   // Sets the line width of the shape //
   inline void SetLineWidth( float lineWidth ) {
      this->lineWidth = lineWidth;
   }
   
   //virtual Poly ToPolygon() const = 0;

   // Returns the line width of the shape //
   inline float GetLineWidth() {
      return lineWidth;
   }
   
   virtual std::string ToString() const = 0;

protected:
   template< class std_container1, class std_container2 >
   Collision LineStripCollision( const std_container1 &vertices, const std_container2 &otherVertices,
                                 const Placement &thisPlacement, const Placement &otherPlacement,
                                 bool getResults, bool thisConnectFirstAndLast, bool otherConnectFirstAndLast ) const;
   
   virtual void ExecDraw() const = 0;

   virtual void ExecDrawOutline() const = 0;

   inline void StartRecording() {
      displayList = glGenLists( 1 );
      glNewList( displayList, GL_COMPILE );
   }


   inline void FinishRecording() {
      glEndList();
   }

   inline void RotateMatrix( float angle ) const {
      #ifdef OL_ANGLES_IN_DEGREES
         glRotatef( angle, 0.0, 0.0, 1.0 );
      #else // OL_ANGLES_IN_DEGREES
         glRotatef( angle * ( 180.0/AL_PI ), 0.0, 0.0, 1.0 );
      #endif // OL_ANGLES_IN_DEGREES
   }

   float lineWidth;
   int displayList;
};


}



#endif // OL_SHAPES_HPP
