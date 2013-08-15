#ifndef OL_SETUP_HPP
#define OL_SETUP_HPP

#include "Includes.hpp"
#include "Declspec.hpp"
#include <string>


namespace ol {

// Setup - Program setup functions //


// Possible devices to setup //

enum {
   KEYBOARD = 0x1,
   MOUSE = 0x2,
   TIMER = 0x4
};


// Possible screen modes (you can pass these instead of true and false
// as the fullscreen -parameter in SetupScreen) //

#define WINDOWED false
#define FULLSCREEN true


class OL_LIB_DECLSPEC Setup {
public:
   // Sets up OpenLayer, Allegro and AllegroGL, returns true on success //
   static bool SetupProgram( bool setupKeyboard = true, bool setupMouse = true,
                             bool setupTimer = true );
   
   // Same as above but the parameters are passed by using logical "or" //
   // of any of the devices listed above the Setup class //
   static bool SetupProgram( int devices );
   
   // Sets up the program window, width and height tell the screen resolution (fullscreen) //
   // or the window size (windowed) //
   // Z-depth is the accuracy of the built-in depth sorting (values: 8, 16 or 32),
   // but you don't usually have to worry about it //
   // Returns true in success //
   static bool SetupScreen( int width, int height, bool fullscreen = true,
                            int colorDepth = 32, int zDepth = 8, int refreshRate = 0 );
   
   // Returns true if SetupProgram is called and the program is succesfully set up //
   static bool IsProgramSetUp();
   
   // Returns true if SetupScreen is called and the program window is created //
   static bool IsScreenSetUp();
   
   // Returns the width of the screen //
   static inline int GetWindowWidth() {
      return windowWidth;
   }
   
   // Returns the height of the screen //
   static inline int GetWindowHeight() {
      return windowHeight;
   }
   
   // Returns the color depth of the screen //
   static inline int GetColorDepth() {
      return colorDepth;
   }
   
   // Returns the path to the executable //
   static inline std::string GetExecutablePath() {
      return executablePath;
   }
   
   // If the passed pathname is relative, an absolute version of the pathname is returned //
   // Otherwise the pathname is returned unmodified //
   static std::string ToAbsolutePathname( std::string pathname );
   
   // Unloads most graphics card resources that OpenLayer uses //
   static void SuspendProgram();
   
   // Reloads resources which were previously unloaded after a call to SuspendProgram //
   static void WakeUpProgram();
   
   // Returns the OpenGL error, or null if no error has occured //
   static const char *GetGLError();
   
private:
   
   static bool programIsSetUp;
   static bool screenIsSetUp;
   static std::string executablePath;
   static int windowWidth;
   static int windowHeight;
   static int colorDepth;
};


}


#endif // OL_SETUP_HPP
