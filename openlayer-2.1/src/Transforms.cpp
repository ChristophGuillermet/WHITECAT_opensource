#include "Transforms.hpp"
#include "Internal.hpp"
#include "Settings.hpp"
#include "Setup.hpp"
#include "Rgba.hpp"
using namespace ol;

// STATIC CLASS VARIABLES //

float Transforms::x = 0.0;
float Transforms::y = 0.0;

float Transforms::angle = 0.0;
float Transforms::pivotX = 0.0;
float Transforms::pivotY = 0.0;

float Transforms::xStretch = 1.0;
float Transforms::yStretch = 1.0;

Rgba Transforms::color = Rgba::INVISIBLE;
Rgba Transforms::colorChannels = Rgba::WHITE;

bool Transforms::transformationStored = false;

int Transforms::stackCounter = 0;


// GENERAL FUNCTIONS //

// * POSITION TRANSFORMATIONS //


void Transforms::
PushPlacement() {
   glPushMatrix();
   stackCounter++;
}



void Transforms::
PopPlacement() {
   if( stackCounter > 0 ) {
      glPopMatrix();
      stackCounter--;
   }
   else {
      OlError( "Transforms::PopPlacement called before calling Transforms::PushPlacement" );
   }
}



void Transforms::
SetPosition( float x, float y ) {
   Transforms::x = x;
   Transforms::y = y;

   ApplyTransforms();
}

void Transforms::
SetPosition( const Vec2D& pos ) {
   Transforms::x = pos.x;
   Transforms::y = pos.y;

   ApplyTransforms();
}


void Transforms::
SetRotation( float angle ) {
   Transforms::angle = angle;

   ApplyTransforms();
}


void Transforms::
SetRotationPivot( float pivotX, float pivotY ) {
   Transforms::pivotX = pivotX;
   Transforms::pivotY = pivotY;

   ApplyTransforms();
}

void Transforms::
SetRotationPivot( const Vec2D& pivot ) {
	Transforms::pivotX = pivot.x;
	Transforms::pivotY = pivot.y;

	ApplyTransforms();
}

void Transforms::
SetStretch( float xStretch, float yStretch ) {
   Transforms::xStretch = xStretch;
   Transforms::yStretch = yStretch;

   ApplyTransforms();
}

void Transforms::
SetStretch( const Vec2D& stretch ) {
	Transforms::xStretch = stretch.x;
	Transforms::yStretch = stretch.y;

	ApplyTransforms();
}

void Transforms::
SetPlacement( const Placement& placement, const Vec2D& pivot ) {
	Transforms::x = placement.GetPosition().x;
	Transforms::y = placement.GetPosition().y;
	Transforms::angle = placement.GetRotation();
	Transforms::pivotX = pivot.x;
	Transforms::pivotY = pivot.y;
	Transforms::xStretch = placement.GetStretch();
	Transforms::yStretch = placement.GetStretch();

	ApplyTransforms();
}

void Transforms::
ResetPlacement() {
   if( transformationStored ) {
      while( stackCounter > 0 ) {
         PopPlacement();
      }
      
      glPopMatrix();
      transformationStored = false;
      
      x = 0;
      y = 0;
      angle = 0.0;
      xStretch = 1.0;
      yStretch = 1.0;
   }
}



void Transforms::
ApplyTransforms() {
   if( transformationStored ) {
      glPopMatrix();
   }
   
   glPushMatrix();
   transformationStored = true;

   glTranslatef( x, y, 0.0 );
   
   if( angle != 0.0 ) {
      glTranslatef( pivotX, pivotY, 0.0 );
      RotateMatrix( angle );
      glTranslatef( -pivotX, -pivotY, 0.0 );
   }
   
   if( xStretch != 1.0 || yStretch != 1.0 ) {
      float xTranslate = -(xStretch - 1.0) * (SCREEN_W/2 - x);
      float yTranslate = -(yStretch - 1.0) * (SCREEN_H/2 - y);

      glTranslatef( xTranslate, yTranslate, 0.0 );

      glScalef( xStretch, yStretch, 0.0 );
   }
}


// * TINTING //


void Transforms::
SetTintColor( const Rgba& color ) {
   Transforms::color = color;
}


void Transforms::
ApplyTinting() {
   if( color.a > 0.0 ) {
      color.Select();
      glDisable( GL_TEXTURE_2D );

      glBegin( GL_QUADS );
         glVertex2f( 0.0, SCREEN_H );
         glVertex2f( SCREEN_W, SCREEN_H );
         glVertex2f( SCREEN_W, 0.0 );
         glVertex2f( 0.0, 0.0 );
      glEnd();

      if( Settings::TextureMappingUsed() )
         glEnable( GL_TEXTURE_2D );
   }
}



// * COLOR CHANNELS //


void Transforms::
SetColorChannels( const Rgba& cofficients ) {
   colorChannels = cofficients;
}



const Rgba& Transforms::
GetColorChannels() {
   return colorChannels;
}

