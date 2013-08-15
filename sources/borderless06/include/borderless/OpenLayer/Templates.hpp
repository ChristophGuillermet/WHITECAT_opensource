#ifndef OL_TEMPLATES_HPP
#define OL_TEMPLATES_HPP


#include "Includes.hpp"


// Template function definitions //


namespace ol {


template< class Functor >
Bitmap::
Bitmap( int width, int heigth, Functor functor )
      : bmp( 0 ), destroyBmp( true ), qFilename(0), qAlphaFilename(0), defaultPivotX( 0 ), defaultPivotY( 0 ), useDefaultPivot( true ) {
   
   AddToCollection();
   
}


}

#endif
