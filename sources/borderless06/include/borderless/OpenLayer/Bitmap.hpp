#ifndef OL_BITMAP_HPP
#define OL_BITMAP_HPP


#include "Includes.hpp"
#include "GarbageCollector.hpp"
#include "Rgba.hpp"
#include "Internal.hpp"
#include "PendingLoad.hpp"
#include "Collisions.hpp"
#include "Vec2D.hpp"
#include "RenderModes.hpp"
#include "GlDriver.hpp"
#include "Declspec.hpp"

#include <vector>
#include <string>
#include <math.h>


namespace ol {



class RenderMode;
class Rect;
class Poly;


// Conversion options used when converting an Allegro BITMAP to an OpenLayer Bitmap //

enum {
   HAS_ALPHA_CHANNEL = 0x1,
   CONVERT_MAGIC_PINK = 0x2
};


enum {
   CREATE_COLLISION_POLY = 0x1
};


// Bitmap - Stores a bitmap and draws it to the screen //

class OL_LIB_DECLSPEC Bitmap : public GarbageCollected {
public:
    Bitmap()                                                                          : bmp( 0 ), collisionPoly( 0 ), destroyBmp( true ), pendingLoad( 0 ), pivot( Vec2D( 0.0, 0.0 )), useDefaultPivot( true ), isSubBitmap( false ) { AddToCollection(); }
    
    // The copy constructor will only create a new handler to the same image data //
    Bitmap( const Bitmap &other )                                                     : bmp( other.bmp ), collisionPoly( other.collisionPoly ), destroyBmp( false ), pendingLoad( 0 ), pivot( other.pivot ), useDefaultPivot( other.useDefaultPivot ), isSubBitmap( true ) { AddToCollection(); }
    
    // ExtraFlags should be zero or MAKE_COLLISION_POLY
    
    // Construct the Bitmap using a bitmap file //
    Bitmap( std::string filename, int extraFlags = 0 )                                : bmp( 0 ), collisionPoly( 0 ), destroyBmp( true ), pendingLoad( 0 ), pivot( Vec2D( 0.0, 0.0 )), useDefaultPivot( true ), isSubBitmap( false ) { AddToCollection(); Load( filename.c_str(), extraFlags ); }
    
    // Construct the Bitmap using a separate bitmap for //
    // the color and the transparency information //
    Bitmap( const char *rgbFilename, const char *alphaFilename, int extraFlags = 0 )  : bmp( 0 ), collisionPoly( 0 ), destroyBmp( true ), pendingLoad( 0 ), pivot( Vec2D( 0.0, 0.0 )), useDefaultPivot( true ), isSubBitmap( false ) { AddToCollection(); Load( rgbFilename, alphaFilename, extraFlags ); }
    
    // Construct a Bitmap from Allegro's BITMAP //
    // Pass true as convertMagicPink if the bitmap has //
    // transparent parts marked with the magic pink //
    Bitmap( OL_MEMORY_IMG *bmp, bool hasAlphaChannel = false,
                         bool convertMagicPink = false, int extraFlags = 0 )          : bmp( 0 ), collisionPoly( 0 ), destroyBmp( true ), pendingLoad( 0 ), pivot( Vec2D( 0.0, 0.0 )), useDefaultPivot( true ), isSubBitmap( false ) { AddToCollection(); Load( bmp, hasAlphaChannel, convertMagicPink, extraFlags ); }
    
    // Same as above but the parameters are passed by using //
    // a logical "or" of the conversion modes listed above the Bitmap class //
    Bitmap( OL_MEMORY_IMG *bmp, int conversionMode, int extraFlags = 0 )              : bmp( 0 ), collisionPoly( 0 ), destroyBmp( true ), pendingLoad( 0 ), pivot( Vec2D( 0.0, 0.0 )), useDefaultPivot( true ), isSubBitmap( false ) { AddToCollection(); Load( bmp, conversionMode, extraFlags ); }
    
    
    // Construct a Bitmap with the specified width and height //
    // The Bitmap will be filled with some random garbage //
    Bitmap( int width, int height );
    
    // Same as above but the Bitmap will be filled with the specified color //
    Bitmap( int width, int height, Rgba fillColor );
    
    // Construct a sub-bitmap of the given Bitmap //
    Bitmap( const Bitmap &other, float x, float y, float width, float height );
    
    // Construct a sub-bitmap of the given Bitmap //
    Bitmap( const Bitmap &other, const Rect &area );
    
