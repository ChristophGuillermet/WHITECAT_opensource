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

 \file arduino_core_6_UNO_anton3.cpp
 \brief {description courte} 
 \author Christoph Guillermet, Anton Langhoff
 \version {numero de version du fichier}
 \date {date description}
 
 White Cat {- catégorie} {- sous catégorie {- sous catégorie}}
 Description détaillée
 
 **/
 
 




///le volatile int dans arduino car possibilité de changer le rate d appel de la fonction read
volatile int ticks_arduino=0;
int old_ticks_arduino=0;
void ticker_arduino()
{
   ticks_arduino++;     
}
END_OF_FUNCTION(ticker_arduino);





int arduino_do_analog_in_whitecat()
{
int vfader=0;
int temp_ardniveauX1=0; 
int temp_ardniveauX2=0;

for(int p=0;p<arduino_max_analog;p++)
{
if(analog_data_from_arduino[p]!=previous_analog_data_from_arduino[p])
{
switch(arduino_analog_function_input[p])
{
case 1://affectation à un fader

vfader=arduino_analog_attribution_input[p];
Fader[vfader]=analog_data_from_arduino[p];  
midi_levels[vfader]=(Fader[vfader]/2);
index_fader_is_manipulated[vfader]=1; 
if(midi_send_out[vfader]==1){ index_send_midi_out[vfader]=1;}
if(lfo_mode_is[vfader]==1 || lfo_mode_is[vfader]==2 || lfo_cycle_is_on[vfader]==1)
{
lfo_mode_is[vfader]=0; lfo_mode_is[vfader]=0; lfo_cycle_is_on[vfader]=0;                           
}
break;
case 2://affectation au speed d un fader
vfader=arduino_analog_attribution_input[p];
lfo_speed[vfader]=analog_data_from_arduino[p]/2;
if(lfo_speed[vfader]<0){lfo_speed[vfader]=0;}
if(lfo_speed[vfader]>127){lfo_speed[vfader]=127;}
midi_levels[196+vfader]=lfo_speed[vfader];
index_send_midi_out[196+vfader]=1;
break;
case 3://grand master
//NIVEAU
if(index_allow_grand_master==1)
{
niveauGMaster=analog_data_from_arduino[p];  
midi_levels[615]=(niveauGMaster/2);
if(midi_send_out[615]==1){ index_send_midi_out[615]=1;}     
}
break;

case 4://sequenciel : Val 1 stage  Val 2 preset Val 3 speed 
if(arduino_analog_attribution_input[p]==0 || arduino_analog_attribution_input[p]==1 )//traitement du crossfade manuel 
{
// stage
if(arduino_analog_attribution_input[p]==0 )
{
temp_ardniveauX1=analog_data_from_arduino[p];     
if(index_get_back_faders_need_to_be_done==0)
{
index_go=0; index_go_back=0; index_pause=0;
niveauX1=analog_data_from_arduino[p];    
midi_levels[491]=niveauX1/2;
if(midi_send_out[491]==1){index_send_midi_out[491]=1;}      
}
}
// preset
if(arduino_analog_attribution_input[p]==1 ) 
{
temp_ardniveauX2=255-analog_data_from_arduino[p];
if(index_get_back_faders_need_to_be_done==0)
{
index_go=0; index_go_back=0; index_pause=0;
niveauX2=255-analog_data_from_arduino[p];   
midi_levels[492]=analog_data_from_arduino[p]/2;//inversion faite dans le midi
if(midi_send_out[492]==1){index_send_midi_out[492]=1;}        
}
}

//remise à jour du crossfade
if(temp_ardniveauX1==0 && temp_ardniveauX2==255)
{
index_go=0; index_go_back=0; index_pause=0;
next_mem_crossfade_finished(position_preset);
index_x1_x2_together=1;
index_get_back_faders_need_to_be_done=1;
refresh_integrated_gridplayer1();
}
if(temp_ardniveauX1==255 && temp_ardniveauX2==0)
{
index_get_back_faders_need_to_be_done=0;            
}
}
// speed
if(arduino_analog_attribution_input[p]==2)
{
crossfade_speed=analog_data_from_arduino[p]/2;
//if(midi_send_out[493]==1){index_send_midi_out[493]=1;}  
prepare_crossfade();
//ventilation_temps_vers_char();//icat
someone_changed_in_time_sequences=1;//icat
}
break;
case 5://MIDI CH0
simulate_midi(4,0,arduino_digital_function_input[p][1],analog_data_from_arduino[p]/2);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Ctrl-Chge");      
break;
case 6://MIDI CH1
simulate_midi(4,1,arduino_digital_function_input[p][1],analog_data_from_arduino[p]/2);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Ctrl-Chge");      
break;
case 7://MIDI CH2
simulate_midi(4,2,arduino_digital_function_input[p][1],analog_data_from_arduino[p]/2);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Ctrl-Chge");      
break;
case 8://MIDI CH3
simulate_midi(4,3,arduino_digital_function_input[p][1],analog_data_from_arduino[p]/2);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Ctrl-Chge");      
break;
case 9://MIDI CH4
simulate_midi(4,4,arduino_digital_function_input[p][1],analog_data_from_arduino[p]/2);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Ctrl-Chge");      
break;
case 10://MIDI CH5
simulate_midi(4,5,arduino_digital_function_input[p][1],analog_data_from_arduino[p]/2);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Ctrl-Chge");      
break;
case 11://MIDI CH6
simulate_midi(4,6,arduino_digital_function_input[p][1],analog_data_from_arduino[p]/2);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Ctrl-Chge");      
break;
case 12://MIDI CH7
simulate_midi(4,7,arduino_digital_function_input[p][1],analog_data_from_arduino[p]/2);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Ctrl-Chge");      
break;
case 13://MIDI CH8
simulate_midi(4,8,arduino_digital_function_input[p][1],analog_data_from_arduino[p]/2);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Ctrl-Chge");      
break;
case 14://MIDI CH9
simulate_midi(4,9,arduino_digital_function_input[p][1],analog_data_from_arduino[p]/2);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Ctrl-Chge");      
break;
case 15://MIDI CH10
simulate_midi(4,10,arduino_digital_function_input[p][1],analog_data_from_arduino[p]/2);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Ctrl-Chge");      
break;
case 16://MIDI CH11
simulate_midi(4,11,arduino_digital_function_input[p][1],analog_data_from_arduino[p]/2);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Ctrl-Chge");      
break;
case 17://MIDI CH12
simulate_midi(4,12,arduino_digital_function_input[p][1],analog_data_from_arduino[p]/2);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Ctrl-Chge");      
break;
case 18://MIDI CH13
simulate_midi(4,13,arduino_digital_function_input[p][1],analog_data_from_arduino[p]/2);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Ctrl-Chge");      
break;
case 19://MIDI CH14
simulate_midi(4,14,arduino_digital_function_input[p][1],analog_data_from_arduino[p]/2);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Ctrl-Chge");      
break;
case 20://MIDI CH15
simulate_midi(4,15,arduino_digital_function_input[p][1],analog_data_from_arduino[p]/2);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Ctrl-Chge");      
break;
default:
break;

}

}
previous_analog_data_from_arduino[p]=analog_data_from_arduino[p];
//mise en boite des datas

} 

return(0);
}



