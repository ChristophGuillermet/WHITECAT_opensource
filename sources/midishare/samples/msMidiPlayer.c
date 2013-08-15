/*====================== A MIDI SHARE TOOL  (© GRAME 99-01) =====================

NAME
      msMidiPlay -- a small MidiShare tool

SYNOPSIS
      msMidiPlay <MidiFile file name>
  
DESCRIPTION
      "msMidiPlay" open a MidiShare session, read a MidiFile, convert its tempo map
      to ms, wait for user commands to play the sequence, and then close the session.

===============================================================================*/
 
#include <stdio.h>

#include <MidiShare.h>
#include "midifile.h"

#define MSALARMAPI

//------------------------ merge utilities ----------------------------

static MidiEvPtr AddSeq( register MidiEvPtr e1, register MidiEvPtr e2)
{
        register MidiEvPtr next;
        
        while( next= Link(e1))                          /* tant qu'une sequence n'est finie */
        {
                if( Date(next) <= Date(e2))             /* date inferieure dans la meme seq */
                        e1= next;                       /* rien a faire : on continue           */
                else                                    /* sinon                                                        */
                {
                        Link( e1)= e2;                  /* on linke avec l'autre sequence       */
                        e1= e2;                         /* et on les inverse                    */
                        e2= next;
                }
        }
        if( Link(e1)= e2)                       /* linke avec la fin de l'autre s?quence    */
                while( Link(e2))
                        e2= Link(e2);
        return e2;                              /* et renvoie le dernier evt de la s?quence */
}

void MixSeq( MidiSeqPtr src, MidiSeqPtr dest)
{
        register MidiEvPtr firstSrc, firstDest;
        
        if( dest && src)                                        /* dest et src existent         */
        {
                if( firstSrc= src->first)                       /* src non vide                 */
                {
                        if( !(firstDest= dest->first))          /* si destination est vide      */
                        {
                                dest->first= firstSrc;                  /* recopie du premier et        */
                                dest->last= src->last;                  /* dernier evt de src           */
                        }
                        else if( Date(firstSrc) < Date(firstDest))
                                                                        /* 1ier evt source pr?c?de      */
                        {                                               /* le 1ier evt destination      */
                                dest->first= firstSrc;                  /* range dans destination       */
                                dest->last= AddSeq( firstSrc, firstDest);       /* et chainage  */
                        }
                        else dest->last= AddSeq( firstDest, firstSrc);  /* et chainage  */
                }
                MidiFreeCell((MidiEvPtr) src);
        }
}



//------------------------ tempo map utilities ----------------------------

void TrsfTempoSeq(MidiSeqPtr s, double ticks) 
{
        double  tt;     // dur?e d'un ticks (micro sec)
        double  t1;     // temps du dernier changement de tempo (micro sec)
        double  t2;     // temps actuel (micro sec)
        long    d1;     // date du dernier changement de tempo (ticks)
        
        MidiEvPtr       e;
        
        t1 = 0.0;
        tt = 500000.0 / ticks;  
        d1 = 0;
        e = First(s);
        
        while (e) {
                t2 = t1 + tt * (Date(e) - d1);
                if (EvType(e) == typeTempo) {
                        tt = MidiGetField(e, 0) / ticks;
                        d1 = Date(e);
                        t1 = t2;
                }
                Date(e) = (long) (t2 / 1000.0);
                e = Link(e);
        }
}

void TrsfSmpteSeq(MidiSeqPtr s, long fps, long tpf)
{
        MidiEvPtr       e;
        long            tps;
        
        tps = fps * tpf;
        e = First(s);
        
        while (e) {
                Date(e) = Date(e) * 1000 / tps;
                e = Link(e);
        }
}

long LenSeq(MidiSeqPtr s) 
{
        long            c; 
        MidiEvPtr       e; 
        
        for (c=0, e=First(s); e; c++, e=Link(e));
        return c;
}

long DurSeq(MidiSeqPtr s) 
{
        long            dur = 0; 
        MidiEvPtr       e = Last(s);
		
		if (e) {
			dur = Date(e);
			if (EvType(e) == typeNote) dur+=Dur(e);
        }
        return dur;
}


//----------------------------- TSequencer ---------------------------------
// definition of a TSequencer

#define idle 0
#define playing 1
#define recording 2

typedef struct TSequencer {
        short           ref;
        short           state;
        MidiSeqPtr      seq;
        MidiEvPtr       playtask;
	MidiEvPtr       looptask;
} TSequencer;

void OpenSeq(TSequencer * s, char * n);
void CloseSeq(TSequencer * s);
void StopSeq(TSequencer * s);
void PlaySeq(TSequencer * s);
void RecSeq(TSequencer * s);

MSALARMAPI void     NullRcv (short refNum);
MSALARMAPI void     RecRcv (short refNum);
MSALARMAPI void     PlayTask (long date, short refNum, TSequencer * s, MidiEvPtr e, long);
MSALARMAPI void     LoopTask (long date, short r, TSequencer * s, long dur, long unused);


