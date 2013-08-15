///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//===================================================================================
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//
//                        MIDISHARE MINI TUTORIAL
//
//       This program is a small tutorial for MidiShare programming.
//
// HOW TO USE IT :
//
// 1) first of all MidiShare must be installed.
//
// 2) Then you need to copy the MidiShare.h file into your include folder.
//
// 3) Now you are ready for the tutorial. Update the project and run the program with 
// the debugger option. Use the GO command to start the program and follow the instructions
// displayed in the console window. Break points are installed in several points of the 
// program in order for you to look at the code and understand how MidiShare works.
//
///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//===================================================================================
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/



#include <MidiShare.h>
#include <stdio.h>

long            ourRefNum = -1;
MidiSeqPtr      ourSequence = 0;
MidiFilterPtr   ourFilter = 0;
long            stopFlag;

static void DebugStr (char * str) 
{
	char c;
	printf (str);
	while ((c = getchar()) && c != 'g' && c != 'G') {}
	
}

//===================================================================================
// Welcome to the tutorial
//===================================================================================

void welcome ()
{
        printf (" <<<< WELCOME TO THE MIDISHARE TUTORIAL >>>> \n\n");
        DebugStr ("--> use g or G to run the first test\n");
}


//===================================================================================
// test 1 : Check if MidiShare is installed
//===================================================================================
// First of all, an application have to make sure that MidiShare is in memory. 
// This checking is done thanks to the MidiShare function. The result is 1 if 
// MidiShare is installed and 0 otherwise. Check the Console Window for the
// result.

void test1 () 
{
        printf("\n\n <TEST 1 : Check if MidiShare is installed> \n");
        DebugStr ("--> use g or G to run the next test\n");
        
        printf("MidiShare installed  : %s\n", (MidiShare() ? "YES" : "NO") );
}


//===================================================================================
// test 2 : current version of MidiShare
//===================================================================================
// MidiGetVersion gives the version number of MidiShare. The result must be
// 168 or greater.

void test2 ()
{       
        long    v;
        
        printf("\n\n<TEST 2> Get MidiShare version\n");
        DebugStr ("--> use g or G to get the MidiShare version and go to the next test\n");
        
        v = MidiGetVersion();
        printf("MidiShare version : %ld.%ld\n", v / 100, v % 100);
}                               


//===================================================================================
// test 3 : how many client applications are running ?
//===================================================================================
// The MidiCountAppls gives the number of active Midi applications on. The result 
// here will probably be 0. It means that no MidiShare client applications are running.

void test3 ()
{
        printf("\n\n<TEST 3> Count MidiShare client applications\n");
        DebugStr ("--> use g or G to count MidiShare client applications and go to the next test\n");

        printf("Count of MidiShare client applications : %hi\n", MidiCountAppls());
}


//===================================================================================
// test 4 : register as a client application
//===================================================================================
// A Midi application need first to register as a client of MidiShare by calling MidiOpen.
//
// MidiOpen allows the recording of some information relative to the application 
// context (its name, the value of the global data register, etc...), to allocate a 
// reception FIFO and to attribute a unique reference number to the application. 
// In counterpart to any MidiOpen call, the application must call the MidiClose 
// function before leaving, by giving its reference number as an argument. 
// MidiShare can thus be aware of the precise number of active Midi applications. 
//
// The result of MidiOpen is a reference number (>0) we need to keep in a global. 
// If some errors occur the result is an error code (<0).

void test4 ()
{
        printf("\n\n<TEST 4> Register as a client application\n");
        DebugStr ("--> use g or G to register as a client application and go to the next test\n");

        ourRefNum = MidiOpen("MacOSX_Tutorial");
        if (ourRefNum > 0) 
                printf("MidiOpen succeed. Our reference number is : %ld\n", ourRefNum);
        else
                printf("MidiOpen fail. The error code is : %ld\n", ourRefNum); 
}


//===================================================================================
// test 5 : how many client applications are running now?
//===================================================================================
// Now lets check again for the number of MidiShare client applications on activity.

