#ifndef OL_POINT_HPP
#define OL_POINT_HPP

#include "Shape.hpp"
#include "Vec2D.hpp"
#include "Declspec.hpp"


namespace ol {


class OL_LIB_DECLSPEC Point : public Shape {
public:
   Point( Vec2D pos )
      : pos( pos ) {}
   
   Point( float x, float y )
      : pos( x, y ) {}
   
   // Draws the shape with the specified color //
   inline void Draw( const Rgba &color ) {
      OL_SHAPE_START_RENDERING()
      color.Select();
      ExecDraw();
      OL_SHAPE_FINISH_RENDERING()
   }
   
   // Draws the outline of the shape with the specified color //
   inline void DrawOutline( const Rgba &color ) {
      Draw( color );
   }

   static inline void StartFastDrawing() {
      glBegin( GL_POINTS );
   }

   inline void DrawFast( const Rgba &color ) {
      color.Select();
      glVertex2f( pos.x + 0.5, pos.y + 0.5 );
   }

   static inline void FinishFastDrawing() {
      glEnd();
   }

   inline void MoveBy( const Vec2D &amount ) {
      pos += amount;
   }
   inline void MoveTo( const Vec2D &position ) {
   	pos = position;
   }
   
   /*
    * return this back to normal if problems arise
      
   virtual void TransformBy( const Placement &placement );
   
   virtual std::string ToString() const;
   */
   
   virtual void TransformBy(const Placement &placement);
 
   virtual std::string ToString() const;
   
   Vec2D pos;

protected:
   inline void ExecDraw() const {
      glBegin( GL_POINTS );
         glVertex2f( pos.x + 0.5, pos.y + 0.5 );
      glEnd();
   }


   inline void ExecDrawOutline() const {
      ExecDraw();
   }
};


}




#endif // OL_POINT_HPP
