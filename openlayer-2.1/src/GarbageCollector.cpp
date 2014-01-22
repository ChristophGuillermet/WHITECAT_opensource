#include "GarbageCollector.hpp"
#include "Internal.hpp"
#include "General.hpp"
#include "Framebuffer.hpp"

using namespace ol;


static GarbageCollection *gbCollection;


GarbageCollection &OlGetCollection() {
   if( !gbCollection ) {
      gbCollection = new GarbageCollection;
   }
   
   return *gbCollection;
}


void OlCollectTheGarbage() {
   OlLog( "Collecting garbage" );
   OlGetCollection().DestroyGarbage();
   OlLog( "Destroying the framebuffer" );
   FrameBuffer::GetInstance().DestroySurfaces();
   FrameBuffer::DestroyFramebuffers();
   OlLog( "OpenLayer shutdown finished" );
}




void GarbageCollection::
ExecuteQueues() {
   std::list< GarbageCollected *> pendings = collection;
   
   OlLog( std::string( "\nAuto Loader: Executing " ) + ToString( pendings.size() ) + " pending loading commands" );
   
   while( !pendings.empty()) {
      
      unsigned int numPendings = pendings.size();
      
      for( std::list< GarbageCollected *> ::iterator iter = pendings.begin(); iter != pendings.end(); ) {
         OlLoadResult result = (*iter)->ExecuteQueuedCommands();
         
         bool removeItem = true;
         
         switch( result ) {
            case OL_LR_FAILURE:
               OlError( "Auto Loader: Couldn't load a resource!" );
              break;
            case OL_LR_SUCCESS:
              break;
            case OL_LR_PENDING:
               removeItem = false;
              break;
            default:
               OlError( "Auto Loader: Unknown OlLoadResult enumeration!" );
         }
         
         if( removeItem )
            iter = pendings.erase( iter );
         else
            iter++;
      }
      
      if( pendings.size() >= numPendings ) {
         OlError( "Auto Loader: Circular dependancy detected! Bailing out!" );
         break;
      }
   }
   
   OlLog( "Auto Loader: Completed\n" );
}



void GarbageCollection::
DestroyGarbage() {
   OlLog( std::string( "\nGarbage Collector: Destroying " ) + ToString( collection.size() ) + " items" );
   
   for( std::list< GarbageCollected *> ::iterator iter = collection.begin(); iter != collection.end(); iter++ ) {
      (*iter)->Destroy();
      
      // If the auto-delete feature is used the object is deleted //
      if((*iter)->AutoDeleteUsed()) {
         delete *iter;
      }
   }
   
   collection.clear();
   
   OlLog( "Garbace Collector: Completed\n" );
}



void GarbageCollection::
UnloadAllToMemory() {
   OlLog( std::string( "\nSuspend: Unloading " ) + ToString( collection.size() ) + " items" );
   
   for( std::list< GarbageCollected *> ::iterator iter = collection.begin(); iter != collection.end(); iter++ )
      (*iter)->UnloadToMemory();
}



void GarbageCollection::
LoadAllToGPU() {
   OlLog( std::string( "\nAwake: Loading " ) + ToString( collection.size() ) + " items" );
   
   for( std::list< GarbageCollected *> ::iterator iter = collection.begin(); iter != collection.end(); iter++ )
      (*iter)->SendToGPU();
}



void GarbageCollection::
Add( GarbageCollected *item ) {
   collection.push_back( item );
}



void GarbageCollection::
Remove( GarbageCollected *item ) {
   collection.remove( item );
}