void test5 ()
{
        printf("\n\n<TEST 5> Count again MidiShare client applications \n");
        DebugStr ("--> use g or G to count again MidiShare client applications and go to the next test\n");

        printf("Count of MidiShare client applications : %hi\n", MidiCountAppls());
}                                       

// The result is probably 2. Actually when the first client opens, a pseudo application 
// with name "MidiShare" and reference number 0 is also started. You can think of it as 
// the Midi drivers of MidiShare. For this reason we have a count of 2 client 
// applications instead of 1. This pseudo application "MidiShare" cannot be closed
// directly. It is closed automatically when the very last client application closes. 


//===================================================================================
// test 6 : list MidiShare client applications
//===================================================================================
// We are now going to list all the client applications on activity.

void listOfAppl ()
{
        long    i, ref;
        long    n = MidiCountAppls();
        
        printf("List of MidiShare client applications\n");
        printf("-------------------------------------\n");
        for (i = 1; i<= n; i++) {
                ref = MidiGetIndAppl(i);        // get the refnum form the order number
                printf(" %2ld : reference number %2ld, name : '%#s' \n", i , ref, MidiGetName(ref));
        }
        printf("-------------------------------------\n\n");
}

void test6 ()
{
        printf("\n\n<TEST 6> List every MidiShare client applications\n");
        DebugStr ("--> use g or G to list every MidiShare client applications and go to the next test\n");

        listOfAppl();
}

// NOTE : The MidiGetIndAppl function allows to know the reference number of any 
// application by giving its order number (a number between 1 and MidiCountAppls).
// The MidiGetName function gives the name (the address of a Pascal string) of a client
// application from its reference number. 
//
// In the console window the result will be :
//
// List of MidiShare client applications
// -------------------------------------
//   1 : reference number  0, name : 'MidiShare'
//   2 : reference number  1, name : 'MacOSX_Tutorial'
// -------------------------------------

// NOTE : The reference number of the pseudo application "MidiShare" is always 0.


//===================================================================================
// test 7 : search for a client application with name "MacOSX Tutorial"
//===================================================================================
// Knowing an application name, it is possible to find its reference number using
// the MidiGetNamedAppl function. If more than one client applications have the same
// name the result is the reference number of the first one (the one with the smallest
// reference number).

void test7 ()
{
        printf("\n\n<TEST 7> search the reference number of 'MacOSX Tutorial'\n");
        DebugStr ("--> use g or G to search the reference number of 'MacOSX Tutorial' and go to the next test\n");

        printf("Reference number of '%#s' : %d\n", "MacOSX Tutorial", MidiGetNamedAppl("MacOSX_Tutorial"));
}




//===================================================================================
// test 8 : search for a client application with name "XYZ"
//===================================================================================
// If we look for a non existant name the result is -4. This means that no application
// with that name was found. Negative reference numbers are used as error codes.

void test8 ()
{
        printf("\n\n<TEST 8> search the reference number of 'XYZ'\n");
        DebugStr ("--> use g or G to search the reference number of 'XYZ' and go to the next test\n");

        printf("Reference number of '%#s' : %d\n", "XYZ", MidiGetNamedAppl("XYZ"));
        printf("A negative result means that the application was not found.\n");
}


//===================================================================================
// test 9 : change the name of client application "MacOSX Tutorial" to "MidiShare Tutorial"
//===================================================================================
// MidiSetName allows to change the name of a client application.

void test9 ()
{
        printf("\n\n<TEST 9> change the name of client application 'MacOSX Tutorial' to 'MidiShare Tutorial'\n");
        DebugStr ("--> use g or G to change the name and go to the next test\n");

        MidiSetName(MidiGetNamedAppl("MacOSX_Tutorial"), "MidiShare_Tutorial");
        listOfAppl ();
}

// In the console window the result will be :
// List of MidiShare client applications
// -------------------------------------
//   1 : reference number  0, name : 'MidiShare'
//   2 : reference number  1, name : 'MidiShare Tutorial'
// -------------------------------------


