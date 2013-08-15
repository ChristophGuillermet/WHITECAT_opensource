/*

  Copyright © Grame 2004

  This library is free software; you can redistribute it and modify it under 
  the terms of the GNU Library General Public License as published by the 
  Free Software Foundation version 2 of the License, or any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public License 
  for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  research@grame.fr

*/
 
 
#ifndef __MIDIFILE_H__
#define __MIDIFILE_H__

#include <stdio.h>
#include "MidiShare.h"

#define nil 0

#if defined(linux) || defined(__MACH__)
# define FAR
# define NEAR
# define MFAPI
# define errno  /* a revoir */

#elif defined(mac_classic)
# include <midisharePPC.h>
# define FAR
# define NEAR
# define MFAPI

#elif defined(WIN32)
# ifdef __BuildLib__
#	define MFAPI	__declspec(dllexport)
# else
#	define MFAPI	__declspec(dllimport)
# endif

#elif defined(WIN16)
# define MFAPI WINAPI _export

#else
#error "architecture undefined"
#endif


/*--------------------------------------------------------------------------*/
extern int MidiFile_errno;		/* used similarly to errno				*/
								/* to read just in error case, if the	*/
								/* code is equal to MidiFileNoErr, look	*/
								/* at errno	*/

#define isMidiFileError( fd) (MidiFile_errno!=MidiFileNoErr || ferror(fd->fd) || feof(fd->fd))

/*--------------------------------------------------------------------------*/
/* implemented file types			 										*/

enum { midifile0= 0, midifile1, midifile2};


/*--------------------------------------------------------------------------*/
/* opening modes for existing files											*/
#define MidiFileRead	0			/* reading the file				*/
#define MidiFileAppend	1			/* appening to the file			*/


/*--------------------------------------------------------------------------*/
/* time definition															*/

#define TicksPerQuarterNote	0
#define Smpte24				24
#define Smpte25				25
#define Smpte29				29
#define Smpte30				30


/*--------------------------------------------------------------------------*/
/* meta events types 														*/
#define	META			0xFF		/* meta event header code			*/

#define MDF_NumSeq		0			/* sequence number					*/
#define MDF_Texte		1			/* texte message					*/
#define MDF_Copyright	2			/* copyright message 				*/
#define MDF_SeqName		3			/* sequence or track name			*/
#define MDF_InstrName	4			/* instrument name					*/
#define MDF_Lyric		5			/* lyrics							*/
#define MDF_Mark		6			/* marker							*/
#define MDF_CuePt		7			/* cue point						*/
#define MDF_ChanPref	0x20		/* MIDI Channel Prefix				*/
#define MDF_PortPref	0x21		/* MIDI Port Prefix					*/
#define MDF_EndTrk		0x2F		/* end track						*/
#define MDF_Tempo		0x51		/* tempo change						*/
#define MDF_Offset		0x54		/* smpte offset						*/
#define MDF_Meas		0x58		/* time signature					*/
#define MDF_Ton			0x59		/* key signature					*/
#define MDF_Extra		0x7F		/* sequencer specific meta event	*/

/* length of the differents meta events */
#define MDF_NumSeqLen	2
#define MDF_ChanPrefLen 1
#define MDF_PortPrefLen 1
#define MDF_EndTrkLen	0
#define MDF_TempoLen	3
#define MDF_OffsetLen	5
#define MDF_MeasLen		4
#define MDF_TonLen		2

/*--------------------------------------------------------------------------*/
/* datas structures															*/
/*--------------------------------------------------------------------------*/

typedef struct MDF_Header{			/* the file header structure	*/
	char 	id[4];					/* header id					*/
	long	len;					/* datas length ( = 6 )			*/
	short	format;					/* file format					*/
	unsigned short ntrks;			/* number of tracks				*/
	short	time;					/* time representation			*/
}MDF_Header;


typedef struct MDF_Trk{				/* track header structure		*/
	char id[4];						/* header id					*/
	unsigned long	len;			/* datas length 				*/
}MDF_Trk;

