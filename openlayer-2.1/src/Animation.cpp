#include "Animation.hpp"

using namespace std;
using namespace ol;

bool Animation::
Load( string filenameBegin, string extension, int numNumbers ) {
   frames = Bitmap::LoadListOfBitmaps( filenameBegin, extension, numNumbers );
   if( frames.empty() ) {
      throw( "No animation frames loaded" );
      return false;
   }
   return true;
}



bool Animation::
Update( float deltaTime ) {
   currentFrame += deltaTime * framesPerTick;
   if( currentFrame >= frames.size() ) {
      currentFrame -= frames.size();
      return true;
   }
   return false;
}



const Bitmap &Animation::
GetFrame() {
   return *(frames[int( currentFrame )]);
}