int arduino_do_digital_in_whitecat()
{
int vfader=0;

for(int p=0;p<=arduino_max_digital;p++)
{
        
if(arduino_digital_type[p]==1||arduino_digital_type[p]==2||arduino_digital_type[p]==7  && digital_data_from_arduino[p]!= previous_digital_data_from_arduino[p] )//si l'affectation est type INPUT
//et qu il y a une différence d'état      
{
switch(arduino_digital_function_input[p][0]) 
{
case 0://desafected
break;
case 1: //fader up  
if(digital_data_from_arduino[p]==1) {
vfader=(arduino_digital_function_input[p][1]-1);  
start_time_for_delays[vfader]=actual_time;
if(lfo_mode_is[vfader]!=1)//up
{
lfo_mode_is[vfader]=1;
faders_in_float[vfader]=Fader[vfader];
if(index_midi_mute_on_lfo==1){is_raccrochage_midi_remote[vfader]=1;}
}
else {lfo_mode_is[vfader]=0;}  
lfo_cycle_is_on[vfader]=0;    
previous_digital_data_from_arduino[p]=toggle(previous_digital_data_from_arduino[p]);//pour ne pas faire un flicker sur le  go/pause/go  
}     

break;
case 2://fader down 
if(digital_data_from_arduino[p]==1) 
{
vfader=(arduino_digital_function_input[p][1]-1); 
start_time_for_delays[vfader]=actual_time;
if(lfo_mode_is[vfader]!=2)//down
{
lfo_mode_is[vfader]=2;
faders_in_float[vfader]=Fader[vfader];
if(index_midi_mute_on_lfo==1){is_raccrochage_midi_remote[vfader]=1;}
}
else {lfo_mode_is[vfader]=0;}
lfo_cycle_is_on[vfader]=0;
previous_digital_data_from_arduino[p]=toggle(previous_digital_data_from_arduino[p]);//pour ne pas faire un flicker sur le  go/pause/go
}

break;
case 3://Fader saw
if(digital_data_from_arduino[p]==1) 
{
vfader=(arduino_digital_function_input[p][1]-1);  
start_time_for_delays[vfader]=actual_time;
faders_in_float[vfader]=Fader[vfader];
if(lfo_cycle_is_on[vfader]==0)//
{
lfo_cycle_is_on[vfader]=1;
lfo_mode_is[vfader]=0;
lfo_running_is_upward[vfader]=1;
if(index_midi_mute_on_lfo==1){is_raccrochage_midi_remote[vfader]=1;}
}
else if(lfo_cycle_is_on[vfader]==1)
{
lfo_cycle_is_on[vfader]=0;
}
previous_digital_data_from_arduino[p]=toggle(previous_digital_data_from_arduino[p]);//pour ne pas faire un flicker sur le  go/pause/go  
}
break;      
case 4://Fader: ToPREVDock  
if(digital_data_from_arduino[p]==1) 
{   
vfader=(arduino_digital_function_input[p][1]-1);  
if (lfo_do_next_step[vfader][0]==0) {lfo_do_next_step[vfader][0]=1;lfo_do_next_step[vfader][1]=0;}   
else if (lfo_do_next_step[vfader][0]==1) {lfo_do_next_step[vfader][0]=0; }    
previous_digital_data_from_arduino[p]=toggle(previous_digital_data_from_arduino[p]);//pour ne pas faire un flicker sur le  go/pause/go  
}
break;
case 5://Fader: ToNEXTDock  
if(digital_data_from_arduino[p]==1) 
{
vfader=(arduino_digital_function_input[p][1]-1);  
if (lfo_do_next_step[vfader][1]==0) {lfo_do_next_step[vfader][1]=1;lfo_do_next_step[vfader][0]=0; ;}   
else if (lfo_do_next_step[vfader][1]==1) {lfo_do_next_step[vfader][1]=0; }        
previous_digital_data_from_arduino[p]=toggle(previous_digital_data_from_arduino[p]);//pour ne pas faire un flicker sur le  go/pause/go  
} 
break;
case 6://Fader: Up/Down
if(digital_data_from_arduino[p]==1) 
{
vfader=(arduino_digital_function_input[p][1]-1);  
lfo_cycle_steps[vfader]= toggle(lfo_cycle_steps[vfader]);
previous_digital_data_from_arduino[p]=toggle(previous_digital_data_from_arduino[p]);//pour ne pas faire un flicker sur le  go/pause/go  
}
break;
case 7://Fader: LOCK
if(digital_data_from_arduino[p]==1) //si appuyé
{
vfader=(arduino_digital_function_input[p][1]-1);  
 switch(FaderLocked[vfader])
  {
   case 0:
   FaderLocked[vfader]=1;
   StateOfFaderBeforeLock[vfader]=Fader[vfader];
   if(StateOfFaderBeforeLock[vfader]==255){LockFader_is_FullLevel[vfader]=1;}
   else if(StateOfFaderBeforeLock[vfader]<255){LockFader_is_FullLevel[vfader]=0;}
   sprintf(string_Last_Order,">> LOCKED Fader %d",vfader+1);
   if(LockFader_is_FullLevel[vfader]==0)//quand mis en lock et pas full level rajout 0.7.6
   {
   lfo_mode_is[vfader]=0;
   lfo_cycle_is_on[vfader]=0;
   }
   break; 
   case 1:
   FaderLocked[vfader]=0;
   //remise à plat du niveau
   Fader[vfader]=(unsigned char)((((float)(StateOfFaderBeforeLock[vfader]))/255)*locklevel);
   midi_levels[vfader]=(int)(((float)Fader[vfader])/2);
   sprintf(string_Last_Order,">> UNLOCKED Fader %d",vfader+1);
   break;     
   }
   previous_digital_data_from_arduino[p]=toggle(previous_digital_data_from_arduino[p]);//pour ne pas faire un flicker sur le  go/pause/go  
}
break;
case 8://FLASH
vfader=(arduino_digital_function_input[p][1]-1);  
FaderIsFlash[vfader]=digital_data_from_arduino[p];
break;
case 9://Fader: All at 0
if(digital_data_from_arduino[p]==1) //si appuyé
{
vfader=(arduino_digital_function_input[p][1]-1);  
           Fader[vfader]=0;
           midi_levels[vfader]=0;
           index_send_midi_out[vfader]=1;
           lfo_speed[vfader]=64;
           midi_levels[196+p]=64;
           index_send_midi_out[196+p]=1;
           FaderLocked[vfader]=0;
           lfo_mode_is[vfader]=0;//no lfo
           lfo_cycle_is_on[vfader]=0;//saw
           lfo_cycle_steps[vfader]=0;
           lfo_do_next_step[vfader][0]=0;
           lfo_do_next_step[vfader][1]=0; 
           for(int d=0;d<6;d++)
           {
           is_dock_for_lfo_selected[vfader][d]=0;        
           }   
previous_digital_data_from_arduino[p]=toggle(previous_digital_data_from_arduino[p]);//pour ne pas faire un flicker sur le  go/pause/go  
}
break;
case 10://Fader: L/Unloop dock
if(digital_data_from_arduino[p]==1) //si appuyé
{
vfader=(arduino_digital_function_input[p][1]-1);
is_dock_for_lfo_selected[vfader][(dock_used_by_fader_is[vfader])]=toggle(is_dock_for_lfo_selected[vfader][(dock_used_by_fader_is[vfader])]);
}  
digital_data_from_arduino[p]=toggle(digital_data_from_arduino[p]);
break;
case 11://Fader: L/Unloop all
if(digital_data_from_arduino[p]==1) //si appuyé
{
vfader=(arduino_digital_function_input[p][1]-1); 
for(int d=0;d<6;d++)
           {
           is_dock_for_lfo_selected[vfader][d]=toggle(is_dock_for_lfo_selected[vfader][d]);        
           }  
previous_digital_data_from_arduino[p]=toggle(previous_digital_data_from_arduino[p]);//pour ne pas faire un flicker sur le  go/pause/go  
}
break;
case 12://Seq: GO
if(digital_data_from_arduino[p]==1) //si appuyé
{
do_go_function();
do_send_bang();
previous_digital_data_from_arduino[p]=toggle(previous_digital_data_from_arduino[p]);//pour ne pas faire un flicker sur le  go/pause/go
}
break;
case 13://Seq: GO BACK
if(digital_data_from_arduino[p]==1) //si appuyé
{
do_go_back_function();
int banger_back_is=Banger_Memoire[position_preset]-1;
do_go_back_bang(banger_back_is);
previous_digital_data_from_arduino[p]=toggle(previous_digital_data_from_arduino[p]);
}
break;
case 14://Seq: JUMP
if(digital_data_from_arduino[p]==1) //si appuyé
{
do_double_go_function();
do_send_bang();  
previous_digital_data_from_arduino[p]=toggle(previous_digital_data_from_arduino[p]);
}
break;
case 15://Seq: SHIFT-W
if(digital_data_from_arduino[p]==1) //si appuyé
{
index_go=0;index_pause=0; index_go_back=0; 
detect_mem_preset_previous();
refresh_mem_onpreset(position_preset);      
niveauX2=0; 
previous_digital_data_from_arduino[p]=toggle(previous_digital_data_from_arduino[p]);
someone_changed_in_sequences=1;//icat
}
break;
case 16://Seq: SHIFT-X
if(digital_data_from_arduino[p]==1) //si appuyé
{
index_go=0;index_pause=0; index_go_back=0;  
detect_next_preset();
refresh_mem_onpreset(position_preset);  
niveauX2=0; 
previous_digital_data_from_arduino[p]=toggle(previous_digital_data_from_arduino[p]);
someone_changed_in_sequences=1;//icat
}
break;
case 17:////As Key-On CH0 P:
if(digital_data_from_arduino[p]==1) //si appuyé
{
simulate_midi(1,0,arduino_digital_function_input[p][1],127);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Key-On");
simulate_midi(2,0,arduino_digital_function_input[p][1],0);
}
break;
case 18:////As Key-On CH1 P:
if(digital_data_from_arduino[p]==1) //si appuyé
{
simulate_midi(1,1,arduino_digital_function_input[p][1],127);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Key-On");
simulate_midi(2,1,arduino_digital_function_input[p][1],0);
}
break;
case 19:////As Key-On CH2 P:
if(digital_data_from_arduino[p]==1) //si appuyé
{
simulate_midi(1,2,arduino_digital_function_input[p][1],127);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Key-On");  
simulate_midi(2,3,arduino_digital_function_input[p][1],0);
}
break;
case 20:////As Key-On CH3 P:
if(digital_data_from_arduino[p]==1) //si appuyé
{
simulate_midi(1,3,arduino_digital_function_input[p][1],127);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Key-On"); 
simulate_midi(2,4,arduino_digital_function_input[p][1],0);
}
break;
case 21:////As Key-On CH4 P:
if(digital_data_from_arduino[p]==1) //si appuyé
{
simulate_midi(1,4,arduino_digital_function_input[p][1],127);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Key-On"); 
simulate_midi(2,4,arduino_digital_function_input[p][1],0);
}
break;
case 22:////As Key-On CH5 P:
if(digital_data_from_arduino[p]==1) //si appuyé
{
simulate_midi(1,5,arduino_digital_function_input[p][1],127);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Key-On"); 
simulate_midi(2,5,arduino_digital_function_input[p][1],0);
}
break;
case 23:////As Key-On CH6 P:
if(digital_data_from_arduino[p]==1) //si appuyé
{
simulate_midi(1,6,arduino_digital_function_input[p][1],127);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Key-On");
simulate_midi(2,6,arduino_digital_function_input[p][1],0);
}
break;
case 24:////As Key-On CH7 P:
if(digital_data_from_arduino[p]==1) //si appuyé
{
simulate_midi(1,7,arduino_digital_function_input[p][1],127);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Key-On");
simulate_midi(2,7,arduino_digital_function_input[p][1],0);
}
break;
case 25:////As Key-On CH8 P:
if(digital_data_from_arduino[p]==1) //si appuyé
{
simulate_midi(1,8,arduino_digital_function_input[p][1],127);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Key-On"); 
simulate_midi(2,8,arduino_digital_function_input[p][1],0);
}
break;
case 26:////As Key-On CH9 P:
if(digital_data_from_arduino[p]==1) //si appuyé
{
simulate_midi(1,9,arduino_digital_function_input[p][1],127);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Key-On");
simulate_midi(2,9,arduino_digital_function_input[p][1],0);
}
break;
case 27:////As Key-On CH10 P:
if(digital_data_from_arduino[p]==1) //si appuyé
{
simulate_midi(1,10,arduino_digital_function_input[p][1],127);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Key-On");  
simulate_midi(2,10,arduino_digital_function_input[p][1],0);
}
break;
case 28:////As Key-On CH11 P:
if(digital_data_from_arduino[p]==1) //si appuyé
{
simulate_midi(1,11,arduino_digital_function_input[p][1],127);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Key-On");
simulate_midi(2,11,arduino_digital_function_input[p][1],0);
}
break;
case 29:////As Key-On CH12 P:
if(digital_data_from_arduino[p]==1) //si appuyé
{
simulate_midi(1,12,arduino_digital_function_input[p][1],127);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Key-On");
simulate_midi(2,12,arduino_digital_function_input[p][1],0);
}
break;
case 30:////As Key-On CH13 P:
if(digital_data_from_arduino[p]==1) //si appuyé
{
simulate_midi(1,13,arduino_digital_function_input[p][1],127);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Key-On"); 
simulate_midi(2,13,arduino_digital_function_input[p][1],0);
}
break;
case 31:////As Key-On CH14 P:
if(digital_data_from_arduino[p]==1) //si appuyé
{
simulate_midi(1,14,arduino_digital_function_input[p][1],127);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Key-On");
simulate_midi(2,14,arduino_digital_function_input[p][1],0);
}
break;
case 32:////As Key-On CH15 P:
if(digital_data_from_arduino[p]==1) //si appuyé
{
simulate_midi(1,15,arduino_digital_function_input[p][1],127);
sprintf (my_midi_string, "Arduino Chan:%-2d Pitch:%-2d Vel:%-2d Typ: %s",	ischan,ispitch,isvel, "Key-On");
simulate_midi(2,15,arduino_digital_function_input[p][1],0);
}
break;
default:
break;                                        
}                                               



}                                  
}

 return(0);   
}