typedef struct midiFILE{			/* MIDI file descriptor		*/
	short 	format;					/* file format					*/
	unsigned short ntrks;			/* number of tracks					*/
	short	time;					/* time representation			*/
					/* for MIDI time: tick count per quarter note           */
					/* for smpte time: b. 15  = 1                           */
					/*                 b.8-14 = frame count per sec         */
					/*			       b.0-7  = tick count per frame        */
	FILE 	*fd;					/* standard file descriptor             */	
	long  	trkHeadOffset;			/* track header offset                  */
									/* nil if the track is closed           */
	long	_cnt;					/* count for end track detection		*/
	MidiSeqPtr keyOff;				/* keyOff coming from typeNote events   */
	long 	curDate;				/* current date 						*/
	Boolean mode;					/* 0/1 : reading/writing                */
	Boolean opened;					/* flag for opened track 				*/
}midiFILE;

typedef midiFILE FAR * MIDIFilePtr;

typedef struct MDF_versions{
	short	src;					/* source code version 					*/
	short 	MidiFile;				/* MIDI file format version				*/
}MDF_versions;


/*--------------------------------------------------------------------------*/
/* error codes								 								*/

#define MidiFileNoErr		0		/* everything is ok						*/
#define MidiFileErrFrmt		(-1)	/* internal datas format error			*/
#define MidiFileErrEvs		(-2)	/* MidiShare memory exhausted			*/
#define MidiFileErrNoTrack	(-3)	/* the track doesn't exists				*/
#define MidiFileErrAdd0		(-4)	/* try to add to a format 0	MIDI file 	*/
#define MidiFileErrMode		(-5)	/* uncorrect file opening mode			*/
#define MidiFileErrUnknow	(-6)	/* unknown event type					*/
#define MidiFileErrReset	(-7)	/* try to write a reset event			*/

#define MidiFileErrTrackClose	(-8)	/* the track is closed				*/

/*--------------------------------------------------------------------------*/
/* some macros															*/

#ifdef __cplusplus
inline Boolean isTrackOpen( midiFILE *fd) { return (fd)->opened; }

inline short smpte(MDF_Header *f)					{ return ((f)->time & 0x8000); }
inline short frame_par_sec(MDF_Header *f)			{ return (((f)->time & 0x8000) >> 8); }
inline short ticks_par_frame(MDF_Header *f)			{ return ((f)->time & 0xFF); }
inline short ticks_par_quarterNote(MDF_Header *f)	{ return (f)->time; }
#else
#define isTrackOpen(fd)	((fd)->opened)

#define smpte(f)					((f)->time & 0x8000)
#define frame_par_sec(f)			(((f)->time & 0x8000) >> 8)
#define ticks_par_frame(f)			((f)->time & 0xFF)
#define ticks_par_quarterNote(f)	(f)->time
#endif

#define MDF_Header_SIZE 14
#define MDF_Trk_SIZE 	8


/*--------------------------------------------------------------------------*/
/* functions declarations													*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32

MFAPI const MDF_versions FAR * MidiFileGetVersion(void);
#else

const MDF_versions FAR * MFAPI MidiFileGetVersion(void);

#endif


/*-------------------------------- opening and closing the MIDI files ------*/
MIDIFilePtr MFAPI MidiFileOpen  (const char FAR *filename, short mode);
MIDIFilePtr MFAPI MidiFileCreate(const char FAR *filename, short format, short timeDef, short ticks);
Boolean    	MFAPI MidiFileClose (MIDIFilePtr fd);

/*-------------------------------- tracks management -----------------------*/
Boolean    MFAPI MidiFileOpenTrack  ( MIDIFilePtr fd);
Boolean    MFAPI MidiFileNewTrack   ( MIDIFilePtr fd);
Boolean    MFAPI MidiFileCloseTrack ( MIDIFilePtr fd);
Boolean    MFAPI MidiFileChooseTrack( MIDIFilePtr fd, short trackIndex);

/*-------------------------------- functions to read -----------------------*/
MidiEvPtr  MFAPI MidiFileReadEv    ( MIDIFilePtr fd);
MidiSeqPtr MFAPI MidiFileReadTrack ( MIDIFilePtr fd);

/*-------------------------------- functions to write ----------------------*/
Boolean    MFAPI MidiFileWriteEv   ( MIDIFilePtr fd, MidiEvPtr ev);
Boolean    MFAPI MidiFileWriteTrack( MIDIFilePtr fd, MidiSeqPtr seq);

/*-------------------------------- error codes access ----------------------*/
int        MFAPI MidiFileGetMFErrno (void);
int        MFAPI MidiFileGetErrno   (void);

#ifdef __cplusplus
}
#endif

#endif
