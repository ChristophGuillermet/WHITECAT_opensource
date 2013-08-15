#ifndef OL_RENDER_MODES_HPP
#define OL_RENDER_MODES_HPP

#include "Rgba.hpp"
#include "TextureInfo.hpp"
#include "Declspec.hpp"


// RENDER MODES //


namespace ol {


class MultiMode;
class Bitmap;


enum ETexCoord {
   OL_TC_TOPLEFT,
   OL_TC_TOPRIGHT,
   OL_TC_BOTTOMRIGHT,
   OL_TC_BOTTOMLEFT
};


// RenderMode - The base class of the mode the bitmap is rendered with //

class OL_LIB_DECLSPEC RenderMode {
public:
   virtual ~RenderMode(){}

   // The sum of two RenderModes returns a combination of those two //
   MultiMode operator+( const RenderMode &other ) const;

   // Internal functions //

   virtual void Select() const {}
   virtual void Unselect() const {}

   virtual bool SetsTexCoords() const { return false; }
   virtual void SetTexCoord( ETexCoord coord, OlRect &renderRect, OlTexCoords &texCoordRect, float w, float h ) const;
   static void PrimarySetTexCoord( ETexCoord coord, OlRect &renderRect, OlTexCoords &texCoordRect, float w, float h );

   virtual OlRect GetRenderRect( OlRect current ) const;
   virtual OlTexCoords GetTexCoords( OlTexCoords current ) const;

   //virtual OlRect GetRenderArea() const;
};



// Renders the bitmap tinted to a color //

class OL_LIB_DECLSPEC TintMode : public RenderMode {
public:

   // The bitmap will be tinted to the given color //
   // The alpha value of the color tells the intensity of the tinting //
   TintMode( Rgba color ) : color( color ) {}
	virtual ~TintMode(){}

   // Internal functions //

   virtual void Select() const;
   virtual void Unselect() const;
protected:
   Rgba color;
};


// Renders the bitmap with an additional alpha channel from an another bitmap //
// The values of the alpha channels are multiplied together //

class OL_LIB_DECLSPEC GainAlphaMode : public RenderMode {
public:
   GainAlphaMode( const Bitmap &alphaFrom, float anchorX = 0.0, float anchorY = 0.0 ) : alphaFrom( alphaFrom ), x( anchorX ), y( anchorY ) {}
	virtual ~GainAlphaMode(){}
   // Internal functions //

   virtual void Select() const;
   virtual void Unselect() const;

   virtual bool SetsTexCoords() const { return true; }
   virtual void SetTexCoord( ETexCoord coord, OlRect &renderRect, OlTexCoords &texCoordRect, float w, float h ) const;

   virtual OlRect GetRenderRect( OlRect current ) const;
   virtual OlTexCoords GetTexCoords( OlTexCoords current ) const;

protected:
   const Bitmap &alphaFrom;
   float x, y;
};



enum OlFlippingChoise {
   HORIZONTAL,
   VERTICAL,
   BOTH
};



// Flips the bitmap in the specified way //

class OL_LIB_DECLSPEC FlippedMode : public RenderMode {
public:
   FlippedMode( OlFlippingChoise mode ) : mode( mode ) {}
   virtual ~FlippedMode(){}

   virtual OlTexCoords GetTexCoords( OlTexCoords current ) const;
protected:
   OlFlippingChoise mode;
};


class Rect;

// Clips the bitmap to a region //

class OL_LIB_DECLSPEC ClippedMode : public RenderMode {
public:
   ClippedMode( float x, float y, float w, float h )
      : clipRect( x, y, w, h ) {}

   ClippedMode( const Rect &clipArea );
   
   virtual ~ClippedMode(){}

   virtual OlRect GetRenderRect( OlRect current ) const;
   virtual OlTexCoords GetTexCoords( OlTexCoords current ) const;
private:
   OlRect clipRect;
};



enum OlSlicingChoise {
   HORIZONTAL_SLICE,
   VERTICAL_SLICE
};


// Stretches a one pixel wide slice of the bitmap to fill the entire bitmap //

class OL_LIB_DECLSPEC SliceMultiplyMode : public RenderMode {
public:
   SliceMultiplyMode( float slicePos, OlSlicingChoise mode )
      : slicePos( slicePos ), mode( mode ) {}
	virtual ~SliceMultiplyMode(){}

   virtual OlTexCoords GetTexCoords( OlTexCoords current ) const;
private:
   float slicePos;
   OlSlicingChoise mode;
};



// Specifies different color channel cofficients for each corner of the Bitmap

class OL_LIB_DECLSPEC GradientMode : public RenderMode {
public:
   GradientMode( Rgba topLeft, Rgba topRight, Rgba bottomRight, Rgba bottomLeft ) {
      corners[0] = topLeft;
      corners[1] = topRight;
      corners[2] = bottomRight;
      corners[3] = bottomLeft;
   }

   // Specifies different alpha values for each corner of the Bitmap
   GradientMode( float topLeft, float topRight, float bottomRight, float bottomLeft ) {
      corners[0] = Rgba(1.0f, 1.0f, 1.0f, topLeft);
      corners[1] = Rgba(1.0f, 1.0f, 1.0f, topRight);
      corners[2] = Rgba(1.0f, 1.0f, 1.0f, bottomRight);
      corners[3] = Rgba(1.0f, 1.0f, 1.0f, bottomLeft);
   }
   virtual ~GradientMode(){}

   virtual OlTexCoords GetTexCoords( OlTexCoords current ) const;
private:
   enum {
      NUM_CORNERS = 4
   };

   Rgba corners[NUM_CORNERS];
};

/*
class Perspective : public RenderMode {
public:
   Perspective( float topStretch, float bottomStretch )
      : topStretch( topStretch ), bottomStretch( bottomStretch ) {}

   virtual OlRect GetRenderRect( OlRect current ) const;
private:
   float topStretch;
   float bottomStretch;
};*/


#ifndef OL_NO_RENDER_MODE_ALIASES

typedef TintMode Tinted;
typedef GainAlphaMode AlphaFrom;
typedef FlippedMode Flipped;
typedef ClippedMode Clipped;
typedef SliceMultiplyMode SliceExpanded;
typedef GradientMode Gradient;

#endif


// Combines two RenderModes to a single entity //
// (You can use the sum operator of BlendingModes which returns a MultiMode) //

class OL_LIB_DECLSPEC MultiMode : public RenderMode {
public:
   MultiMode( const RenderMode &mode1, const RenderMode &mode2 ) : mode1( mode1 ), mode2( mode2 ) {}
   virtual ~MultiMode(){}

   virtual void Select() const;
   virtual void Unselect() const;

   virtual bool SetsTexCoords() const;
   virtual void SetTexCoord( ETexCoord coord, OlRect &renderRect, OlTexCoords &texCoordRect, float w, float h ) const;

   virtual OlRect GetRenderRect( OlRect current ) const;
   virtual OlTexCoords GetTexCoords( OlTexCoords current ) const;
protected:
   const RenderMode &mode1;
   const RenderMode &mode2;
};


}


#endif // OL_RENDER_MODES_HPP
