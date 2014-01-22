#include "FpsCounter.hpp"
#include "Includes.hpp"
using namespace ol;



// CONSTANTS //

static const int MS_PER_TICK = 1;
static const int FRAMES_PER_CHECK = 30;


// STATIC CLASS VARIABLES //

float FpsCounter::defaultFps = 60.0;

volatile int FpsCounter::timer = 0;  
int FpsCounter::frameCount = 0; 

float FpsCounter::fps = 0.0;
float FpsCounter::deltaTime = 0.0;

bool FpsCounter::paused = false;
int FpsCounter::pausedTimer = 0;

int FpsCounter::framesPerCheck = 30;
int FpsCounter::normalFPC = 50;
int FpsCounter::quickFPC = 10;
bool FpsCounter::useAdvanced = true;
float FpsCounter::actionLimit = 0.3;


// GENERAL FUNCTIONS //

void FpsCounter::
SetNumOfAveragedFrames( int averagedFrames ) {
   if( averagedFrames == AUTO ) {
      useAdvanced = true;
      framesPerCheck = 30;
   }
   else {
      useAdvanced = false;
      framesPerCheck = averagedFrames;
   }
}


void FpsCounter::
Start( float defaultFps ) {
   FpsCounter::defaultFps = defaultFps;
   timer = 0;
   frameCount = 0;
   frameCount = 0;
   framesPerCheck = normalFPC;
   
   paused = false;
   
   install_int( FpsCounter::Tick, MS_PER_TICK );
}



float FpsCounter::
NewFrameStarted() {
   if( !paused && ++FpsCounter::frameCount > framesPerCheck && FpsCounter::timer > 0 ) {
      float lastFps = fps;
      fps = 1000.0 * float(FpsCounter::frameCount)/float(FpsCounter::timer * MS_PER_TICK);
      
      if( useAdvanced ) {
         float ratio = fps/lastFps;
         
         if( 1.0 - ratio > actionLimit || 1.0 - 1.0/ratio > actionLimit ) {
            framesPerCheck /= 2;
            if( framesPerCheck < quickFPC )
               framesPerCheck = quickFPC;
         }
         else {
            framesPerCheck *= 2;
            if( framesPerCheck > normalFPC )
               framesPerCheck = normalFPC;
         }
      }
      
      frameCount = 0;
      timer = 0;
      deltaTime = defaultFps/fps;
   }
   
   return deltaTime;
}



void FpsCounter::
Tick() {
   timer++;
}




float FpsCounter::
GetDeltaTime() {
   return deltaTime;
}




float FpsCounter::
GetFps() {
   return fps;
}



void FpsCounter::
Pause() {
   paused = true;
   pausedTimer = timer;
}



void FpsCounter::
Resume() {
   paused = false;
   timer = pausedTimer;
}


