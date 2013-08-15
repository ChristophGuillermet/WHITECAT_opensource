#ifndef OL_GLYPH_HPP
#define OL_GLYPH_HPP

#include <math.h>
#include <map>
#include "Rgba.hpp"
#include "Bitmap.hpp"
#include "Point.hpp"
#include "Canvas.hpp"
#include "Declspec.hpp"

#ifdef USE_NEW_TTF
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#endif

#define GLYPH_PI	3.14159265358979323846
#define GLYPH_SQRT2	1.41421356237309504880

namespace ol
{
	// Forward decleration of Glyph for libFreeType
	class OL_LIB_DECLSPEC Glyph;
	
	/*
	*	These items are so that we can maintain compatibility with Glyph Keeper
	*	until TextRender adopts the new system and replaces references to
	*	GlyphKeeper in the future.
	*/
	
	class OL_LIB_DECLSPEC GLYPH_FACE
	{
		public:
			GLYPH_FACE();
			~GLYPH_FACE();
			
			Glyph *glyphFace;
	};
	
	class OL_LIB_DECLSPEC GLYPH_REND
	{
		public:
			GLYPH_REND();
			~GLYPH_REND();
			
			Glyph *glyphFace;
				
	};
	
	class OL_LIB_DECLSPEC GLYPH_TEXTURE
	{
		public:
			GLYPH_TEXTURE();
			~GLYPH_TEXTURE();
			
			Glyph *glyphFace;
			
	};
	
	OL_LIB_DECLSPEC GLYPH_FACE *gk_load_face_from_file(const char *filename, int index);
	OL_LIB_DECLSPEC GLYPH_REND *gk_create_renderer( GLYPH_FACE* const face, int index );
	OL_LIB_DECLSPEC void gk_rend_set_italic( GLYPH_REND* const rend, int italics );
	OL_LIB_DECLSPEC void gk_rend_set_size_pixels( GLYPH_REND* const rend, const unsigned int width, const unsigned int height);
	OL_LIB_DECLSPEC void gk_rend_set_hinting_default( GLYPH_REND* const rend );
	OL_LIB_DECLSPEC void gk_rend_set_hinting_off( GLYPH_REND* const rend );
	OL_LIB_DECLSPEC void rend_set_render_mode_normal( GLYPH_REND* const rend );
	OL_LIB_DECLSPEC void gk_rend_set_text_alpha_color( GLYPH_REND* const rend, const unsigned alpha_color);
	OL_LIB_DECLSPEC int gk_rend_ascender_pixels( GLYPH_REND* const rend );
	OL_LIB_DECLSPEC int gk_rend_height_pixels( GLYPH_REND* const rend );
	OL_LIB_DECLSPEC int gk_text_width_utf8(GLYPH_REND* const rend,const char* const text);
	OL_LIB_DECLSPEC GLYPH_TEXTURE *gk_create_texture( GLYPH_REND *rend, int rangeStart, int rangeLength );
	OL_LIB_DECLSPEC void gk_unload_texture_from_gpu( GLYPH_TEXTURE *texture );
	OL_LIB_DECLSPEC void gk_destroy_texture( GLYPH_TEXTURE *texture );
	OL_LIB_DECLSPEC void gk_render_line_gl_utf8( GLYPH_TEXTURE *texture, const char *text, int x, int y );
	OL_LIB_DECLSPEC void gk_send_texture_to_gpu( GLYPH_TEXTURE *texture );

//   OL_LIB_DECLSPEC Rgba colorConvert(const unsigned char *c,short ext);
//   OL_LIB_DECLSPEC Rgba colorConvert(const unsigned int c);

  inline Rgba colorConvert(const unsigned char *c,short ext)
  {
    const float component = *c / (float)(ext-1);
    return Rgba(component, component, component, component);
  }

  inline Rgba colorConvert(const unsigned int c)
  {
    return Rgba(
                (((c >> 16) & 0xff )/255.0f)
                ,(((c >>  8) & 0xff )/255.0f)
                ,(((c      ) & 0xff )/255.0f)
                ,(((c >> 24) & 0xff )/255.0f)
               );
  }