// implementation of a TSequencer

void OpenSeq(TSequencer * s, char * n)
{
        midiFILE *      f;
        int             i;
        
 // initialisation
 
 	s->ref = MidiOpen("msMidiPlayer");
        s->state = idle;
        s->seq = MidiNewSeq();
        s->playtask = 0;
	s->looptask = 0;
		
        
 // read the midi file
        
        f = MidiFileOpen(n, MidiFileRead);
		
        if (!f) {
                printf ("Error opening MidiFile %s (%d)\n", n, MidiFile_errno);
                CloseSeq(s);
                exit(MidiFile_errno);
        }
        for (i = f->ntrks; i; i--){ MixSeq( MidiFileReadTrack(f), s->seq); }
	           
        if (smpte(f)) 
                TrsfSmpteSeq(s->seq, frame_par_sec(f), ticks_par_frame(f));
        else
                TrsfTempoSeq(s->seq, ticks_par_quarterNote(f));
	
	if (First(s->seq))
	 	printf("File : %s, Format : %d, Tracks : %d, Length : %d, Duration : %d\n",
                        n, f->format, f->ntrks, LenSeq(s->seq), Date(Last(s->seq)) );
	           
        (void) MidiFileClose(f);
       
 // installation
    	
        MidiSetInfo(s->ref, s);
        MidiSetRcvAlarm(s->ref, NullRcv);
        
        MidiConnect(s->ref, 0, 1);
        MidiConnect(0, s->ref, 1);
}

void CloseSeq(TSequencer * s)
{
        StopSeq(s);
        MidiFreeSeq(s->seq);
        MidiClose(s->ref);
}

void StopSeq(TSequencer * s)
{
        printf("Stop\n");
        switch (s->state) {
                case playing :
                        MidiForgetTask(&s->playtask);
			MidiForgetTask(&s->looptask);
                        s->state = idle;
                        break;
                case recording :
                        MidiSetRcvAlarm(s->ref, NullRcv);
                        s->state = idle;
                        break;
        }
}


void PlaySeq(TSequencer * s)
{
        MidiEvPtr e;
        
        StopSeq(s);
        printf("Play\n");
        if (e = First(s->seq)) {
                s->state = playing;
                PlayTask(MidiGetTime(), s->ref, s, e, 0);
        }
}

void PlayLoop(TSequencer * s)
{
	LoopTask(MidiGetTime(), s->ref, s,DurSeq(s->seq)+500, 0);
}

void RecSeq(TSequencer * s)
{
        StopSeq(s);
        printf("Record\n");
        s->state = recording;
        MidiSetRcvAlarm(s->ref, RecRcv);
}
        
MSALARMAPI void NullRcv (short ref)
{
        MidiEvPtr e;
        while (e = MidiGetEv(ref)) MidiFreeEv(e);
}

MSALARMAPI void RecRcv (short ref)
{
        TSequencer * s;
        MidiEvPtr e;
        
        s = (TSequencer *) MidiGetInfo(ref);
        while (e = MidiGetEv(ref)) MidiAddSeq(s->seq, e);
}

MSALARMAPI void     PlayTask (long date, short r, TSequencer * s, MidiEvPtr e, long d)
{
        d = Date(e);
        while (e && (Date(e) == d)) {
                MidiSendAt(r, MidiCopyEv(e), date+10);
                e = Link(e);
        }
        if (e) {
                s->playtask = MidiTask((TaskPtr)PlayTask, date+Date(e)-d, r, (long)s, (long)e, 0);
        } else {
                s->playtask = 0;
                s->state = idle;
        }
}

MSALARMAPI void     LoopTask (long date, short r, TSequencer * s, long dur, long unused)
{
       	PlaySeq(s);
	s->looptask = MidiTask((TaskPtr)LoopTask, date+dur, r, (long)s, dur, 0);
}
        

//-------------------------------- main ------------------------------------

int main( int argc, char *argv[])
{
        char            c;
        TSequencer      mySequencer;
        
	OpenSeq(&mySequencer, argv[1]);
	    
	printf ( "\nmsMidiPlayer <midifile> \n\n");
	printf ( "msMidiPlayer plays a MIDIfile\n\n");
	printf ( "Use p or P to start playing\n");
	printf ( "Use s or S to stop playing\n");
	printf ( "Use l or L to start playing and loop\n");
	printf ( "Use q or Q to quit the application\n\n");
        
        while ((c = getchar()) && c != 'q' && c != 'Q')  switch (c) {
	
	        case 'P': case 'p': PlaySeq(&mySequencer); break;
                case 'S': case 's': StopSeq(&mySequencer); break;
		case 'L': case 'l': PlayLoop(&mySequencer); break;
        }
        
        CloseSeq(&mySequencer);
}

