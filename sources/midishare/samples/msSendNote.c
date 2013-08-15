/*====================== A MIDI SHARE TOOL  (© GRAME 99-01) =====================

NAME
      msSendNote -- a very simple MidiShare tool


SYNOPSIS
      msSendNote [-pitch <k>] [-vel <v>] [-dur <d>] [-port <p>] [-chan <c>]

DESCRIPTION
        "msSendNote" open a MidiShare session, create and send a single note, and 
        then close the MidiShare session

===============================================================================*/
 
#include <stdio.h>
#include <MidiShare.h>


#define nil     0
#define true    1
#define false   0


//_______________________________________________________________________
long lopt (char *argv[], char *name, long def)
{
        int     i;
        for (i=0; argv[i]; i++) if (!strcmp(argv[i], name)) return atoi(argv[i+1]);
        return def;
}


//_______________________________________________________________________

void wait(long d)
{       d += MidiGetTime();
        while (MidiGetTime() < d);
}


//_______________________________________________________________________

int main( int argc, char *argv[])
{
        short           ref;
        MidiEvPtr       e;
        
        ref = MidiOpen("msSendNote");                           // open a MidiShare session
        MidiConnect(ref, 0, true);                            	// connect to physical Midi outputs
        
        if (e = MidiNewEv(typeNote)) {                       	// allocate a note event
        
                Pitch(e) = lopt(argv, "-pitch", 60);    	// fill its fields
                Vel(e) = lopt(argv, "-vel", 80);
                Dur(e) = lopt(argv, "-dur", 250);
                Chan(e) = lopt(argv, "-chan", 0);
                Port(e) = lopt(argv, "-port", 0);
			   
                MidiSendIm(ref, e);                             // send it now
                wait(lopt(argv, "-dur", 250));                  // wait the duration of the note
        }
        MidiClose (ref);                                        // close the MidiShare session
}