//===================================================================================
// test 10 : connect 'MidiShare Tutorial' to 'MidiShare'
//===================================================================================
// For an application to be able to transmit and receive events, it must first connect
// to some source and destination applications. The MidiConnect function is used to 
// connect or disconnect a source and a destination. The function takes 3 arguments :
// the reference number of the source, the reference number of the destination and a
// boolean (1 to connect and 0 to disconnect).

void test10 ()
{
        printf("\n\n<TEST 10> connect the output of 'MidiShare Tutorial' to 'MidiShare'\n");
        DebugStr ("--> use g or G to make the connection and go to the next test\n");

        MidiConnect(ourRefNum, 0, true);
}

// Now we are able to send events to the pseudo application "MidiShare", i.e. to
// external Midi devices.


//===================================================================================
// test 11 : test if 'MidiShare Tutorial' is connected to MidiShare
//===================================================================================
// We can test the connections using MidiIsConnected.

void test11 ()
{
        printf("\n\n<TEST 11> test a connection\n");
        DebugStr ("--> use g or G to test the connection and go to the next test\n");

        if (MidiIsConnected(ourRefNum, 0))
                printf("We are connected to the MidiShare output driver\n");
        else
                printf("We are not connected\n");
}


//===================================================================================
// test 12 : list the destinations of an application
//===================================================================================
// We can list all the destinations of an application by using MidiIsConnected for 
// each possible destination

void listOfDest (long src)
{
        long    i, dst;
        long    n = MidiCountAppls();
        
        printf("List of destinations of '%#s' \n", MidiGetName(src));
        printf("-------------------------------------\n");
        for (i = 1; i<= n; i++) {
                dst = MidiGetIndAppl(i);        // get the refnum form the order number
                if (MidiIsConnected(src, dst))
                        printf(" --> reference number %2ld, name : '%#s' \n", dst, MidiGetName(dst));
        }
        printf("-------------------------------------\n\n");
}

void test12 ()
{
        printf("\n\n<TEST 12> list the destinations of our application\n");
        DebugStr ("--> use g or G to list the destinations of our application and go to the next test\n");

        listOfDest(ourRefNum);
}


//===================================================================================
// test 13 : list the sources of an application
//===================================================================================
// We can list all the sources of an application by using MidiIsConnected for 
// each possible source

void listOfSrc (long dst)
{
        long    i, src;
        long    n = MidiCountAppls();
        
        printf("List of sources of '%#s' \n", MidiGetName(dst));
        printf("-------------------------------------\n");
        for (i = 1; i<= n; i++) {
                src = MidiGetIndAppl(i);        // get the refnum form the order number
                if (MidiIsConnected(src, dst))
                        printf(" <-- reference number %2ld, name : '%#s' \n", src, MidiGetName(src));
        }
        printf("-------------------------------------\n\n");
}

void test13 ()
{
        printf("\n\n<TEST 13> list the sources of the MidiShare output driver\n");
        DebugStr ("--> use g or G to list the sources of the MidiShare output driver and go to the next test\n");

        listOfSrc(0);
}


//===================================================================================
// test 14 : send a note with a pitch, a velocity and a duration in milliseconds
//===================================================================================
// We are now ready to send a note event. Be sure to have a Midi equipment connected
// to the Modem port.

void sendNote(long pitch)
{
        MidiEvPtr e = MidiNewEv(typeNote);
        
        if (e) {
                Chan(e) = 0;                    // channels have numbers from 0 to 15
                Port(e) = 0;                    // port 0 is Modem port, port 1 is Printer port
                MidiSetField(e, 0, pitch);      // the pitch
                MidiSetField(e, 1, 80);         // the velocity
                MidiSetField(e, 2, 1000);       // the duration in milliseconds
                
                MidiSendIm(ourRefNum, e);
        }
}

void test14 ()
{
        printf("\n\n<TEST 14> send a note\n");
        DebugStr ("--> use g or G to send a note and go to the next test\n");

        sendNote(60);
}

// The note was sent to your Midi equipment. Actually two messages where sent, a keyOn
// and a keyOn with velocity 0 after the duration of the note.

