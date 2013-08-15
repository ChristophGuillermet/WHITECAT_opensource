#ifndef OL_SETTINGS_HPP
#define OL_SETTINGS_HPP

#include "Includes.hpp"
#include "Declspec.hpp"


namespace ol {

// Settings - General settings of OpenLayer //

class OL_LIB_DECLSPEC Settings {
public:
   // Turns the anti-aliasing on or off //
   // Anti-aliasing produces smoother graphics but slows down the rendering //
   static void SetAntialiasing( bool turnedOn );
   
   // Sets the accuracy of the circle drawing functions //
   // The value should be between 0.0 and 1.0 but it should never be zero //
   // The more accuracy, the better the circles will generally look //
   // but rendering them will be slower //
   // This setting can be overridden by passing a different value
   // to the circle drawing function //
   static void SetCircleAccuracy( float accuracy );
   
   // Returns the accuracy of the circles //
   inline static float GetCircleAccuracy() {
      return circleAccuracy;
   }
   
   // If you use OpenGL directly you might wish to choose //
   // if the texture mapping is turned on or not //
   // This doesn't affect OpenLayer's functions //
   static void SetTextureMapping( bool turnedOn );
   
   // Returns true if texture mapping is used //
   static bool TextureMappingUsed() { return useTextures; }
   
   // Sets the orthographic projection mode (the "2D drawing mode") //
   // Ortographic projection is on by default //
   static void SetOrthographicProjection( int screenWidth = SCREEN_W, int screenHeight = SCREEN_H );
   
   // Restores old projection that got changed when you called SetOrtographicProjection() //
   static void RestoreOldProjection();
   
   // Turn the memory bitmap storage on or off //
   // Off by default //
   static void StoreMemoryBitmaps( bool turnedOn );
   
   // Returns true if the Bitmaps are stored in the memory //
   static bool MemoryBitmapsStored();
   
   // Sets the accuracy of the generated collision polygons //
   // Accuracy should always be positive and the highest value is 1.0 //
   // The default value is 0.3 //
   static void SetCollisionPolyAccuracy( float accuracy );
   
   // Returns the accuracy of the generated collision polygons //
   static float GetCollisionPolyAccuracy();
   
   // Sets the maxium alpha value which is considered transparent //
   // by the collision polygon generation //
   static void SetCollisionPolyAlphaLimit( float limit ) {
      collisionPolyAlphaLimit = int( limit * 255.0 );
   }
   
   // Returns the maxium alpha value which is considered transparent //
   // by the collision polygon generation //
   static float GetCollisionPolyAlphaLimit() {
      return float( collisionPolyAlphaLimit )/255.0;
   }
   
   static bool GetWindowBorder() {
      return windowBorder;
   }
   static void SetWindowBorder(bool border) {
      windowBorder = border;
   }
   
   friend class GfxRend;
   friend class Bitmap;
private:
   
   static float circleAccuracy;
   static float collisionPolyAccuracy;
   
   static int collisionPolyAlphaLimit;
   
   static bool useAntiAlias;
   static bool useTextures;
   static bool storeMemoryBitmaps;
   
   static bool windowBorder;
};



}



#endif // OL_SETTINGS_HPP
