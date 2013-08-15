/*

  Copyright © Grame 2001

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

import java.awt.*;
import java.util.*;
import java.applet.Applet;
import java.awt.event.*;
import java.applet.*;

import grame.midishare.*;


public class TutorialPart1 extends Applet 
 {
    TextArea text;
	Button start, go;
	int testnumber ;
	int ourRefNum  = -1;

	 public void init() 
	 {
		testnumber = 0;
		ourRefNum = -1;
	
		Panel bottomPanel = new Panel();
		Panel topPanel = new Panel();
		go = new Button ("Next");

		setLayout(new BorderLayout());
		topPanel.setLayout(new GridLayout(2,1,1,1));

		topPanel.add (new Label ("Tutorial Part1"));

		add("North",topPanel);

		text = new  TextArea(1,1);
		text.appendText( "Welcome to the MidiShare Java tutorial ! ");
		add("Center",text);

		bottomPanel.setLayout(new GridLayout(1,2,5,5));
		bottomPanel.add (go);
		add("South",bottomPanel);
		 
		
		text.replaceText( "" , 0 , text.getText().indexOf( "\n" ) + 1 ) ;
		text.appendText( "\n\nYou are now ready to follow the tutorial");
		text.appendText( "\nUse NEXT button to execute the next test\n");
		testnumber = 0;
	}
	
	public void stop() 
	{
	 	if (ourRefNum > 0) Midi.Close(ourRefNum);
	 	ourRefNum = -1;
	}
	
	void test1()
	{
		text.appendText("\n <TEST 1> : Check if MidiShare is installed : calling the Midi.Share() function is REQUIRED to properly load the JMidi native library");
		text.appendText("\n");
		text.appendText("MidiShare installed : ");
		text.appendText(String.valueOf(Midi.Share()));
		text.appendText("\n");
		text.appendText("1 means that MidiShare is installed\n");
		text.appendText("0 means that MidiShare is not installed");
	}

	void test2()
	{
		text.appendText("\n\n<TEST 2> Get MidiShare version\n");
		text.appendText("MidiShare version :  ");
		text.appendText (String.valueOf(Midi.GetVersion()));
	}
	
	void test3()
	{
		text.appendText("\n\n<TEST 3> Count MidiShare client applications\n");
		text.appendText("Count of MidiShare client applications : ");
		text.appendText( String.valueOf(Midi.CountAppls()));
	}
	
	void test4()
	{
		text.appendText("\n\n<TEST 4> Register a client application\n");
		ourRefNum = Midi.Open("Tutorial");
		if (ourRefNum > 0){
			text.appendText("MidiOpen succeed. Our reference number is : ");
			text.appendText( String.valueOf(ourRefNum));
		}else{
			text.appendText("MidiOpen fail. The error code is : ");
			text.appendText( String.valueOf(ourRefNum));
		}
	}
	
	void test5()
	{
		text.appendText("\n\n<TEST 5> Count again MidiShare client applications \n");
		text.appendText("Count of MidiShare client applications : ");
		text.appendText(String.valueOf(Midi.CountAppls()));
	}
	
	void listOfAppl()
	{
		short i;
		int ref, n = Midi.CountAppls();
	
		text.appendText("List of MidiShare client applications\n");
		text.appendText("-------------------------------------\n");
		for (i = 1; i<= n; i++) {
			ref = Midi.GetIndAppl(i);			// get the refnum from the order number
			text.appendText(String.valueOf(i));
			text.appendText(" : reference number ");
			text.appendText(String.valueOf(ref));
			text.appendText(" name : ");
			text.appendText(Midi.GetName(ref));
			text.appendText("\n");
		}
		text.appendText("-------------------------------------");
	}
	
	void test6()
	{
		text.appendText("\n\n<TEST 6> List every MidiShare client applications\n");
		listOfAppl();
	}
	
	void test7()
	{
		text.appendText("\n\n<TEST 7> search the reference number of: ");
		text.appendText("Tutorial");
		text.appendText("\n");
		text.appendText("Reference number of : ");
		text.appendText("Tutorial");
		text.appendText(" : ");
		text.appendText(String.valueOf (Midi.GetNamedAppl("Tutorial")));
	}
	
	void test8()
	{
		text.appendText("\n\n<TEST 8> search the reference number of 'XYZ'\n");
		text.appendText("Reference number of  XYZ: ");
		text.appendText(String.valueOf (Midi.GetNamedAppl("XYZ")));
		text.appendText("\n");
		text.appendText("A negative result means that the application was not found.");
	}
	
	void test9()
	{
		text.appendText("\n\n<TEST 9> Change the name of client application Tutorial to JavaTutorial\n");
		Midi.SetName (Midi.GetNamedAppl ("Tutorial"), "JavaTutorial");
		listOfAppl();
	}
	
	void test10()
	{
		text.appendText("\n\n<TEST 10> Connect JavaTutorial to MidiShare\n");
		Midi.Connect (ourRefNum, 0, 1);
		
	}
	
	void test11()
	{
		text.appendText("\n\n<TEST 11> Test if JavaTutorial is connected to MidiShare\n");
		text.appendText("Connection to MidiShare : ");
		text.appendText(String.valueOf(Midi.IsConnected (ourRefNum, 0)));
		text.appendText("\n");
		text.appendText("Connection from MidiShare : ");
		text.appendText(String.valueOf(Midi.IsConnected (0,ourRefNum)));
	}
	
	void listDest(int ref1)
	{
		short i;
		int ref2, n = Midi.CountAppls();
	
		text.appendText("List of the destinations of ");
		text.appendText(String.valueOf(ref1));
		text.appendText("\n");
		text.appendText("-------------------------------------\n");
		for (i = 1; i<= n; i++) {
		
			ref2 = Midi.GetIndAppl(i);	// get the refnum from the order number
			
			if (Midi.IsConnected( ref1, ref2) == 1) {
				text.appendText("--> ");
				text.appendText(Midi.GetName(ref2));
				text.appendText(" refnum = " );
				text.appendText(String.valueOf(ref2));
				text.appendText("\n");
			}
		}
		text.appendText("-------------------------------------");
	}
	
	void test12()
	{
		text.appendText("\n\n<TEST 12> List the destinations of an application\n");
		listDest (ourRefNum);
	}
	
	void listSrc(int ref1)
	{
		short i;
		int ref2,	n = Midi.CountAppls();
	
		text.appendText("List of the sources of ");
		text.appendText(String.valueOf(ref1));
		text.appendText("\n");
		text.appendText("-------------------------------------\n");
		for (i = 1; i<= n; i++) {
		
			ref2 = Midi.GetIndAppl(i);	// get the refnum form the order number
			
			if (Midi.IsConnected( ref2, ref1) == 1) {
				text.appendText("<-- ");
				text.appendText(Midi.GetName(ref2));
				text.appendText(" refnum = " );
				text.appendText(String.valueOf(ref2));
				text.appendText("\n");
			}
		}
		text.appendText("-------------------------------------");
	}
		
	void test13()
	{
		text.appendText("\n\n<TEST 13> List the sources of an application\n");
		listSrc (0);
	}
		
	void sendNote(int pitch) 
	{
		int event = Midi.NewEv(Midi.typeNote);  // ask for a new note event
		
		if (event != 0) {				// if the allocation was succesfull
			Midi.SetChan(event,0); 			// set the Midi channel
			Midi.SetPort(event,0); 			// set the destination port
			Midi.SetField(event,0,pitch); 		// set the pitch field
			Midi.SetField(event,1,80); 		// set the velocity field
			Midi.SetField(event,2,1000); 		// set the duration field
			Midi.SendIm(ourRefNum, event);  	// send the note immediatly
		}
	}
	
	void test14()
	{	int ref;
		text.appendText("\n\n<TEST 14>Send a note with pitch, velocity and duration\n");
		if ((ref = Midi.GetNamedAppl("msdisplay")) > 0)
			Midi.Connect(ourRefNum,ref, 1);
		sendNote (72); 
	}
	
	void sendMultipleNotes (int n, int pitch, int delay) 
	{
		int event = Midi.NewEv(Midi.typeNote);  	// ask for a new note event
		int date = Midi.GetTime();              	// remember the current time
		
		if (event != 0) {				// if the allocation was succesfull
			Midi.SetChan(event,0); 			// set the Midi channel
			Midi.SetPort(event,0); 			// set the destination port
			Midi.SetField(event,0,pitch); 		// set the pitch field
			Midi.SetField(event,1,80); 		// set the velocity field
			Midi.SetField(event,2,(delay - 1)); 	// set the duration field
			
			for (int i = 0; i< n ; i++) {			// ; loop for the requested number of events
				Midi.SendAt(ourRefNum, Midi.CopyEv(event), (date + (i * delay))); // send a copy of the original note
			}
			Midi.FreeEv (event);  // dispose the original note
		}
	}
	
	void test15()
	{
		text.appendText("\n\n<TEST 15>Send multiple notes\n");
		sendMultipleNotes (10, 72, 1000); 
	}
	
	void sendLyric(String str) 
	{
		int event = Midi.NewEv(Midi.typeLyric); 	// ask for a new lyric event
			
		if (event != 0) {				// if the allocation was succesfull
			Midi.SetChan(event,0); 			// set the Midi channel
			Midi.SetPort(event,0); 			// set the destination port
			Midi.SetText(event, str);		// set the text field
			Midi.SendIm(ourRefNum, event);  	// send the event immediatly
		}
	}
	
	void test16()
	{
		text.appendText("\n\n<TEST 16>Send 'hello' lyric\n");
		sendLyric("Hello"); 
	}
		
	void sendText(int type, String str) 
	{
		int event = Midi.NewEv(type);  			// ask for a event
			
		if (event != 0) {				// if the allocation was succesfull
			Midi.SetChan(event,0); 			// set the Midi channel
			Midi.SetPort(event,0); 			// set the destination port
			Midi.SetText(event, str);		// set the text field
			Midi.SendIm(ourRefNum, event);  	// send the event immediatly
		}
	}
	
	void test17()
	{
		text.appendText("\n\n<TEST 17>Send text events\n");
		sendText (Midi.typeText, "Hello");
		sendText (Midi.typeCopyright, "Mozart");
		sendText (Midi.typeSeqName, "Concerto");
		sendText (Midi.typeInstrName ,"Piano");
		sendText (Midi.typeLyric, "Hiiiiiii");
		sendText (Midi.typeMarker, "mark 1");
		sendText (Midi.typeCuePoint, "Reverb here") ;
	}
	
	void listOfDrivers()
	{
		int ref, n = Midi.CountDrivers();

		text.appendText("\n\nList of MidiShare drivers\n");
		text.appendText("-------------------------------------\n");
		for (int i = 1; i<= n; i++) {
			ref = Midi.GetIndDriver(i);		// get the refnum from the order number
			printDriverInfo(ref);
			text.appendText("-------------------------------------\n");
		}
	}
	
	void printDriverInfo(int ref)
	{
		DriverInfos infos = new DriverInfos();
		SlotInfos slot = new SlotInfos();
		
		Midi.GetDriverInfos(ref,infos);	// get info for the driver
		text.appendText("Reference number ");
		text.appendText(String.valueOf(ref));
		text.appendText("\nname : ");
		text.appendText(infos.name);
		text.appendText("\nslots : ");
		text.appendText(String.valueOf(infos.slots));
		text.appendText("\nversion : ");
		text.appendText(String.valueOf(infos.version));
		text.appendText("\n");
		
		for (int i = 0 ;i < infos.slots; i++) {
			Midi.GetSlotInfos(Midi.GetIndSlot(ref,i+1), slot);
			text.appendText("slot name : " );
			text.appendText(slot.name);
			text.appendText("\nslot direction : ");
			text.appendText(String.valueOf(slot.direction));
			text.appendText("\n");
		}
	}

	void test18()
	{
		text.appendText( "\n\n<TEST 18>Display MidiShare drivers informations");
	 	listOfDrivers();
	}
	
	void printPortInfo(int ref, int port)
	{
		DriverInfos infos = new DriverInfos();
		SlotInfos slot = new SlotInfos();
		Midi.GetDriverInfos(ref,infos);	// get info for the driver
			
		for (int i = 0 ;i < infos.slots; i++) {
			int slotref = Midi.GetIndSlot(ref,i+1);
			int res = Midi.IsSlotConnected(port, slotref);
			if (res > 0) {  // if port is connected this this slot
				text.appendText("\n-------------------------------------\n");
				Midi.GetSlotInfos(slotref, slot);
				text.appendText("port number : " );
				text.appendText(String.valueOf(port));
				text.appendText("\nslot name : " );
				text.appendText(slot.name);
				text.appendText("\nslot direction : ");
				text.appendText(String.valueOf(slot.direction));
				text.appendText("\n");
			}
		}
	}
	
	void test19()
	{
		text.appendText( "\n\n<TEST 19>Display port to slot connection state");
		int ref, n = Midi.CountDrivers();
		
		for (int i = 1; i<= n; i++) {
			ref = Midi.GetIndDriver(i);		// get the refnum from the order number
			printPortInfo(ref,0);			// display connection state of port 0
			printPortInfo(ref,1);			// display connection state of port 1
		}	 	
	}
	
	void connectPort(int ref, int port)
	{
		DriverInfos infos = new DriverInfos();
		Midi.GetDriverInfos(ref,infos);	// get info for the driver
			
		for (int i = 0 ;i < infos.slots; i++) {
			int slotref = Midi.GetIndSlot(ref,i+1);
			Midi.ConnectSlot(port,slotref,1);
		}
	}
	
	void test20()
	{
		text.appendText( "\n\n<TEST 20>Connect port to slot and dsiplay new connection state");
		int ref, n = Midi.CountDrivers();
		
		for (int i = 1; i<= n; i++) {
			ref = Midi.GetIndDriver(i);		// get the refnum from the order number
			connectPort (ref,10);		    // connect port 10 to all slots of all drivers
		}
		
		for (int i = 1; i<= n; i++) {
			ref = Midi.GetIndDriver(i);		// get the refnum from the order number
			printPortInfo(ref,10);			// display connection state of port 10
		}
	}
	
	void test21()
	{
		text.appendText("\n\n<TEST 21>Close the JavaTutorial application\n");
		Midi.Close (ourRefNum);
		listOfAppl ();
	}

	public boolean action( Event e , Object o) 
	{
		if (e.target == go) {
			testnumber++;
			switch (testnumber) {
				
				case 1 : test1(); break;
				case 2 : test2(); break;
				case 3 : test3(); break;
				case 4 : test4(); break;
				case 5 : test5(); break;
				case 6 : test6(); break;
				case 7 : test7(); break;
				case 8 : test8(); break;
				case 9 : test9(); break;
				case 10 : test10(); break;
				case 11 : test11(); break;
				case 12 : test12(); break;
				case 13 : test13(); break;
				case 14 : test14(); break;
				case 15 : test15(); break;
				case 16 : test16(); break;
				case 17 : test17(); break;
				case 18 : test18(); break;
				case 19 : test19(); break;
				case 20 : test20(); break;
				case 21 : test21(); break;
			}
			return true;
		}
		
	 	return true;
	}
	
	public static void main(String args[])
	{
		java.util.Hashtable params = new java.util.Hashtable();
		
		// Create the applet.
		TutorialPart1 applet = new TutorialPart1();
	
		// And fire up the applet frame.
		AppletFrame.startApplet(applet, "TutorialPart1", params, 400, 300);
	}

}

