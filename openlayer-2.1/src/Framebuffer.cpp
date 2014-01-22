#include "Framebuffer.hpp"
#include "Internal.hpp"
#include "GlDriver.hpp"
#include "Setup.hpp"
#include "Canvas.hpp"
#include "Blenders.hpp"


using namespace ol;
using namespace std;


// STATIC CLASS VARIABLES //

std::vector< FrameBuffer *> FrameBuffer::possibleFramebufs;
FrameBuffer *FrameBuffer::frameBuffer;



// GENERAL FUNCTIONS //

FrameBuffer::
~FrameBuffer() {}



OlFramebufferObjExt::
~OlFramebufferObjExt() {
   OlFramebufferObjExt::DestroySurfaces();
}


FrameBuffer &FrameBuffer::
GetInstance() {
   if( !frameBuffer ) {
      InitFramebuf();
   }

   return *frameBuffer;
}



void FrameBuffer::
Register( FrameBuffer *buffer ) {
   if( !buffer ) {
      OlError( "Tried to register a null FrameBuffer!" );
      return;
   }

   possibleFramebufs.push_back( buffer );
}


void FrameBuffer::
InitFramebuf() {
   //Register( new OlFramebufferObjExt );
   Register( new BackbufFramebuf );

   for( vector< FrameBuffer *> ::iterator iter = possibleFramebufs.begin();
        iter != possibleFramebufs.end(); iter++ ) {
      if((*iter)->Initialize() ) {
         frameBuffer = *iter;
         OlLog( string( "Used framebuffer: " ) + frameBuffer->GetName() );
         break;
      }
   }

   if( !frameBuffer ) {
      OlError( "Couldn't retrieve a possible framebuffer!" );
   }
}



void BackbufFramebuf::
ReadPixels( int x, int y, int width, int height, GLenum textureFormat, unsigned char *pixelData ) {
   glReadPixels( x, Setup::GetWindowHeight() - ( y + Canvas::Height() ), width, height, textureFormat,
                 GL_UNSIGNED_BYTE, pixelData );
}



void OlFramebufferObjExt::
ReadPixels( int x, int y, int width, int height, GLenum textureFormat, unsigned char *pixelData ) {
   glReadPixels( x, height - y, width, height, textureFormat, GL_UNSIGNED_BYTE, pixelData );
}



void BackbufFramebuf::
CopyTexSubImage( int x, int y, int width, int height, int yOffset ) {
   int readY = Setup::GetWindowHeight() - ( y + height );

   printf( "%d %d, %d\n", y, height, readY );
   glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, yOffset, x, readY, width, height );
}



void OlFramebufferObjExt::
CopyTexSubImage( int x, int y, int width, int height, int yOffset ) {
   printf( "%d %d, %d\n", y, height, Canvas::Height() - (y + height));
   glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, yOffset, x,
                        Canvas::Height() - (y + height), width, height );
}



const char *BackbufFramebuf::
GetName() {
	return "Framebuffer emulation";
}



bool BackbufFramebuf::
Initialize() {
	return true;
}



void BackbufFramebuf::
BindToTexture( const OlTextureInfo &texture ) {
   boundTexture = &texture;
   //Canvas::SetPixelWriteMode( COLOR_AND_ALPHA );

   glDrawBuffer( GL_BACK );

   //glViewport( 0, 0, texture.texWidth, texture.texHeight );

   glPushMatrix();
   /*
   glScalef( float(Setup::GetWindowWidth()) / texture.texWidth,
             float(Setup::GetWindowHeight()) / texture.texHeight, 1.0);
   */

   Blenders::Push();
   Blenders::Set( COPY_BLENDER );

   //glDrawBuffer( GL_FRONT );

   boundTexture->GetReadyToRender();
   boundTexture->OutputTexturedQuadXY( 0, 1 );//SpecialWorkaround(); // TODO

   Blenders::Pop();

   //glDrawBuffer( GL_BACK );
   //readkey();
   /*
   glFlush();
   glFinish();
   allegro_gl_flip();
   readkey();
   allegro_gl_flip();
   */
   //allegro_gl_flip();
   //readkey();
}


void BackbufFramebuf::
DestroySurfaces() {
   boundTexture = 0;
}



void BackbufFramebuf::
Release() {
   glViewport( 0, 0, Setup::GetWindowWidth(), Setup::GetWindowHeight() );
   glPopMatrix();

   boundTexture = 0;
   Canvas::DisableClipping();
}



void BackbufFramebuf::
Destroy( OlTextureInfo &texture ) {
   if( boundTexture->index == texture.index )
      Release();
}



