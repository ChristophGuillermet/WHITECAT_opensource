
#include "pro_driver_pro.h"


// old school globals
DMXUSBPROParamsType PRO_Params;
FT_HANDLE device_handle = NULL ;

#define DMX_DATA_LENGTH 513 // Includes the start code 

	// declare all needed variables 
	WORD wVID = 0x0403;
	WORD wPID = 0x6001;
	uint8_t Num_Devices =0;
	uint16_t device_connected =0;

	uint32_t length =DMX_DATA_LENGTH;
	int i=0;
	int device_num=0;
	BOOL resEnttecPro = 0;
	

/* Function : FTDI_ClosePort
 * Author	: ENTTEC
 * Purpose  : Closes the Open DMX USB PRO Device Handle
 * Parameters: none
 **/
void FTDI_ClosePort()
{
	if (device_handle != NULL)
		FT_Close(device_handle);
}

/* Function : FTDI_ListDevices
 * Author	: ENTTEC
 * Purpose  : Returns the no. of PRO's conneced to the PC
 * Parameters: none
 **/
int FTDI_ListDevices()
{
	FT_STATUS ftStatus;
	DWORD numDevs=0;
	ftStatus = FT_ListDevices((PVOID)&numDevs,NULL,FT_LIST_NUMBER_ONLY);
	if(ftStatus == FT_OK) 
		return numDevs;
	return NO_RESPONSE; 
}


/* Function : FTDI_SendData
 * Author	: ENTTEC
 * Purpose  : Send Data (DMX or other packets) to the PRO
 * Parameters: Label, Pointer to Data Structure, Length of Data
 **/
int FTDI_SendData(int label, unsigned char *data, int length)
{
	unsigned char end_code = DMX_END_CODE;
	FT_STATUS resEnttecPro = 0;
	DWORD bytes_to_write = length;
	DWORD bytes_written = 0;
	HANDLE event = NULL;
	int size=0;
	// Form Packet Header
	unsigned char header[DMX_HEADER_LENGTH];
	header[0] = DMX_START_CODE;
	header[1] = label;
	header[2] = length & OFFSET;
	header[3] = length >> BYTE_LENGTH;
	// Write The Header
	resEnttecPro = FT_Write(	device_handle,(unsigned char *)header,DMX_HEADER_LENGTH,&bytes_written);
	if (bytes_written != DMX_HEADER_LENGTH) return  NO_RESPONSE;
	// Write The Data
	resEnttecPro = FT_Write(	device_handle,(unsigned char *)data,length,&bytes_written);
	if (bytes_written != length) return  NO_RESPONSE;
	// Write End Code
	resEnttecPro = FT_Write(	device_handle,(unsigned char *)&end_code,ONE_BYTE,&bytes_written);
	if (bytes_written != ONE_BYTE) return  NO_RESPONSE;
	if (resEnttecPro == FT_OK)
		return true;
	else
		return false; 
}

/* Function : FTDI_ReceiveData
 * Author	: ENTTEC
 * Purpose  : Receive Data (DMX or other packets) from the PRO
 * Parameters: Label, Pointer to Data Structure, Length of Data
 **/
