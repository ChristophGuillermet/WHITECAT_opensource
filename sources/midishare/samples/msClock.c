/*====================== A MIDI SHARE TOOL  (© GRAME 99-01) =====================

NAME
      msClock -- a very simple MidiShare tool

SYNOPSIS
      msClock [-delay <d>]
      
      delay :the delay between 2 sucessives clock
  
DESCRIPTION
     "msClock" produce produces synchronization events: start, stop, continue,
     clock

===============================================================================*/

#include <stdio.h>
#include <MidiShare.h>


#define nil     0
#define true    1
#define false   0

#define MSALARMAPI


// ------------------------------------------------------------------
// VARIABLES
// ------------------------------------------------------------------

short		myRefNum; // application reference number
MidiName	AppliName = "msClock";
MidiEvPtr       myClockTask;
long		myDelay;


void MSALARMAPI ClockTask( long date,short r,long a1,long a2,long a3);
void InstallFilter();

//_______________________________________________________________________
long lopt (char *argv[], char *name, long def)
{
        int     i;
        for (i=0; argv[i]; i++) if (!strcmp(argv[i], name)) return atoi(argv[i+1]);
        return def;
}

/* ----------------------------------------------------------------------------

MidiShare setup

---------------------------------------------------------------------------- */
Boolean SetUpMidi(void)
{
  if ( !MidiShare() ) {
                printf("MidiShare is not installed\n");
                return false;
  }
  myRefNum = MidiOpen(AppliName);
  if ( myRefNum == MIDIerrSpace ) {
                printf("Too much MidiShare client applications\n");
                return false;
  }
  MidiConnect (myRefNum, 0, true);
  return true;
}



/* ------------------------------------------------------------------------- */
void Start() 
{
	MidiForgetTask(&myClockTask);
	MidiSendIm(myRefNum, MidiNewEv(typeStart));
	ClockTask( MidiGetTime(),myRefNum,0,0,0);
}

/* ------------------------------------------------------------------------- */
void Continue() 
{
	MidiForgetTask(&myClockTask);
	MidiSendIm(myRefNum, MidiNewEv(typeContinue));
	ClockTask( MidiGetTime(),myRefNum,0,0,0);
}

/* ------------------------------------------------------------------------- */
void Stop() 
{
	MidiForgetTask(&myClockTask);
	MidiSendIm(myRefNum, MidiNewEv(typeStop));
}

/* ------------------------------------------------------------------------- */
void QuitMidi() 
{
	Stop();
	MidiClose(myRefNum);
}


/* ------------------------------------------------------------------------- */
void Up() { myDelay+=5; }

/* ------------------------------------------------------------------------- */
void Down() { if (myDelay > 5 ) myDelay-=5; }


/* --------------------------------------------------------
Real time clock task

-------------------------------------------------------- */
void MSALARMAPI ClockTask( long d,short ref,long el,long a2, long a3)
{
	int i;
	MidiSendAt(ref, MidiNewEv(typeClock), d);
  	myClockTask = MidiTask(ClockTask, d+myDelay, ref, 0, 0, 0);
                
}

//_______________________________________________________________________

int main( int argc, char *argv[])
{
	char c;
	
        if (!SetUpMidi()) return 0;     // open the MidiShare session
        	
	myDelay = lopt(argv, "-delay", 100); 
	
	printf ( "\nmsClock [-delay <d>] \n\n");
	printf ( "msClock send synchronisation events: start, stop, continue and clock\n\n");
	printf ( "Use p or P to start the synchro\n");
	printf ( "Use s or S to stop the synchro\n");
	printf ( "Use c or C to continue the synchro\n");
	printf ( "Use - to decrease the delay between clocks\n");
	printf ( "Use + to decrease the delay between clocks\n");
	printf ( "Use q or Q to quit the application\n\n");
      
	while ((c = getchar()) && c != 'q' && c != 'Q')  switch (c) {

                case 'P': case 'p': Start(); break;
                case 'S': case 's': Stop(); break;
                case 'C': case 'c': Continue(); break;
		case '-':  Up(); break;
		case '+':  Down(); break;
        }
   	
        QuitMidi();       // close the MidiShare session
}

