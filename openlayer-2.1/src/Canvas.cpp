#include "Canvas.hpp"
#include "Framebuffer.hpp"
#include "Blenders.hpp"
#include "Setup.hpp"
#include "Rectangle.hpp"
#include "Includes.hpp"

using namespace ol;


// CONSTANTS //

static unsigned int SCREENSHOT_BPP = 3;
static int SCREENSHOT_FORMAT = GL_RGB;


int Canvas::
GetScreenshotBPP() {
   return SCREENSHOT_BPP;
}



int Canvas::
GetScreenshotFormat() {
   return SCREENSHOT_FORMAT;
}


// STATIC CLASS VARIABLES //

Canvas::SurfaceInfo Canvas::activeSurface( SCREEN_BACKBUF, 0, 0 );
std::map< RenderTarget, Canvas::SurfaceInfo > Canvas::storedSurfaces;
std::stack< Canvas::SurfaceInfo > Canvas::pushedSurfaces;


// GENERAL FUNCTIONS //


void Canvas::
ReadBufferSizes() {
   activeSurface.SelectDefaultBufferSize();
   
   for( std::map< RenderTarget, Canvas::SurfaceInfo > ::iterator iter = storedSurfaces.begin();
        iter != storedSurfaces.end(); iter++ ) {
      iter->second.SelectDefaultBufferSize();
   }
}



void Canvas::SurfaceInfo::
SelectDefaultBufferSize() {
   width = Setup::GetWindowWidth();
   height = Setup::GetWindowHeight();
}



void Canvas::SurfaceInfo::
SetPixelWriteMode( PixelWriteMode mode ) {
   writeMode = mode;
   ApplyPixelWriteMode();
}



void Canvas::
ReleaseSurface() {
   switch( activeSurface.GetType() ) {
      case BITMAP_TARGET:
         Refresh();
         FrameBuffer::GetInstance().Release();
        break;
      case SCREEN_BACKBUF: case SCREEN_FRONTBUF:
         storedSurfaces[activeSurface.GetType()] = activeSurface;
        break;
      default:
         OlError( "Unknown rendering target in ReleaseSurface: " + ToString((int) activeSurface.GetType() ));
   }
}



void Canvas::
SetTo( const Bitmap &image ) {
   ReleaseSurface();
   
   activeSurface = SurfaceInfo( BITMAP_TARGET, image.Width(), image.Height(), &image );
   SelectImage( image );
   
   FinishSurfaceSelection();
}



void Canvas::
SetTo( RenderTarget buffer ) {
   if( buffer == BITMAP_TARGET ) {
      OlError( "Can't select BITMAP_TARGET as the active rendering target without using the overloaded version of Canvas::SetTo" );
      return;
   }
   
   ReleaseSurface();
   
   SelectBuffer( buffer );
   
   if( storedSurfaces.find( buffer ) == storedSurfaces.end() )
      activeSurface = SurfaceInfo( buffer, Setup::GetWindowWidth(), Setup::GetWindowHeight() );
   else
      activeSurface = storedSurfaces[buffer];
   
   FinishSurfaceSelection();
}


void Canvas::
SetTo( const SurfaceInfo &info ) {
   ReleaseSurface();
   
   activeSurface = info;
   
   switch( info.GetType() ) {
      case SCREEN_BACKBUF: case SCREEN_FRONTBUF:
         SelectBuffer( info.GetType() );
        break;
      case BITMAP_TARGET: {
         const Bitmap *image = activeSurface.GetImage();
         
         if( !image ) {
            OlError( "The selected Canvas with Bitmap information is incorrect - No Bitmap found" );
            SetTo( SCREEN_BACKBUF );
            return;
         }
         
         SelectImage( *image );
        break; }
      default:
         OlError( "Unknown rendering target in Canvas::SetTo (private): " + ToString((int) info.GetType() ));
   }
   
   FinishSurfaceSelection();
}


void Canvas::
SelectBuffer( RenderTarget buffer ) {
   switch( buffer ) {
      case SCREEN_BACKBUF:
         glDrawBuffer( GL_BACK );
         glReadBuffer( GL_BACK );
        break;
      case SCREEN_FRONTBUF:
         glDrawBuffer( GL_FRONT );
         glReadBuffer( GL_FRONT );
        break;
      default:
         OlError( "Unknown buffer in SelectBuffer: " + ToString((int) buffer ));
   }
}


void Canvas::
SelectImage( const Bitmap &image ) {
   FrameBuffer::GetInstance().BindToTexture( image.textureInfo );
}


void Canvas::
FinishSurfaceSelection() {
   activeSurface.ApplySettings();
}


