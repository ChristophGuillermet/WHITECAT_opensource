/*====================== A MIDI SHARE TOOL  (© GRAME 99-01) =====================

NAME
      msDisplay -- a very simple MidiShare tool

SYNOPSIS
      msDisplay 

DESCRIPTION
     "msDisplay" display incoming events in a textual format

===============================================================================*/

#include <stdio.h>
#include <MidiShare.h>


#define MSALARMAPI
 
// ------------------------------------------------------------------
// VARIABLES
// ------------------------------------------------------------------

short		myRefNum; // application reference number
MidiFilterPtr	myFilter; // events filter
MidiName	AppliName = "msDisplay";

char	  	TblLibEv[256][20];

void MSALARMAPI ReceiveEvents(short r);
void MSALARMAPI ReceivePolling ( long date, short r, long a, long b, long c);

static int min (int a, int b) { return (a < b) ?a : b;}

/* ------------------------------------------------------------------------- */
static char * DatasToText(MidiEvPtr e , char *buff)
{
	char *ptr= buff;
	int i, max;

	*ptr= 0;
	switch (EvType(e)) {
		case typeTextual:
		case typeCopyright:
		case typeSeqName:
		case typeInstrName:
		case typeLyric:
		case typeMarker:
		case typeCuePoint:
			for( i=0, max= min(MidiCountFields(e), 18); i< max; i++)
				*ptr++= (char)MidiGetField(e, i);
			*ptr= 0;
			break;
		case typeSMPTEOffset:
			for( i=0; i< 6; i++, ptr+=3) 
				sprintf( ptr, "%-2ld ", MidiGetField(e, i));
			break;
		default:
			for( i=0, max= min(MidiCountFields(e), 5); i< max; i++, ptr+=4)
				sprintf( ptr, "%-3ld ", MidiGetField(e, i));
	}
	return buff;
}

/* ------------------------------------------------------------------------- */
static char * EvToText ( MidiEvPtr e, char *buff)
{
	unsigned long mn, sec;
	unsigned int h, ms;
	char sDatas[50];

	sec	= Date(e)/1000;
	ms	= Date(e)%1000;
	mn 	= sec / 60;
	sec	%= 60;
	h	= mn / 60;
	mn	%= 60;
	*buff= 0;
	sprintf (buff, "%02d:%02d:%02d:%03d  %s %-3d %-2d  %s",
					h, (int)mn, (int)sec, ms, TblLibEv[EvType(e)],
					(int)Port(e), (int)Chan(e), DatasToText(e, sDatas));
	return buff;
}



/* ------------------------------------------------------------------------- */
void Display ( MidiEvPtr e)
{
	char s[120];

	if (e) {
		EvToText (e, s);
		printf(s);
		printf("\n");
		MidiFreeEv(e);
	}
}


/* ------------------------------------------------------------------------- */
void DisplayTask (short ref)
{
	long n;

	n= MidiCountEvs(ref);
	while (n > 16) {
		MidiFreeEv (MidiGetEv (ref));
		n--;
	}
	while (n--) {
		Display (MidiGetEv (ref));
	}
}


/* ------------------------------------------------------------------------- */
void MSALARMAPI ReceiveEvents(short r)
{
	if (MidiTask(ReceivePolling,MidiGetTime(),r,(long)MidiGetInfo(r),0,0))
		MidiSetRcvAlarm (r, 0);
	else 
		MidiFlushEvs(r);
}


/* ------------------------------------------------------------------------- */
void MSALARMAPI ReceivePolling ( long date, short r, long a, long b, long c)
{
	long n;

	n= MidiCountEvs (r);
	if (n) {
		DisplayTask(r);
		MidiTask (ReceivePolling, date+100, r, a, 0, 0);
	}
	else MidiSetRcvAlarm (r, ReceiveEvents);
}