int FTDI_ReceiveData(int label, unsigned char *data, unsigned int expected_length)
{

	FT_STATUS resEnttecPro = 0;
	DWORD length = 0;
	DWORD bytes_to_read = 1;
	DWORD bytes_read =0;
	unsigned char byte = 0;
	HANDLE event = NULL;
	char buffer[600];
	// Check for Start Code and matching Label
	while (byte != label)
	{
		while (byte != DMX_START_CODE)
		{
			resEnttecPro = FT_Read(device_handle,(unsigned char *)&byte,ONE_BYTE,&bytes_read);
			if(bytes_read== NO_RESPONSE) return  NO_RESPONSE;
		}
		resEnttecPro = FT_Read(device_handle,(unsigned char *)&byte,ONE_BYTE,&bytes_read);
		if (bytes_read== NO_RESPONSE) return  NO_RESPONSE;
	}
	// Read the rest of the Header Byte by Byte -- Get Length
	resEnttecPro = FT_Read(device_handle,(unsigned char *)&byte,ONE_BYTE,&bytes_read);
	if (bytes_read== NO_RESPONSE) return  NO_RESPONSE;
	length = byte;
	resEnttecPro = FT_Read(device_handle,(unsigned char *)&byte,ONE_BYTE,&bytes_read);
	if (resEnttecPro != FT_OK) return  NO_RESPONSE;
	length += ((uint32_t)byte)<<BYTE_LENGTH;	
	// Check Length is not greater than allowed
	if (length > DMX_PACKET_SIZE)
		return  NO_RESPONSE;
	// Read the actual Response Data
	resEnttecPro = FT_Read(device_handle,buffer,length,&bytes_read);
	if(bytes_read != length) return  NO_RESPONSE;
	// Check The End Code
	resEnttecPro = FT_Read(device_handle,(unsigned char *)&byte,ONE_BYTE,&bytes_read);
	if(bytes_read== NO_RESPONSE) return  NO_RESPONSE;
	if (byte != DMX_END_CODE) return  NO_RESPONSE;
	// Copy The Data read to the buffer passed
	memcpy(data,buffer,expected_length);
	return true;
}

/* Function : FTDI_PurgeBuffer
 * Author	: ENTTEC
 * Purpose  : Clears the buffer used internally by the PRO
 * Parameters: none
 **/
void FTDI_PurgeBuffer()
{
	FT_Purge (device_handle,FT_PURGE_TX);
	FT_Purge (device_handle,FT_PURGE_RX);
}


/* Function : FTDI_OpenDevice
 * Author	: ENTTEC
 * Purpose  : Opens the PRO; Tests various parameters; outputs info
 * Parameters: device num (returned by the List Device fuc), Fw Version MSB, Fw Version LSB 
 **/