// IMPORTANT NOTE :
// MidiShare includes its own memory manager to store events and sequences. MidiNewEv 
// allocates the required memory to store the note event and returns a pointer to it. 
// This space is automatically disposed by MidiShare when the event is sent. This means 
// that once you have sent an event you MUST NOT use it anymore. In particular you 
// MUST NOT send it a second time, free it or make a copy of it.


//===================================================================================
// test 15 : send multiple notes
//===================================================================================
// As we just said, once an event is sent it must not be used or accessed any more.
// Here is an example of how to send several copies of the same event.

void SendMultipleNotes (long n, long pitch, long delay)
{
        long            i;
        MidiEvPtr       e = MidiNewEv(typeNote);
        long            dt = MidiGetTime();
        
        if (e) {
                Chan(e) = 0;
                Port(e) = 0;
                MidiSetField(e, 0, pitch);      // the pitch
                MidiSetField(e, 1, 80);         // the velocity
                MidiSetField(e, 2, delay-1);    // the duration
                
                for (i=0; i<n; i++) MidiSendAt(ourRefNum, MidiCopyEv(e), dt + i*delay);
                MidiFreeEv(e);
        }
}

void test15 ()
{
        printf("\n\n<TEST 15> send many notes in the future\n");
        DebugStr ("--> use g or G to send many notes in the future and go to the next test\n");

        SendMultipleNotes(10, 60, 250);
        
        printf("\n\n<TEST 15 bis> send more notes in the future\n");
        DebugStr ("--> use g or G to send more notes in the future and go to the next test\n");

        SendMultipleNotes(6, 60, 800);
        SendMultipleNotes(8, 72, 600);
}


//===================================================================================
// test 16 : send 'hello' lyric
//===================================================================================
// MidiShare defines several types of events. Some of them correspond to real Midi
// messages, some others (like notes) are translated to Midi messages and some others
// correspond to Midi File 1.0 data. Here is an example of sending Midi File Lyrics
// data to another application. 
//
// <<<< For this test you need to launch 'msDisplay' application (in the 'MidiShare suite' folder) 


void sendLyric(char* c)
{
        MidiEvPtr e = MidiNewEv(typeLyric);
        
        if (e) {
                Chan(e) = 0;                            // channels have numbers from 0 to 15
                Port(e) = 0;                            // port 0 is Modem port, port 1 is Printer port
                while (*c) MidiAddField(e, *c++);       // Fill the event with the string
                
                MidiSendIm(ourRefNum, e);
        }
}

void test16 ()
{
        printf("\n\n<TEST 16> send a Lyric to 'msDisplay'\n");
        printf("YOU NEED TO LAUNCH 'msDisplay' FOR THIS TEST!!!\n");
        DebugStr ("--> use g or G to send a Lyric to 'msDisplay' and go to the next test\n");

        if (MidiGetNamedAppl ("msDisplay") < 0) {
                printf("You need to launch msDisplay for this test\n");
        } else {
                MidiConnect(0, MidiGetNamedAppl ("msDisplay"), false);
                MidiConnect(ourRefNum, MidiGetNamedAppl ("msDisplay"), true);
                sendLyric("Hello");
        }
}
// msDisplay shows the 'Hello' message in its window


//===================================================================================
// test 17 : send a text event
//===================================================================================
// Here is a more general way to send Midi File textual events 


void sendText(int aTextType, char* c)
{
        MidiEvPtr       e = MidiNewEv(aTextType);
        
        if (e) {
                Chan(e) = 0;                            // channels have numbers from 0 to 15
                Port(e) = 0;                            // port 0 is Modem port, port 1 is Printer port
                while (*c) MidiAddField(e, *c++);       // Fill the event with the string
                
                MidiSendIm(ourRefNum, e);
        }
}

