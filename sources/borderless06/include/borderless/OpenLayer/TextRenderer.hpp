//#ifndef OL_NO_TTF

#ifndef OL_TEXT_RENDERER_HPP
#define OL_TEXT_RENDERER_HPP

#ifdef OL_NO_TTF
#define USE_NEW_TTF
#endif


#include "Includes.hpp"
#include "Rgba.hpp"
#include "GarbageCollector.hpp"
#include "General.hpp"
#include "Declspec.hpp"

#ifdef USE_NEW_TTF

#include "Glyph.hpp"

#else

#define GLYPH_TARGET GLYPH_TARGET_ALLEGGL
#include <glyph.h>

#endif /* USE_NEW_TTF */

namespace ol {

#define UNDEFINED_ITALICS -361



// TextRenderer - Renders multi-lined text to the screen using a TrueType font //
// Notice that you can use a newline character ('\n') in the middle of the text //
// to start a new text line. //
// Works the same way as Glyph Keeper's Rend structure //
// Uses GlyphKeeper to render text //


class OL_LIB_DECLSPEC TextRenderer : public GarbageCollected {
public:
   // Construct the font by loading the face from a .ttf file //
   // The loaded font face will be automatically destroyed when the TextRenderer is destroyed //
   TextRenderer( const char *filename, int width = 9, int height = 12, 
                 Rgba col = Rgba::BLACK, int italics = 0, bool useHinting = true );
   
   // Construct the font by using the face from an another TextRenderer //
   // If you don't specify any color or italics angle they'll be inherited from the other renderer! //
   TextRenderer( const TextRenderer &otherRenderer, int width = 12, int height = 10,
                 Rgba col = Rgba(), int italics = UNDEFINED_ITALICS );
   
   // Construct the font by using a GlyphKeeper font face //
   TextRenderer( GLYPH_FACE *face, int width = 12, int height = 10, Rgba col = Rgba::BLACK,
                 int italics = 0, bool useHinting = true );
   
   // The default constructor leaves the font invalid (IsValid() returns false) //
   TextRenderer();
   
   // The destructor destroys the font and makes sure that it's not garbage collected //
   virtual ~TextRenderer() { Destroy( true ); }
   
   
   // Loads a font from a .ttf file, returns true on success //
   bool Load( const char *filename, int width, int height, Rgba col = Rgba::BLACK, 
              int italics = 0, bool useHinting = true );
   
   // Loads a font using the face from an another TextRenderer, returns true on success //
   // If you don't specify any color or italics angle they'll be inherited from the other renderer! //
   bool Load( const TextRenderer &otherRenderer, int width, int height,
              Rgba col = Rgba(), int italics = UNDEFINED_ITALICS );
   
   // Loads a font from a Glyph Keeper face, returns true on success //
   // Pass true to destroyFace if you wish the font face to be automatically destroyed //
   // when the Destroy() -method or the deconstructor is called //
   bool Load( GLYPH_FACE *face, int width, int height, Rgba col = Rgba::BLACK, int italics = 0,
              bool useHinting = true );
   
   // Draws multi-lined text to the screen //
   void Print( const std::string &text, int x, int baselineY ) const;
   
   // Draws multi-lined text to the screen with automatic line wrapping //
   void Print( const std::string &text, int x, int baselineY,
               int maxLineWidth, TextAlignment alignment = LEFT ) const;
   
   // Calling this function will invalidiate the font //
   // Frees all memory allocated by this TextRenderer //
   // and unloads the font from the graphics card //
   // Notice that the font face isn't unloaded by Glyph Keeper //
   // until the program exits! //
   // You can unload it manually if you want by calling GetFace() //
   // and unloading that //
   virtual void Destroy( bool eraseFromGarbageCollection = false );
   
   // Sets the font color //
   void SetColor( const Rgba& col );
   
   // Returns the color of the font //
   const Rgba& GetColor() const;
   
