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
  grame@grame.fr
  
  modifications history:

*/

#include <windows.h>
#include <MidiShare.h>
#include "mscnct.h"

// ------------------------------------------------------------------
// Constants  - types
// ------------------------------------------------------------------

#define kApplAlarm	WM_USER

#define kMaxAppls		64
#define kMaxNameLen		32
#define noRef			-1

#define kDefaultXPos	10
#define kDefaultYPos	30
enum { false, true };

typedef short	RefList	[kMaxAppls];

// ------------------------------------------------------------------
// PROTOTYPES
// ------------------------------------------------------------------
void 		NEAR 	AlertUser(LPCSTR);
Boolean 	NEAR 	SetUpMidi(void);

void MSALARMAPI ApplAlarm(short,long);
BOOL CALLBACKAPI msCnctDlg(HWND hDlg, UINT message, UINT wParam, LONG lParam);
BOOL CALLBACKAPI AboutDlgProc(HWND hDlg,UINT message,
													UINT wParam,LONG lParam);


// ------------------------------------------------------------------
// VARIABLES
// ------------------------------------------------------------------
#ifdef WIN32
#pragma data_seg(".global")
DWORD	gInstanceCount = 0;
#pragma data_seg()
#endif

HINSTANCE	ghInst;
HWND		abouthDlg;
HWND	FAR ghDlg;
DLGPROC		AboutDlgProcInst;

RefList	gRefNums;
short		myRefNum, selectedRef= noRef;
MidiFilterPtr	myFilter;
#ifndef WIN32
TFilter	gFilter;
#endif

UINT		gStartupMsg;
MidiName	AppliName = "msConnect";
char * profileName = "msConnect32.ini";
char * sectionName = "Window position";
char * xEntryName  = "xpos";
char * yEntryName  = "ypos";

WORD 		dlgXPos, dlgYPos;

//________________________________________________________________________
void SaveState (void)
{
	char buff[30];

	wsprintf (buff, "%d", dlgXPos);
	WritePrivateProfileString (sectionName, xEntryName, buff, profileName);
	wsprintf (buff, "%d", dlgYPos);
	WritePrivateProfileString (sectionName, yEntryName, buff, profileName);
}

//________________________________________________________________________
void LoadState (void)
{
	dlgXPos = GetPrivateProfileInt (sectionName, xEntryName, kDefaultXPos, profileName);
	dlgYPos = GetPrivateProfileInt (sectionName, yEntryName, kDefaultYPos, profileName);
	if (!dlgXPos) dlgXPos =  kDefaultXPos;
	if (!dlgYPos) dlgYPos =  kDefaultYPos;
}

/* ------------------------------------------------------------------
			application main
------------------------------------------------------------------ */
int PASCAL WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
						  LPSTR lpszCmdLine, int nCmdShow)
{
  if ( hPrevInstance ) return FALSE ;
#ifdef WIN32
  gStartupMsg = RegisterWindowMessage("msConnectToFront");
  if (gInstanceCount) {
	PostMessage (HWND_BROADCAST, gStartupMsg, 0, 0);
	return TRUE;
  }
  InterlockedIncrement (&gInstanceCount);
#endif
  ghInst = hInstance;
  if ( !SetUpMidi() ) return FALSE;
  DialogBox(hInstance, "CNCTDLG", NULL, MakeProcInstance(msCnctDlg,hInstance));
  FreeProcInstance((FARPROC)msCnctDlg);
  SaveState();
  MidiClose(myRefNum);
#ifdef WIN32
  MidiFreeFilter (myFilter);
#endif
  return TRUE;
}


/* ------------------------------------------------------------------
				initialization
------------------------------------------------------------------ */
void AddName (HWND hDlg, char *name)
{
	SendDlgItemMessage(hDlg, IDD_LB_SRC, LB_ADDSTRING,0,(LPARAM)name);
	SendDlgItemMessage(hDlg, IDD_LB_APL, LB_ADDSTRING,0,(LPARAM)name);
	SendDlgItemMessage(hDlg, IDD_LB_DST, LB_ADDSTRING,0,(LPARAM)name);
}

void ChangeName (HWND hDlg, short index, char *name)
{
	SendDlgItemMessage(hDlg, IDD_LB_SRC, LB_INSERTSTRING,index,(LPARAM)name);
	SendDlgItemMessage(hDlg, IDD_LB_SRC, LB_DELETESTRING,index+1,0);

	SendDlgItemMessage(hDlg, IDD_LB_APL, LB_INSERTSTRING,index,(LPARAM)name);
	SendDlgItemMessage(hDlg, IDD_LB_APL, LB_DELETESTRING,index+1,0);

	SendDlgItemMessage(hDlg, IDD_LB_DST, LB_INSERTSTRING,index,(LPARAM)name);
	SendDlgItemMessage(hDlg, IDD_LB_DST, LB_DELETESTRING,index+1,0);
}

