#ifndef OL_TRANSFORM_MATRIX_HPP
#define OL_TRANSFORM_MATRIX_HPP


#include "Matrix.hpp"
#include "Vec2D.hpp"
#include "Declspec.hpp"


namespace ol {


class OL_LIB_DECLSPEC Placement {
public:
   Placement( Vec2D position = Vec2D( 0.0, 0.0 ),
              float rotation = 0.0,
              float stretch = 1.0,
              Vec2D rotationPivot = Vec2D( 0.0, 0.0 ))
   : position( position ), rotation( rotation ), stretch( stretch ), rotationPivot( rotationPivot ) {}
   
   
   inline float GetDistance( const Placement &other ) const{
      return (position - other.position).GetMagnitude();
   }


   inline void MoveBy( Vec2D value ) {
      position += value;
   }


   inline void RotateBy( float value ) {
      rotation += value;
   }


   inline void StretchBy( float factor ) {
      stretch *= factor;
   }


   inline void SetPosition( Vec2D position ) {
      this->position = position;
   }


   inline Vec2D GetPosition() const {
      return position;
   }


   inline void SetRotation( float rotation ) {
      this->rotation = rotation;
   }


   inline float GetRotation() const {
      return rotation;
   }
   
   
   inline void SetRotationPivot( Vec2D rotationPivot ) {
      this->rotationPivot = rotationPivot;
   }
   
   
   inline const Vec2D &GetRotationPivot() const {
      return rotationPivot;
   }


   inline void SetStretch( float stretch ) {
      this->stretch = stretch;
   }


   inline float GetStretch() const {
      return stretch;
   }

   inline Placement& operator +( const Vec2D& amount ) {
		position += amount;
		return *this;
   }

   inline Placement& operator -( const Vec2D& amount ) {
		position -= amount;
		return *this;
   }

   inline Placement& operator +( const Placement& amount ) {
		position += amount.position;
		rotation += amount.rotation;
		stretch *= amount.stretch;
		return *this;
   }
   
   inline Placement& operator +=( const Placement& amount ) {
		position += amount.position;
		rotation += amount.rotation;
		stretch *= amount.stretch;
		return *this;
   }
   
   inline Placement& operator -( const Placement& amount ) {
		position -= amount.position;
		rotation -= amount.rotation;
		stretch /= amount.stretch;
		return *this;
   }
   
   inline Placement& operator -=( const Placement& amount ) {
		position += amount.position;
		rotation += amount.rotation;
		stretch *= amount.stretch;
		return *this;
   }
   
   // Returns the rotation and stretch in a matrix //
   Matrix2D Get2DMatrix() const;
   
   // Applies the placement
   inline void Apply() const {
      Apply( rotationPivot );
   }
   
   // Same as above but overrides the pivot
   void Apply( const Vec2D &pivot ) const;
   
   std::string ToString() const;

private:
   Vec2D position;
   float rotation;
   float stretch;
   Vec2D rotationPivot;
};



}



#endif // OL_TRANSFORM_MATRIX_HPP
