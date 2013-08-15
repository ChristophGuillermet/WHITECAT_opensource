// Project: odmxusb.lib
// Interface library for Enttec Open USB to DMX device
// *************************************************
// open_dmxusb.cpp  

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



#include "odmxusb.h"

// ***********************************************************************
// Class stuff
// ***********************************************************************

// Constructor
Open_USB_DMX::Open_USB_DMX() 
{ 
	ftDeviceCount = 0;
	m_connected = false;
	m_hThread = NULL;
}

// Destructor
Open_USB_DMX::~Open_USB_DMX()
{
	close_dmx_devices();
}

int Open_USB_DMX::open_dmx_devices()
{
    ftDeviceCount = 0;

	FT_STATUS ftStatus;

	if (!m_connected) {

		ftStatus = FT_Open(0, &ftHandle);
		if(!FT_SUCCESS(ftStatus))			
			return 0;

	    ftDeviceCount = 1;

		// reset the devices
	    ftStatus = FT_ResetDevice(ftHandle);
	    if (!FT_SUCCESS(ftStatus)) 
			return -1; // Device not responding

		// Set the baud rate 12 will give us 250Kbits
		ftStatus = FT_SetDivisor(ftHandle,12);
	    if (!FT_SUCCESS(ftStatus)) 
			return -2; // Set baud rate error

		// Set the data characteristics
	    ftStatus = FT_SetDataCharacteristics(ftHandle,FT_BITS_8,FT_STOP_BITS_2,FT_PARITY_NONE);
	    if (!FT_SUCCESS(ftStatus)) 
			return -3; // Set Data Characteristics error

		// Set flow control
		ftStatus = FT_SetFlowControl(ftHandle, FT_FLOW_NONE,0, 0 );
	    if (!FT_SUCCESS(ftStatus)) {
			return -4;
		}

		// set RS485 for sending
		FT_ClrRts(ftHandle);

		// Clear TX RX buffers
	    FT_Purge(ftHandle,FT_PURGE_TX | FT_PURGE_RX);

		Sleep(1000L);

		// Start the thread
		DWORD thrId;
		m_hThread = CreateThread(NULL, 0, OutputDataThread, this, 0, &thrId);
		if (!m_hThread) {
			close_dmx_devices();
			return -5;
		}

		m_connected = true;

		return ftDeviceCount;
	}

	return 0;
}

void Open_USB_DMX::close_dmx_devices()
{
	if (m_hThread) {
		// Kill off the output thread
		m_killThread = true;
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	if ((m_connected) && (ftDeviceCount>0)) {
		FT_Close(ftHandle);
	}

	ftDeviceCount = 0;
	m_connected = false;
}

void Open_USB_DMX::send_dmx_packet(unsigned char* data)
{
	memcpy(m_DMXData_frame, data, 512);
}

DWORD WINAPI Open_USB_DMX::OutputDataThread(LPVOID lParam)
{
	Open_USB_DMX *oud = (Open_USB_DMX *)lParam;

	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
	oud->m_killThread = false; // flag used to stop the thread
	while (!oud->m_killThread) { // run until "kill" flag is set
		oud->SendDataToHardware(0);
		Sleep(30);
	}

	return 0;
}

void Open_USB_DMX::SendDataToHardware(int universe)
{
	ULONG bytesWritten;

	FT_SetBreakOn(ftHandle);
    FT_SetBreakOff(ftHandle);

	unsigned char StartCode = universe;
	FT_Write(ftHandle, &StartCode, 1, &bytesWritten);
	FT_Write(ftHandle, m_DMXData_frame, 512, &bytesWritten);
}