/* ------------------------------------------------------------------------- */
void InitTblLibEv(void) {

  int i;

  for (i=0;i<255;i++) strcpy(TblLibEv[i],"");

  strcpy(TblLibEv[typeNote],		 "Note        ");
  strcpy(TblLibEv[typeKeyOn],		 "Key On      ");
  strcpy(TblLibEv[typeKeyOff],		 "Key Off     ");
  strcpy(TblLibEv[typeKeyPress],	 "Key Press   ");
  strcpy(TblLibEv[typeCtrlChange],	 "Ctrl Change ");
  strcpy(TblLibEv[typeProgChange],	 "Prog Change ");
  strcpy(TblLibEv[typeChanPress],	 "Chan Press  ");
  strcpy(TblLibEv[typePitchWheel],	 "Pitch Wheel ");
  strcpy(TblLibEv[typeSongPos],	 	 "SongPos     ");
  strcpy(TblLibEv[typeSongSel],	 	 "SongSel     ");
  strcpy(TblLibEv[typeClock],		 "Clock       ");
  strcpy(TblLibEv[typeStart],		 "Start       ");
  strcpy(TblLibEv[typeContinue],	 "Continue    ");
  strcpy(TblLibEv[typeStop],		 "Stop        ");
  strcpy(TblLibEv[typeTune],		 "Tune        ");
  strcpy(TblLibEv[typeActiveSens],	 "Active Sens.");
  strcpy(TblLibEv[typeReset],		 "Reset       ");
  strcpy(TblLibEv[typeSysEx],		 "System Excl.");
  strcpy(TblLibEv[typeStream],		 "Stream      ");
  strcpy(TblLibEv[typePrivate],	 	 "Private     ");
  strcpy(TblLibEv[typeProcess],	  	 "Process     ");
  strcpy(TblLibEv[typeDProcess],	 "DProcess    ");
  strcpy(TblLibEv[typeQuarterFrame], "QuarterFrame");
  strcpy(TblLibEv[typeCtrl14b],	 	 "Ctrl 14bits ");
  strcpy(TblLibEv[typeNonRegParam],  "NonRegParam.");
  strcpy(TblLibEv[typeRegParam],	 "Reg Param   ");
  strcpy(TblLibEv[typeSeqNum],		 "Seq Number  ");
  
  
  strcpy(TblLibEv[typeTextual],	 	 "Text        ");
  
  strcpy(TblLibEv[typeCopyright],	 "Copyright   ");
  strcpy(TblLibEv[typeSeqName],	 	 "Seq Name    ");
  strcpy(TblLibEv[typeInstrName],	 "Instr Name  ");
  strcpy(TblLibEv[typeLyric],		 "Lyric       ");
  strcpy(TblLibEv[typeMarker],		 "Marker      ");
  strcpy(TblLibEv[typeCuePoint],	 "CuePoint    ");
  strcpy(TblLibEv[typeChanPrefix],	 "Chan Prefix ");
  strcpy(TblLibEv[typePortPrefix],	 "Port Prefix ");
  strcpy(TblLibEv[typeEndTrack],	 "End Track   ");
  strcpy(TblLibEv[typeTempo],		 "Tempo       ");
  strcpy(TblLibEv[typeSMPTEOffset],  "SMPTE Offset");
  strcpy(TblLibEv[typeTimeSign],	 "Time Sign.  ");
  strcpy(TblLibEv[typeKeySign],	 	 "Key Sign.   ");
  strcpy(TblLibEv[typeSpecific],	 "Specific    ");
  strcpy(TblLibEv[typeReserved],	 "Reserved    ");
  strcpy(TblLibEv[typeDead],		 "Dead        ");
}

/* ------------------------------------------------------------------------- */
void InstallFilter( short refNum, MidiFilterPtr filter )
{
	short i;

	if (!filter) return;
	for (i = 0 ; i < 256 ; i++ )
	{
		MidiAcceptType(filter,i,true);
		MidiAcceptPort(filter,i,true);
	}
	for (i = 0 ; i < 16 ; i++ ) MidiAcceptChan(filter,i,true);
	MidiSetFilter(refNum, filter);
}


/* ------------------------------------------------------------------------- */
Boolean SetUpMidi(void)
{
	char buff[32], *name;

	InitTblLibEv();
	
	if ( !MidiShare()) {
		printf ("MidiShare not available\n");
		return false;
	}

	myRefNum = MidiOpen(AppliName);
	if ( myRefNum == MIDIerrSpace ){
		printf ( "Too much MidiShare client applications\n");
		return false;
	}
	
	MidiSetRcvAlarm(myRefNum,ReceiveEvents);
	MidiConnect( 0, myRefNum, true);
	myFilter = MidiNewFilter();
	InstallFilter( myRefNum, myFilter);
	return true;
}

/* ------------------------------------------------------------------------- */
void QuitMidi() 
{
	MidiFreeFilter(myFilter);
	MidiClose(myRefNum);
}

/* ------------------------------------------------------------------------- */
int main( int argc, char *argv[])
{
        char c;
		
        if (!SetUpMidi()) return ;                         // open the MidiShare session
		
		printf ( "\nmsDisplay displays incomming events in a textual format\n\n");
		printf ( "use q or Q to quit the application\n\n");
      
		while (getchar()!) {Display ( MidiEvPtr e);}
       
       	QuitMidi();                                        // close the MidiShare session
}