void test17 ()
{
        printf("\n\n<TEST 17> send other textual events\n");
        DebugStr ("--> use g or G to send other textual events to 'msDisplay' and go to the next test\n");

        sendText (typeTextual, "Hello");
        sendText (typeCopyright, "Mozart");
        sendText (typeSeqName, "Concerto");
        sendText (typeInstrName, "Piano");
        sendText (typeLyric, "Hiiiiiii");
        sendText (typeMarker, "mark 1");
        sendText (typeCuePoint, "Reverb here");
}


//===================================================================================
// test 18 : send an SMPTE offset
//===================================================================================

void sendSMPTEOffset (long format, long hours, long minutes, long seconds, long frames, long subframes)
{
        MidiEvPtr       e = MidiNewEv(typeSMPTEOffset);
        
        if (e) {
                Chan(e) = 0;                            // channels have numbers from 0 to 15
                Port(e) = 0;                            // port 0 is Modem port, port 1 is Printer port
        	MidiSetField(e, 0, format);             
        	MidiSetField(e, 1, hours);              
        	MidiSetField(e, 2, minutes);            
        	MidiSetField(e, 3, seconds);            
        	MidiSetField(e, 4, frames);             
        	MidiSetField(e, 5, subframes);  
                
                MidiSendIm(ourRefNum, e);
        }
}

void test18 ()
{
        printf("\n\n<TEST 18> send an SMPTE offset\n");
        DebugStr ("--> use g or G to send an SMPTE offset to 'msDisplay' and go to the next test\n");

        sendSMPTEOffset(1, 10, 24, 59, 12, 00); 
}

//===================================================================================
// test 19 : thru incomming events
//===================================================================================
// We are now going to see how to receive events. Every client application have a 
// reception fifo where incoming events are stored. Applications can be informed
// in real-time of incoming events using a receive alarm. The receive alarm is called
// at interrupt level when new events are received. In this example, the receive alarm
// just thru the events
//
// WARNING : don't set breakpoints in the 'thru' routine!!!!. It is called
// at interrupt level

void thru (short aRefNum)
{
        MidiEvPtr e;
        
        while (e = MidiGetEv(aRefNum)) MidiSend(aRefNum, e);
}

void test19 ()
{
        printf("\n\n<TEST 19> thru incoming events\n");
        DebugStr ("--> use g or G to start thruing incoming events\n");

        MidiFlushEvs(ourRefNum);
  //      MidiSetRcvAlarm(ourRefNum, thru);
        MidiConnect(0, ourRefNum, true);
        MidiConnect(ourRefNum, 0, true);

        printf("Now play on the Midi keyboard\n");
        DebugStr ("--> use g or G to stop thruing incoming events and go to the next test\n");

        MidiConnect(0, ourRefNum, false);
 //       MidiSetRcvAlarm(ourRefNum, 0);
}

//===================================================================================
// test 20 : delay and transpose incoming notes
//===================================================================================
// In this example, note, keyOn and keyOff events are transposed and sent with a delay. 
// Other received events are deleted. 

 void transposeAndDelay (short aRefNum)
{
        MidiEvPtr e;
        
        while (e = MidiGetEv(aRefNum)) {
                switch (EvType(e)) {
                        case typeNote:
                        case typeKeyOn:
                        case typeKeyOff:
                                MidiSetField(e, 0, 7 + MidiGetField(e,0));      // transpose pitch + 7
                                Date(e) += 1000;                                // delay 1000 ms
                                MidiSend(aRefNum, e);
                                break;
                        default:
                                MidiFreeEv(e);
                }
        }
}

void test20 ()
{
        printf("\n\n<TEST 20> transpose and delay in real-time\n");
        DebugStr ("--> use g or G to start transposing incoming events\n");

        MidiFlushEvs(ourRefNum);
//        MidiSetRcvAlarm(ourRefNum, transposeAndDelay);
        MidiConnect(0, ourRefNum, true);
        MidiConnect(ourRefNum, 0, true);

        printf("Now play on the Midi keyboard, notes are transposed and delayed\n");
        DebugStr ("--> use g or G to stop transposing events and go to the next test\n");

        MidiConnect(0, ourRefNum, false);
    //    MidiSetRcvAlarm(ourRefNum, 0);
}

