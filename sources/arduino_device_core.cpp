#include "serial.cpp"
CSerial serial0;

int arduino_init(int device)
{
if(device==0)
{
if(serial0.Open(arduino_com0,arduino_baud_rate0))
{
sprintf(string_Arduino_status,">>ARDUINO : Open Arduino on PORT %d rate: %d >> OK !",arduino_com0,arduino_baud_rate0);  
arduino_device_0_is_ignited=1;
//rest(1000); //temps d'initialisation avant communication ( sinon pense recuperer script)
}
else 
{
sprintf(string_Arduino_status,">>ARDUINO : Open Arduino on PORT %d >> FAILED !",arduino_com0);
arduino_device_0_is_ignited=0;  
}
}
return(0);              
}

int arduino_close(int device)
{
if(device==0)
{
 serial0.Close();
 sprintf(string_Arduino_status,">>ARDUINO : Closed Arduino on PORT %d >> OK !",arduino_com0); 
}
 
return(0);   
}
