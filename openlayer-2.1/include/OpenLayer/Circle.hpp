#ifndef OL_CIRCLE_HPP
#define OL_CIRCLE_HPP


#include "Shape.hpp"
#include "Settings.hpp"
#include "Rgba.hpp"
#include "Placement.hpp"
#include "Declspec.hpp"
#include <cmath>
#include <vector>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif


namespace ol {


// The ellipse shape //

class OL_LIB_DECLSPEC Ellipse : public Shape {
public:
   virtual ~Ellipse() {}

   Ellipse()
      : Shape( 1.0 )
      { SetAccuracy( Settings::GetCircleAccuracy()); }

   Ellipse( float x, float y, float xRadius, float yRadius, float lineWidth = 1.0,
      float angle = 0.0, float accuracy = Settings::GetCircleAccuracy() )

      : Shape( lineWidth ), pos( x, y ), angle( angle ), xRad( xRadius ), yRad( yRadius )
      { SetAccuracy( accuracy ); }

   Ellipse( Vec2D pos, Vec2D radiuses, float lineWidth = 1.0,
      float angle = 0.0, float accuracy = Settings::GetCircleAccuracy() )

      : Shape( lineWidth ), pos( pos ), angle( angle ), xRad( radiuses.x ), yRad( radiuses.y )
      { SetAccuracy( accuracy ); }

   inline void Draw( const Rgba &color ) const {
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

   // Draws the circle with colors smoothly changing from center to outer edge //
   void Draw( const Rgba &innerColor, const Rgba &outerColor ) const;

   // Draws a slice of the circle //
   void DrawSlice( const Rgba &color, float startAngle, float angleSweep ) const;

   // Draws a gradient filled slice //
   void DrawSlice( const Rgba &innerColor, const Rgba &outerColor,
                   float startAngle, float angleSweep ) const;

   // Draws a filled disk //
   void DrawDisk( const Rgba &color, float innerXRadius, float innerYRadius ) const {
      color.Select();
      DiskRender( color, color, innerXRadius, innerYRadius, false );
   }

   // Draws a gradient filled disk //
   inline void DrawDisk( const Rgba &innerColor, const Rgba &outerColor,
                  float innerXRadius, float innerYRadius ) const {
      DiskRender( innerColor, outerColor, innerXRadius, innerYRadius, true );
   }

   // Draws an arc of the ellipse //
   void DrawArc( const Rgba &color, float startAngle, float angleSweep,
                 float innerXRadius, float innerYRadius ) const;

   // Draws an arc of the ellipse //
   void DrawArc( const Rgba &innerColor, const Rgba &outerColor,
                 float startAngle, float angleSweep,
                 float innerXRadius, float innerYRadius ) const;
   
   inline std::vector< Vec2D > ToPolygon() const {
      return ToPolygon( 0.0, 2.0 * AL_PI );
   }
   
   std::vector< Vec2D > ToPolygon( float startAngle, float angleSweep ) const;
   
   // Moves the circle by the specified amount //
   virtual void MoveBy( const Vec2D &amount ) {
      pos += amount;
   }

   virtual void MoveTo( const Vec2D &position ) {
   	pos = position;
   }
   
   virtual void TransformBy( const Placement &placement ) {
      pos += placement.GetPosition();
      angle += placement.GetRotation();
      xRad *= placement.GetStretch();
      yRad *= placement.GetStretch();
   }

   // Sets the x or y radius of the ellipse //
   // Axis should be: X_AXIS or Y_AXIS //
   inline void SetRadius( Axis axis, float radius ) {
      switch( axis ) {
         case X_AXIS: xRad = radius; break;
         case Y_AXIS: yRad = radius; break;
      }
   }

   // Returns the x or y radius of the ellipse //
   inline float GetRadius( Axis axis ) {
      switch( axis ) {
         case X_AXIS: return xRad; break;
         case Y_AXIS: return yRad; break;
      }
   }
   
   virtual void RotateBy( float angle );
   
   // Sets the visual accuracy of the circle //
   void SetAccuracy( float accuracy ) {
      float radius = std::max( xRad, yRad );
      
      angleIncrement = std::min(( 2.0 * asin( 1.0/radius )/accuracy ), 0.35 * AL_PI );
   }
   
   Vec2D pos;
   float angle;

   void ExecDraw() const;
   // Draws the circle //

   // Draws an outline of the circle //
   void ExecDrawOutline() const;

   // Raw disk rendering function //
   void DiskRender( const Rgba &innerCol, const Rgba &outerCol,
                    float innerXRad, float innerYRad, bool setCols ) const;
   
   virtual std::string ToString() const;

protected:
   float xRad;
   float yRad;

   float angleIncrement;
};


// The circle shape //

class OL_LIB_DECLSPEC Circle : public Ellipse {
public:
   Circle( Vec2D pos, float radius,
           float lineWidth = 1.0, float accuracy = Settings::GetCircleAccuracy() )
      : Ellipse( pos, Vec2D( radius, radius ), lineWidth, 0.0, accuracy ) {}
   
   Circle( float x, float y, float radius,
           float lineWidth = 1.0, float accuracy = Settings::GetCircleAccuracy() )
      : Ellipse( Vec2D( x, y ), Vec2D( radius, radius ), lineWidth, 0.0, accuracy ) {}
   
   // Draws a filled disk //
   inline void DrawDisk( const Rgba &color, float innerRadius ) const {
      Ellipse::DrawDisk( color, innerRadius, innerRadius );
   }

   // Draws a gradient filled disk //
   inline void DrawDisk( const Rgba &innerColor, const Rgba &outerColor, float innerRadius ) const {
      Ellipse::DrawDisk( innerColor, outerColor, innerRadius, innerRadius );
   }

   // Draws an arc of the circle //
   inline void DrawArc( const Rgba &color, float startAngle, float angleSweep, float innerRadius ) const {
      Ellipse::DrawArc( color, startAngle, angleSweep, innerRadius, innerRadius );
   }

   // Sets the radius of the circle //
   inline void SetRadius( float radius ) {
      xRad = radius;
      yRad = radius;
   }
};


}



#endif // OL_CIRCLE_HPP
