#ifndef OL_RGBA_HPP
#define OL_RGBA_HPP


#include "Includes.hpp"
#include "Transforms.hpp"
#include "Declspec.hpp"

#include <string>
// #include <sstream>
// #include <iomanip>



namespace ol {

// Rgba - the Color structure //

class OL_LIB_DECLSPEC Rgba {
public:
   // Construct the color from float (0.0 ... 1.0) or integer color components (0..255) /
   // or using the 24-bit packed color value and an alpha value //
   Rgba( float  r, float  g, float  b, float  a = 1.0 )              : r( r ), g( g ), b( b ), a( a ) {}
   Rgba( double r, double g, double b, double a = 1.0 )              : r( r ), g( g ), b( b ), a( a ) {}
   Rgba( int    r, int    g, int    b, int    a = 255 )              : r( Rgba::CompToF(r)), g( Rgba::CompToF(g)), b( Rgba::CompToF(b)), a( Rgba::CompToF(a)) {}
   Rgba( int    col,                          int a )                { *this = Rgba( getr32( col ), getg32( col ), getb32( col ), a ); }
   
   explicit Rgba( const std::string &hexPresentation );
   
   Rgba() : r( 0.0 ), g( 0.0 ), b( 0.0 ), a( 0.0 ) {}
   
   // Color components //
   float r, g, b, a;
   
   // Some default colors //
   static const Rgba BLACK, WHITE, RED, YELLOW, GREEN, BLUE;
   static const Rgba INVISIBLE; // Has zero alpha //
   
   // Returns a color interpolated between this and otherColor //
   // using the factor (0...1) such that if factor is zero, it //
   // returns the calling color, if factor is 1.0 it returns otherColor //
   // and otherwise it returns a color between this and otherColor //
   Rgba MixWith( const Rgba &otherColor, float factor ) const;
   
   
   // Returns a color which has the same color componenets as this one //
   // except a differet alpha value //
   inline Rgba WithAlpha( float newAlpha ) const                           { return Rgba( r, g, b, newAlpha ); }
   
   // (You'll only need this function if you use OpenGL directly somewhere) //
   // Selects the color as the current OpenGL color //
   // Affected by color channel cofficients //
   
   // Like Select but unaffected by color channel cofficients //
   inline void SelectRaw() const {
      glColor4f( r, g, b, a );
   }
   
   #ifdef NO_COLOR_CHANNELS
      
   inline void Select() const {
     SelectRaw();
   }
      
   #else // NO_COLOR_CHANNELS
   
   inline void Select() const {
     const Rgba& colorChannels = ol::Transforms::GetColorChannels();
     glColor4f( colorChannels.r * r, colorChannels.g * g, colorChannels.b * b, colorChannels.a * a );
   }
      
   #endif // NO_COLOR_CHANNELS
   
   std::string ToString() const;
   std::string ToHex() const;
   
   // Returns the color in a packed 32-bit integer //
   int Packed() const;
   
   
   // A previously used name for the function MixWith //
   inline Rgba InterpolateWith( const Rgba &otherColor, float factor ) const {
      return MixWith( otherColor, factor );
   }
   
   friend class TextRenderer;
   
private:
   unsigned int parseHex( const std::string &hex, int pos );
   
   explicit Rgba( bool invalidiated );
   Rgba( int specialPackedColor, bool notUsed );
   
   static inline int CompToI( float c ) { return int( 255.0 * c ); }
   static inline float CompToF( int c ) { return float(c)/255.0; }
   int SpecialPacked() const;
   
   inline bool IsValid()
   {
     return r >= 0.0 && g >= 0.0 && b >= 0.0 && a >= 0.0;
   }
};


}



#endif
