#include "Setup.hpp"
#include "Includes.hpp"
#include "Settings.hpp"
#include "Blenders.hpp"
#include "Transforms.hpp"
#include "GarbageCollector.hpp"
#include "General.hpp"
#include "Internal.hpp"
#include "GlDriver.hpp"
#include "Rgba.hpp"
#include "Canvas.hpp"

#ifndef OL_NO_PNG
   #include <loadpng.h>
#endif // OL_NO_PNG


using namespace ol;



// STATIC CLASS VARIABLES //

bool Setup::programIsSetUp = false;
bool Setup::screenIsSetUp = false;
std::string Setup::executablePath = "";
int Setup::windowWidth = 0;
int Setup::windowHeight = 0;
int Setup::colorDepth = 0;


// GENERAL FUNCTIONS //


void OlCollectTheGarbage();



bool Setup::
SetupProgram( int devices ) {
   return SetupProgram( devices & KEYBOARD, devices & MOUSE, devices & TIMER );
}



bool Setup::
SetupProgram( bool setupKeyboard, bool setupMouse, bool setupTimer ) {
   GlDriver::Get()->SetupProgram( setupKeyboard, setupMouse, setupTimer );
   
   atexit( OlCollectTheGarbage );
   
   executablePath = GlDriver::Get()->GetExecutablePath();
   
   programIsSetUp = true;
   
#ifndef OL_NO_PNG
   register_png_file_type();
#endif
   
   OlLog( "OpenLayer started up succesfully" );
   
   return true;
}



GarbageCollection &OlGetCollection();


bool Setup::
SetupScreen( int w, int h, bool fullscreen, int colorDepth, int zDepth, int refreshRate ) {
   
   GlDriver::Get()->SetupScreen( w, h, fullscreen, colorDepth, zDepth, refreshRate );
   
   Setup::windowWidth = w;
   Setup::windowHeight = h;
   Setup::colorDepth = colorDepth;
   
   screenIsSetUp = true;
	
	Settings::SetOrthographicProjection();
   
	Blenders::Set( ALPHA_BLENDER );
	
	Transforms::SetRotationPivot( SCREEN_W/2, SCREEN_H/2 );
   Transforms::SetColorChannels( Rgba::WHITE );
	
	Settings::SetAntialiasing( true );
	
	Canvas::ReadBufferSizes();
	Canvas::SetTo( SCREEN_BACKBUF );
	Canvas::Fill( Rgba( 0.0, 0.0, 0.0, 1.0 ), ALPHA_ONLY );
	Canvas::Refresh();
	Canvas::Fill( Rgba( 0.0, 0.0, 0.0, 1.0 ), ALPHA_ONLY );
	
	OlGetCollection().ExecuteQueues();
   
   OlLog( "Screen set up succesfully\n" );
   
	return true;
}



bool Setup::
IsScreenSetUp() {
   return screenIsSetUp;
}



bool Setup::
IsProgramSetUp() {
   return programIsSetUp;
}



std::string Setup::
ToAbsolutePathname( std::string pathname ) {
   return GlDriver::Get()->ToAbsolutePathname( pathname );
}


void Setup::
SuspendProgram() {
   OlGetCollection().UnloadAllToMemory();
}


void Setup::
WakeUpProgram() {
   OlGetCollection().LoadAllToGPU();
}


const char *Setup::
GetGLError() {
   GLenum error = glGetError();
   
   if( error == GL_NO_ERROR )
      return 0;
   
   if( error == GL_INVALID_ENUM )
      return "GL_INVALID_ENUM";
   
   if( error == GL_INVALID_VALUE )
      return "GL_INVALID_VALUE";
   
   if( error == GL_INVALID_OPERATION )
      return "GL_INVALID_OPERATION";
   
   if( error == GL_STACK_OVERFLOW )
      return "GL_STACK_OVERFLOW";
   
   if( error == GL_STACK_UNDERFLOW )
      return "GL_STACK_UNDERFLOW";
   
   if( error == GL_OUT_OF_MEMORY )
      return "GL_OUT_OF_MEMORY";
   
   OlLog( "Unknown OpenGL error code" );
   OlLog( ToString( error ));
   
   return "OTHER";
}

