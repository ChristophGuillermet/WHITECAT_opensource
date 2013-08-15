

#include "odmxusb.h"


// le pointeur ENTTEC sur la classe to access to the class definned by terry
Open_USB_DMX *pUsbDmx = NULL;
unsigned char DmxBlock[512];



int openingOpenDmx()
{
    //creation of a new pointer
   pUsbDmx = new Open_USB_DMX();
		if (pUsbDmx == NULL)
		{   
            cout<<"\a\nImpossible to load DLL";
			return(0);
		}
		//opening just by calling the ODmx
    		if (pUsbDmx != NULL)
		   {	
			 int iRet;
			 iRet = pUsbDmx->open_dmx_devices();
                  cout<<"Code retour Open ENTTEC= "<<iRet;
                  if (iRet==0)
                  {  
    	    	  	cout<<"\a\nImpossible to open interface, is it PLUGGED ?";
                    return(0);
                  }
                  pUsbDmx->send_dmx_packet(DmxBlock); 
        	}
     	 else {cout<<"\nInterface Opened";} 
return(0);
}

int closingOpenDmx()
{
 	if (pUsbDmx != NULL)
		{
		pUsbDmx->close_dmx_devices();
		}
    return(0);   
}

int SendDataOpenDmx()
{
       if (pUsbDmx!= NULL)
		{
		int iRet;
		iRet = pUsbDmx->get_dmx_device_count();
   	    pUsbDmx->send_dmx_packet(DmxBlock); 
        }
}
