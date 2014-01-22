#include "General.hpp"


using namespace ol;


namespace ol {
   

int ToNextPowOfTwo( int num ) {
   int pow2 = 1;
   while( pow2 < num )
      pow2 <<= 1;
   
   return pow2;
}


}