int arduino_do_pwm_out_whitecat()
{
index_send_pwm_data=0;

char temp_send_pwm[(3+arduino_max_digital+3+1)];    //header/tableau/headerfin/return

if(need_send_pwm==1)
{
if(ticks_arduino_passe!=ticks_arduino+1&&ticks_arduino<ticks_arduino_passe+2)
{
need_send_pwm=1;
index_send_pwm_data=0;
} 
if(ticks_arduino>ticks_arduino_passe+2)
{
need_send_pwm=0;
index_send_pwm_data=1;
} 
}

for(int p=0;p<=arduino_max_digital;p++)
{


if(arduino_digital_type[p]==6 || arduino_digital_type[p]==4|| arduino_digital_type[p]==5)//si l'affectation est type OUTPUT   PWM
{
//CHANNEL 
if(arduino_digital_function_output[p][0]==1 && arduino_digital_function_output[p][1]>0 && arduino_digital_function_output[p][1]<513) 
{    
pwm_data_to_arduino[p]=MergerArray[(arduino_digital_function_output[p][1])];
}  
//FADER
else if(arduino_digital_function_output[p][0]==2 && arduino_digital_function_output[p][1]>0 && arduino_digital_function_output[p][1]<49) 
{   
pwm_data_to_arduino[p]=(int)Fader[(arduino_digital_function_output[p][1])-1];
}  

//if(index_send_pwm_data==0){index_send_pwm_data=1;}
if(pwm_data_to_arduino[p]!=previous_pwm_data_to_arduino[p]||need_send_pwm==1){
                                                       index_send_pwm_data=1;}
                                                       
if(need_send_pwm==0)
{previous_pwm_data_to_arduino[p]=pwm_data_to_arduino[p];} 

}
}

if(index_send_pwm_data==1 && need_send_pwm==0)
{
sprintf(temp_send_pwm,"");
temp_send_pwm[0]='P';temp_send_pwm[1]='W';temp_send_pwm[2]='/';

for(int k=0;k<=arduino_max_digital;k++)
{
if(pwm_data_to_arduino[k]<255)
{
temp_send_pwm[3+k]=pwm_data_to_arduino[k]+1;//+1 pour éviter caractere 0
}
else {temp_send_pwm[3+k]=255;}
}
temp_send_pwm[(3+arduino_max_digital+1)]='E';
temp_send_pwm[(3+arduino_max_digital+2)]='D';
temp_send_pwm[(3+arduino_max_digital+3)]='/';
temp_send_pwm[(3+arduino_max_digital+4)]='\0';

int nBytesSendtoArduino=serial0.SendData(temp_send_pwm,sizeof(temp_send_pwm));
} 
                     
 return(0);   
}