uint16_t FTDI_OpenDevice(int device_num)
{
int RTimeout =120;
	int WTimeout =100;
	int VersionMSB =0;
	int VersionLSB =0;
	uint8_t temp[4];
	long version;
	uint8_t major_ver,minor_ver,build_ver;
	int recvd =0;
	unsigned char byte = 0;
	int size = 0;
	int res = 0;
	int tries =0;
	uint8_t latencyTimer;
	FT_STATUS ftStatus;
	int BreakTime;
	int MABTime;
	// Try at least 3 times 
	do  {
		sprintf(string_display_dmx_params,"D2XX Opening [Device %d] Try %d",device_num,tries);
		// Open the PRO 
		ftStatus = FT_Open(device_num,&device_handle);
		// delay for next try
		Sleep(750);
		tries ++;
	} while ((ftStatus != FT_OK) && (tries < 3)); 
	// PRO Opened succesfully
	if (ftStatus == FT_OK) 
	{
		// GET D2XX Driver Version
		ftStatus = FT_GetDriverVersion(device_handle,(LPDWORD)&version);
		if (ftStatus == FT_OK) 
		{
			major_ver = (uint8_t) version >> 16;
			minor_ver = (uint8_t) version >> 8;
			build_ver = (uint8_t) version & 0xFF;
			sprintf(string_display_dmx_params,"D2XX Driver Version:: %02X.%02X.%02X ",major_ver,minor_ver,build_ver);
		}
		else
			sprintf(string_display_dmx_params,"Unable to Get D2XX Driver Version") ;*/

		// GET Latency Timer
		ftStatus = FT_GetLatencyTimer (device_handle,(PUCHAR)&latencyTimer);
		if (ftStatus == FT_OK) 
			sprintf(string_display_dmx_params,"Latency Timer:: %d ",latencyTimer);		
		else
			sprintf(string_display_dmx_params,"Unable to Get Latency Timer") ;
		// SET Default Read & Write Timeouts (in micro sec ~ 100)
		FT_SetTimeouts(device_handle,RTimeout,WTimeout);
		// Piurges the buffer
		FT_Purge (device_handle,FT_PURGE_RX);
		// Send Get Widget Parameters to get Device Info
		sprintf(string_display_dmx_params,"Sending GET_WIDGET_PARAMS packet... ");
 		res = FTDI_SendData(GET_WIDGET_PARAMS,(unsigned char *)&size,2);
		// Check Response
		if (res == NO_RESPONSE)
		{
			FT_Purge (device_handle,FT_PURGE_TX);
 			res = FTDI_SendData(GET_WIDGET_PARAMS,(unsigned char *)&size,2);
			if (res == NO_RESPONSE)
			{
				FTDI_ClosePort();
				return  NO_RESPONSE;
			}
		}
		else
			sprintf(string_display_dmx_params,"PRO Connected Succesfully");
		// Receive Widget Response
		sprintf(string_display_dmx_params,"Waiting for GET_WIDGET_PARAMS_REPLY packet... ");
		res=FTDI_ReceiveData(GET_WIDGET_PARAMS_REPLY,(unsigned char *)&PRO_Params,sizeof(DMXUSBPROParamsType));
		// Check Response
		if (res == NO_RESPONSE)
		{
			// Recive Widget Response packet
			res=FTDI_ReceiveData(GET_WIDGET_PARAMS_REPLY,(unsigned char *)&PRO_Params,sizeof(DMXUSBPROParamsType));
			if (res == NO_RESPONSE)
			{
				FTDI_ClosePort();
				return  NO_RESPONSE;
			}
		}
		else
			sprintf(string_display_dmx_params,"GET WIDGET REPLY Received ... ");*/
		// Firmware  Version
		VersionMSB = PRO_Params.FirmwareMSB;
		VersionLSB = PRO_Params.FirmwareLSB;
		// GET PRO's serial number 
		res = FTDI_SendData(GET_WIDGET_SN,(unsigned char *)&size,2);
		res=FTDI_ReceiveData(GET_WIDGET_SN,(unsigned char *)&temp,4);
		// Display All PRO Parametrs & Info avialable
		sprintf(string_display_dmx_params,"PRO Connected [Information Follows]::------------");
		sprintf(string_display_dmx_params,"FIRMWARE VERSION: %d.%d",VersionMSB,VersionLSB);
		BreakTime = (int) (PRO_Params.BreakTime * 10.67) + 100;
		sprintf(string_display_dmx_params,"BREAK TIME: %d micro sec ",BreakTime);
		MABTime = (int) (PRO_Params.MaBTime * 10.67);
		sprintf(string_display_dmx_params,"MAB TIME: %d micro sec",MABTime);
		sprintf(string_display_dmx_params,"SEND REFRESH RATE: %d packets/sec",PRO_Params.RefreshRate);
		// return success
		return true;
	}		
	else // Can't open Device 
	{	sprintf(string_display_dmx_params,"Can't open Pro");	return FALSE;}
	return(0);
}


// Read a DMX packet
uint8_t FTDI_RxDMX(uint8_t label, unsigned char *data, uint32_t* expected_length)
{

	FT_STATUS resEnttecPro = 0;
	DWORD length = 0;
	DWORD bytes_to_read = 1;
	DWORD bytes_read =0;
	unsigned char byte = 0;
	unsigned char header[3];
	HANDLE event = NULL;
	unsigned char buffer[600];
	// Check for Start Code and matching Label
	while (byte != DMX_START_CODE)
	{
		resEnttecPro = FT_Read(device_handle,(unsigned char *)&byte,ONE_BYTE,&bytes_read);
		if(bytes_read== NO_RESPONSE) return  NO_RESPONSE;
	}
	resEnttecPro = FT_Read(device_handle,header,3,&bytes_read);
	if(bytes_read== NO_RESPONSE) return  NO_RESPONSE;
	if(header[0] != label) return NO_RESPONSE;
	length = header[1];
	length += ((uint32_t)header[2])<<BYTE_LENGTH;	
	length += 1;
	// Check Length is not greater than allowed
	if (length > DMX_PACKET_SIZE +3)
		return  NO_RESPONSE;
	// Read the actual Response Data
	resEnttecPro= FT_Read(device_handle,buffer,length,&bytes_read);
	if(bytes_read != length) return  NO_RESPONSE;
	// Check The End Code
	if (buffer[length-1] != DMX_END_CODE) return  NO_RESPONSE;
	*expected_length = (unsigned int)length;
	// Copy The Data read to the buffer passed
	memcpy(data,buffer,*expected_length);
	return true;
}

