#include "RenderModes.hpp"
#include "TextureInfo.hpp"
#include "Bitmap.hpp"
#include "Rgba.hpp"
#include "Rectangle.hpp"

using namespace ol;

// RENDER MODES //


// * GENERAL FUNCTIONS //

ClippedMode::
ClippedMode( const Rect &clipArea )
   : clipRect( clipArea.pos.x, clipArea.pos.y, clipArea.size.x, clipArea.size.y ) {}


OlRect RenderMode::
GetRenderRect( OlRect current ) const {
   return current;
}



OlTexCoords RenderMode::
GetTexCoords( OlTexCoords current ) const {
   return current;
}


void RenderMode::
PrimarySetTexCoord( ETexCoord coord, OlRect &renderRect, OlTexCoords &texCoords, float w, float h ) {
   switch( coord ) {
      case OL_TC_TOPLEFT:
         glTexCoord2f( texCoords.convx1, texCoords.convy1 );
        break;
      case OL_TC_TOPRIGHT:
         glTexCoord2f( texCoords.convx2, texCoords.convy1 );
        break;
      case OL_TC_BOTTOMLEFT:
         glTexCoord2f( texCoords.convx1, texCoords.convy2 );
        break;
      case OL_TC_BOTTOMRIGHT:
         glTexCoord2f( texCoords.convx2, texCoords.convy2 );
        break;
      default:
         OlError( "Unknown enumeration in SetTextCoord!" );
   }
}



void RenderMode::
SetTexCoord( ETexCoord coord, OlRect &renderRect, OlTexCoords &texCoordRect, float w, float h ) const {
   PrimarySetTexCoord( coord, renderRect, texCoordRect, w, h );
}



// * TINT MODE //


void TintMode::
Select() const {
   // Use linear interpolation //
   glFogi( GL_FOG_MODE, GL_LINEAR );
   
   #ifdef NO_COLOR_CHANNELS
      float fogCol[] = { color.r, color.g, color.b, 1.0 };
   #else
      Rgba channels = Transforms::GetColorChannels();
      float fogCol[] = { channels.r * color.r, channels.g * color.g, channels.b * color.b, 1.0 };
   #endif
   
   glFogfv( GL_FOG_COLOR, fogCol );
   glFogf( GL_FOG_DENSITY, 1.0 );
   
   glFogf( GL_FOG_START, -color.a );
   glFogf( GL_FOG_END, (1.0 - color.a ));
   
   // We don't need per-pixel fog, per-verted will do the same //
   glHint( GL_FOG_HINT, GL_FASTEST );
   
   glEnable( GL_FOG );
}



void TintMode::
Unselect() const {
   glDisable( GL_FOG );
}



// * GAIN ALPHA MODE //

// Thanks to Graham Goring for submitting the following two functions //

void GainAlphaMode::
Select() const {
   glActiveTexture(GL_TEXTURE1);
   glEnable(GL_TEXTURE_2D);
   alphaFrom.Select();
   glActiveTexture(GL_TEXTURE0);

   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
   glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
   glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE);
   glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PRIMARY_COLOR);
   glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
   glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_TEXTURE);
   glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA, GL_PRIMARY_COLOR);
   
   glActiveTexture(GL_TEXTURE1);
   
   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
   glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);
   glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_PREVIOUS);
   glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
   glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_TEXTURE);
   glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA, GL_PREVIOUS);
   
   glActiveTexture(GL_TEXTURE0);
}



void GainAlphaMode::
Unselect() const {
   glActiveTexture(GL_TEXTURE1);
   glDisable(GL_TEXTURE_2D);
   
   glActiveTexture(GL_TEXTURE0);
   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
   
   glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
   glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE);
   glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PRIMARY_COLOR);
   
   glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
   glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_TEXTURE);
   glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA, GL_PRIMARY_COLOR);
   glDisable( GL_TEXTURE_2D );
}