    // Same as above but for each pixel in the Bitmap //
    // the color will be retrieved by calling the function: //
    // Rgba operator()(int x, int y)
    // of the passed functor object //
    template< class Functor >
    Bitmap( int width, int height, Functor functor )                                   : bmp( 0 ), collisionPoly( 0 ), destroyBmp( true ), pendingLoad( 0 ), pivot( Vec2D( 0.0, 0.0 )), useDefaultPivot( true ), isSubBitmap( false ) { AddToCollection(); Load( width, height, functor ); }
    
    
    // ADVANCED: Constructs the Bitmap from the specified pixel data //
    // The width and height are the width and height of the actual image //
    // while textureWidth and textureHeight are should be the next powers of two //
    // The pixel data should be an array of 4 * textureWidth * textureHeight elements //
    // which represent the color components of the pixels in groups of four components, //
    // line by line from top to bottom and each line left to right //
    Bitmap( int width, int height, int textureWidth, int textureHeight,
            float *pixelData, bool hasAlphaChannel = true )                            : bmp( 0 ), collisionPoly( 0 ), destroyBmp( false ), pendingLoad( 0 ), pivot( Vec2D( 0.0, 0.0 )), useDefaultPivot( true ), isSubBitmap( false ) { AddToCollection(); Load( width, height, textureWidth, textureHeight, pixelData, hasAlphaChannel ); }
    
    // Create a bitmap by taking a copy of the contents of the screen //
    //Bitmap( int x, int y, int w, int h );
    
    // The destructor automatically frees the allocated memory and unloads the texture //
    // and makes sure that the Bitmap won't be garbage collected //
    virtual ~Bitmap()  { Destroy( true ); }
    
    // Loads the Bitmap from a bitmap file, returns true in a success //
    bool Load( const char *filename, int extraFlags = 0 );
    
    // Loads the Bitmap from separate bitmaps for //
    // the color and the transparency information //
    bool Load( const char *rgbFilename, const char *alphaFilename, int extraFlags = 0 );
    
    // Loads the Bitmap from a Allegro's BITMAP, returns true in a success //
    bool Load( OL_MEMORY_IMG *bmp, bool hasAlphaChannel = false, bool convertMagicPink = false, int extraFlags = 0 );
    
    // Same as above but the parameters are passed by using //
    // a logical "or" of the conversion modes listed above the Bitmap class //
    bool Load( OL_MEMORY_IMG *bmp, int conversionMode, int extraFlags = 0 );
    
    // Same as above but for each pixel in the Bitmap //
    // the color will be retrieved by calling the function: //
    // Rgba operator()(int x, int y)
    // of the passed functor object //
    template< class Functor >
    bool Load( int width, int height, Functor functor );
    
    // ADVANCED: Loads the Bitmap from the specified pixel data //
    // The width and height are the width and height of the actual image //
    // while textureWidth and textureHeight are should be the next powers of two //
    // The pixel data should be an array of 4 * textureWidth * textureHeight elements //
    // which represent the color components of the pixels in groups of four components, //
    // line by line from top to bottom and each line left to right //
    void Load( int width, int height, int textureWidth, int textureHeight,
               GLfloat *data, GLenum format = GL_RGBA );
    
    // Saves the Bitmap to disk with the specified filename //
    // The type of the file depends on the extension of the filename //
    bool Save( const char *filename );
    
    
    // Draws the bitmap to the screen at the specified top-left coordinates //
    // Opacity is in range 0.0 ... 1.0, where 1.0 is completely opaque //
    // and 0.0 completely transparent //
    // Pass true as horizontallyFlipped to flip the image horizontally //
    void Blit( float x, float y, float opacity = 1.0 ) const;
    
    // Same as the original Blit but uses a RenderMode to modify the output //
    void Blit( float x, float y, const RenderMode &mode, float opacity = 1.0 ) const;
    
    // Selects the default pivot point of the Bitmap //
    // The default is the center //
    void SetDefaultPivot( Vec2D point );
    
    inline void SetDefaultPivot( float x, float y ) {
       SetDefaultPivot( Vec2D( x, y ));
    }
    
    // Returns the default pivot of the Bitmap //
    inline Vec2D GetDefaultPivot() {
      return pivot;
    }
    
    // Draws the bitmap to the screen rotated around the default pivot point, angle is in radians //
    // The defautl pivot point of the bitmap will be positioned at (x, y) in the screen //
    void BlitRotated( float x, float y, float angle,
                      float opacity = 1.0 ) const;
    
    // The passed RenderMode affects the rendering, see the classes RenderMode and TintMode below //
    void BlitRotated( float x, float y, float angle, const RenderMode &mode,
                      float opacity = 1.0 ) const;
    