// our good main function with everything to do the test
/*
int main(int argc, char**argv)
{
	// declare all needed variables 
	WORD wVID = 0x0403;
	WORD wPID = 0x6001;
	uint8_t Num_Devices =0;
	uint16_t device_connected =0;
	uint32_t length =DMX_DATA_LENGTH;
	int i=0;
	int device_num=0;
	BOOL res = 0;
	// startup message for example
	printf("\nEnttec Pro - C - Windows - FTDI Test\n");
	printf("\nLooking for a PRO's connected to PC ... ");
	Num_Devices = FTDI_ListDevices(); 
	// Number of Found Devices
	if (Num_Devices == 0)
	{
		printf("\n Looking for Devices  - 0 Found");
	}
	else
	{
		printf("\n Looking for Devices  - %d Found",Num_Devices);
		// Just to make sure the Device is correct
		printf("\n Press Enter to Intialize 1st Device :");
		getch();
		 // we'll open the first one only
			device_num = 0;
			device_connected = FTDI_OpenDevice(device_num);
*/
		// If you want to open all; use for loop ; uncomment the folllowing 
		/*
		 for (i=0;i<Num_Devices;i++)
		 {
			if (device_connected)
				break;
			device_num = i;
			device_connected = FTDI_OpenDevice(device_num);		
		 }
		 */

		// Send DMX Code
/*		if (device_connected) 
		{
			unsigned char myDmx[DMX_DATA_LENGTH];
			printf("\n Press Enter to Send DMX data :");
			getch();
			// Looping to Send DMX data
			for (int i = 0; i < 1000 ; i++)
			{
				// initialize with data to send
				memset(myDmx,i,DMX_DATA_LENGTH);
				// Start Code = 0
				myDmx[0] = 0;
				// actual send function called 
				res = FTDI_SendData(SET_DMX_TX_MODE, myDmx, DMX_DATA_LENGTH);
				// check response from Send function
				if (res < 0)
				{
					printf("FAILED: Sending DMX to PRO \n");
					FTDI_ClosePort();
					break;
					return -1;
				}
				// output debug
				printf("Iteration: %d\n", i);
				printf("DMX Data SENT from 0 to 10: ");
				for (int j = 0; j <= 8; j++)
					printf (" %d ",myDmx[j]);				
			}
		}

		// DMX Receive Code in a loop 
		if (device_connected)
		{
			unsigned char myDmxIn[DMX_DATA_LENGTH];
			// Looping to receiving DMX data
			printf("\n Press Enter to receive DMX data :");
			getch();
			for (int i = 0; i < 1000 ; i++)
			{
				// re-initailize before each read
				memset(myDmxIn,0,DMX_DATA_LENGTH);
				// actual recieve function called
				res = FTDI_RxDMX(SET_DMX_RX_MODE, myDmxIn, &length);
				// check response from Receive function
				if (res < 0)
				{
					printf("FAILED: Reading DMX from PRO \n");
					FTDI_ClosePort();
					break;
					return -1;
				}
				// output debug
				printf("Length Recieved: %d\n", length);
				printf("DMX Data from 0 to 10: ");
				for (int j = 0; j <= 20; j++)
					printf (" %d",myDmxIn[j]);
			}
		}
	}

	// Finish all done
	printf("\n Press Enter to Exit :");
	getch();

	return 0;
}
*/
