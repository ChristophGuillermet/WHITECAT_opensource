#ifndef OL_FPS_COUNTER_HPP
#define OL_FPS_COUNTER_HPP

#include "Declspec.hpp"

namespace ol {

// FpsCounter - Make the game to run at the same speed in every computer and calculate the fps //


#define AUTO -1


class OL_LIB_DECLSPEC FpsCounter {
public:
   // Start the timer, defaultFps is the fps you're planning the game to run in //
   static void Start( float defaultFps );
   
   // Pause the fps counter //
   static void Pause();
   
   // Resumes the paused fps counter //
   static void Resume();
   
   // The old name "Continue" for the Resume-function can still be used //
   inline static void Continue() {
      Resume();
   }
   
   // You'll have to call this once per frame! //
   // This function now also returns the delta time //
   static float NewFrameStarted();
   
   // Get the delta time of the frame. You'll have to multiply all movements and //
   // accelerations by the delta time to keep the game speed undependant of the fps //
   static float GetDeltaTime();
   
   // Retrieve the fps //
   static float GetFps();
   
   // Called from the timer, don't touch ;) //
   static void Tick();
   
   // Sets the number of frames between which the delta time will be calculated //
   // Pass AUTO to let OpenLayer to dynamically adjust the settings for //
   // the current situation //
   static void SetNumOfAveragedFrames( int averagedFrames );
   
private:
   static float defaultFps;
   static volatile int timer;
   static int frameCount;
   
   static float fps;
   static float deltaTime;
   
   static bool paused;
   static int pausedTimer;
   
   static int framesPerCheck;
   static int quickFPC;
   static int normalFPC;
   static float actionLimit;
   static bool useAdvanced;
};


}


#endif // OL_FPS_COUNTER_HPP