    // Draws the bitmap rotated around the point (pivotX, pivotY) in the bitmap //
    // such that the pivot point will be positioned at (x, y) in the screen //
    void BlitRotated( float x, float y, float pivotX, float pivotY,
                      float angle, float opacity = 1.0 ) const;
                      
    // The passed RenderMode affects the rendering //
    void BlitRotated( float x, float y, float pivotX, float pivotY, float angle,
                      const RenderMode &mode, float opacity = 1.0 ) const;
    
    // Draws the bitmap stretched to the specified width and height //
    void BlitStretched( float x, float y, float width, float height,
                        float opacity = 1.0 ) const;
                        
    // The passed RenderMode affects the rendering //
    void BlitStretched( float x, float y, float width, float height, const RenderMode &mode,
                        float opacity = 1.0 ) const;
    
    // Draws the bitmap to the screen rotated and stretched //
    void BlitTransformed( float x, float y, float width, float height, float angle,
                          float opacity = 1.0 ) const;
                          
    // The passed RenderMode affects the rendering //
    void BlitTransformed( float x, float y, float width, float height, float angle,
                          const RenderMode &mode, float opacity = 1.0 ) const;
    
    // Draws the bitmap to the screen rotated around the point (pivotX, pivotY) and stretched //
    void BlitTransformed( float x, float y, float width, float height, float pivotX, float pivotY,
                          float angle, float opacity = 1.0 ) const;
                          
    // The passed RenderMode affects the rendering //
    void BlitTransformed( float x, float y, float width, float height, float pivotX, float pivotY,
                          float angle, const RenderMode &mode, float opacity = 1.0 ) const;
    
    // Draws the bitmap distorted such that the given coordinates describe the corner points //
    // of the bitmap in the screen in clockwise order starting from the top-left coordinate. //
    // The bitmap will be stretched to fill the quadrate area inside the corner points //
    void BlitDistorted( float x1, float y1, float x2, float y2, float x3, float y3,
                        float x4, float y4, float opacity = 1.0 ) const;
                        
    // The passed RenderMode affects the rendering //
    void BlitDistorted( float x1, float y1, float x2, float y2, float x3, float y3,
                        float x4, float y4, const RenderMode &mode, float opacity = 1.0 ) const;
    
    
    // Returns the width and height of the bitmap //
    inline int Width() const {
      return textureInfo.imgWidth;
    }
    
    inline int Height() const {
      return textureInfo.imgHeight;
    }
    
    
    // Loads a list of bitmaps stored as filenameBegin01.extension, filenameBegin02.extension etc. //
    // Where numNumbers is the amount of numbers in the filename //
    static std::vector< Bitmap *> LoadListOfBitmaps( std::string filenameBegin, std::string extension,
                                                     unsigned int numNumbers = 2, int extraFlags = 0 );
    
    
    // Fills this Bitmap with a portion of the active rendering surface //
    // The coordinates specify the top-left coordinates of the copied portion in the screen //
    void CopyFromCanvas( int x, int y );
    
    
    // The old name of the CopyFromCanvas-function, used for backwards compability //
    inline void CopyFromScreen( int x, int y ) {
      CopyFromCanvas( x, y );
    }
    
    
    // Returns a memory bitmap copy of the Bitmap //
    OL_MEMORY_IMG *GetMemoryBitmap() const;
    
    // Returns a memory bitmap copy of the specified region of the Bitmap //
    OL_MEMORY_IMG *GetMemoryBitmap( int x, int y, int width, int height ) const;
    
    inline Rgba GetPixelFromMemory( int x, int y ) const { return GlDriver::Get()->GetPixel( bmp, x, y ); }
    
    // Retrurns the color value of the specified pixel as an Rgba color //
    // Make sure that you don't try to read out of the bounds of the bitmap //
    inline Rgba GetPixel( int x, int y ) const                    { float pixels[3]; glReadPixels( x, y, 1, 1, textureInfo.format, GL_FLOAT, pixels ); return Rgba( pixels[0], pixels[1], pixels[2] ); }
    
    // Returns the color value of the specified pixel in a packed 32-bit integer //
    // Make sure that you don't try to read out of the bounds of the bitmap //
    inline int GetPixelPacked( int x, int y ) const               { return GetPixel( x, y ).Packed(); }
    
    
    // Returns true if the bitmap is ready to be used //
    virtual bool IsValid() const;
    