//________________________________________________________________________
BOOL Initialize (HWND hDlg)
{
	short i, r;
	char appName[kMaxNameLen];

	selectedRef= noRef;
	SendDlgItemMessage(hDlg, IDD_LB_SRC, LB_RESETCONTENT,0,0L);
	SendDlgItemMessage(hDlg, IDD_LB_APL, LB_RESETCONTENT,0,0L);
	SendDlgItemMessage(hDlg, IDD_LB_DST, LB_RESETCONTENT,0,0L);
	for (i=0; i<kMaxAppls; i++)
		gRefNums[i] = noRef;
	for (i=0; i<MidiCountAppls(); i++) {
		gRefNums[i] = r = MidiGetIndAppl(i+1);
		wsprintf(appName, MidiGetName (r));
		AddName (hDlg, appName);
	}
	return TRUE ;
}

/* ------------------------------------------------------------------
	Convert from an index to a MidiShare reference number and so on...
	------------------------------------------------------------------ */
short Index2MSRef (short index)
{
	return  gRefNums[index];
}

short MSRef2Index (short ref)
{
	short i=0;

	while (i < kMaxAppls) {
		if (gRefNums[i] == ref)
			return i;
		i++;
	}
	return noRef;
}

/* ------------------------------------------------------------------ */
void DisplayConnections (HWND hDlg, short ref)
{
	short i;

	for (i=0; i<MidiCountAppls(); i++) {
		SendDlgItemMessage(hDlg, IDD_LB_SRC, LB_SETSEL,
						MidiIsConnected(Index2MSRef(i), ref),MAKELPARAM(i,0));
		SendDlgItemMessage(hDlg,IDD_LB_DST,LB_SETSEL,
						MidiIsConnected(ref, Index2MSRef(i)),MAKELPARAM(i,0));
	}
}

/* ------------------------------------------------------------------ */
void UpdateConnections (HWND hDlg, short ref)
{
	short i;

	i = MSRef2Index (ref);
	if (ref== selectedRef)
		DisplayConnections (hDlg, ref);
	else if (ref!= noRef) {
		SendDlgItemMessage(hDlg, IDD_LB_SRC, LB_SETSEL,
							MidiIsConnected(ref, selectedRef), MAKELPARAM(i,0));
		SendDlgItemMessage(hDlg, IDD_LB_DST, LB_SETSEL,
							MidiIsConnected(selectedRef, ref), MAKELPARAM(i,0));
	}
}

/* ------------------------------------------------------------------
	User connections  management
	------------------------------------------------------------------ */
void CnxManagement (HWND hDlg, WORD ctrl)
{
	short ref, sel;

//	switch (GetDlgCtrlID((HWND)lParam)) {
	switch (ctrl) {
		case IDD_LB_SRC:
			if (selectedRef!=noRef) {
				sel = (short)SendDlgItemMessage (hDlg, IDD_LB_SRC, LB_GETCARETINDEX,0,0L);
				ref = Index2MSRef (sel);
				if (ref != noRef)
					MidiConnect (ref, selectedRef, !MidiIsConnected (ref, selectedRef));
			}
			break;
		case IDD_LB_APL :
			sel = (short)SendDlgItemMessage (hDlg, IDD_LB_APL, LB_GETCURSEL,0,0L);
			ref = Index2MSRef (sel);
			if (ref != noRef)
				DisplayConnections (hDlg, ref);
			selectedRef = ref;
			break;
		case IDD_LB_DST :
			if (selectedRef!=noRef) {
				sel = (short)SendDlgItemMessage (hDlg, IDD_LB_DST, LB_GETCARETINDEX,0,0L);
				ref = Index2MSRef (sel);
				if (ref != noRef)
					MidiConnect (selectedRef, ref, !MidiIsConnected (selectedRef, ref));
			}
			break;
	}
}

//_____________________________________________________________________________
BOOL ProcessPrivateMessage (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	char s[kMaxNameLen];
	short i, ref;

	if (message == kApplAlarm) {
		ref = LOWORD(lParam);
		i = MSRef2Index (ref);
		switch ( wParam ) {
			case MIDIOpenAppl:
			case MIDICloseAppl:
				Initialize (hDlg);
				break;
			case MIDIChgName:
				if (i != noRef) {
					wsprintf(s, MidiGetName (ref));
					ChangeName (hDlg, i, s);
					UpdateConnections (hDlg, ref);
				}
				break;
			case MIDIChgConnect:
				UpdateConnections (hDlg, ref);
				break;
		}
	}
	else if (message == gStartupMsg) {
		if (IsIconic(hDlg))
			ShowWindow (hDlg, SW_RESTORE);
		else
			SetForegroundWindow (hDlg);
	}
	else return FALSE;
	return TRUE;
}

