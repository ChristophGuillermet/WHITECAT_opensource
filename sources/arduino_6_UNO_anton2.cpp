/*-------------------------------------------------------------------------------------------------------------
                                 |
          CWWWWWWWW              | Copyright (C) 2009-2013  Christoph Guillermet
       WWWWWWWWWWWWWWW           | 
     WWWWWWWWWWWWWWWWWWW         | This file is part of White Cat.
    WWWWWWWWWWWWWWWWWCWWWW       | 
   WWWWWWWWWWWWWWWWW tWWWWW      | White Cat is free software: you can redistribute it and/or modify
  WWWW   WWWWWWWWWW  tWWWWWW     | it under the terms of the GNU General Public License as published by
 WWWWWt              tWWWWWWa    | the Free Software Foundation, either version 3 of the License, or
 WWWWWW               WWWWWWW    | (at your option) any later version.
WWWWWWWW              WWWWWWW    | 
WWWWWWWW               WWWWWWW   | White Cat is distributed in the hope that it will be useful,
WWWWWWW               WWWWWWWW   | but WITHOUT ANY WARRANTY; without even the implied warranty of
WWWWWWW      CWWW    W WWWWWWW   | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
WWWWWWW            aW  WWWWWWW   | GNU General Public License for more details.
WWWWWWWW           C  WWWWWWWW   | 
 WWWWWWWW            CWWWWWWW    | You should have received a copy of the GNU General Public License
 WWWWWWWWW          WWWWWWWWW    | along with White Cat.  If not, see <http://www.gnu.org/licenses/>. 
  WWWWWWWWWWC    CWWWWWWWWWW     |   
   WWWWWWWWWWWWWWWWWWWWWWWW      | 
    WWWWWWWWWWWWWWWWWWWWWW       |    
      WWWWWWWWWWWWWWWWWWa        |     
        WWWWWWWWWWWWWWW          |     
           WWWWWWWWt             |
                                 |
---------------------------------------------------------------------------------------------------------------*/

/**

 \file arduino_6_UNO_anto2.cpp
 \brief {description courte} 
 \author Christoph Guillermet, Anton Langhoff
 \version {numero de version du fichier}
 \date {date description}
 
 White Cat {- catégorie} {- sous catégorie {- sous catégorie}}
 Description détaillée
 
 **/
 
 

int arduino_decode_array_method(char *tmp_ard)
{
if(strncmp(tmp_ard,"DG/",3)==0)//3= le code
{
for(int o=3;o<3+arduino_max_digital;o++)
{

digital_data_from_arduino[o-3]=tmp_ard[o];    

}
sprintf( string_Arduino_status,">>ARDUINO : Received Digital ");
}

else if(strncmp(tmp_ard,"AN/",3)==0)
{
for(int o=3;o<3+arduino_max_analog;o++)
{
analog_data_from_arduino[o-3]=(int)(tmp_ard[o]) &0xff;     //cast si val=127 -> 127 si val=-1 -> val 255 si val=-128 ->val 128
}
sprintf( string_Arduino_status,">>ARDUINO : Received Analogic");
}
return(0);   
}


int read_arduino_data(int device)
{
if(device==0)
{
if(arduino_device_0_is_ignited==1)
{
sprintf(arduino_order,"");
arduino_order[0]='S';arduino_order[1]='D';arduino_order[2]='/';arduino_order[3]='\0';
//arduino_order[3]='\0';//pour UNO [3]='\0' pour 2009 =0
int nBytesSendtoArduino=serial0.SendData(arduino_order,4); 
//capture de l'état précédent
for(int y=0;y<=arduino_max_digital;y++)
{
previous_digital_data_from_arduino[y]=digital_data_from_arduino[y];        
}
nBytesReadArduino0=serial0.ReadData(tmp_str_arduino,128);
arduino_decode_array_method(tmp_str_arduino);
}
} 
return(0);   
}


int arduino_read()
{

 read_arduino_data(0);  

 return(0);   
}

