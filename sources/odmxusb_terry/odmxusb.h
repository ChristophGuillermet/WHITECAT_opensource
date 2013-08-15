/*
**
**	Open_USB_DMX static library for Win32
**

This interface library sends DMX data via the "Enttec Open USB to DMX" hardware.

Requirements
===========
FTD2XX.dll runtime dll must be distributed with the final library

Credits
=======
Written by Terry Wells (Long Beach, CA, USA)
Based on DLL by Martin Searancke of LightFactory Software Ltd (Auckland, New Zealand)
Based on example originally written by Nicolas Moreau of Enttec Pty Ltd. (Melbourne Australia)
	
*/


#ifndef t_dmx_senderH
#define t_dmx_senderH

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include "FTD2XX.H"

class Open_USB_DMX
{
	static DWORD WINAPI OutputDataThread(LPVOID lParam); // Thread to send dmx data continiously
	void SendDataToHardware(int universe);
	
	bool m_connected;
	bool m_killThread;		// Flag used to kill the thread
	FT_HANDLE ftHandle; 
	int ftDeviceCount;

	HANDLE m_hThread;
	unsigned char m_DMXData_frame[512];

public:
	Open_USB_DMX();
	~Open_USB_DMX();

	int open_dmx_devices();
	void close_dmx_devices();
	int get_dmx_device_count() const { return ftDeviceCount; }
	void send_dmx_packet(unsigned char* data);
};

#endif