/* ------------------------------------------------------------------
				main window management
------------------------------------------------------------------ */
BOOL CALLBACKAPI msCnctDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
//	char s[kMaxNameLen];
//	short i, ref;

	if (ProcessPrivateMessage (hDlg, message, wParam, lParam))
		return TRUE;

	switch (message) {
		case WM_INITDIALOG:
			ghDlg = hDlg;
#ifdef WIN32
			SetClassLong (hDlg, GCL_HICON, (LONG)LoadIcon (ghInst, "ICON"));
			SendDlgItemMessage(hDlg, AboutButton, BM_SETIMAGE, 0, (LONG)LoadBitmap (ghInst, (char *)1800));	
#endif
			AboutDlgProcInst= (DLGPROC)MakeProcInstance((FARPROC)AboutDlgProc,ghInst);
			LoadState();
			SetWindowPos(hDlg, HWND_TOP, dlgXPos, dlgYPos,0,0,SWP_NOSIZE);
			return Initialize (hDlg);

		case WM_COMMAND:
			switch (LOWORD(wParam)){

				case IDD_QUITTER:
					if (abouthDlg)
						SendMessage (abouthDlg, WM_COMMAND, IDOK, 0);
					EndDialog(hDlg,TRUE);
					break;
				case AboutButton :
//					EnableWindow(GetDlgItem(ghDlg,AboutButton),false);
					DialogBox(ghInst, "AboutDlg", hDlg, AboutDlgProcInst);
					break;

				default:
					if (HIWORD(wParam)==LBN_SELCHANGE)
						CnxManagement (hDlg, LOWORD(wParam));
			}
			break;

		case WM_MOVE:
			dlgXPos= LOWORD(lParam) - 5;
			dlgYPos= HIWORD(lParam) - 24;
			break;
	}
	return FALSE ;
}

/* ----------------------------------------------------------------------------
			Alert messages
---------------------------------------------------------------------------- */
void NEAR AlertUser(LPCSTR mess)
{
	MessageBox(ghDlg, mess, AppliName,MB_OK|MB_ICONSTOP);
	SendMessage(ghDlg,WM_SYSCOMMAND,SC_CLOSE,0L);
}


/*---------------------------------------------------------------------------
		MidiShare part
  --------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
		Input filter installation
---------------------------------------------------------------------------- */
void NEAR InstallFilter(short refNum)
{
	register short i;

#ifdef WIN32
	myFilter = MidiNewFilter ();
#else
	myFilter = &gFilter;
#endif
	if (myFilter) {
		for (i=0;i<256;i++) {
			MidiAcceptPort(myFilter, i, false);
			MidiAcceptType(myFilter, i, false);
		}
		for (i=0;i<16;i++)
			MidiAcceptChan(myFilter, i, false);
		MidiSetFilter(refNum,myFilter);
	}
}

/*-------------------------------------------------------------------------- */
Boolean NEAR SetUpMidi()
{
	if ( !MidiShare() ) {
		AlertUser("Can't access to 'MSHARE.DLL'");
		return false;
	}
	myRefNum = MidiOpen(AppliName); 		// open a MidiShare application

	if ( myRefNum == MIDIerrSpace ) {
		AlertUser("Too much MidiShare client applications");
		return false;
	}
	InstallFilter(myRefNum);
	MidiSetApplAlarm(myRefNum, ApplAlarm);
	return true;
}

/* ----------------------------------------------------------------------------
		MidiShare application alarm

Parameters
----------
	refNum	: reference number of the MidiShare application
	code		: context modificaiton code

---------------------------------------------------------------------------- */
void MSALARMAPI ApplAlarm(short refNum,long code) {
  PostMessage(ghDlg, kApplAlarm, LOWORD(code), HIWORD(code));
}

BOOL CALLBACKAPI AboutDlgProc(HWND hDlg,UINT message, UINT wParam,LONG lParam)
{
	char buff[10]; BOOL ret;

	switch (message) {
		case WM_INITDIALOG : {
			abouthDlg = hDlg;
			wsprintf(buff,"%d.%02d",MidiGetVersion()/100,MidiGetVersion()%100);
			ret = SetDlgItemText(hDlg, Version, buff);
			return TRUE ;
		}
		case WM_COMMAND : {
			switch(wParam) {
				case IDOK :
					EnableWindow( GetDlgItem(ghDlg,AboutButton),true);
					EndDialog(hDlg,0);
					abouthDlg = 0 ;
					return TRUE;
			}
		}
		break;
	}
	return FALSE ;
}