int arduino_do_digital_out_whitecat()
{
index_send_digital_data=0;   
char temp_send[(3+arduino_max_digital+3+1)];    //header/tableau/headerfin/return
int value_dm=0;
switch(dmx_view)
{
case 0:
     value_dm=(int)(10*2.55);
break;
case 1:
     value_dm=10;
break;                
}

for(int p=0;p<=arduino_max_digital;p++)
{
previous_digital_data_to_arduino[p]=digital_data_to_arduino[p]; 
if(arduino_digital_type[p]==3 )//si l'affectation est type OUTPUT   
{
if(arduino_digital_function_output[p][0]==1) //CHANNEL >10
{
if(MergerArray[(arduino_digital_function_output[p][1])]>value_dm)

{digital_data_to_arduino[p]=1;}
else {digital_data_to_arduino[p]=0;}

if(digital_data_to_arduino[p]!=previous_digital_data_to_arduino[p])
{need_send_pwm=1;index_send_digital_data=1;ticks_arduino_passe=ticks_arduino;
previous_digital_data_to_arduino[p]=digital_data_to_arduino[p]; }

} 
 
else if(arduino_digital_function_output[p][0]==2 && arduino_digital_function_output[p][1]>0 && arduino_digital_function_output[p][1]<49) //Fader >10
{
//previous_digital_data_to_arduino[p]=digital_data_to_arduino[p];      
 
if(Fader[(arduino_digital_function_output[p][1]-1)]>value_dm)
{digital_data_to_arduino[p]=1;}
else {digital_data_to_arduino[p]=0;}

if(digital_data_to_arduino[p]!=previous_digital_data_to_arduino[p])
{need_send_pwm=1;index_send_digital_data=1;ticks_arduino_passe=ticks_arduino;
previous_digital_data_to_arduino[p]=digital_data_to_arduino[p]; }
 
//index_send_digital_data=1;
//if(digital_data_to_arduino[p]!=previous_digital_data_to_arduino[p]){index_send_digital_data=1;}
}  
 
}
}

if(index_send_digital_data)
{
sprintf(arduino_order,"");
temp_send[0]='D';temp_send[1]='O';temp_send[2]='/';

for(int k=0;k<=arduino_max_digital;k++)
{
switch(digital_data_to_arduino[k])
{
case 0:
temp_send[3+k]=32; 
break;

case 1:
temp_send[3+k]=127;
break ;
}
}
temp_send[(3+arduino_max_digital+1)]='E';
temp_send[(3+arduino_max_digital+2)]='D';
temp_send[(3+arduino_max_digital+3)]='/';
temp_send[(3+arduino_max_digital+4)]='\0';

if(need_send_pwm==1)
{int nBytesSendtoArduino=serial0.SendData(temp_send,sizeof(temp_send));} 

}                       

 return(0);   
}




