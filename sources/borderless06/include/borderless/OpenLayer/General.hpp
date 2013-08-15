#ifndef OL_GENERAL_HPP
#define OL_GENERAL_HPP


#include <sstream>
#include "Declspec.hpp"

namespace ol {


// ToString - Converts a variable to a string //

template< class Type >
std::string ToString( Type value ) {
   std::stringstream s;
   s << value;
   return s.str();
}


template< class Type >
std::string VarToString( Type value ) {
   return ToString( value );
}


OL_LIB_DECLSPEC int ToNextPowOfTwo( int num );


enum Axis {
   X_AXIS,
   Y_AXIS
};


enum OutlineTextureMode {
   SHRINK,
   STRETCH,
   OPTIMIZE
};


enum TextAlignment {
   LEFT,
   RIGHT,
   CENTER,
   JUSTIFY
};


}



#endif // OL_GENERAL_HPP