   // Sets the italics angle of the text //
   void SetItalics( int italicsAngle );
   
   // Returns the italics angle of the text //
   int GetItalics() const;

   // Font height
   int FontHeight() const;
   // Font total height
   int FontTotalHeight() const;
   
   // Returns the width and height of multi-lined text //
   int Width( const std::string &text ) const;
   int Height( const std::string &text ) const;

   // Returns the width and height of the first text line of multi-lined text //
   int FirstLineWidth( const std::string &text ) const;
   int FirstLineHeight( const std::string &text ) const;
   
   // Returns true if the TextRenderer is ready to use (loaded and not destroyed) //
   virtual bool IsValid() const;
    
   // The boolean operator returns the same as the IsValid-method //
   inline operator bool() {
      return IsValid();
   }
   
   inline void UseTags( bool turnedOn ) {
      useTags = turnedOn;
   }
   
   static std::string GetColoredText( const std::string &str, Rgba color );
   
   inline Vec2D GetTextSize( const std::string& str ) {
      return Vec2D( Width( str ), Height( str ));
   }
   
   
   // ADVANCED FUNCTIONS //
   // (You don't usually need to call these) //
   
   
   // Returs the Glyph Keeper face of the font in case if you need it somewhere //
   GLYPH_FACE *GetFace() const { return face; }
   
   // Sends the font to the graphics card //
   // Automatically done when the font is loaded //
   virtual void SendToGPU();
   
   // Unloads the font from the graphics card //
   // Automatically done when the font is destroyed //
   void UnloadFromGPU();
   
   // Returns true if a loading command is waiting for Setup::SetupScreen to be called //
   virtual bool IsLoadingQueued() const;
   
   // Called by Setup::SetupScreen //
   virtual OlLoadResult ExecuteQueuedCommands();
   
    // Adds the TextRenderer to the garbage/auto-loader collection //
    // (automatically done) //
    virtual void AddToCollection();
    
    // Suspending OpenGL will simply destroy the font texture(s) //
    virtual void UnloadToMemory() { UnloadFromGPU(); }
   
private:
   void StartRendering() const {
      SelectColor( col );
   }
   void SelectColor( const Rgba &color ) const;
   void FinishRendering() const;
   void RenderLineAligned( const std::string &line, int x, int y,
                           int maxLineWidth, TextAlignment alignment ) const;
   
   GLYPH_REND *rend;
   GLYPH_TEXTURE *texture;
   GLYPH_FACE *face;
   
   // If you modify this library note that modifying this variable //
   // doesn't change the italics of the font! //
   int italics;
   bool useHinting;
   Rgba col;
   
   bool useTags;
   
   // AUTO LOADER ROUTINES //
   
   class Loader {
   public:
      Loader( int w, int h, int italics, Rgba col )
         : w( w ), h( h ), italics( italics ), col( col ) {}
      virtual ~Loader(){}
      virtual OlLoadResult Load( TextRenderer &subject ) = 0;
      
   protected:
      TextRenderer *renderer;
      int w, h;
      int italics;
      Rgba col;
   };
   
   Loader *loader;
   
   
   class FileLoader : public Loader {
   public:
      FileLoader( const char *filename, int w, int h, int italics, Rgba col )
         : Loader( w, h, italics, col ), filename( filename ) {}
      virtual ~FileLoader(){}
      virtual OlLoadResult Load( TextRenderer &subject );
      
   protected:
      const char *filename;
   };
   
   
   class RendLoader : public Loader {
   public:
      RendLoader( const TextRenderer &rend, int w, int h, int italics, Rgba col )
         : Loader( w, h, italics, col ), rend( rend ) {}
      virtual ~RendLoader(){}
      virtual OlLoadResult Load( TextRenderer &subject );
      
   protected:
      const TextRenderer &rend;
   };
};



}



#endif // OL_TEXT_RENDERER_HPP

//#endif // OL_NO_TTF
