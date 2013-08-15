/*====================== A MIDI SHARE TOOL  (© GRAME 99-01) =====================

NAME
      msEcho -- a very simple MidiShare tool

SYNOPSIS
      msEcho [-delay <d> -pitch <p> -vel <v> -chan <c>]
      
      delay :the delay between 2 sucessives echo in millisecond
      pitch : the pitch offset between 2 sucessives echo
      vel : the vel offset between 2 sucessives echo
      chan : if 0, echo on all channel, otherwise echo only on the specified channel

DESCRIPTION
     "msEcho" produce an echo on incoming Note and KeyOn events

===============================================================================*/
 
#include <stdio.h>
#include <MidiShare.h>


#define nil     0
#define true    1
#define false   0

#define MSALARMAPI

// ------------------------------------------------------------------
// CONSTANTS
// ------------------------------------------------------------------

enum { kDelay, kVel, kPitch, kChan, kParamCount };

// ------------------------------------------------------------------
// VARIABLES
// ------------------------------------------------------------------

short    MSParam[kParamCount];

short		myRefNum; // application reference number
MidiFilterPtr	myFilter; // events filter
MidiName	AppliName = "msEcho";

void MSALARMAPI ReceiveEvents( short r);
void MSALARMAPI EchoTask( long date,short r,long a1,long a2,long a3);
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
  MidiSetRcvAlarm(myRefNum,ReceiveEvents);
  InstallFilter();
  MidiConnect (myRefNum, 0, true);
  MidiConnect (0, myRefNum, true);
  
  return true;
}

/* ------------------------------------------------------------------------- */
void QuitMidi() 
{
	MidiFreeFilter(myFilter);
	MidiClose(myRefNum);
}

/* --------------------------------------------------------
Application receive alarm 
-------------------------------------------------------- */
void MSALARMAPI ReceiveEvents ( short r)
{
  MidiEvPtr e;
  unsigned long d;
  
  while (e = MidiGetEv(r)) {
  		
         d = Date(e) + MSParam[kDelay];
         if ( (MSParam[kChan]!=0) && (MSParam[kChan]!=Chan(e)+1) )
                MidiFreeEv(e);
         else if ( EvType(e) == typeNote ) {
                if( !MidiTask(EchoTask,d,r,(long)e,0,0))
                        MidiFreeEv(e);
         }
         else if ( Vel(e) > 0 )  {
	        EvType(e) = typeNote;
                Dur(e) = 100;
                if( !MidiTask(EchoTask,d,r,(long)e,0,0))
                        MidiFreeEv(e);
         }
         else MidiFreeEv(e);
  }
}

/* --------------------------------------------------------
Real time echo management task
-------------------------------------------------------- */
void MSALARMAPI EchoTask( long d,short ref,long el,long a2, long a3)
{
  MidiEvPtr e;
  short v,p;

  e= (MidiEvPtr)el;
  v = Vel(e)+MSParam[kVel];
  p = Pitch(e)+MSParam[kPitch];
  if ( ((v>0)&&(v<128)) && ((p>=0)&&(p<128)) ) {
         Vel(e) = v;
         Pitch(e) = p;
         MidiSendAt(ref, MidiCopyEv(e), d);
         if( !MidiTask(EchoTask, d+MSParam[kDelay], ref, el, 0, 0))
                MidiFreeEv(e);
  }
  else MidiFreeEv(e);
}

/* ----------------------------------------------------------------------------
For incoming events filtering
---------------------------------------------------------------------------- */
void InstallFilter(void) {

  short i;
  
  myFilter = MidiNewFilter();
  
  if (myFilter) {

 	for (i=0;i<256;i++) {
         	MidiAcceptType(myFilter,i,false);
         	MidiAcceptPort(myFilter,i,true);
 	}
  	MidiAcceptType(myFilter,typeNote,true);
  	MidiAcceptType(myFilter,typeKeyOn,true);

  	for (i=0;i<16;i++) MidiAcceptChan(myFilter,i,true);
  	MidiSetFilter(myRefNum,myFilter);
  }
}

//_______________________________________________________________________

int main( int argc, char *argv[])
{
	char c;
	
    if ( !SetUpMidi() ) return 0;                  // open the MidiShare session
        	
	MSParam[kDelay] = lopt(argv, "-delay", 500); 
	MSParam[kVel] = lopt(argv, "-vel", -10); 
	MSParam[kPitch] = lopt(argv, "-pitch", 0); 
	MSParam[kChan] = lopt(argv, "-chan", 0); 
	
	printf ( "\nmsEcho [-delay <d> -pitch <p> -vel <v> -chan <c>]\n\n");
	printf ( "msEcho produce an echo on incoming Note and KeyOn events\n\n");
	printf ( "Use q or Q to quit the application\n\n");
      
	while ((c = getchar()) && c != 'q' && c != 'Q')  switch (c) {}
 	
    QuitMidi();                                      // close the MidiShare session
}

