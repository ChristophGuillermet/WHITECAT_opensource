#ifndef OL_ANIMATION_HPP
#define OL_ANIMATION_HPP


#include "Includes.hpp"
#include "Bitmap.hpp"
#include "Declspec.hpp"
#include <vector>


namespace ol {


// Animation - Loads and animates a sequence of bitmaps //
// Animation speed is in animation frames per one game frame //

class OL_LIB_DECLSPEC Animation {
public:
   // Loads the animation frames as filenameBegin01.extension, filenameBegin02.extension etc. //
   Animation( float framesPerTick, std::string filenameBegin, std::string extension, int numNumbers = 2 ) : framesPerTick( framesPerTick ), currentFrame( 0.0 ) { Load( filenameBegin, extension, numNumbers ); }
   
   // Loads the animation by using a list of frames //
   // A copy of the list will be stored in the Animation //
   Animation( float framesPerTick, const std::vector< Bitmap *> &frames ) : frames( frames ), framesPerTick( framesPerTick ), currentFrame( 0.0 ) {}
   
   // Loads the animation frames as filenameBegin01.extension, filenameBegin02.extension etc. //
   // Where numNumbers is the amount of numbers in the filiname //
   bool Load( std::string filenameBegin, std::string extension, int numNumbers = 2 );
   
   // Returns a reference to the current visible frame //
   const Bitmap &GetFrame();
   
   // Updates the animation, deltaTime is the number of game frames elapsed //
   // Returns true if the animation is finished and stared again at the first frame //
   bool Update( float deltaTime );
   
private:
   std::vector< Bitmap *> frames;
   float framesPerTick;
   float currentFrame;
};


}



#endif