	class OL_LIB_DECLSPEC dimension
	{
		public:
		dimension()
		{
			width = height = 0;
			italics = 0;
		}
		~dimension(){}
		bool operator<(const dimension &d) const
		{
			if(width < d.width || height < d.height || italics < d.italics)return true;
			return false;
		}
		int width;
		int height;
		double italics;
	};
	
	class OL_LIB_DECLSPEC character
	{
		public:
			//! Constructor
			character();
			
			//! Destructor
			~character();
			
			//! Unicode representation of character
			signed long unicode;
			
			//! Width of character
			int width;
			
			//! Height of character
			int height;
			
			//! Space on the left of a character (assists on positioning the character)
			int left;
			
			//! Space on top of the character (assists on positioning the character)
			int top;
			
			//! Space on the right of a character (assists on positioning the character)
			int right;
			
			//! Pitch of a character (assists on positioning the character)
			int pitch;
			
			//! Amount of shades of grays the FT_Bitmap holds
			int grays;
			
			//! Entire rows of the FT_Bitmap
			int rows;
			
			//! Entire length of the character with spacing and all
			int length;
			
			//! FT_Bitmap raw data
			unsigned char *line;
	};
	
	// This class handles face objects
	class OL_LIB_DECLSPEC Glyph
	{
		private:
			//! ID
			int ID;
			
			//! Comparison of IDs
			bool operator==(Glyph *g);

			//! Current file
			std::string currentFilename;
			
			//! Is the face loaded
			bool faceLoaded;
			
			//! Does the face have kerning
			bool kerning;
			
			//! Current index default 0
			int currentIndex;
			
			//! Font size
			dimension size;
			
			//! Workspace bitmap
			Bitmap *workBitmap;
#ifdef USE_NEW_TTF
			//! Face
			FT_Face face;
#endif	
			//! Face Name
			std::string faceName;
			
			//! Current character
			character *currentChar;
			
			//! Lookup Table by size
			std::map<dimension, std::map<signed long, character> >fontTable;
			
			//! Load flags
			unsigned hintingFlag;
			unsigned renderFlag;
			
			//! Extract glyph
			character extractGlyph(signed long unicode);
			
			//! Create single index
			void createIndex();
			
			//! Render a character from the lookup table (utilizing the workBitmap)
			void drawCharacter(signed long unicode, double &x1, double &y1, Bitmap *bitmap, const Rgba & col);
			
			friend class GLYPH_FACE;
			friend class GLYPH_REND;
			friend class GLYPH_TEXTURE;
		public:
			//! Constructor
			Glyph();
			
			//! Destructor
			~Glyph();
			
			//! Load font from memory
			bool loadFromMemory(const unsigned char *memoryFont, unsigned int length, int index=0, unsigned int width=14, unsigned int height=8);
			
			//! Load font from file
			bool load(const std::string & filename, int index=0, unsigned int width=14, unsigned int height=8);
			
			//! Get text length
			double getLength(const char* text);
			
			//! Render font to a bitmap
			void render(double x, double y, const Rgba& col, Bitmap *bmp, int alignment, const char* text, ...);

      void renderFixed(double x, double y, const Rgba& col, Bitmap *bmp, int alignment, const char* text);
			
			//! Set size
			void setSize(int w, int h);
			
			//! Set italics
			void setItalics(int i);
			
			//! Set FreeType LoadFlags
			void setHinting(bool on=true);
			
			//! Set FreeType LoadFlags
			void setAntialias(bool on=false);
			
			//! Get width
			int getWidth();
			
			//! Get height
			int getHeight();
			
			//! Get total height
			int getTotalHeight();
			
			//! Get italics
			int getItalics();
			
			//! Color
			Rgba color;
			
			//! Enumerator for positioning of text when rendering
			enum
			{
				LEFT=0,
				CENTERED,
				RIGHT
			};
	};

}

#endif /* OL_GLYPH_HPP */
