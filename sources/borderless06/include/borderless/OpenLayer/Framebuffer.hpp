#ifndef OL_FRAMEBUFFER_HPP
#define OL_FRAMEBUFFER_HPP
#include "Includes.hpp"
#include "GlDriver.hpp"
#include "Declspec.hpp"
#include <vector>
#include <map>


namespace ol {



class OL_LIB_DECLSPEC FrameBuffer {
public:
   virtual ~FrameBuffer();

   virtual const char *GetName() = 0;
   virtual bool Initialize() = 0;
   virtual void BindToTexture( const OlTextureInfo &texture ) = 0;
   virtual void Release() = 0;
   virtual void Destroy( OlTextureInfo &texture ) = 0;
   virtual void DestroySurfaces() = 0;
   virtual void RefreshSurface() = 0;
   virtual void ReadPixels( int x, int y, int width, int height, GLenum textureFormat, unsigned char *pixelData ) = 0;
   virtual void CopyTexSubImage( int x, int y, int width, int height, int yOffset ) = 0;

   static FrameBuffer &GetInstance();
   static void Register( FrameBuffer *buffer );
   static void DestroyFramebuffers();

private:
   static void InitFramebuf();

   static std::vector< FrameBuffer *> possibleFramebufs;

   static FrameBuffer *frameBuffer;
};



class OL_LIB_DECLSPEC BackbufFramebuf : public FrameBuffer {
public:
   BackbufFramebuf()
      : boundTexture(0) {}
	virtual ~BackbufFramebuf(){}
   virtual const char *GetName();
   virtual bool Initialize();
   virtual void BindToTexture( const OlTextureInfo &texture );
   virtual void Release();
   virtual void Destroy( OlTextureInfo &texture );
   virtual void DestroySurfaces();


   virtual void RefreshSurface();
   virtual void ReadPixels( int x, int y, int width, int height, GLenum textureFormat, unsigned char *pixelData );
   virtual void CopyTexSubImage( int x, int y, int width, int height, int yOffset );

private:
   const OlTextureInfo *boundTexture;
};



class OL_LIB_DECLSPEC OlFramebufferObjExt : public FrameBuffer {
public:
   OlFramebufferObjExt()
      : boundTexture( 0 ) {}

   virtual ~OlFramebufferObjExt();

   virtual const char *GetName();
   virtual bool Initialize();
   virtual void BindToTexture( const OlTextureInfo &texture );
   virtual void Release();
   virtual void Destroy( OlTextureInfo &texture );
   virtual void DestroySurfaces();
   virtual void RefreshSurface();
   virtual void ReadPixels( int x, int y, int width, int height, GLenum textureFormat, unsigned char *pixelData );
   virtual void CopyTexSubImage( int x, int y, int width, int height, int yOffset );

private:

   #ifdef GL_EXT_framebuffer_object
      #ifdef _WIN32
         #ifndef PFNGLGENFRAMEBUFFERSEXTPROC
            typedef void (APIENTRY * PFNGLGENFRAMEBUFFERSEXTPROC)
                                 (GLsizei n, GLuint *renderbuffers);
         #endif

         #ifndef PFNGLFRAMEBUFFERTEXTURE2DEXTPROC
            typedef void (APIENTRY * PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)
                                (GLenum target, GLenum attachment,
                                 GLenum textarget, GLuint texture,
                                 GLenum level);
         #endif

         #ifndef PFNGLBINDFRAMEBUFFEREXTPROC
            typedef void (APIENTRY * PFNGLBINDFRAMEBUFFEREXTPROC)
                                 (GLenum target, GLuint renderbuffer);
         #endif

         #ifndef PFNGLDELETERENDERBUFFERSEXTPROC
            typedef void (APIENTRY * PFNGLDELETERENDERBUFFERSEXTPROC)
                                 (GLsizei n, const GLuint *renderbuffers);
         #endif

         PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT;
         PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT;
         PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT;
         PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffersEXT;
      #endif // _WIN32
   #endif // GL_EXT_framebuffer_object

   //static const int INTERNAL_FORMAT;
   std::map< GLuint, GLuint > bufferMap;
   const OlTextureInfo *boundTexture;
};


}



#endif // OL_FRAMEBUFFER_HPP

