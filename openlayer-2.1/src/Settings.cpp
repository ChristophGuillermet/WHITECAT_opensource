#include "Settings.hpp"
#include "Includes.hpp"
#include "General.hpp"
#include "Internal.hpp"
using namespace ol;


// CONSTANTS //

static const float OL_NEAR_ZERO = 0.000001;


// STATIC CLASS VARIABLES //

float Settings::circleAccuracy = 0.5;
float Settings::collisionPolyAccuracy = 0.8;
int Settings::collisionPolyAlphaLimit = 128;

bool Settings::useAntiAlias = true;
bool Settings::useTextures = true;
bool Settings::storeMemoryBitmaps = false;



// GENERAL FUNCTIONS //


void Settings::
SetCollisionPolyAccuracy( float accuracy ) {
   if( accuracy > OL_NEAR_ZERO && accuracy <= 1.0 ) {
      collisionPolyAccuracy = accuracy;
   }
   else {
      OlError( std::string("Illegal accuracy value in SetCollisionPolyAccuracy: ")
               + ToString( accuracy ));
   }
}



float Settings::
GetCollisionPolyAccuracy() {
   return collisionPolyAccuracy;
}



void Settings::
SetAntialiasing( bool turnedOn ) {
   useAntiAlias = turnedOn;
   if( useAntiAlias ) {
      glEnable( GL_LINE_SMOOTH );
      //glEnable( GL_POINT_SMOOTH );
      //glEnable( GL_POLYGON_SMOOTH );
   }
   else {
      glDisable( GL_LINE_SMOOTH );
      glDisable( GL_POINT_SMOOTH );
      //glDisable( GL_POLYGON_SMOOTH );
   }
}



void Settings::
SetCircleAccuracy( float accuracy ) {
   if( accuracy > OL_NEAR_ZERO && accuracy <= 1.0 )
      circleAccuracy = accuracy;
}



void Settings::
SetTextureMapping( bool turnedOn ) {
   useTextures = turnedOn;
   if( useTextures ) {
      glEnable( GL_TEXTURE_2D );
   }
   else {
      glDisable( GL_TEXTURE_2D );
   }
}



void Settings::
SetOrthographicProjection( int w, int h ) {
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    glOrtho( 0, w, 0, h, -1, 1000 );
    glScalef( 1, -1, 1 );
    glTranslatef( 0, -h, 0 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glDisable( GL_DEPTH_TEST );
}



void Settings::
RestoreOldProjection() {
	glMatrixMode( GL_PROJECTION );
   glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
}



void Settings::
StoreMemoryBitmaps( bool turnedOn ) {
   storeMemoryBitmaps = turnedOn;
}



bool Settings::
MemoryBitmapsStored() {
   return storeMemoryBitmaps;
}
