#ifndef OL_TRANSFORMS_HPP
#define OL_TRANSFORMS_HPP

#include "Includes.hpp"
#include "Vec2D.hpp"
#include "Placement.hpp"
#include "Declspec.hpp"

//#include "Rgba.hpp"

namespace ol {


// Transforms - Global transformations class //
// Note that the transformations only apply to the objects which  //
// are renderd after the transformation is set //

// This class was renamed from Screen to Transforms for compability reasons //


// Advanced users: Note that the original transformation state is stored //
// in the matrix stack //


class Rgba;

class OL_LIB_DECLSPEC Transforms {
public:
   // Set the position of the whole screen (default: (0.0, 0.0)) //
   static void SetPosition( float x, float y );
   static void SetPosition( const Vec2D& pos );

   // Rotate the contents of the whole screen around the pivot (default: 0.0 ) //
   static void SetRotation( float angle );

   // Set the pivot point of the screen rotation (default: centre of the screen) //
   static void SetRotationPivot( float pivotX, float pivotY );
   static void SetRotationPivot( const Vec2D& pivot );

   // Sets the stretch of the whole screen, 1.0 is the normal size //
   static void SetStretch( float xStretch, float yStretch );
   static void SetStretch( const Vec2D& stretch );

	// Sets the current transform state to the
	static void SetPlacement( const Placement& placement, const Vec2D& pivot );

   // Get rid of all active position transformations //
   // NOTE: Empties the Transforms stack!! //
   static void ResetPlacement();

   // The old name of the ResetPlacement -function //
   inline static void ResetTransforms() {
      ResetPlacement();
   }
   

   // Pushes the placement state of Transforms (position, rotation and stretch) in a stack //
   static void PushPlacement();

   // Pops the placement state of Transforms from the stack (see above) //
   static void PopPlacement();


   // Tint the whole screen to a color (default: Rgba::INVISIBLE) //
   static void SetTintColor( const Rgba& color );

   // Sets the coefficients of the red, green, blue and alpha channels //
   // The default is Rgba::WHITE (all channels are 1.0) //
   static void SetColorChannels( const Rgba& cofficients );

   // Returns the coefficients of the color channels //
   static const Rgba& GetColorChannels();

   // You don't usually need to call any of the following functions //

   // Applies the active transformations //
   static void ApplyTransforms();

   // Applies the active tinting //
   static void ApplyTinting();


private:
   static bool transformationStored;
   static float x, y;
   static float angle;
   static float pivotX, pivotY;
   static float xStretch, yStretch;
   static Rgba color;
   static Rgba colorChannels;
   static int stackCounter;
};



// You can #define OL_USE_SCREEN_ALIAS to use //
// the name Screen instead of Transforms //

#ifdef OL_USE_SCREEN_ALIAS

   #define Screen Transforms

#endif // OL_USE_SCREEN_ALIAS


}

#endif // OL_TRANSFORMS_HPP
