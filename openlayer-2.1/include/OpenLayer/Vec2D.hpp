#ifndef OL_VEC2D_HPP
#define OL_VEC2D_HPP

#include <cmath>
#include <string>
#include <sstream>
#include "General.hpp"
#include "Declspec.hpp"


namespace ol {



class OL_LIB_DECLSPEC Vec2D {
public:
   float x, y;


   // CONSTRUCTORS //

   Vec2D( float x = 0.0, float y = 0.0 )
      : x( x ), y( y ) {}


   static inline Vec2D PolarCoords( float angle, float magnitude ) {
      return Vec2D( magnitude * std::cos( angle ), magnitude * std::sin( angle ));
   }


   // METHODS //

   inline float GetAngle() const {
      return std::atan2( y, x );
   }


   inline float GetMagnitude() const {
      return std::sqrt( GetMagnitudeSquared() );
   }


   inline float GetMagnitudeSquared() const {
      return x * x + y * y;
   }


   inline Vec2D Normalized() const {
      float magnitude = GetMagnitude();

      return Vec2D( x / magnitude, y / magnitude );
   }


   // OPERATORS //


   inline void operator += ( const Vec2D &other ) {
      x += other.x; y += other.y;
   }

   inline void operator -= ( const Vec2D &other ) {
      x -= other.x; y -= other.y;
   }


   inline void operator *= ( float factor ) {
      x *= factor; y *= factor;
   }


   inline void operator /= ( float divisor ) {
      x /= divisor; y /= divisor;
   }

   inline bool operator == ( const Vec2D &other ) const {
		return fabs(x - other.x) < 0.01 && fabs(y - other.y) < 0.01;
   }
   
   inline bool operator != ( const Vec2D &other ) const {
		return !(*this == other);
   }
   
   inline std::string ToString() const {
      std::ostringstream str;
      str << "( " << x << ", " << y << " )";
      return str.str();
   }
   
   inline std::string GetString() const {
      return ToString();
   }
};


// ADDITION AND SUBTRACTION //

inline Vec2D operator + ( Vec2D first, Vec2D second ) {
   return Vec2D( first.x + second.x, first.y + second.y );
}


inline Vec2D operator - ( Vec2D first, Vec2D second ) {
   return Vec2D( first.x - second.x, first.y - second.y );
}


// MULTIPLICATION AND DIVISION

inline Vec2D operator * ( Vec2D vec, float factor ) {
   return Vec2D( factor * vec.x, factor * vec.y );
}


inline Vec2D operator * ( float factor, Vec2D vec ) {
   return Vec2D( factor * vec.x, factor * vec.y );
}


inline Vec2D operator / ( Vec2D vec, float divisor ) {
   return Vec2D( vec.x / divisor, vec.y / divisor );
}


// DOT PRODUCT //

inline float operator * ( Vec2D first, Vec2D second ) {
   return first.x * second.x + first.y * second.y;
}


// SIGN //

inline Vec2D operator - ( Vec2D vec ) {
   return Vec2D( -vec.x, -vec.y );
}


// NORMALIZATION

inline Vec2D operator ~ ( Vec2D vec ) {
   return vec.Normalized();
}


// TESTS


// Checks if the points are in counter clockwise order //

inline OL_LIB_DECLSPEC bool IsCounterClockwise( const Vec2D first, const Vec2D second, const Vec2D third ) {
  float dx1, dx2, dy1, dy2;

  dx1 = second.x - first.x;
  dy1 = second.y - first.y;
  dx2 = third.x - second.x;
  dy2 = third.y - second.y;

  return dy1*dx2 < dy2*dx1;
}


}











#endif



