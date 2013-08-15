/* 
   ===========================================================================
   This file is Copyright (c) Grame, Computer Music Research Laboratory 
   1996-1999. It can be freely used as source code example of the MIDIFile
   Library functions.

   Grame : Computer Music Research Laboratory
   Web : http://www.grame.fr/Research
   E-mail : midishare@grame.fr
   ===========================================================================
*/

/*--------------------------------------------------------------------------
 *
 * a typical example of code to read a MIDI file might be 
 * the following one. 
 *
 * Warning! take care of that this code doesn't check for errors
 *
 *--------------------------------------------------------------------------*/
MidiSeqPtr ReadMidiFile( char *itsName)
{
	MidiSeqPtr seq, tmp;
	midiFILE *fd;
	unsigned short n;
	
	seq= MidiNewSeq()			/* allocate a new MidiShare sequence */
	if( fd= MidiFileOpen( itsName, MidiFileRead))
	{
		n= fd->ntrks;						/* get the number of tracks */
		while( n--) {
			tmp= MidiFileReadTrack( fd);	/* read every track			*/

			Mix( tmp, seq);		/* the Mix function is to provide		*/
								/* it transfers the content of the first*/
								/* sequence to the second one, 			*/
								/* its interface might be :				*/
							/* void Mix( MidiSeqPtr src, MidiSeqPtr dst)*/

			MidiFreeSeq( tmp);	/* this sequence is now empty, we can	*/
								/* free it without freing the readed	*/
								/* events								*/
		}
		MidiFileClose( fd);
	}
	return seq;
}


/*--------------------------------------------------------------------------
 *
 * a typical example of code to create a format 1 MIDI file might be 
 * the following one. 
 *
 * Warning again! take care of that this code doesn't check for errors
 *
 *--------------------------------------------------------------------------*/
void WriteMIDIFile( char *itsName)
{
	midiFILE *fd;

	/* we first create a new MIDI file using a format 1	*/
	if( fd= MidiFileCreate( itsName, midifile1, TicksPerQuarterNote, 500))
	{
		/* for the file consistency, the first track 	*/
		/* to write is the tempo map					*/				
		MidiFileWriteTrack( fd, myTempoMap);

		/* then we can write all the other tracks		*/
						/* 
						   it is the program responsibility to determine 
						   the content of the tracks. Here, every track is
						   stored in separate MidiShare sequences (myTempoMap,
						   track1, track2,...trackn). They are supposed to be
						   global variables. Of course, events in the file will
						   keep exactly the same order than in the sequence
						 */
		MidiFileWriteTrack( fd, track1);
		MidiFileWriteTrack( fd, track2);
		/* ...*/
		MidiFileWriteTrack( fd, trackn);
		
		/* and we finaly close the file					*/
		MidiFileClose( fd);
	}
}
