#ifndef OL_MOUSE_HPP
#define OL_MOUSE_HPP


#include "Includes.hpp"
#include "Vec2D.hpp"
#include "Declspec.hpp"

namespace ol {


class OL_LIB_DECLSPEC Mouse {
public:
   enum MouseButton {
      LEFT = 0,
      RIGHT = 1,
      CENTER = 2
   };
   
   
   bool Click( MouseButton button = LEFT ) {
      return mouse_b & ((int) button );
   }
   
   Vec2D GetPos() {
      return Vec2D( mouse_x, mouse_y );
   }
   
   bool MouseHits( const OlRect &area );
};


}




#endif // OL_MOUSE_HPP
