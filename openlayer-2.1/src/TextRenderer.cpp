//#ifndef OL_NO_TTF

#ifndef OL_TEXT_RENDERER_CPP
#define OL_TEXT_RENDERER_CPP

#include "TextRenderer.hpp"
#include "Internal.hpp"
#include "Setup.hpp"
#include "Settings.hpp"
#include "Blenders.hpp"
#include "Transforms.hpp"

using namespace ol;
using namespace std;


// GENERAL FUNCTIONS //

TextRenderer::
TextRenderer( const char *filename, int width, int height, Rgba col, int italics, bool useHinting )
      : rend( 0 ), texture( 0 ), face( 0 ), italics( italics ), useHinting( useHinting ), col( col ),
        useTags( true ), loader( 0 ) {

   AddToCollection();

   if( !Load( filename, width, height, col, italics, useHinting )) {
      OlError( string("Cannot construct a TextRenderer - Loading font '") + filename + "' failed!" );
   }
}



TextRenderer::
TextRenderer( const TextRenderer &otherRend, int width, int height, Rgba col, int italics )
      : GarbageCollected(), rend( 0 ), texture( 0 ), face( 0 ), italics( italics ),
        useHinting( otherRend.useHinting ), col( col ), useTags( true ), loader( 0 ) {

   AddToCollection();

   if( !Load( otherRend, width, height, col, italics )) {
      OlError( "Cannot construct a TextRenderer - The font face from the other TextRenderer is invalid!" );
   }
}



TextRenderer::
TextRenderer( GLYPH_FACE *face, int width, int height, Rgba col, int italics, bool useHinting )
      : rend( 0 ), texture( 0 ), face( 0 ), italics( italics ),
        useHinting( useHinting ), col( col ), useTags( true ), loader( 0 ) {

   AddToCollection();

   if( !Load( face, width, height, col, italics, useHinting )) {
      OlError( string("Cannot construct a TextRenderer - The passed font face is ") + string((face == 0)? "null" : "invalid" ) + string(" !" ));
   }
}



TextRenderer::
TextRenderer()
      : rend( 0 ), texture( 0 ), face( 0 ), italics( 0 ), useHinting( true ), useTags( true ), loader( 0 ) {
   AddToCollection();
}




bool TextRenderer::
Load( const char *filename, int w, int h, Rgba col, int italics, bool useHinting ) {
   if( !Setup::IsScreenSetUp() ) {
      OlLog( string( "Loading queued for font: " ) + filename );
      loader = new FileLoader( filename, w, h, italics, col );
      return true;
   }

   if( filename == 0 ) {
      OlError( "Attempted to load a true type font with a null filename!" );
      return false;
   }

   string absoluteFilename = Setup::ToAbsolutePathname( filename );

   OlLog( string( "Loading a true type font: " ) + absoluteFilename );

   face = gk_load_face_from_file( absoluteFilename.c_str(), 0 );

   bool valid = false;

   if( face ) {
      valid = Load( face, w, h, col, italics, useHinting );
   }

   if( !valid ) {
      OlError( "Font loading failed!" );
   }

   return valid;
}



bool TextRenderer::
Load( const TextRenderer &otherRenderer, int w, int h, Rgba col, int italics ) {
   if( !Setup::IsScreenSetUp() ) {
      OlLog( "Font copying queued" );
      loader = new RendLoader( otherRenderer, w, h, italics, col );
      return true;
   }

   OlLog( "Copying font" );

   if( col.IsValid() == false ) {
      col = otherRenderer.GetColor();
   }

   if( italics == UNDEFINED_ITALICS ) {
      italics = otherRenderer.GetItalics();
   }

   bool valid = Load( otherRenderer.GetFace(), w, h, col, italics );

   return valid;
}