void Canvas::
Refresh() {
   Transforms::ApplyTinting();
   
   switch( activeSurface.GetType() ) {
      case BITMAP_TARGET:
         //SetPixelWriteMode( COLOR_AND_ALPHA );
         FrameBuffer::GetInstance().RefreshSurface();
        break;
      case SCREEN_BACKBUF:
         allegro_gl_flip();
        break;
      case SCREEN_FRONTBUF:
         glFlush();
         glFinish();
        break;
      default:
         OlError( "Unknown rendering target in Canvas::Refresh: " + ToString((int) activeSurface.GetType() ));
         return;
   }
   
   Blenders::SelectBlender();
}



void Canvas::
Fill( Rgba col, PixelWriteMode filledComponents ) {
   PixelWriteMode oldMode = GetPixelWriteMode();
   SetPixelWriteMode( filledComponents );
   
   glClearColor( col.r, col.g, col.b, col.a );
   glClear( GL_COLOR_BUFFER_BIT );
   
   SetPixelWriteMode( oldMode );
}


void Canvas::SurfaceInfo::
SetClipping( int x, int y, int w, int h ) {
   glEnable( GL_SCISSOR_TEST );
   
   OlRectangle< int > clippingRegion( 0, 0, width, height );
   
   clippingRegions.push( OlRectangle< int >( x, y, w, h ));
   
   ApplyClipping();
}



void Canvas::SurfaceInfo::
ApplySettings() {
   ApplyClipping();
   ApplyPixelWriteMode();
}



void Canvas::SurfaceInfo::
ApplyClipping() {
   if( clippingRegions.empty() ) {
      DisableClipping();
   }
   else {
      const OlRectangle< int > &clippingRegion = clippingRegions.top();
      
      glScissor( clippingRegion.x, height - (clippingRegion.y + clippingRegion.h),
         clippingRegion.w, clippingRegion.h );
      
      glEnable( GL_SCISSOR_TEST );
   }
}



void Canvas::SurfaceInfo::
ApplyPixelWriteMode() {
   switch( writeMode ) {
      case COLOR_AND_ALPHA:
         glColorMask( true, true, true, true );
        break;
      case COLOR_ONLY:
         glColorMask( true, true, true, false );
        break;
      case ALPHA_ONLY:
         glColorMask( false, false, false, true );
        break;
   }
}



void Canvas::SurfaceInfo::
DisableClipping() {
   clippingRegions = std::stack< OlRectangle< int > > ();
   glDisable( GL_SCISSOR_TEST );
}


Rect Canvas::SurfaceInfo::
GetClippingRegion() {
   if(!clippingRegions.empty()) {
      const OlRectangle< int > &clippingRegion = clippingRegions.top();
      return Rect( clippingRegion.x, clippingRegion.y, clippingRegion.w, clippingRegion.h );
   }
   
   return Rect( 0, 0, Width(), Height() );
}



void Canvas::SurfaceInfo::
ChooseDefaultPixelWriteMode() {
   switch( target ) {
      case BITMAP_TARGET:
         writeMode = COLOR_AND_ALPHA;
        break;
      case SCREEN_BACKBUF:
         writeMode = COLOR_ONLY;
        break;
      default:
         OlError( "Unknown render target: " + ToString((int) target ));
   }
}



OL_MEMORY_IMG *Canvas::
GetMemoryBitmap() {
   int width = Canvas::Width();
   int height = Canvas::Height();
   
   unsigned char *pixelData = new unsigned char[width * height * SCREENSHOT_BPP];
   glReadPixels( 0, 0, width, height, SCREENSHOT_FORMAT, GL_UNSIGNED_BYTE, pixelData );
   OL_MEMORY_IMG *img = GlDriver::Get()->GetMemoryBitmap( width, height, SCREENSHOT_BPP, pixelData );
   
   delete[] pixelData;
   return img;
}



void Canvas::
Save( std::string filename ) {
   switch( activeSurface.GetType() ) {
      case SCREEN_BACKBUF: case SCREEN_FRONTBUF: {
         OL_MEMORY_IMG *screenshotImg = GetMemoryBitmap();
         GlDriver::Get()->SaveMemoryBitmap( screenshotImg, Setup::ToAbsolutePathname( filename ).c_str() );
         GlDriver::Get()->DestroyMemoryBitmap( screenshotImg );
        break; }
      case BITMAP_TARGET: {
         const Bitmap *image = activeSurface.GetImage();
         
         if( !image ) {
            OlError( "The saved Canvas with Bitmap information is incorrect - No Bitmap found" );
            SetTo( SCREEN_BACKBUF );
            return;
         }
         
         image->SaveIfSurface( filename );
        break; }
      default:
         OlError( "Unknown render target: " + ToString((int) activeSurface.GetType() ));
   }
}


