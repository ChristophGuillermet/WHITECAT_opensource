#ifndef OL_CANVAS_HPP
#define OL_CANVAS_HPP


#include "Includes.hpp"
#include "Bitmap.hpp"
#include "Rectangle.hpp"
#include "OlRectangle.hpp"
#include "Declspec.hpp"

#include <stack>
#include <map>
#include <string>


namespace ol {


enum RenderTarget {
   BITMAP_TARGET,
   SCREEN_BACKBUF,
   SCREEN_FRONTBUF
};



enum PixelWriteMode {
   COLOR_AND_ALPHA,
   COLOR_ONLY,
   ALPHA_ONLY
};


class Rect;

// The rendering surface //

class OL_LIB_DECLSPEC Canvas {
public:
   // Sets the Bitmap as the active rendering surface //
   static void SetTo( const Bitmap &image );
   
   // Sets the specified OpenGL buffer as the active backbuffer //
   // Restores the clipping and pixel write settings of the RenderTarget //
   static void SetTo( RenderTarget buffer );
   
   // Refresh the active canvas (backbuffer) //
   static void Refresh();
   
   // Fills the active rendering surface with the specified color //
   static void Fill( Rgba fillColor, PixelWriteMode filledComponents = COLOR_AND_ALPHA );
   
   // Sets the clipping region of the active rendering surface //
   static inline void SetClipping( int x, int y, int w, int h ) {
      activeSurface.SetClipping( x, y, w, h );
   }
   
   // Sets the clipping region of the active rendering surface //
   // Warning: The area will be rounded down to whole pixels! //
   static inline void SetClipping( const Rect &area ) {
      SetClipping( int(floor( area.pos.x )), int(floor( area.pos.y )), int(ceil( area.size.x )), int(ceil( area.size.y )));
   }
   
   // Restores previous clipping region //
   static inline void RevertClipping() {
      activeSurface.RevertClipping();
   }
   
   // Disables the clipping region //
   static inline void DisableClipping() {
      activeSurface.DisableClipping();
   }
   
   // Returns the active clipping region //
   static inline Rect GetClippingRegion() {
      return activeSurface.GetClippingRegion();
   }
   
   // Returns the width of the canvas //
   inline static int Width() {
      return activeSurface.width;
   }
   
   // Returns the height of the canvas //
   inline static int Height() {
      return activeSurface.height;
   }
   
   // Sets which color components OpenLayer will render to the canvas //
   // Defaults:
   // - For SCREEN_BACKBUF: COLOR_ONLY (Alpha channel of the canvas is not modified) //
   // - For Bitmaps: COLOR_AND_ALPHA (All color componens of the canvas are modified) //
   static void SetPixelWriteMode( PixelWriteMode mode ) {
      return activeSurface.SetPixelWriteMode( mode );
   }
   
   // Returns which color componenents will be rendered to the canvas (see above) //
   static PixelWriteMode GetPixelWriteMode() {
      return activeSurface.GetPixelWriteMode();
   }
   
   // Stores the Canvas in a stack //
   inline static void Push() {
      pushedSurfaces.push( activeSurface );
   }
   
   // Pops the topmost Canvas from the stack //
   // Returns true if there was a Canvas stored in the stack //
   inline static bool Pop() {
      if( pushedSurfaces.size() > 0 ) {
         SetTo( pushedSurfaces.top() );
         pushedSurfaces.pop();
         return true;
      }
      
      return false;
   }
   
   // Saves the contents of the surface with the given name //
   static void Save( std::string filename );
   
   // Returns the contents of the surface as a memory bitmap //
   static OL_MEMORY_IMG *GetMemoryBitmap();
   
   // Retrieves the sizes of the screen buffers from Setup //
   static void ReadBufferSizes();
   
   static int GetScreenshotBPP();
   
   static int GetScreenshotFormat();
   
private:
   static void FinishSurfaceSelection();
   static void ReleaseSurface();
   
   static void SelectBuffer( RenderTarget buffer );
   static void SelectImage( const Bitmap &image );
   
   class SurfaceInfo {
   public:
      SurfaceInfo()
         : width( 0 ), height( 0 ), target( BITMAP_TARGET ), image( 0 ) {}
      
      SurfaceInfo( RenderTarget target, int width, int height, const Bitmap *image = 0 )
         : width( width ), height( height ), target( target ), image( image ) {}
      
      void ApplySettings();
      
      inline RenderTarget GetType() const {
         return target;
      }
      
      inline const Bitmap *GetImage() const {
         return image;
      }
      
      void SetClipping( int x, int y, int w, int h );
      Rect GetClippingRegion();
      void RevertClipping();
      void DisableClipping();
      
      void SelectDefaultBufferSize();
      
      void SetPixelWriteMode( PixelWriteMode mode );
      inline PixelWriteMode GetPixelWriteMode() {
         return writeMode;
      }
      void ChooseDefaultPixelWriteMode();
      
      int width, height;
   private:
      void ApplyClipping();
      void ApplyPixelWriteMode();
      
      std::stack< OlRectangle< int > > clippingRegions;
      PixelWriteMode writeMode;
      RenderTarget target;
      const Bitmap *image;
   };
   
   static void SetTo( const Canvas::SurfaceInfo &info );
   
   static SurfaceInfo activeSurface;
   static std::map< RenderTarget, SurfaceInfo > storedSurfaces;
   static std::stack< SurfaceInfo > pushedSurfaces;
};


};





#endif // OL_CANVAS_HPP
