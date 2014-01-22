// BITMAP RENDERING DEMO //

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

   // RENDERING //
	// Empty the screen to black //
	Canvas::Fill( Rgba::BLACK );

   // Draw the bitmap to the screen with the top-left coordinates of x = 200.0, y = 100.0 //
   bmp.Blit( 200.0, 100.0 );

	// Refresh the screen contents to show the rendered image //
	Canvas::Refresh();

   // Wait for a keypress //
   readkey();

   return 0;
}
END_OF_MAIN()