bool TextRenderer::
Load( GLYPH_FACE *face, int w, int h, Rgba col, int italics, bool useHinting ) {
   if( !face )
      return false;

   this->col = col;
   this->face = face;
   this->useHinting = useHinting;
   
   rend = gk_create_renderer( face, 0 );

   if( !rend )
      return false;

   gk_rend_set_italic( rend, italics );

   gk_rend_set_size_pixels( rend, w, h );

   if( useHinting ) {
      gk_rend_set_hinting_default( rend );
   }
   else {
      gk_rend_set_hinting_off( rend );
   }

   rend_set_render_mode_normal( rend );

   SendToGPU();

   return true;
}



void TextRenderer::
SelectColor( const Rgba &col ) const {
   Rgba channels = Transforms::GetColorChannels();
   Rgba affectedCol( col.r * channels.r, col.g * channels.g, col.b * channels.b, col.a * channels.a );
   gk_rend_set_text_alpha_color( rend, affectedCol.SpecialPacked() );
}


void TextRenderer::
FinishRendering() const {
   Blenders::SelectBlender();
   if( Settings::TextureMappingUsed() ) {
      glEnable( GL_TEXTURE_2D );
   }
   else {
      glDisable( GL_TEXTURE_2D );
   }
}



const static char TAG_START = '[';
const static string COLOR_START_TAG_BEGIN = "[color ";
const static char TAG_END = ']';
const static string COLOR_END_TAG = "[/color]";


void TextRenderer::
Print( const string &text, int x, int y ) const {
   if( !IsValid()) {
      return;
   }
   
   StartRendering();

   int textHeight = gk_rend_ascender_pixels( rend );
   unsigned int start = 0;
   
   const Rgba originalColor = GetColor();
   
   int lineX = 0;
   
   for( unsigned int i = 0; i < text.length(); ) {
      unsigned int newStart = start;
      bool startNewLine = false;
      bool changeColor = false;
      Rgba newColor;
      
      if( text[i] == '\n' ) {
         newStart = i+1;
         startNewLine = true;
      }
      else if( text[i] == TAG_START && useTags ) {
         string::size_type tagStartLength = COLOR_START_TAG_BEGIN.length();
         
         if( text.substr( i, tagStartLength ) == COLOR_START_TAG_BEGIN ) {
            string::size_type tagEndPos = text.find( TAG_END, i + tagStartLength );
            
            string colorStr = text.substr( i + tagStartLength, tagEndPos - ( i + tagStartLength ));
            
            changeColor = true;
            newColor = Rgba( colorStr );
            newStart = tagEndPos+1;
         }
         else if( text.substr( i, COLOR_END_TAG.length() ) == COLOR_END_TAG ) {
            changeColor = true;
            newColor = originalColor;
            newStart = i + COLOR_END_TAG.length();
         }
      }
      
      if( newStart != start ) {
         string substr( text, start, i-start );
         gk_render_line_gl_utf8( texture, substr.c_str(), x + lineX, y );
         
         i = start = newStart;
         
         if( startNewLine ) {
            y += textHeight;
            lineX = 0;
         }
         else {
            lineX += Width( substr );
         }
         
         if( changeColor ) {
            SelectColor( newColor );
         }
      }
      else {
         i++;
      }
   }
   
   if( start != text.length() ) {
      string substr( text, start, text.length()-start );

      gk_render_line_gl_utf8( texture, substr.c_str(), x + lineX, y );
   }
}



