#include "Blenders.hpp"
#include "Includes.hpp"

using namespace ol;


// STATIC CLASS VARIABLES //


Blenders::BlenderObj Blenders::activeBlender( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
std::stack< Blenders::BlenderObj > Blenders::blenderStack;



// GENERAL FUNCTIONS //


void Blenders::
Set( Blender blender ) {
   switch( blender ) {
      case ALPHA_BLENDER:
         activeBlender.source = GL_SRC_ALPHA;
         activeBlender.dest = GL_ONE_MINUS_SRC_ALPHA;
        break;
      case ADDITIVE_BLENDER:
         activeBlender.source = GL_SRC_ALPHA;
         activeBlender.dest = GL_ONE;
        break;
      case SUBTRACTIVE_BLENDER:
         activeBlender.source = GL_ZERO;
         activeBlender.dest = GL_ONE_MINUS_SRC_COLOR;
        break;
      case COPY_BLENDER:
         activeBlender.source = GL_ONE;
         activeBlender.dest = GL_ZERO;
        break;
      case FULL_ADDITIVE_BLENDER:
         activeBlender.source = GL_ONE;
         activeBlender.dest = GL_ONE;
        break;
      case CUSTOM:
	      //Nothing
        break;
   }
   
   SelectBlender();
}



void Blenders::
Set( GLenum sourceFactor, GLenum destFactor ) {
   activeBlender.source = sourceFactor;
   activeBlender.dest = destFactor;
   
   SelectBlender();
}



void Blenders::
SelectBlender() {
   if( activeBlender.source == GL_ONE && activeBlender.dest == GL_ZERO ) {
      glDisable( GL_BLEND );
   }
   else {
      glBlendFunc( activeBlender.source, activeBlender.dest );
      glEnable( GL_BLEND );
   }
}



void Blenders::
Pop() {
   activeBlender = blenderStack.top();
   SelectBlender();
   blenderStack.pop();
}