void GainAlphaMode::
SetTexCoord( ETexCoord coord, OlRect &renderRect, OlTexCoords &texCoords, float w, float h ) const {
   
   OlRect otherTCR(( x < 0.0 )? -x/alphaFrom.Width() * alphaFrom.GetTextureEndX() : 0.0,
                   ( y < 0.0 )? -y/alphaFrom.Height() * alphaFrom.GetTextureEndY() : 0.0,
                   ( x + alphaFrom.Width() > w )? ( w - x )/alphaFrom.Width() * alphaFrom.GetTextureEndX() : alphaFrom.GetTextureEndX(),
                   ( y + alphaFrom.Height() > h )? ( h - y )/alphaFrom.Height() * alphaFrom.GetTextureEndY() : alphaFrom.GetTextureEndY());
   
   switch( coord ) {
      case OL_TC_TOPLEFT:
         glMultiTexCoord2f( GL_TEXTURE0, texCoords.convx1, texCoords.convy1 );
         glMultiTexCoord2f( GL_TEXTURE1, otherTCR.x, 1.0 - otherTCR.y );
        break;
      case OL_TC_TOPRIGHT:
         glMultiTexCoord2f( GL_TEXTURE0, texCoords.convx2, texCoords.convy1 );
         glMultiTexCoord2f( GL_TEXTURE1, otherTCR.w, 1.0 - otherTCR.y );
        break;
      case OL_TC_BOTTOMLEFT:
         glMultiTexCoord2f( GL_TEXTURE0, texCoords.convx1, texCoords.convy2 );
         glMultiTexCoord2f( GL_TEXTURE1, otherTCR.x, 1.0 - ( otherTCR.h ));
        break;
      case OL_TC_BOTTOMRIGHT:
         glMultiTexCoord2f( GL_TEXTURE0, texCoords.convx2, texCoords.convy2 );
         glMultiTexCoord2f( GL_TEXTURE1, otherTCR.w, 1.0 - ( otherTCR.h ));
        break;
      default:
         OlError( "Unknown enumeration in SetTextCoord!" );
   }
}



OlRect GainAlphaMode::
GetRenderRect( OlRect current ) const {
   OlRect clipRect( x, y, alphaFrom.Width(), alphaFrom.Height() );
   return current.ClippedTo( clipRect );
}



OlTexCoords GainAlphaMode::
GetTexCoords( OlTexCoords current ) const {
   OlRect clipRect( x, y, alphaFrom.Width(), alphaFrom.Height() );
   return current.ClippedTo( clipRect );
}



// * SLICE MULTIPLY MODE //


OlTexCoords SliceMultiplyMode::
GetTexCoords( OlTexCoords current ) const {
   switch( mode ) {
      case HORIZONTAL_SLICE:
         current.y1 += slicePos;
         current.y2 = current.y1;
        break;
      case VERTICAL_SLICE:
         current.x1 += slicePos;
         current.x2 = current.x1;
        break;
      default:
         OlError( "Unknown enumeration in SliceMultiplyMode::GetTexCoords!" );
   }
   
   return current;
}


// * CLIPPED MODE //


OlTexCoords ClippedMode::
GetTexCoords( OlTexCoords current ) const {
   return current.ClippedTo( clipRect );
}



OlRect ClippedMode::
GetRenderRect( OlRect current ) const {
   return current.ClippedTo( clipRect );
}



// * FLIPPED MODE //


OlTexCoords FlippedMode::
GetTexCoords( OlTexCoords current ) const {
   switch( mode ) {
      case HORIZONTAL:
         current.FlipHorizontal();
        break;
      case VERTICAL:
         current.FlipVertical();
        break;
      case BOTH:
         current.FlipHorizontal();
         current.FlipVertical();
        break;
      default:
         OlError( "Unknown enumeration in FlippedMode::Select!" );
   }
   
   return current;
}



// * GRADIENT MODE //


OlTexCoords GradientMode::
GetTexCoords( OlTexCoords current ) const {
   current.SetColors( corners );
   return current;
}


// * MULTI MODE //


MultiMode RenderMode::
operator+( const RenderMode &other ) const {
   return MultiMode( *this, other );
}



void MultiMode::
Select() const {
   mode1.Select();
   mode2.Select();
}



void MultiMode::
Unselect() const {
   mode1.Unselect();
   mode2.Unselect();
}




bool MultiMode::
SetsTexCoords() const {
   return mode1.SetsTexCoords() || mode2.SetsTexCoords();
}




void MultiMode::
SetTexCoord( ETexCoord coord, OlRect &renderRect, OlTexCoords &texCoordRect, float w, float h ) const {
   if( mode1.SetsTexCoords() )
      mode1.SetTexCoord( coord, renderRect, texCoordRect, w, h );
   
   if( mode2.SetsTexCoords() )
      mode2.SetTexCoord( coord, renderRect, texCoordRect, w, h );
}



OlRect MultiMode::
GetRenderRect( OlRect current ) const {
   current = mode1.GetRenderRect( current );
   return mode2.GetRenderRect( current );
}



OlTexCoords MultiMode::
GetTexCoords( OlTexCoords current ) const {
   return mode2.GetTexCoords( mode1.GetTexCoords( current ));
}

