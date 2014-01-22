#ifndef OL_EFFECTS_HPP
#define OL_EFFECTS_HPP


#include <list>
#include "Bitmap.hpp"
#include "Declspec.hpp"


namespace ol {


class Effect;


class OL_LIB_DECLSPEC EffectSystem {
public:
   // Enables the specified effect //
   static void Enable( Effect *effect );
   
   // Disables the specified effect //
   static void Disable( Effect *effect );
   
   // Disables all effects //
   static void DisableAll();
   
   // Applies all enabled effects to the active canvas //
   static void Apply();
   
private:
   
   static std::list< Effect *> enabledEffects;
};


// The parent class of all effects //

class OL_LIB_DECLSPEC Effect {
public:
   Effect( float duration ) :
      duration( duration ) {}
   
   virtual ~Effect(){}
   
   // Applies the effect to the active canvas //
   virtual void Apply() = 0;
   
   // Stores the contents of the screen to a Bitmap buffer //
   static void TakeScreenDump();
   
   // Returns the Bitmap holding the dump of the screen //
   static Bitmap &GetScreenDump();
   
   // Sets the Bitmap to be used to store the screen dump //
   // This function will disable the TakeScreenDump function //
   static void SetScreenDump( Bitmap &bmp );
   
   // Initializes the screen dump //
   // You don't have to call this function in order to use the effects, //
   // however it's recommended to call this function during the loading time //
   // so that initializing the effects won't suddenly slow down the program //
   // when it runs in realtime //
   static void InitScreenDump();
   
private:
   static Bitmap *screenDump;
   float duration;
};


class OL_LIB_DECLSPEC MotionBlur : public Effect {
public:
   // Create a new motion blur with the given strength //
   MotionBlur( float duration, float strength )
      : Effect( duration ), strength( strength ) {}
   
   virtual ~MotionBlur(){}
   
   virtual void Apply();
   
private:
   float strength;
};


class OL_LIB_DECLSPEC RadialBlur : public Effect {
public:
   // Create a new radial blur with the given strength //
   RadialBlur( float duration, float strength )
      : Effect( duration ), strength( strength ) {}
   
   virtual ~RadialBlur(){}
   
   virtual void Apply();
   
private:
   float strength;
};


class OL_LIB_DECLSPEC SwirlBlur : public Effect {
public:
   // Create a new swirl blur with the given strength, accuracy and angle spread //
   SwirlBlur( float duration, float strength, float accuracy, float spread )
      : Effect( duration ), strength( strength ), accuracy( accuracy ), spread( spread ) {}
   
   virtual ~SwirlBlur(){}
   
   virtual void Apply();
   
private:
   float strength, accuracy, spread;
};



};





#endif // OL_EFFECTS_HPP