    // The boolean operator returns the same as the IsValid-method //
    inline operator bool() const { return IsValid(); }
    
    
    // Returns the generated collision polygon for the Bitmap //
    // Null if no collision polygon was generated in the constructor //
    Poly *GetCollisionPoly() const;
    
    // You don't usually need to call any of the following functions! //
    
    
    // Creates a collision polygon for the specified memory bitmap //
    static Poly *GetCollisionPolygon( OL_MEMORY_IMG *bitmap, int alphaLimit,
                                      int numSkips, Vec2D rotationPivot = Vec2D( 0.0, 0.0 ));
    
    // Returns true if a loading command is waiting for Setup::SetupScreen to be called //
    virtual bool IsLoadingQueued() const;
    
    // Returns true if the bitmap has an alpha channel //
    bool HasAlphaChannel() const;
    
    // Frees the Bitmap from both the memory and the gfx card //
    virtual void Destroy( bool eraseFromGarbageCollection = false );
    
    
    // Sends the Bitmap to the Gfx card //
    // Works only if Settings::StoreMemoryBitmaps is turned on //
    virtual void SendToGPU();
    
    // Unloads the Bitmap from the Gfx card //
    // Works only if Settings::StoreMemoryBitmaps is turned on //
    void UnloadFromGPU();
    
    // Unloads the Bitmap from the Gfx card but //
    // keeps the image data saved in the Bitmap //
    // so that it can be restored with SendToGPU() //
    virtual void UnloadToMemory();
    
    // Selects the Bitmap to be used in texture mapping //
    inline void Select() const { textureInfo.Select(); }
    
    // Execute the queued loading commands (Called by SetupScreen) //
    virtual OlLoadResult ExecuteQueuedCommands();
    
    // Adds the bitmap to the garbage/auto-loader collection //
    // (Automatically done) //
    virtual void AddToCollection();
    
    
    // LOW LEVEL RENDERING ROUTINES //
    
    // This should be called before calling FastBlit //
    static void StartFastBlitting( float opacity = 1.0 );
    
    // A faster Blit but works only after StartFastBlitting has been called //
    inline void FastBlit( float x, float y ) const {
      Select();
      textureInfo.OutputTexturedQuadXY( x, y );
    }
    
    // This function should be called after calling FastBlit //
    static void FinishFastBlitting();
    
    // Dumps a raw textured quad to the screen //
    void TexturedQuad( float w, float h, float fact ) const;
    
    // Same as above but using a RenderMode //
    void TexturedQuad( float w, float h, float fact, const RenderMode &mode ) const;
    
    inline int TextureWidth() const {
      return textureInfo.texWidth;
    }
    
    inline int TextureHeight() const {
      return textureInfo.texHeight;
    }
    
    void GetReadyToRender( float opacity ) const;
    
    
    friend class GainAlphaMode;
    friend class ClippedMode;
    friend class Canvas;
    friend class TexturedPoly;
    
    OlTextureInfo textureInfo;
protected:
   
    // A faster save function to call if the Bitmap is already selected as Canvas //
    void SaveIfSurface( std::string filename ) const;
    
    void HandleExtraFlags( int flags );
    
    void SelectDefaultPivot( int imageWidth, int imageHeight );
    
    inline float GetTextureEndX() const { return textureInfo.rect.x + textureInfo.rect.w; }
    inline float GetTextureEndY() const { return textureInfo.rect.y + textureInfo.rect.h; }
    
    /*
    inline int GetTextureW() const { return textureInfo.texWidth; }
    inline int GetTextureH() const { return textureInfo.texHeight; }
    */
    
    OL_MEMORY_IMG *bmp;
    Poly *collisionPoly;
    bool destroyBmp;
    PendingBitmapLoad *pendingLoad;
    Vec2D pivot;
    bool useDefaultPivot;
    
    bool isSubBitmap;
    bool isSolid;
    
};


// Template functions //

template< class Functor >
bool Bitmap::
Load( int width, int height, Functor functor ) {
   
   int textureWidth = width;
   int textureHeight = height;
   
   int dataSize = 4 * textureWidth * textureHeight;
   
   GLfloat *data = new GLfloat[dataSize];
   
   int x = 0;
   int y = 0;
   
   GLfloat *iter = data;
   
   for( int y = 0; y < height; y++ ) {
      for( int x = 0; x < width; x++ ) {
         Rgba color = functor( x, y );
         
         *(iter++) = color.r;
         *(iter++) = color.g;
         *(iter++) = color.b;
         *(iter++) = color.a;
      }
   }
   
   Load( width, height, textureWidth, textureHeight, data );
   delete[] data;
   
   return true;
}



}



#endif