void TextRenderer::
Print( const string &text, int x, int y, int maxLineWidth, TextAlignment alignment ) const {
   if( !IsValid()) {
      return;
   }

   StartRendering();

   int textHeight = gk_rend_ascender_pixels( rend );
   unsigned int start = 0;
   unsigned int lastSpace = 0;
   bool noLastSpace = true;

   for( unsigned int i = 0; i < text.length(); i++ ) {
      switch( text[i] ) {
         case ' ': case '\n': {
            string substr( text, start, i-start );

            int lineWidth = gk_text_width_utf8( rend, substr.c_str() );

            if( lineWidth > maxLineWidth && !noLastSpace ) {
               string line( text, start, lastSpace - start );

               RenderLineAligned( line, x, y, maxLineWidth, alignment );

               y += textHeight;
               start = lastSpace+1;
            }

            if( text[i] == ' ' ) {
               lastSpace = i;
               noLastSpace = false;
               break;
            }

            string line( text, start, i-start );

            lineWidth = gk_text_width_utf8( rend, substr.c_str() );

            TextAlignment realAlignment = ( alignment == JUSTIFY )? LEFT : alignment;

            RenderLineAligned( line, x, y, maxLineWidth, realAlignment );

            y += textHeight;
            start = i+1;

            lastSpace = 0;
            noLastSpace = true;
           break; }
      }
   }

   if( start != text.length() ) {
      string line( text, start, text.length()-start );

      int lineWidth = gk_text_width_utf8( rend, line.c_str() );

      if( lineWidth > maxLineWidth && !noLastSpace ) {
         string linePart( text, start, lastSpace - start );
         RenderLineAligned( linePart, x, y, maxLineWidth, alignment );
         y += textHeight;
         start = lastSpace+1;

         line = string( text, start, text.length()-start );
      }

      TextAlignment realAlignment = ( alignment == JUSTIFY )? LEFT : alignment;
      RenderLineAligned( line, x, y, maxLineWidth, realAlignment );
   }
}


void TextRenderer::
RenderLineAligned( const string &line, int x, int y,
                   int maxLineWidth, TextAlignment alignment ) const {
   if( alignment != JUSTIFY ) {
      int textX = x;

      switch( alignment ) {
         case LEFT:
           break;
         case RIGHT:
           textX += maxLineWidth - gk_text_width_utf8( rend, line.c_str() );
           break;
         case CENTER:
           textX += (maxLineWidth - gk_text_width_utf8( rend, line.c_str() ))/2;
           break;
	      case JUSTIFY:
          break; //nothing
      }

      gk_render_line_gl_utf8( texture, line.c_str(), textX, y );
   }
   else {
      int numSpaces = 0;

      for( unsigned int i = 0; i < line.length(); i++ ) {
         if( line[i] == ' ' ) numSpaces++;
      }

      float pixelsPerSpace = float( maxLineWidth - gk_text_width_utf8( rend, line.c_str() ))/numSpaces;
      int wordStart = 0;
      float wordX = x;

      for( unsigned int i = 0; i < line.length(); i++ ) {
         if( line[i] == ' ' ) {
            string word( line, wordStart, i - wordStart );
            gk_render_line_gl_utf8( texture, word.c_str(), int( wordX ), y );
            wordX += gk_text_width_utf8( rend, ( word + " " ).c_str() ) + pixelsPerSpace;

            wordStart = i+1;
         }
      }

      if( wordStart != (signed)line.length() ) {
         string word( line, wordStart, line.length() - wordStart );
         gk_render_line_gl_utf8( texture, word.c_str(), int( wordX ), y );
      }
   }
}

int TextRenderer::
FontHeight() const {
   return gk_rend_ascender_pixels( rend );
}

int TextRenderer::
FontTotalHeight() const {
   return gk_rend_height_pixels( rend );
}


int TextRenderer::
FirstLineWidth( const string &text ) const {
   int width = 0;

   for( unsigned int i = 0; i < text.length(); i++ ) {
      if( text[i] == '\n' ) {
         string substr( text, 0, i );
         width = gk_text_width_utf8( rend, substr.c_str() );
         break;
      }
   }

   if( width == 0 ) {
      width = gk_text_width_utf8( rend, text.c_str() );
   }

   return width;
}



int TextRenderer::
FirstLineHeight( const std::string &text ) const {
   return gk_rend_ascender_pixels( rend );
}



string TextRenderer::
GetColoredText( const std::string &str, Rgba color ) {
   return COLOR_START_TAG_BEGIN + color.ToHex() + TAG_END + str + COLOR_END_TAG;
}



