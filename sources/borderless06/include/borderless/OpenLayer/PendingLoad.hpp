#ifndef PENDING_LOADING_HPP
#define PENDING_LOADING_HPP


#include "Includes.hpp"
#include "GarbageCollector.hpp"
#include "TextureInfo.hpp"
#include "Declspec.hpp"


namespace ol {


class Bitmap;


class OL_LIB_DECLSPEC PendingBitmapLoad {
public:
	virtual ~PendingBitmapLoad(){}
   virtual OlLoadResult ExecuteLoading( Bitmap &bmp ) = 0;
};



class OL_LIB_DECLSPEC PendingFileLoad : public PendingBitmapLoad {
public:
   PendingFileLoad( const char *filename )
      : filename( filename ) {}
   virtual ~PendingFileLoad(){}
   virtual OlLoadResult ExecuteLoading( Bitmap &bmp );

private:
   const char *filename;
};



class OL_LIB_DECLSPEC PendingFileAlphaLoad : public PendingBitmapLoad {
public:
   PendingFileAlphaLoad( const char *filename, const char *alphaFilename )
      : filename( filename ), alphaFilename( alphaFilename ) {}
   virtual ~PendingFileAlphaLoad(){}
   virtual OlLoadResult ExecuteLoading( Bitmap &bmp );

private:
   const char *filename;
   const char *alphaFilename;
};



class OL_LIB_DECLSPEC PendingDataLoad : public PendingBitmapLoad {
public:
   PendingDataLoad( float *data, OlTextureInfo textureInfo )
      : data( data ), textureInfo( textureInfo ) {}
   virtual ~PendingDataLoad(){}
   virtual OlLoadResult ExecuteLoading( Bitmap &bmp );

private:
   float *data;
   OlTextureInfo textureInfo;
};



template< class Functor >
class OL_LIB_DECLSPEC PendingFunctorLoad : public PendingBitmapLoad {
public:
   PendingFunctorLoad( Functor functor, int width, int height )
      : functor( functor ), width( width ), height( height ) {}
   virtual ~PendingFunctorLoad(){}
   virtual OlLoadResult ExecuteLoading( Bitmap &bmp );

private:
   Functor functor;
   int width, height;
};



}



#endif // PENDING_LOADING_HPP
