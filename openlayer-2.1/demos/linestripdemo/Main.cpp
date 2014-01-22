/***
OpenLayer LineStrip Demo
---
Purpose:     This demo demonstrates usage of the ol::LineStrip class, as well as
				 common usage of ol::FpsCounter
Author: 		 Esa Tanskanen
Modified by: Brady Eidson
"Use it wherever you want, as long as you don't claim the code to be your own!"
***/

#include <allegro.h>
#include <OpenLayer.hpp>
#include <loadpng.h>


using namespace ol;
using namespace std;


// A helper function to return a random number between min and max //
float FRand( float min, float max ) {
   return min + float( rand() )/float( RAND_MAX ) * ( max - min );
}

// Enumeration to keep track of the color scheme //
enum ColorMode
{
	WHITE_TO_BLACK = 0,
	RED_TO_BLUE,
	BLUE_TO_GREEN,
	GREEN_TO_RED,
	CYCLING
};

int main() {
   // SETUP //
   // Set up the program with all possible drivers //
   Setup::SetupProgram();

   // Set up the screen in windowed mode with the window size of 800 x 600 //
   Setup::SetupScreen( 800, 600, WINDOWED );

   // Load the font with the size 30 x 24 and a white color //
   TextRenderer neuropol( "Fonts/Neuropol.ttf", 25, 20, Rgba::WHITE );

   // Test if the font was really loaded (maybe it didn't exist) //
   if( !neuropol ) {
      allegro_message( "Couldn't load Fonts/Neuropol.ttf!" );
      exit( -1 );
   }

   // Set up the delta time routines with fps of 100.0 //
   FpsCounter::Start( 100.0 );

   // The initial coordinates of the particle //
   float x = 200.0, y = 100.0;

   // The speed of the particle should be totally random in the beginning //
   float speedX = FRand( 4.0, 6.0 ), speedY = FRand( 4.0, 6.0 );

   // We'll hold a list of the last 100 positions of the particle //
   const int NUM_STORED_COORDS = 100;

   // We have to know when it's the time to add a new position to the list //
   float recordTimer = 0.0;

   // The LineStrip Object we will be using //
	LineStrip strip;

	// Keeps track of the color scheme //
	int colorMode = WHITE_TO_BLACK;

	// For the CYCLING color mode, we will start with a base gray color //
	Rgba cycleColor( 0.5, 0.5, 0.5 );

	// Setup the "speed" at which the cycling color will shift //
	float deltaR = 0.005;
	float deltaG = 0.01;
	float deltaB = 0.02;

	// Keep track of whether or not we're reversing the color gradient when we //
	// render the LineStrip object //
	bool reverseMode = false;

   // Run the game loop until esc is pressed //
   while( !key[KEY_ESC] ) {

      // LOGIC //
      // Inform the fps counter that a new frame has started //
      FpsCounter::NewFrameStarted();

      // Get the delta time //
      float deltaTime = FpsCounter::GetDeltaTime();

      // Keep track of time passed so we can record a point each time at //
      // least 1 frame has passed //
      recordTimer += deltaTime;

      if( recordTimer > 1.0 ) {
         recordTimer = 0.0;

         // If we already have our maximum number of vertices, dump one //
         if( strip.GetNumOfVertices() == NUM_STORED_COORDS )
            strip.DeleteLast();

         // Add the current position to the start of the line strip
         strip.AddToBegin( Vec2D( x, y ));
      }

		// If we are in the CYCLING color mode, we shift our cycling color here //
		// based on the deltaTime //
		if( colorMode == CYCLING ) {
			if( (cycleColor.r += deltaR * deltaTime) > 1.0 ) {
				cycleColor.r = 1.0;
				deltaR *= -1;
			}else if( cycleColor.r < 0.0 ) {
				cycleColor.r = 0.0;
				deltaR *= -1;
			}

			if( (cycleColor.g += deltaG * deltaTime) > 1.0 ) {
				cycleColor.g = 1.0;
				deltaG *= -1;
			}else if( cycleColor.g < 0.0 ) {
				cycleColor.g = 0.0;
				deltaG *= -1;
			}

			if( (cycleColor.b += deltaB * deltaTime) > 1.0 ) {
				cycleColor.b = 1.0;
				deltaB *= -1;
			}else if( cycleColor.b < 0.0 ) {
				cycleColor.b = 0.0;
				deltaB *= -1;
			}
		}


      // Move the particle //
      x += deltaTime * speedX;
      y += deltaTime * speedY;

      // Make sure that the particle stays in the screen //
      if( x < 0.0 ) {
         x = 0.0;
         speedX = -speedX;
      }

      if( x > SCREEN_W ) {
         x = SCREEN_W;
         speedX = -speedX;
      }

      if( y < 0.0 ) {
         y = 0.0;
         speedY = -speedY;
      }

      if( y > SCREEN_H ) {
         y = SCREEN_H;
         speedY = -speedY;
      }

      const float SPEED_MAX_CHANGE = 10.0;
      const float MAX_SPEED = 15.0;

      // Change the speed of the particle randomly //
      speedX += deltaTime * FRand( -SPEED_MAX_CHANGE, SPEED_MAX_CHANGE );

      // Make sure that the maxium speed isn't exceeded //
      if( speedX > MAX_SPEED )
         speedX = MAX_SPEED;

      if( speedX < -MAX_SPEED )
         speedX = -MAX_SPEED;

      speedY += deltaTime * FRand( -SPEED_MAX_CHANGE, SPEED_MAX_CHANGE );

      // Make sure that the maxium speed isn't exceeded //
      if( speedY > MAX_SPEED )
         speedY = MAX_SPEED;

      if( speedY < -MAX_SPEED )
         speedY = -MAX_SPEED;

      // RENDERING //

      // Clear the background of the screen to black
      Canvas::Fill( Rgba::BLACK );

		// Based on the selected color mode, decide which two colors will be //
		// our color gradient's start and finish color //
		Rgba start, finish;
		switch( colorMode )
		{
		case WHITE_TO_BLACK:
			start = Rgba::WHITE;
			finish = Rgba::BLACK;
			break;
		case RED_TO_BLUE:
			start = Rgba::RED;
			finish = Rgba::BLUE;
			break;
		case BLUE_TO_GREEN:
			start = Rgba::BLUE;
			finish = Rgba::GREEN;
			break;
		case GREEN_TO_RED:
			start = Rgba::GREEN;
			finish = Rgba::RED;
			break;
		case CYCLING:
			start = cycleColor;
			finish = Rgba::BLACK;
		};

		// If we're in reversing mode, swap the start and the finish color //
		if( reverseMode ) {
			Rgba tempColor = finish;
			finish = start;
			start = tempColor;
		}

		// Tell the LineStrip object to render the lines on our specified //
		// color gradient //
      strip.Draw( start, finish );

		// Display the instructions //
		neuropol.Print( "Hit ESC to exit.\nHit 'C' to change the color scheme\nHit 'R' to reverse the color gradient", 5, 25 );

      // Refresh the screen contents to show this frame //
      Canvas::Refresh();

		// Check for keyboard input from user //
      while( keypressed())
      {
      	int key = readkey();
      	if( key >> 8 == KEY_ESC )
				return 0;

			if( key >> 8 == KEY_R ) {
				reverseMode = !reverseMode;
				continue;
			}

			if( key >> 8 == KEY_C )
				if( colorMode == CYCLING )
					colorMode = WHITE_TO_BLACK;
				else
					colorMode = colorMode + 1;
      }

      // Yielding the CPU is good etiquette //
      rest(0);
   }

   return 0;
}
END_OF_MAIN()



