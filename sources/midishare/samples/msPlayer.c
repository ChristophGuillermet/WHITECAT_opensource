#include "player.h"
#include <stdio.h>

int main (int argc, char * const argv[])
{
	MidiSeqPtr seq;
	short ref; 
	MidiFileInfos info;
	char c;
	int res;
	
	ref = OpenPlayer("msPlayer");
	MidiConnect(ref,0,1);
	
	seq = MidiNewSeq();
	res = MidiFileLoad(argv[1], seq, &info); 
	if (res != 0) {
		printf("File not found %s\n", argv[1]);
		exit(1);
	}else {
		printf("Filename %s\n", argv[1]);
	}
	SetAllTrackPlayer(ref, seq, info.clicks);
	
	printf("Type 'p' to start playing\n");
	printf("Type 's' to stop playing\n");
	printf("Type 'c' to continue playing\n");
	printf("Type 'q' to quit\n");
	
	while ((c = getchar()) != 'q') {
		switch (c) {
			case 'p' : StartPlayer(ref); break;
			case 's' : StopPlayer(ref);  break;
			case 'c' : ContPlayer(ref);  break;
		} 
	}	
	ClosePlayer(ref);
	return 0;
}