//===================================================================================
// test 21 : record and playback a sequence
//===================================================================================
// We are now going to see how to record incoming events in a sequence and play back
// this sequence. To playback we use a task, a function called in the future.

void InstallFilter (short aRefNum, MidiFilterPtr aFilter)
{
        short i;

        for (i = 0; i<256; i++)                                                 /* initialise the filter :                      */
        {                                                                               
                MidiAcceptType(aFilter,i,true);                           /*  accept every MidiShare events       */
                MidiAcceptPort(aFilter,i,true);                                     /*  on every Midi ports                         */
        }
                                                                                        
        for (i = 0; i<16; i++)
                MidiAcceptChan(aFilter,i,true);                          /*  on every Midi channels                      */
                
        MidiAcceptType(aFilter, typeActiveSens,false);             /* reject active sensing events         */
                
        MidiSetFilter( aRefNum, aFilter );                      /* install the filter                           */
}

 void record (short aRefNum)
{
        MidiEvPtr e;
        
        while (e = MidiGetEv(aRefNum)) {
                MidiSendIm(aRefNum, MidiCopyEv(e)); // Remove this line if you don't want to thru events
                MidiAddSeq(ourSequence,e);
        }
}

 void play (long time, short refnum, long nextEv, long unused1, long unused2)
{
        MidiEvPtr       e = (MidiEvPtr) nextEv;
        long            date = Date(e);
        
        if (!stopFlag) {
                while (e && Date(e) == date) {                  // play all the events at the same date
                        MidiSendIm(refnum, MidiCopyEv(e));
                        e = e->link;
                }
                if (e)                                                                  // still events to play?
                        MidiCall(play, time + Date(e) - date, refnum, (long)e, 0, 0);
        }
}

long size (MidiSeqPtr s)
{
        long            n;
        MidiEvPtr       e;
        
        for (n = 0, e = FirstEv(s); e; e = e->link) n++;
        return n;
}

void test21 ()
{
        long    n;
        
        printf("\n\n<TEST 21> record and playback a sequence\n");
        DebugStr ("--> use g or G to start recording\n");

        ourSequence = MidiNewSeq();
    	ourFilter = MidiNewFilter();
        InstallFilter(ourRefNum,ourFilter);
        MidiFlushEvs(ourRefNum);
        MidiSetRcvAlarm(ourRefNum, record);
        MidiConnect(0, ourRefNum, true);
        MidiConnect(ourRefNum, 0, true);
        
        printf("Now play on the Midi keyboard, every thing is recorded\n");
        DebugStr ("--> use g or G to stop recording\n");

        MidiConnect(0, ourRefNum, false);
        MidiSetRcvAlarm(ourRefNum, 0);
        
        printf (" %ld events where recorded\n", size(ourSequence));

        DebugStr ("--> use g or G to play back the sequence\n");

        stopFlag = 0;
        if (FirstEv(ourSequence)) {
                MidiCall(play, MidiGetTime(), ourRefNum, (long) FirstEv(ourSequence), 0, 0);
        }
        
        printf("Now playing the sequence\n");
        DebugStr ("--> use g or G to stop play back and go to the next test\n");
        
        stopFlag = 1;
        MidiFreeSeq(ourSequence);
}
        

//===================================================================================
// test 22 : close MidiShare Session
//===================================================================================

void test22 ()
{
        printf("\n\n<TEST 22> close the MidiShare session\n");
        DebugStr ("--> use g or G to end the session and exit the tutorial\n");
		
		MidiFreeFilter(ourFilter);
        MidiClose(ourRefNum);
        printf("\nTHE TUTORIAL IS NOW FINISHED!\n");
}

int main(void)
{
        welcome();
        test1();	
        test2();	
        test3();
        test4();		
        test5();		
        test6();		
        test7();	
        test8();	
        test9();	
        test10();	
        test11();	
        test12();	
        test13();
        test14();	
        test15();
        test16();
        test17();
        test18();
        test19();
     /*   test20();
        test21();       
        test22();*/
}
