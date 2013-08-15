#ifndef OL_GARBAGE_COLLECTOR
#define OL_GARBAGE_COLLECTOR


#include <list>
#include "Includes.hpp"
#include "Declspec.hpp"

namespace ol {


// The type of an Auto Loader result, only used by OpenLayer's internals //

enum OlLoadResult {
   OL_LR_SUCCESS,
   OL_LR_PENDING,
   OL_LR_FAILURE
};


// Virtual garbace collectector class used by OpenLayer's routines //
// to automate the loading and destroying of global variables //
// GarbageCollected only defines the interface for garbage collected objects //

class OL_LIB_DECLSPEC GarbageCollected {
public:
   GarbageCollected() : autoDelete( false ) {}
   
   virtual ~GarbageCollected() {}
   
   virtual void Destroy( bool eraseFromGarbageCollection = false ) = 0;
   virtual OlLoadResult ExecuteQueuedCommands() = 0;
   
   virtual void UnloadToMemory() = 0;
   virtual void SendToGPU() = 0;
   
   virtual bool IsValid() const = 0;
   virtual bool IsLoadingQueued() const = 0;
   
   virtual void AddToCollection() = 0;
   
   // Chooses this object to be automatically freed from the memory //
   // when the program quits //
   void UseAutoDelete() {
      autoDelete = true;
   }
   
   // Returns true if the auto delete feature is used for this object //
   bool AutoDeleteUsed() {
      return autoDelete;
   }
private:
   
   bool autoDelete;
};


// Garbage collection which stores all Bitmaps and TextRenderers //
// Automatically loads and destroys global Bitmaps and TextRenderers //

class OL_LIB_DECLSPEC GarbageCollection {
public:
   void Add( GarbageCollected *item );
   void Remove( GarbageCollected *item );
   
   void ExecuteQueues();
   void DestroyGarbage();
   
   void UnloadAllToMemory();
   void LoadAllToGPU();
   
private:
   std::list< GarbageCollected *> collection;
};



}



#endif // OL_GARBAGE_COLLECTOR
