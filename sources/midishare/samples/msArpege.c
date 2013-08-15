/*====================== A MIDI SHARE TOOL  (© GRAME 99-O1) =====================*/
 
#include <stdio.h>
#include <MidiShare.h>


//----------------------------- utilities ---------------------------------

// Read Command Line Arguments

long lopt (char *argv[], char *name, long def) 
{
     int     i;
     for (i=0; argv[i]; i++) if (!strcmp(argv[i], name)) return atoi(argv[i+1]);
     return def;
}


//----------------------------- TArpegiator Definition ---------------------------------

// TArpegiator states

   #define stopped      0
   #define playing      1


// TArpegiator structure

        typedef struct TArpegiator {
                short           ref;                     // MidiShare client reference number
                short           state;                   // state stopped or playing
                long            phase;                   // current position in the keyList
                long            speed;                   // scanning speed of the keyList (in ms)
                MidiEvPtr       keyList;                 // list of the keys currently pressed
                MidiEvPtr       task;                    // real time task for scanning the keyList
        } TArpegiator;


// TArpegiator functions

        // main Functions
        void     OpenArp (TArpegiator * s);                      // Init and open a MidiShare session
        void     CloseArp (TArpegiator * s);                     // Stop and close the MidiShare session
        void     StopArp (TArpegiator * s);                      // Stop playing
        void     PlayArp (TArpegiator * s);                      // Start playing
        
        // RealTime Behaviour
        void     ThruRcv (short refNum);                         // MidiShare receive alarm when stopped
        void     ArpRcv  (short refNum);                         // MidiShare receive alarm when playing
	// RealTime task to scan and play the keyList
        void     ArpTask (long date, short refNum, TArpegiator * s, long, long);
                  
        
        // Key Managing
        void            AddKey (TArpegiator * s, MidiEvPtr key);        // add a key to the keyList
        void            RemKey (TArpegiator * s, MidiEvPtr key);        // remove a key off the keyList 
        MidiEvPtr       GetNextKey (TArpegiator * s);                   // get the next key to play
        void            FreeKeys (TArpegiator * s);                     // free all the key of the keyList



//----------------------------- TArpegiator Implementation ---------------------------------


// TArpegiator Open and Close

        void OpenArp (TArpegiator * s)
        {
                s->ref = MidiOpen ("msArpege");                     	// open the MidiShare session
                
                s->state = stopped;                                     // init the other fields
                s->phase = 0;
                s->speed = 250;
                s->task = 0;
                s->keyList = 0;
                
                MidiSetInfo (s->ref, s);                                // save a pointer to itself for future uses
                MidiSetRcvAlarm (s->ref, ThruRcv);                      // install the Thru receive alarm
                
                MidiConnect (s->ref, 0, 1);                             // connect to physical midi outputs
                MidiConnect (0, s->ref, 1);                             // connect from physical midi inputs
        }
        
        void CloseArp (TArpegiator * s)
        {
                StopArp (s);                                             // stop all activities
                MidiClose (s->ref);                                      // close the MidiShare session
        }


// TArpegiator Stop and Play

        void StopArp (TArpegiator * s)
        {
                if (s->state == playing) {                                      
                        printf ("Stop\n");
                        MidiSetRcvAlarm (s->ref, ThruRcv);              // install the Thru receive alarm
                        MidiForgetTask (&s->task);                      // forget the current playing task
                        FreeKeys (s);                                   // free all the keys of the keyList
                        s->state = stopped;
                }
        }
        
        void PlayArp (TArpegiator * s)
        {
                StopArp (s);                                            // just in case
                printf ("Play\n");
                s->state = playing;
                s->phase = 0;                                           // scan start from the first key
                MidiSetRcvAlarm (s->ref, ArpRcv);                       // install the arpegiator receive alarm
                ArpTask (MidiGetTime(), s->ref, s, 0, 0);       	// start scanning and playing now
        }