void BackbufFramebuf::
RefreshSurface() {
   if( !boundTexture ) {
      OlError( "No bound texture for BackbufFramebuf!" );
      return;
   }

   GLint buffer = 0;
   glGetIntegerv( GL_READ_BUFFER, &buffer );

   glReadBuffer( GL_BACK );

   glFlush();
   glFinish();

   glBindTexture( GL_TEXTURE_2D, boundTexture->index );

   glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, boundTexture->texHeight - boundTexture->imgHeight,
                        0, Setup::GetWindowHeight() - boundTexture->imgHeight-1,
                        boundTexture->imgWidth, boundTexture->imgHeight );

   glReadBuffer( buffer );
}



const char *OlFramebufferObjExt::
GetName() {
	return "GL_EXT_framebuffer_object";
}



bool OlFramebufferObjExt::
Initialize() {
   const char *extName = "GL_EXT_framebuffer_object";

   #ifndef GL_EXT_framebuffer_object

      OlLog( string( "OpenGL extension " ) + extName + " wasn't found in compile time." );
      return false;

   #else // GL_EXT_framebuffer_object
      if( !GlDriver::IsExtensionAlvailable( extName )) {
         return false;
      }

      #ifdef _WIN32

         glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC)
            wglGetProcAddress( "glGenFramebuffersEXT" );

         glFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)
            wglGetProcAddress( "glFramebufferTexture2DEXT" );

         glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC)
            wglGetProcAddress( "glBindFramebufferEXT" );

         glDeleteRenderbuffersEXT = (PFNGLDELETERENDERBUFFERSEXTPROC)
            wglGetProcAddress( "glDeleteRenderbuffersEXT" );

         if( !glGenFramebuffersEXT || !glFramebufferTexture2DEXT
             || !glBindFramebufferEXT || !glDeleteRenderbuffersEXT ) {
            OlError( "GL_EXT_framebuffer_object functions couldn't be loaded!" );
         }

         return true;
      #endif // _WIN32

   #endif // GL_EXT_framebuffer_object

	 //Default
   return false;
}



void OlFramebufferObjExt::

BindToTexture( const OlTextureInfo &texture ) {

   #ifdef GL_EXT_framebuffer_object
      std::map< GLuint, GLuint > ::iterator iter = bufferMap.find( texture.index );
      GLuint framebufferID = 0;

      if( iter != bufferMap.end() ) {
         framebufferID = iter->second;
         glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, framebufferID );
      }

      else {
        glGenFramebuffersEXT( 1, &framebufferID );
        glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, framebufferID );
        glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                                GL_TEXTURE_2D, texture.index, 0 );

        bufferMap[texture.index] = framebufferID;
      }
   #endif

   glViewport( 0, 0, texture.texWidth, texture.texHeight );
   glPushMatrix();
   glScalef( float(Setup::GetWindowWidth()) / texture.texWidth,
             float(Setup::GetWindowHeight()) / texture.texHeight, 1.0);
   boundTexture = &texture;

}


void OlFramebufferObjExt::
Release() {
   glViewport( 0, 0, Setup::GetWindowWidth(), Setup::GetWindowHeight() );
   glPopMatrix();

   #ifdef GL_EXT_framebuffer_object
      glFlush();
      glFinish();
      glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
   #endif

   boundTexture = 0;
}


void OlFramebufferObjExt::
Destroy( OlTextureInfo &texture ) {
   #ifdef GL_EXT_framebuffer_object
      std::map< GLuint, GLuint > ::iterator iter = bufferMap.find( texture.index );
      if( iter != bufferMap.end() ) {
         GLuint framebufferID = iter->second;
         glDeleteRenderbuffersEXT( 1, &framebufferID );
         OlLog( "Framebuffer destroyed: " + ToString( framebufferID ));
      }
      else {
         OlError( "No framebuffer exists for the specified texture ID!" );
      }
   #endif

}

void OlFramebufferObjExt::
DestroySurfaces() {
   #ifdef GL_EXT_framebuffer_object
      for( std::map< GLuint, GLuint > ::iterator iter = bufferMap.begin(); iter != bufferMap.end(); iter++ ) {
         GLuint framebufferID = iter->second;
         glDeleteRenderbuffersEXT( 1, &framebufferID );
      }
   #endif

}

void OlFramebufferObjExt::
RefreshSurface() {

   glFlush();
   glFinish();
}



void FrameBuffer::
DestroyFramebuffers() {
   for( std::vector< FrameBuffer *> ::iterator iter = possibleFramebufs.begin(); iter != possibleFramebufs.end(); iter++ ) {
      delete *iter;
   }

   possibleFramebufs.clear();
   frameBuffer = 0;
}


