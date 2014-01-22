// TEXT RENDERING DEMO //

#include <allegro.h>
#include <OpenLayer.hpp>
#include <loadpng.h>
#include <string>

using namespace ol;
using namespace std;


int main() {
   // SETUP //
   
   // Set up the program with all possible drivers //
   Setup::SetupProgram();
   
   // Set up the screen in windowed mode with the window size of 800 x 600 //
   Setup::SetupScreen( 800, 600, WINDOWED );
   
   Canvas::Fill( Rgba::BLACK );

   // Load the font with the size 30 x 24 and a white color //
   TextRenderer neuropol;
   neuropol.Load( "Fonts/Neuropol.ttf", 30, 24, Rgba::WHITE );
   
   // Test if the font was really loaded (maybe it didn't exist) //
   if( !neuropol ) {
      allegro_message( "Couldn't load Fonts/Neuropol.ttf!" );
      exit( -1 );
   }
   
   
   // RENDERING //
   
   // This is the text we wish to print to the screen //
   string text = "Hello World!";
   
   // Print the text to the screen such that the text starts at x = 200 //
   // and the baseline of the text is positioned at y = 100 //
   neuropol.Print( text, 200, 100 );
   
   // Refresh the screen contents to show the rendered image //
   Canvas::Refresh();
   
   // Wait for a keypress //
   readkey();
   return 0;
}
END_OF_MAIN()