// TArpegiator Real Time Behaviour

         void ThruRcv (short ref)
        {
                MidiEvPtr e;

                while (e = MidiGetEv (ref))                             // read received events
                        MidiSend (ref, e);                              // and send
        }
                
         void ArpRcv (short ref)
        {
                MidiEvPtr e;
                TArpegiator * s;
                
                s = (TArpegiator *) MidiGetInfo (ref);          // get the previously saved pointer to itself
                while (e = MidiGetEv (ref)) {                   // read received events
                        if (EvType(e) == typeKeyOn && Vel(e))
                                AddKey (s, e);                               // keyOns are added to the keyList
                        else if (EvType(e) == typeKeyOn && ! Vel(e) 
                                        || EvType(e) == typeKeyOff)             
                                RemKey (s, e);                               // keyOffs are removed off the keyList
                        else
                                MidiSend (ref, e);                           // all other events are simply re-sent                                          
                }
        }
        
         void   ArpTask (long date, short ref, TArpegiator * s, long unused1, long unused2)
        {
                MidiEvPtr       e;
				  
                if (e = MidiCopyEv (GetNextKey (s))){           // make a copy of the next key to play
                        EvType(e) = typeNote;                   // transform the copy in a note with duration
                        Dur(e) = s->speed;                      // set the duration to the current speed
                        MidiSendIm (ref, e);                    // send the note now
                }
                s->task = MidiTask ((TaskPtr)ArpTask, date + s->speed, ref, (long)s, 0, 0);
                                                                                                        // re-schedule the task for the next time
        }


// TArpegiator Key Managing

        void AddKey (TArpegiator * s, MidiEvPtr key)    // insert a new key in the keyList in 
        {                                               // ascending order of pitches
                MidiEvPtr       cur, nxt;
                
                if ((cur = s->keyList) && Pitch(cur) < Pitch(key)) {
                        for (nxt = Link(cur); nxt && Pitch(nxt) < Pitch(key); cur = nxt, nxt = Link(cur));
                        Link(key) = nxt;
                        Link(cur) = key;
                } else {
                        s->keyList = key;
                        Link(key) = cur;
                }
        }
                        
        void RemKey (TArpegiator * s, MidiEvPtr key)    // remove a key from the key List
        {
                MidiEvPtr       cur, nxt;

                if ((cur = s->keyList) && Pitch(cur) != Pitch(key)) {
                        for (nxt = Link(cur); nxt && Pitch(nxt) != Pitch(key); cur = nxt, nxt = Link(cur));
                        if (nxt) {
                                Link(cur) = Link(nxt);
                                MidiFreeEv (nxt);
                        }
                } else if (cur) {
                        s->keyList = Link(cur);
                        MidiFreeEv (cur);
                }
                MidiFreeEv (key);
        }
        
        MidiEvPtr GetNextKey (TArpegiator * s)                  // get the next key to be played using the 
        {                                                       // phase as an index in the keyList
                MidiEvPtr       key;
                long            i;
        
                for (i = s->phase, key = s->keyList; key && i; key = Link(key), i--);
                if (key) {
                        s->phase++;
                } else {
                        key = s->keyList;
                        s->phase = 1;
                }
                return key;
        }

        void FreeKeys (TArpegiator * s)                         // free all the keys in the keyList
        {
                MidiEvPtr       cur, nxt;
                
                for (nxt = s->keyList; cur = nxt;) {
                        nxt = Link(cur);
                        MidiFreeEv (cur);
                }
                s->keyList = 0;
         }


//-------------------------------- main ------------------------------------

        int main( int argc, char *argv[])
        {
                char            c;
                TArpegiator     myArpegiator;
                
                OpenArp (&myArpegiator);
                myArpegiator.speed = lopt (argv, "-speed", 250);
				
			    while ((c = getchar()) && c != 'q' && c != 'Q')  switch (c) {
        
                        case 'P': case 'p': PlayArp(&myArpegiator); break;
                        case 'S': case 's': StopArp(&myArpegiator); break;
                }
			   
                CloseArp (&myArpegiator);
        }