int arduino__send_config()
{
index_send_digital_data=0; 
index_send_pwm_data=0;  
if(arduino_device_0_is_ignited==1 && index_send_arduino_config==1)
{
int type_pin[arduino_max_digital];
char temp_send_config[(3+arduino_max_digital+3+1)];    //header/tableau/headerfin/return
    

                        
for(int p=0;p<=arduino_max_digital;p++)
{
  type_pin[p]= arduino_digital_type[p];
} 

sprintf(temp_send_config,"");
temp_send_config[0]='C';temp_send_config[1]='O';temp_send_config[2]='/';

for(int k=0;k<=arduino_max_digital;k++)
{ 

switch ( type_pin[k])
{   
case 0://si l'affectation est type None   
temp_send_config[3+k]=30;   
break;

case 1: ////si l'affectation est type INPUT   
temp_send_config[3+k]=31;   
break;

case 2 ://si l'affectation est type PULL UP   
temp_send_config[3+k]=33; 
break;

case 3 ://si l'affectation est type OUTPUT   
temp_send_config[3+k]=34;   
break;

case 4 ://si l'affectation est type PWM   
temp_send_config[3+k]=35;   
break;

case 5 ://si l'affectation est type SERVO   
temp_send_config[3+k]=36;   
break;

case 6 ://si l'affectation est type RF12  OUT  
temp_send_config[3+k]=37;   
break;

case 7 ://si l'affectation est type RF12  IN
temp_send_config[3+k]=38; 
break;  
}
}


temp_send_config[(3+arduino_max_digital+1)]='E';
temp_send_config[(3+arduino_max_digital+2)]='D';
temp_send_config[(3+arduino_max_digital+3)]='/';
temp_send_config[(3+arduino_max_digital+4)]='\0';
int nBytesSendtoArduino=serial0.SendData(temp_send_config,sizeof(temp_send_config)); 
}
index_send_arduino_config=0;
index_send_pwm_data=1;
index_send_digital_data=1; 
 return(0);   
}