int TextRenderer::
Width( const string &text ) const {
   int width = 0;
   int currW = 0;
   unsigned int start = 0;
   
   for( unsigned int i = 0; i < text.length(); i++ ) {
      if( text[i] == '\n' ) {
         string substr( text, start, i-start );
         currW += gk_text_width_utf8( rend, substr.c_str() );
         if( currW > width )
            width = currW;
         
         start = i+1;
         currW = 0;
      }
      else if( text[i] == TAG_START && useTags ) {
         string::size_type tagEndPos = text.find( TAG_END, i + 1 );
         
         if( tagEndPos != string::npos ) {
            string substr( text, i, tagEndPos - i + 1 );
            
            if( substr.substr( 0, COLOR_START_TAG_BEGIN.size() ) == COLOR_START_TAG_BEGIN
                  || substr == COLOR_END_TAG ) {
               string textThusFar( text, start, i-start );
               currW += gk_text_width_utf8( rend, textThusFar.c_str() );
               start = tagEndPos+1;
               i = start-1;
            }
         }
      }
   }
   
   if( start != text.length() || currW != 0 ) {
      string substr( text, start, text.length()-start );
      
      currW += gk_text_width_utf8( rend, substr.c_str() );
      if( currW > width )
         width = currW;
   }
   
   return width;
}



int TextRenderer::
Height( const string &text ) const {
   int textHeight = gk_rend_ascender_pixels( rend );//text_height_utf8( rend, text.c_str());
   int height = 0;
   unsigned int start = 0;
   for( unsigned int i = 0; i < text.length(); i++ ) {
      if( text[i] == '\n' ) {
         height += textHeight;
         start = i+1;
      }
   }

   if( start != text.length() ) {
      height += textHeight;
   }

   return height;
}



void TextRenderer::
SendToGPU() {
   if( texture ) {
      UnloadFromGPU();
   }

   texture = gk_create_texture( rend, 32, 96 );
   gk_send_texture_to_gpu( texture );
}



void TextRenderer::
UnloadFromGPU() {
   if( texture ) {
      gk_unload_texture_from_gpu( texture );
      gk_destroy_texture( texture );
      texture = 0;
   }
}



bool TextRenderer::
IsValid() const {
   return rend != 0 && texture != 0 && face != 0;
}



void TextRenderer::
SetColor( const Rgba& col ) {
   this->col = col;
}



const Rgba& TextRenderer::
GetColor() const {
   return col;
}




void TextRenderer::
SetItalics( int italics ) {
   if( italics != this->italics ) {
      UnloadFromGPU();
      gk_rend_set_italic( rend, italics );
      SendToGPU();
   }
}



int TextRenderer::
GetItalics() const {
   return italics;
}



GarbageCollection &OlGetCollection();


void TextRenderer::
Destroy( bool eraseFromGarbageCollection ) {
   UnloadFromGPU();

   if( eraseFromGarbageCollection ) {
      OlGetCollection().Remove( this );
   }
}



// GARBAGE COLLECTION / AUTOLOADER ROUTINES //

OlLoadResult TextRenderer::FileLoader::
Load( TextRenderer &subject ) {
   if( subject.Load( filename, w, h, col, italics ) == true ) {
      return OL_LR_SUCCESS;
   }

   return OL_LR_FAILURE;
}



OlLoadResult TextRenderer::RendLoader::
Load( TextRenderer &subject ) {
   if( !rend.IsValid() ) {
      return OL_LR_PENDING;
   }

   if( subject.Load( rend, w, h, col, italics ) == true ) {
      return OL_LR_SUCCESS;
   }

   return OL_LR_FAILURE;
}




OlLoadResult TextRenderer::
ExecuteQueuedCommands() {
   OlLoadResult result = OL_LR_SUCCESS;

   if( loader ) {
      result = loader->Load( *this );
      loader = 0;
   }

   return result;
}



void TextRenderer::
AddToCollection() {
   OlGetCollection().Add( this );
}



bool TextRenderer::
IsLoadingQueued() const {
   return loader != 0;
}

#endif /* OL_TEXT_RENDERER_CPP */

//#endif // OL_NO_TTF
