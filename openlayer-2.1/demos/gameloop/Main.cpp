// GAME LOOP DEMO //
// Note that due to the extremely high fps (possibly over 2000) the movement may not feel smooth! //

#include <OpenLayer.hpp>


using namespace ol;


int main() {
   // SETUP //

   // Set up the program with all possible drivers //
   Setup::SetupProgram();

   // Set up the screen in windowed mode with the window size of 800 x 600 //
   Setup::SetupScreen( 800, 600, WINDOWED );

   // Load a bitmap //
   Bitmap bmp( "Gfx/Bitmap.png" );

   // Test if the bitmap was really loaded (maybe it didn't exist) //
   if( !bmp ) {
      allegro_message( "Couldn't load the bitmap!" );
      exit( -1 );
   }

   // Set up the delta time routines with the default fps of 100.0 //
   FpsCounter::Start( 100.0 );

   // The top-left coordinates of the bitmap //
   float x = 200.0, y = 100.0;

   // The speed of the bitmap movement //
   float speed = 2.0;


   // Run the game loop until esc is pressed //
   while( !key[KEY_ESC] ) {
      // LOGIC //

      // Inform the fps counter that a new frame has started //
      FpsCounter::NewFrameStarted();

      // Get the delta time //
      float deltaTime = FpsCounter::GetDeltaTime();

      // Move the bitmap with the arrow keys //
      if( key[KEY_LEFT] ) x -= deltaTime * speed;
      if( key[KEY_RIGHT] ) x += deltaTime * speed;

      if( key[KEY_UP] ) y -= deltaTime * speed;
      if( key[KEY_DOWN] ) y += deltaTime * speed;

		printf( "%f, %f\n", x, y );

      // RENDERING //

      // Clear the screen to white //
      Canvas::Fill( Rgba::WHITE );

      // Draw the bitmap to the screen with the top-left coordinates //
      bmp.Blit( x, y );

      // Refresh the screen contents to show this frame //
	  Canvas::Refresh();
   }
   return 0;
}
END_OF_MAIN()