int do_arduino_config(int cfg_X, int cfg_Y)
{
//port COM
petitchiffre.Print("DEVICE ON COM:",cfg_X+20,cfg_Y+45); 
Rect Comport(Vec2D(cfg_X+120,cfg_Y+30),Vec2D(50,25));
Comport.SetRoundness(7.5);
Comport.SetLineWidth(epaisseur_ligne_fader);  
Comport.Draw(CouleurFond.WithAlpha(0.5));
if(window_focus_id==W_CFGMENU && mouse_x>cfg_X+120 && mouse_x<cfg_X+170 && mouse_y>cfg_Y+30 && mouse_y<cfg_Y+55)
{
Comport.DrawOutline(CouleurLevel);
if(mouse_button==1 && mouse_released==0)
{
Comport.Draw(CouleurBlind);
int new_comport=atoi(numeric);
reset_numeric_entry();
if(  new_comport>0 && new_comport<18)
{
if(arduino_device_0_is_ignited==1){arduino_close(0);}
arduino_com0=new_comport;           
arduino_init(0);
index_send_arduino_config=1;     
mouse_released=1; 
}                   
}                     
}
petitchiffre.Print(ol::ToString(arduino_com0),cfg_X+140,cfg_Y+45);

//BAUD RATE
petitchiffre.Print("BAUD-RATE:",cfg_X+20,cfg_Y+75); 
Rect BaudrateB(Vec2D(cfg_X+120,cfg_Y+60),Vec2D(50,25));
BaudrateB.SetRoundness(7.5);
BaudrateB.SetLineWidth(epaisseur_ligne_fader);  
BaudrateB.Draw(CouleurFond.WithAlpha(0.5));
if(window_focus_id==W_CFGMENU && mouse_x>cfg_X+120 && mouse_x<cfg_X+170 && mouse_y>cfg_Y+60 && mouse_y<cfg_Y+85)
{
BaudrateB.DrawOutline(CouleurLevel);
if(mouse_button==1 && mouse_released==0)
{
BaudrateB.Draw(CouleurBlind);
int new_Baudrate=atoi(numeric);
reset_numeric_entry();
if( new_Baudrate>0 && (new_Baudrate ==300 || new_Baudrate==1200 || new_Baudrate==2400 || new_Baudrate==4800 || new_Baudrate==9600 || 
new_Baudrate==14400 || new_Baudrate==19200 || new_Baudrate==28800 || new_Baudrate==38400 || new_Baudrate==57600 || new_Baudrate==115200))
{
if(arduino_device_0_is_ignited==1){arduino_close(0);}
arduino_baud_rate0=new_Baudrate;       
arduino_init(0); 
index_send_arduino_config=1;      
mouse_released=1; 
}                   
}                     
}
petitchiffre.Print(ol::ToString(arduino_baud_rate0),cfg_X+128,cfg_Y+75);



//REQUEST RATE
petitchiffre.Print("REQUEST-RATE:",cfg_X+20,cfg_Y+105); 
Rect ReqrateB(Vec2D(cfg_X+120,cfg_Y+90),Vec2D(50,25));
ReqrateB.SetRoundness(7.5);
ReqrateB.SetLineWidth(epaisseur_ligne_fader);  
ReqrateB.Draw(CouleurFond.WithAlpha(0.5));
if(window_focus_id==W_CFGMENU && mouse_x>cfg_X+120 && mouse_x<cfg_X+170 && mouse_y>cfg_Y+90 && mouse_y<cfg_Y+115)
{
ReqrateB.DrawOutline(CouleurLevel);
if(mouse_button==1 && mouse_released==0)
{
ReqrateB.Draw(CouleurBlind);
int new_Reqrate=atoi(numeric);
reset_numeric_entry();
if( new_Reqrate>0 && new_Reqrate<=100 )
{
ARDUINO_RATE=new_Reqrate;
install_int_ex(ticker_arduino ,BPS_TO_TIMER(ARDUINO_RATE));         
mouse_released=1; 
}                   
}                     
}
petitchiffre.Print(ol::ToString(ARDUINO_RATE),cfg_X+140,cfg_Y+105);

////////////////LIMITATION DE CARTES ///////////////////////////////////////////
petitchiffre.Print("LAST I/O:",cfg_X+20,cfg_Y+135); 
Rect MaxIOB(Vec2D(cfg_X+120,cfg_Y+120),Vec2D(50,25));
MaxIOB.SetRoundness(7.5);
MaxIOB.SetLineWidth(epaisseur_ligne_fader);  
MaxIOB.Draw(CouleurFond.WithAlpha(0.5));
if(window_focus_id==W_CFGMENU && mouse_x>cfg_X+120 && mouse_x<cfg_X+170 && mouse_y>cfg_Y+120 && mouse_y<cfg_Y+145)
{
MaxIOB.DrawOutline(CouleurLevel);
if(mouse_button==1 && mouse_released==0)
{
MaxIOB.Draw(CouleurBlind);
int new_maxio=atoi(numeric);
reset_numeric_entry();
if( new_maxio>=0 && new_maxio<=digital_limit )
{
arduino_max_digital=new_maxio;
mouse_released=1; 
}
if(position_line_io>arduino_max_digital-12)
{position_line_io=0;}     
                   
}                     
}
petitchiffre.Print(ol::ToString(arduino_max_digital),cfg_X+140,cfg_Y+135);
///////////////////////////////
petitchiffre.Print("LAST ANALOG:",cfg_X+20,cfg_Y+165); 
Rect MaxAnB(Vec2D(cfg_X+120,cfg_Y+150),Vec2D(50,25));
MaxAnB.SetRoundness(7.5);
MaxAnB.SetLineWidth(epaisseur_ligne_fader);  
MaxAnB.Draw(CouleurFond.WithAlpha(0.5));
if(window_focus_id==W_CFGMENU && mouse_x>cfg_X+120 && mouse_x<cfg_X+170 && mouse_y>cfg_Y+150 && mouse_y<cfg_Y+175)
{
MaxAnB.DrawOutline(CouleurLevel);
if(mouse_button==1 && mouse_released==0)
{
MaxAnB.Draw(CouleurBlind);
int new_maxan=atoi(numeric);
reset_numeric_entry();
if( new_maxan>=0 && new_maxan<=analog_limit )
{
arduino_max_analog=new_maxan;
mouse_released=1; 
} 
if(position_line_ana>arduino_max_analog-12)
{position_line_ana=0;}                   
}                     
}
arduino_total_pin = arduino_max_out_digi+arduino_max_analog+2;
petitchiffre.Print(ol::ToString(arduino_max_analog),cfg_X+140,cfg_Y+165);

///////////////////////////////DIGITAL
petitchiffre.Print("DIGITAL:",cfg_X+20,cfg_Y+195); 
Rect MaxDIB(Vec2D(cfg_X+120,cfg_Y+180),Vec2D(50,25));
MaxDIB.SetRoundness(7.5);
MaxDIB.SetLineWidth(epaisseur_ligne_fader);  
MaxDIB.Draw(CouleurFond.WithAlpha(0.5));
if(mouse_x>cfg_X+120 && mouse_x<cfg_X+170 && mouse_y>cfg_Y+180 && mouse_y<cfg_Y+205)
{
MaxDIB.DrawOutline(CouleurLevel);
if(mouse_button==1 && mouse_released==0)
{
MaxDIB.Draw(CouleurBlind);
int new_maxdi=atoi(numeric);
reset_numeric_entry();
if( new_maxdi>=0 && new_maxdi<=digital_limit )
{
arduino_max_out_digi=new_maxdi;
mouse_released=1; 
}                   
}                     
}
arduino_total_pin = arduino_max_out_digi+arduino_max_analog+2;
petitchiffre.Print(ol::ToString(arduino_max_out_digi),cfg_X+140,cfg_Y+195);
/////////////////////////////////
petitchiffre.Print("OPEN:",cfg_X+20,cfg_Y+225); 
Rect OpenB(Vec2D(cfg_X+120,cfg_Y+210),Vec2D(50,25));
OpenB.SetRoundness(7.5);
OpenB.SetLineWidth(epaisseur_ligne_fader);  
OpenB.Draw(CouleurFond.WithAlpha(0.5));
switch (arduino_device_0_is_ignited)
{
case 0:
petitchiffre.Print("is OFF",cfg_X+130,cfg_Y+225);      
break;
case 1:
petitchiffre.Print("is ON",cfg_X+130,cfg_Y+225); 

        
break;       
}
if(window_focus_id==W_CFGMENU && mouse_x>cfg_X+120 && mouse_x<cfg_X+170 && mouse_y>cfg_Y+210 && mouse_y<cfg_Y+235)
{
OpenB.DrawOutline(CouleurLevel);
if(mouse_button==1 && mouse_released==0)
{ 
ticks_arduino=0;
OpenB.Draw(CouleurBlind);
switch (arduino_device_0_is_ignited)
{
case 0:
   arduino_init(0);
index_send_arduino_config=1; 
index_do_light_send_config=1.0; 
break;
case 1:  
   arduino_close(0); 
   arduino_device_0_is_ignited=0;    
break;       
}
mouse_released=1; 
}                     
}

//Arduino config
petitchiffre.Print("SEND CONFIG",cfg_X+20,cfg_Y+255); 
Rect arduino_send(Vec2D(cfg_X+120,cfg_Y+240),Vec2D(50,25));
arduino_send.SetRoundness(7.5);  
arduino_send.Draw(CouleurFond.WithAlpha(0.5));
arduino_send.Draw(CouleurBlind.WithAlpha(index_do_light_send_config));      
if(index_do_light_send_config>0){index_do_light_send_config-=0.2;if(index_do_light_send_config<0) index_do_light_send_config=0;}
if(window_focus_id==W_CFGMENU && arduino_device_0_is_ignited==1 && mouse_x>cfg_X+120 && mouse_x<cfg_X+170 && mouse_y>cfg_Y+240 && mouse_y<cfg_Y+265)
{
arduino_send.DrawOutline(CouleurLigne.WithAlpha(0.5));

if(mouse_button==1 && mouse_released==0)
{
index_send_arduino_config=1;             
index_do_light_send_config=1.0;          
}                    
}

///////////////////////////////////////////////////////////

petitchiffre.Print("Digital affectation:",cfg_X+200,cfg_Y+20);
petitchiffre.Print("Analog affectation:",cfg_X+510,cfg_Y+20);
///////////////////BOX AFFECTATION DIGITAL////////////////

char string_temp_def[25];
char string_temp_familly[25];



for (int y=0;y<=arduino_max_digital;y++)
{
////////DIG/////////////////////////////////////////////////////////////////
if(cfg_Y+43+(y*20)<cfg_Y+285&&position_line_io+y<=arduino_max_digital)
{ 
                      
Rect OverTyp(Vec2D(cfg_X+200+5,(cfg_Y+40+(y*20)-10)),Vec2D(70,18));
OverTyp.SetRoundness(7.5);
OverTyp.Draw(CouleurFond.WithAlpha(0.3));

Rect OverAction(Vec2D(cfg_X+280+5,(cfg_Y+40+(y*20)-10)),Vec2D(140,18));
OverAction.SetRoundness(7.5);
OverAction.Draw(CouleurFond.WithAlpha(0.3));

Rect OverVal(Vec2D(cfg_X+430+5,(cfg_Y+40+(y*20)-10)),Vec2D(30,18));
OverVal.SetRoundness(7.5);
OverVal.Draw(CouleurFond.WithAlpha(0.3));

/////////ANA///////////////////////////////////////////////////////////////

Rect OverAff(Vec2D(cfg_X+520+5,(cfg_Y+40+(y*20)-10)),Vec2D(105,18));
OverAff.SetRoundness(7.5);
if(position_line_ana+y<=arduino_max_analog)
{OverAff.Draw(CouleurFond.WithAlpha(0.3));}

Rect OverValA(Vec2D(cfg_X+630+5,(cfg_Y+40+(y*20)-10)),Vec2D(30,18));
OverValA.SetRoundness(7.5);
if(position_line_ana+y<=arduino_max_analog)
{OverValA.Draw(CouleurFond.WithAlpha(0.3));}


if(window_focus_id==W_CFGMENU && mouse_y>(cfg_Y+30+(y*20)) && mouse_y<(cfg_Y+45+(y*20)))
{
////////////////////DIG//////////////////////////////////////////////////////////
//changement type
if(mouse_x>cfg_X+205 && mouse_x<cfg_X+275)
{
OverTyp.DrawOutline(CouleurLigne);                  
if(mouse_button==1 && mouse_released==0)
{
arduino_digital_type[position_line_io+y]++;
arduino_digital_function_input[position_line_io+y][1]=0;
arduino_digital_function_input[position_line_io+y][0]=0;
arduino_digital_function_output[position_line_io+y][1]=0;
arduino_digital_function_output[position_line_io+y][0]=0;

if((position_line_io+y)<=arduino_total_pin)
{if(arduino_digital_type[position_line_io+y]>6)
{arduino_digital_type[position_line_io+y]=0;}}

else if((position_line_io+y)>=arduino_total_pin)
{
     if(arduino_digital_type[position_line_io+y]>6)
     {arduino_digital_type[position_line_io+y]=0;}
    if(arduino_digital_type[position_line_io+y]>0&&arduino_digital_type[position_line_io+y]<6)
     {arduino_digital_type[position_line_io+y]=6;}
}


OverTyp.Draw(CouleurFader);    
mouse_released=1;               

}
}
//////L'ASSIGNATION INPUT/////////////////////////////////////////////
//arduino_digital_function_input[128][2];//action // Val 1 

if(mouse_x>cfg_X+285 && mouse_x<cfg_X+425)//famille d action
{
OverAction.DrawOutline(CouleurLigne);                  
if(mouse_button==1 && mouse_released==0)
{
if(arduino_digital_type[position_line_io+y]==1|| arduino_digital_type[position_line_io+y]==7|| arduino_digital_type[position_line_io+y]==2)//input
{
arduino_digital_function_input[position_line_io+y][0]++;
if(arduino_digital_function_input[position_line_io+y][0]>32)
{arduino_digital_function_input[position_line_io+y][0]=0;}
arduino_digital_function_input[position_line_io+y][1]=0;
}
if(arduino_digital_type[position_line_io+y]==6 || arduino_digital_type[position_line_io+y]==3 || arduino_digital_type[position_line_io+y]==4|| arduino_digital_type[position_line_io+y]==5)//output ON OF ET PWM
{
arduino_digital_function_output[position_line_io+y][0]++;
if(arduino_digital_function_output[position_line_io+y][0]>2)
{arduino_digital_function_output[position_line_io+y][0]=0;}
arduino_digital_function_output[position_line_io+y][1]=0;
}

OverAction.Draw(CouleurFader);    
mouse_released=1;               
}


}           


///////La valeur de l'ordre si il y en a une ( par ex. num fader)///////

if(mouse_x>cfg_X+435 && mouse_x<cfg_X+465)//famille d action
{
OverVal.DrawOutline(CouleurLigne);                  
if(mouse_button==1 && mouse_released==0)
{
int valois=atoi(numeric);

///INPUT/////////////////////////////////////////////////////////////////////
//data Val1 range pour un input de type fader ( 0 à 11 )
if(
(arduino_digital_type[position_line_io+y]==1|| arduino_digital_type[position_line_io+y]==2|| arduino_digital_type[position_line_io+y]==7)
&& arduino_digital_function_input[position_line_io+y][0]>=0 && arduino_digital_function_input[position_line_io+y][0]<=11 
&& valois>0 && valois<=48
)
{
arduino_digital_function_input[position_line_io+y][1]=valois;                                                            
}
//faire data range pour le picth: 0 à 127
if(
(arduino_digital_type[position_line_io+y]==1|| arduino_digital_type[position_line_io+y]==2|| arduino_digital_type[position_line_io+y]==7)
&& ( arduino_digital_function_input[position_line_io+y][0]>=17 || arduino_digital_function_input[position_line_io+y][0]<=32) 
&& valois>=0 && valois<=127
)
{
arduino_digital_function_input[position_line_io+y][1]=valois;                                                            
}
///////////OUTPUT ON OFF ET PWM///////////////////////////////////////////////////////////////
if((arduino_digital_type[position_line_io+y]==6 || arduino_digital_type[position_line_io+y]==3|| arduino_digital_type[position_line_io+y]==4|| arduino_digital_type[position_line_io+y]==5) && arduino_digital_function_output[position_line_io+y][0]==1
&& valois>0 && valois<513)//CHANNEL
{
arduino_digital_function_output[position_line_io+y][1]=valois;                                                 
}
if((arduino_digital_type[position_line_io+y]==6 || arduino_digital_type[position_line_io+y]==3|| arduino_digital_type[position_line_io+y]==4|| arduino_digital_type[position_line_io+y]==5) && arduino_digital_function_output[position_line_io+y][0]==2
&& valois>0 && valois<49)//Fader
{
arduino_digital_function_output[position_line_io+y][1]=valois;                                                 
}
reset_numeric_entry();
OverVal.Draw(CouleurFader);    
mouse_released=1;               
}
}
///////////////////////////////////////          
///////////////////BOX AFFECTATION ANALOG////////////////


//////L'ASSIGNATION Analog/////////////////////////////////////////////

//changement affectation
if(mouse_x>cfg_X+525 && mouse_x<cfg_X+630)
{
OverAff.DrawOutline(CouleurLigne);                  
if(mouse_button==1 && mouse_released==0)
{
arduino_analog_function_input[position_line_ana+y]++;
arduino_analog_attribution_input[position_line_ana+y]=0;
if(arduino_analog_function_input[position_line_ana+y]>20)
{arduino_analog_function_input[position_line_ana+y]=0;}
OverAff.Draw(CouleurFader);    
mouse_released=1;               
}
}
///////////////LA VALEUR/////////////////////////////
if(mouse_x>cfg_X+635 && mouse_x<cfg_X+665)//valeur
{
OverValA.DrawOutline(CouleurLigne);                  
if(mouse_button==1 && mouse_released==0)
{
int valana=0;
valana=atoi(numeric);
reset_numeric_entry();
/////////////INPUT
//limitation des valeurs aux types d entrees
if(arduino_analog_function_input[position_line_ana+y]==1 || arduino_analog_function_input[position_line_ana+y]==2)//faders level et speed
{
 if(valana>0 && valana<=48)
 {arduino_analog_attribution_input[position_line_ana+y]=valana-1;}
}
if(  arduino_analog_function_input[position_line_ana+y]==4)//seq scene et preset
{
 if(valana>0 && valana<=3)//1 stage 2 preset 3 speed
 {arduino_analog_attribution_input[position_line_ana+y]=valana-1;}
}
if(  arduino_analog_function_input[position_line_ana+y]>=5 &&  arduino_analog_function_input[position_line_ana+y]<=20 )//simulation midi
{
 if(valana>=0 && valana<=127)//Pitch
 {arduino_analog_attribution_input[position_line_ana+y]=valana;}
}



OverValA.Draw(CouleurFader);   
mouse_released=1;               
}
}           
          




/////////////////////////////////////////////////////////////////////////////// 
}
//////////////////AFFICHAGE///////////////////////////////

if(position_line_io+y==0)
{
sprintf(string_temp_def,"0:Serial",position_line_io+y);                          
sprintf(string_temp_familly,"Rx");
OverTyp.Draw(CouleurFond);
petitchiffre.Print(string_temp_familly,cfg_X+293,cfg_Y+43+(y*20));  
petitchiffre.Print(string_temp_def,cfg_X+215,cfg_Y+43+(y*20));  
petitchiffrerouge.Print(ol::ToString(digital_data_from_arduino[position_line_io+y]),cfg_X+475,cfg_Y+43+(y*20));    
}
if(position_line_io+y==1)
{
sprintf(string_temp_def,"1:Serial",position_line_io+y);
sprintf(string_temp_familly,"Tx");  
OverTyp.Draw(CouleurFond);  
petitchiffre.Print(string_temp_familly,cfg_X+293,cfg_Y+43+(y*20));  
petitchiffre.Print(string_temp_def,cfg_X+215,cfg_Y+43+(y*20));  
petitchiffrerouge.Print(ol::ToString(digital_data_from_arduino[position_line_io+y]),cfg_X+475,cfg_Y+43+(y*20));                        
}

if(position_line_io+y>1 && position_line_io+y<=arduino_max_digital)
{

///////LE TYPE ////////////////////////////////////////////
switch(arduino_digital_type[position_line_io+y])
{
case 0://desafected
sprintf(string_temp_def,"%d:-",position_line_io+y);
break;
case 1://input      
sprintf(string_temp_def,"%d:Input",position_line_io+y);
OverTyp.Draw(CouleurRed);  
break;
case 2://PUL UP      
sprintf(string_temp_def,"%d:Pull Up",position_line_io+y);
OverTyp.Draw(CouleurRed);  
break;
case 3://output
sprintf(string_temp_def,"%d:Output",position_line_io+y);
OverTyp.Draw(CouleurGreen);  
break;
case 4://pwm
sprintf(string_temp_def,"%d:PWM",position_line_io+y);
OverTyp.Draw(CouleurYellow);  
break;
case 5://servo
sprintf(string_temp_def,"%d:SERVO",position_line_io+y);
OverTyp.Draw(CouleurBleu8);  
break;      
case 6://HF out
sprintf(string_temp_def,"%d:HF OUT",position_line_io+y);
OverTyp.Draw(CouleurGrisClair);  
break;   
case 7://HF in
sprintf(string_temp_def,"%d:HF IN",position_line_io+y);
OverTyp.Draw(CouleurGrisClair);  
break;  
default:
break;                                        
}      
                             
////////////ACTIONS /////////////////////////////////////////////////
////////////////GRANDE FAMILLE INPUT////////////////////////////////////////
if(arduino_digital_type[position_line_io+y]==1|| arduino_digital_type[position_line_io+y]==2|| arduino_digital_type[position_line_io+y]==7)//si de la famille des inputs
{
switch(arduino_digital_function_input[position_line_io+y][0])
{
case 0://desafected
sprintf(string_temp_familly,"-");
break;
case 1: //fader up                                                
sprintf(string_temp_familly,"Fader: UP");
break;
case 2://fader down
sprintf(string_temp_familly,"Fader: DOWN");
break;
case 3://Fader saw
sprintf(string_temp_familly,"Fader: SAW");
break;      
case 4:
sprintf(string_temp_familly,"Fader: ToPREVDock");     
break;
case 5:
sprintf(string_temp_familly,"Fader: ToNEXTDock");     
break;
case 6:
sprintf(string_temp_familly,"Fader: Up/Down");
break;
case 7:
sprintf(string_temp_familly,"Fader: LOCK");
break;
case 8:
sprintf(string_temp_familly,"Fader: FLASH");
break;
case 9:
sprintf(string_temp_familly,"Fader: All at 0");
break;
case 10:
sprintf(string_temp_familly,"Fader: L/Unloop dock");
break;
case 11:
sprintf(string_temp_familly,"Fader: L/Unloop all");
break;
case 12:
sprintf(string_temp_familly,"Seq: GO");
break;
case 13:
sprintf(string_temp_familly,"Seq: GO BACK");
break;
case 14:
sprintf(string_temp_familly,"Seq: JUMP");
break;
case 15:
sprintf(string_temp_familly,"Seq: SHIFT-W");
break;
case 16:
sprintf(string_temp_familly,"Seq: SHIFT-X");
break;
case 17:
sprintf(string_temp_familly,"As Key-On CH0 Pitch:");
break;
case 18:
sprintf(string_temp_familly,"As Key-On CH1 Pitch:");
break;
case 19:
sprintf(string_temp_familly,"As Key-On CH2 Pitch:");
break;
case 20:
sprintf(string_temp_familly,"As Key-On CH3 Pitch:");
break;
case 21:
sprintf(string_temp_familly,"As Key-On CH4 Pitch:");
break;
case 22:
sprintf(string_temp_familly,"As Key-On CH5 Pitch:");
break;
case 23:
sprintf(string_temp_familly,"As Key-On CH6 Pitch:");
break;
case 24:
sprintf(string_temp_familly,"As Key-On CH7 Pitch:");
break;
case 25:
sprintf(string_temp_familly,"As Key-On CH8 Pitch:");
break;
case 26:
sprintf(string_temp_familly,"As Key-On CH9 Pitch:");
break;
case 27:
sprintf(string_temp_familly,"As Key-On CH10 Pitch:");
break;
case 28:
sprintf(string_temp_familly,"As Key-On CH11 Pitch:");
break;
case 29:
sprintf(string_temp_familly,"As Key-On CH12 Pitch:");
break;
case 30:
sprintf(string_temp_familly,"As Key-On CH13 Pitch:");
break;
case 31:
sprintf(string_temp_familly,"As Key-On CH14 Pitch:");
break;
case 32:
sprintf(string_temp_familly,"As Key-On CH15 Pitch:");
break;
default:
break;                                        
} 
if(cfg_Y+43+(y*20)<cfg_Y+285)
{
petitchiffre.Print(string_temp_familly,cfg_X+293,cfg_Y+43+(y*20));  
petitchiffre.Print(ol::ToString(arduino_digital_function_input[position_line_io+y][1]),cfg_X+440+3,cfg_Y+43+(y*20)); 
}
}

if(cfg_Y+43+(y*20)<cfg_Y+285)
{    
petitchiffrerouge.Print(ol::ToString(digital_data_from_arduino[position_line_io+y]),cfg_X+475,cfg_Y+43+(y*20));  
petitchiffre.Print(string_temp_def,cfg_X+215,cfg_Y+43+(y*20));   
} 
}

if(arduino_digital_type[position_line_io+y]==3)//si de la famille des Outputs
{
switch(arduino_digital_function_output[position_line_io+y][0])
{
case 0://desafected
sprintf(string_temp_familly,"-");
break;
case 1: //relié à un circuit  
sprintf(string_temp_familly,"Channel >10");            
break;
case 2://relié à un fader
sprintf(string_temp_familly,"Fader >10");
break;
}
if(cfg_Y+43+(y*20)<cfg_Y+285)
{
petitchiffre.Print(string_temp_familly,cfg_X+293,cfg_Y+43+(y*20));  
petitchiffre.Print(ol::ToString(arduino_digital_function_output[position_line_io+y][1]),cfg_X+443,cfg_Y+43+(y*20)); 
}
}
if(arduino_digital_type[position_line_io+y]==6||arduino_digital_type[position_line_io+y]==4||arduino_digital_type[position_line_io+y]==5)//si de la famille pwm
{
switch(arduino_digital_function_output[position_line_io+y][0])
{
case 0://desafected
sprintf(string_temp_familly,"-");
break;
case 1: //relié à un circuit  
sprintf(string_temp_familly,"Channel");            
break;
case 2://relié à un fader
sprintf(string_temp_familly,"Fader");
break;
}
if(cfg_Y+43+(y*20)<cfg_Y+285)
{
petitchiffre.Print(string_temp_familly,cfg_X+293,cfg_Y+43+(y*20));  
petitchiffre.Print(ol::ToString(arduino_digital_function_output[position_line_io+y][1]),cfg_X+443,cfg_Y+43+(y*20)); 
}
}


//affichage analog
if(position_line_ana+y<=arduino_max_analog)
{
sprintf(string_temp_def,"%d:",position_line_ana+y);
petitchiffre.Print(string_temp_def,cfg_X+510,cfg_Y+43+(y*20)); 
switch(arduino_analog_function_input[position_line_ana+y])
{
case 0://desafected
sprintf(string_temp_familly,"-");
break;
case 1: //faders  lvl                                           
sprintf(string_temp_familly,"Fader Level:");
petitchiffre.Print(ol::ToString((arduino_analog_attribution_input[position_line_ana+y]+1)),cfg_X+645,cfg_Y+43+(y*20));     
break;
case 2://faders speeed
sprintf(string_temp_familly,"Fader Speed:");
petitchiffre.Print(ol::ToString(arduino_analog_attribution_input[position_line_ana+y]+1),cfg_X+645,cfg_Y+43+(y*20)); 
break;
case 3://master
sprintf(string_temp_familly,"Grand Master");
break;      
case 4://seq scene et preset et vitesse
sprintf(string_temp_familly,"Sequence:");
petitchiffre.Print(ol::ToString(arduino_analog_attribution_input[position_line_ana+y]+1),cfg_X+645,cfg_Y+43+(y*20)); 
break;
case 5://Midi CH0
sprintf(string_temp_familly,"As CC CH0 Pitch:");
petitchiffre.Print(ol::ToString(arduino_analog_attribution_input[position_line_ana+y]),cfg_X+645,cfg_Y+43+(y*20)); 
break;
case 6://Midi CH1
sprintf(string_temp_familly,"As CC CH1 Pitch:");
petitchiffre.Print(ol::ToString(arduino_analog_attribution_input[position_line_ana+y]),cfg_X+645,cfg_Y+43+(y*20)); 
break;
case 7://Midi CH2
sprintf(string_temp_familly,"As CC CH2 Pitch:");
petitchiffre.Print(ol::ToString(arduino_analog_attribution_input[position_line_ana+y]),cfg_X+645,cfg_Y+43+(y*20)); 
break;
case 8://Midi CH3
sprintf(string_temp_familly,"As CC CH3 Pitch:");
petitchiffre.Print(ol::ToString(arduino_analog_attribution_input[position_line_ana+y]),cfg_X+645,cfg_Y+43+(y*20)); 
break;
case 9://Midi CH4
sprintf(string_temp_familly,"As CC CH4 Pitch:");
petitchiffre.Print(ol::ToString(arduino_analog_attribution_input[position_line_ana+y]),cfg_X+645,cfg_Y+43+(y*20)); 
break;
case 10://Midi CH5
sprintf(string_temp_familly,"As CC CH5 Pitch:");
petitchiffre.Print(ol::ToString(arduino_analog_attribution_input[position_line_ana+y]),cfg_X+645,cfg_Y+43+(y*20)); 
break;
case 11://Midi CH6
sprintf(string_temp_familly,"As CC CH6 Picth:");
petitchiffre.Print(ol::ToString(arduino_analog_attribution_input[position_line_ana+y]),cfg_X+645,cfg_Y+43+(y*20)); 
break;
case 12://Midi CH7
sprintf(string_temp_familly,"As CC CH7 Pitch:");
petitchiffre.Print(ol::ToString(arduino_analog_attribution_input[position_line_ana+y]),cfg_X+645,cfg_Y+43+(y*20)); 
break;
case 13://Midi CH8
sprintf(string_temp_familly,"As CC CH8 Pitch:");
petitchiffre.Print(ol::ToString(arduino_analog_attribution_input[position_line_ana+y]),cfg_X+645,cfg_Y+43+(y*20)); 
break;
case 14://Midi CH9
sprintf(string_temp_familly,"As CC CH9 Pitch:");
petitchiffre.Print(ol::ToString(arduino_analog_attribution_input[position_line_ana+y]),cfg_X+645,cfg_Y+43+(y*20)); 
break;
case 15://Midi CH10
sprintf(string_temp_familly,"As CC CH10 Picth:");
petitchiffre.Print(ol::ToString(arduino_analog_attribution_input[position_line_ana+y]),cfg_X+645,cfg_Y+43+(y*20)); 
break;
case 16://Midi CH11
sprintf(string_temp_familly,"As CC CH11 Pitch:");
petitchiffre.Print(ol::ToString(arduino_analog_attribution_input[position_line_ana+y]),cfg_X+645,cfg_Y+43+(y*20)); 
break;
case 17://Midi CH12
sprintf(string_temp_familly,"As CC CH12 Pitch:");
petitchiffre.Print(ol::ToString(arduino_analog_attribution_input[position_line_ana+y]),cfg_X+645,cfg_Y+43+(y*20)); 
break;
case 18://Midi CH13
sprintf(string_temp_familly,"As CC CH13 Pitch:");
petitchiffre.Print(ol::ToString(arduino_analog_attribution_input[position_line_ana+y]),cfg_X+645,cfg_Y+43+(y*20)); 
break;
case 19://Midi CH14
sprintf(string_temp_familly,"As CC CH14 Pitch:");
petitchiffre.Print(ol::ToString(arduino_analog_attribution_input[position_line_ana+y]),cfg_X+645,cfg_Y+43+(y*20)); 
break;
case 20://Midi CH15
sprintf(string_temp_familly,"As CC CH15 Pitch:");
petitchiffre.Print(ol::ToString(arduino_analog_attribution_input[position_line_ana+y]),cfg_X+645,cfg_Y+43+(y*20)); 
break;
default:
break;
}
petitchiffre.Print(string_temp_familly,cfg_X+530,cfg_Y+43+(y*20));     
petitchiffrerouge.Print(ol::ToString(analog_data_from_arduino[position_line_ana+y]),cfg_X+675,cfg_Y+43+(y*20));                                             
}
}
//fin boucle des 12 lignes
}
/////////////////UP DOWN LINE IO /////////////////////
Circle LineUp(Vec2D(cfg_X+400,cfg_Y+15),12);
LineUp.Draw(CouleurFond.WithAlpha(0.3));
Circle LineDown(Vec2D(cfg_X+440,cfg_Y+15),12);
LineDown.Draw(CouleurFond.WithAlpha(0.3));

if(mouse_y>cfg_Y+3 && mouse_y<cfg_Y+27)
{  
if(mouse_x>cfg_X+388 && mouse_x<cfg_X+412)
{                    
LineUp.Draw(CouleurSurvol);
if(mouse_button==1)
{
LineUp.Draw(CouleurFader);
if(position_line_io>0){position_line_io--;}
}                 
}
else if(mouse_x>cfg_X+428 && mouse_x<cfg_X+452)
{
LineDown.Draw(CouleurSurvol);
if(mouse_button==1)
{
LineDown.Draw(CouleurFader);
if(position_line_io<=arduino_max_digital-11){position_line_io++;}
}                  
}                                                
}
petitchiffre.Print("-",cfg_X+398,cfg_Y+19);
petitchiffre.Print("+",cfg_X+436,cfg_Y+19);
LineUp.DrawOutline(CouleurLigne.WithAlpha(0.6));
LineDown.DrawOutline(CouleurLigne.WithAlpha(0.6));

/////////////////UP DOWN ANA /////////////////////
Circle LineUpA(Vec2D(cfg_X+660,cfg_Y+15),12);
LineUpA.Draw(CouleurFond.WithAlpha(0.3));
Circle LineDownA(Vec2D(cfg_X+700,cfg_Y+15),12);
LineDownA.Draw(CouleurFond.WithAlpha(0.3));

if(window_focus_id==W_CFGMENU && mouse_y>cfg_Y+3 && mouse_y<cfg_Y+27)
{  
if(mouse_x>cfg_X+648 && mouse_x<cfg_X+672)
{                    
LineUpA.Draw(CouleurSurvol);
if(mouse_button==1)
{
LineUpA.Draw(CouleurFader);
if(position_line_ana>0){position_line_ana--;}
}                 
}
else if(mouse_x>cfg_X+688 && mouse_x<cfg_X+712)
{
LineDownA.Draw(CouleurSurvol);
if(mouse_button==1)
{
LineDownA.Draw(CouleurFader);
if(position_line_ana<=arduino_max_analog-11){position_line_ana++;}
}                  
}                                                
}
petitchiffre.Print("-",cfg_X+658,cfg_Y+19);
petitchiffre.Print("+",cfg_X+696,cfg_Y+19);
LineUpA.DrawOutline(CouleurLigne.WithAlpha(0.6));
LineDownA.DrawOutline(CouleurLigne.WithAlpha(0.6));



//FEEDBACK
petitpetitchiffre.Print("Bytes received:",cfg_X+700,cfg_Y+60);  
petitchiffre.Print(ol::ToString( nBytesReadArduino0),cfg_X+700,cfg_Y+70); 
petitpetitchiffre.Print("Last str. received:",cfg_X+700,cfg_Y+80);  
petitpetitchiffre.Print(tmp_str_arduino,cfg_X+700,cfg_Y+90);  
petitchiffrerouge.Print(string_Arduino_status,cfg_X+10,cfg_Y+285);



return(0);                              
}

