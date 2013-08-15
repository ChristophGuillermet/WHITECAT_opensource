/*====================== A MIDI SHARE TOOL  (© GRAME 99-01) =====================

NAME
      msSendSeq -- a very simple MidiShare tool

SYNOPSIS
      msSendSeq [-pitch <k>] [-vel <v>] [-dur <d>] [-port <p>] [-chan <c>] 

DESCRIPTION
        "msSendSeq" open a MidiShare session, create and play a MidiShare sequence
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

void playSeq(short ref, int pitch, int vel, int dur, int chan, int port, int n) 
{
	MidiEvPtr e;
	int i,time = MidiGetTime();
	
	for (i = 0; i < n ; i++) {

		if (e = MidiNewEv(typeNote)) {          // allocate a note event
        
			Date (e) = time + (i * dur);
                	Pitch(e) = pitch;    		// fill its fields
                	Vel(e) = vel;
                	Dur(e) = dur-5;
                	Chan(e) = chan;
                	Port(e) = port;
			   
                	MidiSend(ref, e);               // send it now
        	}
	}
}

//_______________________________________________________________________

int main( int argc, char *argv[])
{
        short ref;
    	int pitch, vel, chan,port,dur,len;
	char c;
        
        ref = MidiOpen("msSendSeq");    	// open a MidiShare session
        MidiConnect(ref, 0, true);              // connect to physical Midi outputs
		
	pitch = lopt(argv, "-pitch", 60);
	vel = lopt(argv, "-vel", 80);
	dur = lopt(argv, "-dur", 250);
	chan = lopt(argv, "-chan", 0);
	port = lopt(argv, "-port", 0);
	len =lopt(argv, "-len", 10);
		
	printf ( "\nmsSendSeq [-pitch <p> -vel <v> -dur <d> -chan <c> -port <p> -len <n>] \n\n");
	printf ( "msSendSeq create and play a MidiShare sequence\n\n");
	printf ( "Use p or P to create and play the sequence\n");
	printf ( "Use q or Q to quit the application\n\n");
  		
	while ((c = getchar()) && c != 'q' && c != 'Q')  switch (c) {
                case 'P': case 'p': playSeq(ref,pitch,vel,dur,chan,port,len); break;
        }
	    
        MidiClose (ref);       // close the MidiShare session
}

