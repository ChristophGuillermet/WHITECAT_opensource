#include "Effects.hpp"
#include "Setup.hpp"

using namespace ol;
using namespace std;


// STATIC CLASS VARIABLES //

std::list< Effect *> EffectSystem::enabledEffects;
Bitmap *Effect::screenDump;

// EFFECTSYSTEM FUNCTIONS //

void EffectSystem::
Enable( Effect *effect ) {
   enabledEffects.push_back( effect );
}


void EffectSystem::
Disable(Effect *effect ) {
   enabledEffects.remove( effect );
}


void EffectSystem::
DisableAll() {
   enabledEffects.clear();
}


void EffectSystem::
Apply() {
   for( list< Effect *> ::iterator iter = enabledEffects.begin(); iter != enabledEffects.end(); iter++ ) {
      (*iter)->Apply();
   }
}


// EFFECTS //

Bitmap &Effect::
GetScreenDump() {
   if( !screenDump ) {
      InitScreenDump();
   }
   
   return *screenDump;
}


void Effect::
InitScreenDump() {
   screenDump = new Bitmap( Setup::GetWindowWidth(), Setup::GetWindowHeight() );
}
