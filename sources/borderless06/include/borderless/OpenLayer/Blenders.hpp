#ifndef OL_BLENDERS_HPP
#define OL_BLENDERS_HPP

#include "Includes.hpp"
#include "Declspec.hpp"
#include <stack>

namespace ol {

// Blenders - different color blending styles

// Alpha blender (default): renders translucent bitmaps,
//                          alpha values of the source pixels tell the opacity
//
// Additive blender: Increases the lightness of the underlying pixels
//                   alpha values of the source pixels tell the intensity
//
// Subtractive blender: Decreases the lightness of the underlying pixels
//                      alpha values the source pixels tell the intensity


enum Blender {
   ALPHA_BLENDER,
   ADDITIVE_BLENDER,
   SUBTRACTIVE_BLENDER,
   COPY_BLENDER,
   FULL_ADDITIVE_BLENDER,
   CUSTOM
};


class OL_LIB_DECLSPEC Blenders {
public:
   // Sets the active blending function //
   // Additive blender lits the destination bitmap, //
   // subtractive blender darkens it. //
   static void Set( Blender blender );
   
   // Sets a custom OpenGL blender as the active blending function //
   static void Set( GLenum sourceFactor, GLenum destFactor );
   
   // Pushes the active blender to the blender stack //
   inline static void Push() {
      blenderStack.push( activeBlender );
   }
   
   // Pops the most recently added blender from the blender stack //
   static void Pop();
   
   // Selects the active blender (automatically done) //
   static void SelectBlender();
   
private:
   
   class BlenderObj {
   public:
      BlenderObj()
         : source( GL_ONE ), dest( GL_ZERO ) {}
      
      BlenderObj( GLenum source, GLenum dest )
         : source( source ), dest( dest ) {}
      
      GLenum source, dest;
   };
   
   static BlenderObj activeBlender;
   
   static std::stack< BlenderObj > blenderStack;
};


}



#endif // OL_BLENDERS_HPP
