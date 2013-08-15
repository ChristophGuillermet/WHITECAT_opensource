/*====================== A MIDI SHARE TOOL  (© GRAME 99-01) =====================

NAME
      msSimpleSeq -- a simple MidiShare tool

SYNOPSIS
      msSimpleSeq

DESCRIPTION
        "msSimpleSeq" open a MidiShare session, wait for user commands to record and
        play sequences, and then close the MidiShare session

===============================================================================*/
 
#include <stdio.h>
#include <MidiShare.h>


//----------------------------- utilities ---------------------------------

long lopt (char *argv[], char *name, long def) 
{
        int     i;
        for (i=0; argv[i]; i++) if (!strcmp(argv[i], name)) return atoi(argv[i+1]);
        return def;
}

int flag (char *argv[], char *name) 
{
        int     i;
        for (i=0; argv[i]; i++) if (!strcmp(argv[i], name)) return 1;
        return 0;
}

void Wait(long d) 
{       d += MidiGetTime();
        while (MidiGetTime() < d) {}
}



//----------------------------- TSequencer ---------------------------------
// definition of a TSequencer

#define idle    	0
#define playing 	1
#define recording 	2

typedef struct TSequencer {
        short           ref;                                         // MidiShare client reference number
        short           state;                                       // state idle, playing or recording
        MidiSeqPtr      seq;                                         // the sequence of MidiShare events
        MidiEvPtr       task;                                        // a pointer to the real time play task
} TSequencer;

void OpenSeq(TSequencer * s);                                        // Init and open a MidiShare session
void CloseSeq(TSequencer * s);                                       // Stop and close the MidiShare session
void StopSeq(TSequencer * s);                                        // Stop recording or playing
void PlaySeq(TSequencer * s);                                        // Start playing
void RecSeq(TSequencer * s);                                         // Start recording

void NullRcv (short refNum);                                         // MidiShare receive alarm when idle or playing
void RecRcv (short refNum);                                          // MidiShare receive alarm when recording
void PlayTask (long date, short refNum, TSequencer * s, MidiEvPtr e, long);  // RealTime task to play the sequence

// implementation of a TSequencer

void OpenSeq(TSequencer * s)
{
        s->ref = MidiOpen("msSequencer");                                       // open the MidiShare session
        
        s->state = idle;                                                        // first state is idle
        s->seq = MidiNewSeq();                                                  // create an empty sequence
        s->task = 0;                                                            // no task at the momment
        
        MidiSetInfo(s->ref, s);                                                 // save a pointer to itself for future uses
        MidiSetRcvAlarm(s->ref, NullRcv);                                       // install a receive alarm to delete incomming events
        
        MidiConnect(s->ref, 0, 1);                                              // connect to physical midi outputs
        MidiConnect(0, s->ref, 1);                                              // connect from physical midi inputs
}

void CloseSeq(TSequencer * s)                           
{
        StopSeq(s);                                                              // stop current activities
        MidiFreeSeq(s->seq);                                                     // free the sequence
        MidiClose(s->ref);                                                       // close the session
}

void StopSeq(TSequencer * s)
{
        printf("Stop\n");
        switch (s->state) {
                case playing :                                                    // stop playing
                        MidiForgetTask(&s->task);                                 // forget the current play task
                        s->state = idle;
                        break;
                case recording :                                                  // stop recording
                        MidiSetRcvAlarm(s->ref, NullRcv);                         // install the Null receive alarm
                        s->state = idle;
                        break;
        }
}

void PlaySeq(TSequencer * s)
{
        MidiEvPtr e;
        
        StopSeq(s);                                                     // stop current activities
        printf("Play\n");
        if (e = First(s->seq)) {                                        // if sequence not empty
                s->state = playing;                                             
                PlayTask(MidiGetTime(), s->ref, s, e, 0);               // call the playTask
        }
}

void RecSeq(TSequencer * s)
{
        StopSeq(s);                                                      // stop current activities
        printf("Record\n");
        s->state = recording;
        MidiClearSeq(s->seq);                                            // clear the sequence
        MidiSetRcvAlarm(s->ref, RecRcv);                                 // install the recording receive alarm
}
        
 void NullRcv (short ref)
{
        MidiEvPtr e;
        while (e = MidiGetEv(ref)) MidiFreeEv(e);                       // delete all the received events
}

 void RecRcv (short ref)
{
        TSequencer * s;
        MidiEvPtr e;
        
        s = (TSequencer *) MidiGetInfo(ref);                            // get a pointer to itself
        while (e = MidiGetEv(ref)) {
                MidiSendIm(ref, MidiCopyEv(e));
                MidiAddSeq(s->seq, e);  // add to the sequence all the received events
        }
}

 void     PlayTask (long date, short r, TSequencer * s, MidiEvPtr e, long d)
{
        d = Date(e);                                                            // remember the date of the current event to play
        while (e && (Date(e) == d)) {                                           // for all the events at this date
                MidiSendIm(r, MidiCopyEv(e));                                   // send a copy
                e = Link(e);                                                                    // go to next event
        }
        if (e) {                                                                                        // if not the end, schedule the task for next event
                s->task = MidiTask((TaskPtr)PlayTask, date+Date(e)-d, r, (long)s, (long)e, 0);
        } else {
                s->task = 0;                                                                    // set the task pointer to NIL
                s->state = idle;                                                                // change the state to idle
        }
}
        

//-------------------------------- main ------------------------------------

int main( int argc ,char *argv[])
{
        char            c;
        TSequencer      mySequencer;
        
        OpenSeq(&mySequencer);
	
	printf ( "\nmsSequencer] \n\n");
	printf ( "msSequencer record and play a MidiShare sequence\n\n");
	printf ( "Use r or R to start recording the sequence\n");
	printf ( "Use p or P to start playing the sequence\n");
	printf ( "Use s or S to stop recording (or playing) the sequence\n");
	printf ( "Use q or Q to quit the application\n\n");
         
        while ((c = getchar()) && c != 'q' && c != 'Q')  switch (c) {

                case 'P': case 'p': PlaySeq(&mySequencer); break;
                case 'R': case 'r': RecSeq(&mySequencer); break;
                case 'S': case 's': StopSeq(&mySequencer); break;
        }
        
        CloseSeq(&mySequencer);
}

