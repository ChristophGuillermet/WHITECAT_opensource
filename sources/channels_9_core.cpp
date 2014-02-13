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

 \file channels_9_core.cpp
 \brief {description courte}
 \author Christoph Guillermet
 \version {numero de version du fichier}
 \date {date description}

 White Cat {- catégorie} {- sous catégorie {- sous catégorie}}
 Description détaillée

 **/


#include <allegro.h>

int snap_channels_selection_array()//pour saisie continue des circuits, au click button
{
for (int i=0;i<513;i++)
{
 Temp_Selected_Channel[i]=Selected_Channel[i];
}
return(0);
}


int channel_copy()
{
    for(int ck=1;ck<513;ck++)
    {
    bufferCopyPaste[ck]=0;
    SelectedForCopyPaste[ck]=0;
    if(index_blind==0)
    {
    if(Selected_Channel[ck]==1)
    {
    SelectedForCopyPaste[ck]=1;
    bufferCopyPaste[ck]=Tmax (bufferSequenciel[ck],bufferFaders[ck]);
    }
    }
    else
    {
    if(Selected_Channel[ck]==1)
    {
    SelectedForCopyPaste[ck]=1;
    bufferCopyPaste[ck]=bufferBlind[ck];
    }
    }
    }
    sprintf(string_Last_Order,">> Pasted selected channels in memory");
return(0);
}

int channel_paste()
{
    for(int ck=1;ck<513;ck++)
    {
    if(index_blind==0)
    {
    if(SelectedForCopyPaste[ck]==1){bufferSaisie[ck]=bufferCopyPaste[ck];}
        sprintf(string_Last_Order,">> Pasted channels from memory to Stage");
    }
    else{
    if(SelectedForCopyPaste[ck]==1){bufferBlind[ck]=bufferCopyPaste[ck];}
        sprintf(string_Last_Order,">> Pasted channels from memory to Blind");
    }
    }

return(0);
}



int reset_blind()
{
 for (int ci=1;ci<514;ci++)
 {
 bufferBlind[ci]=0;
 }
 return(0);
}


int Channel_select_all()
{
 if(index_blind==0)
 {
 for (int ci=1;ci<514;ci++)
 {
 if(bufferSaisie[ci]==0)    {Selected_Channel[ci]=0;}
 else if (bufferSaisie[ci]>0)    {Selected_Channel[ci]=1;}
 }
 }
 else if(index_blind==1)
 {
 for (int ci=1;ci<514;ci++)
 {
 if(bufferBlind[ci]==0)    {Selected_Channel[ci]=0;}
 else if (bufferBlind[ci]>0)    {Selected_Channel[ci]=1;}
 }
 }
 sprintf(string_Last_Order,">> Selected All lighting channels ( ALL )");
return(0);
}

int Channel_select_inv()
{
 if(index_blind==0)
 {
 for (int ci=1;ci<514;ci++)
 {
 if(bufferSaisie[ci]>0 && Selected_Channel[ci]==1)    {Selected_Channel[ci]=0;}
 else if (bufferSaisie[ci]>0 && Selected_Channel[ci]==0)    {Selected_Channel[ci]=1;}
 }
 }
 else if(index_blind==1)
 {
 for (int ci=1;ci<514;ci++)
 {
 if(bufferBlind[ci]>0 && Selected_Channel[ci]==1)    {Selected_Channel[ci]=0;}
 else if (bufferBlind[ci]>0 && Selected_Channel[ci]==0)    {Selected_Channel[ci]=1;}
 }
 }
 sprintf(string_Last_Order,">> Selected All lighting channels non selected ( INV )");
return(0);
}


int Channel_select_thruth(int fromch, int toch)
{
 if(fromch<toch)
 {
  for (int cit=fromch;cit<=toch;cit++)
 {Selected_Channel[cit]=1;}
 }
 if (fromch>toch)
 {
  for (int cit=toch;cit<=fromch;cit++)  {Selected_Channel[cit]=1;}
 }
  sprintf(string_Last_Order,">> Selected from Ch %d to Ch %d", fromch, toch);
 return(0);
}


int Channel_at_level()
{
 int chlevelis=0;
 if(dmx_view==0)
 {chlevelis=(int)( atof(numeric)*2.55); }
 else if (dmx_view==1)
 {chlevelis= atol(numeric);}
 if(chlevelis>255){chlevelis=255;}
 if(chlevelis<0){chlevelis=0;}

 for (int ci=1;ci<514;ci++)
 {
 if(Selected_Channel[ci]==1 && index_blind==0)    {bufferSaisie[ci]=chlevelis;}
 else if(Selected_Channel[ci]==1  && index_blind==1)    {bufferBlind[ci]=chlevelis;}
 }

 sprintf(string_Last_Order,">> Selection AT %d", chlevelis);
 return(0);
}

int DoMouseLevel()
{
 if (mouse_z>last_scroll_mouse_for_chan )
 {

 simulate_keypress(KEY_UP << 8); last_scroll_mouse_for_chan= mouse_z;
 }
 else if (mouse_z<last_scroll_mouse_for_chan )
 {

  simulate_keypress(KEY_DOWN << 8);  last_scroll_mouse_for_chan= mouse_z;
 }

 if (window_focus_id==W_BANGER &&  mouse_x>=X_banger && mouse_x<=X_banger+480 && mouse_y>=Y_banger && mouse_y<=Y_banger+300  )//banger
 {
 switch(over_family)
 {
 case 0:
 if(editing_banger_family ==1 &&  over_banger_event>0)
 {
 if(mouse_z>mouse_level_for_event)
 {
 bangers_type[index_banger_selected][ over_banger_event-1]++;
 reset_banger_event(index_banger_selected, over_banger_event-1);
 constrain_banger_type( over_banger_event-1);
 position_mouse_z(0);  //reset
 mouse_level_for_event=mouse_z;
 mouse_level_for_banger=mouse_z;
 editing_banger_family=0;editing_banger_action=0;
 }
 if(mouse_z<mouse_level_for_event)
 {
 bangers_type[index_banger_selected][ over_banger_event-1]--;
 if(bangers_type[index_banger_selected][ over_banger_event-1]<0){bangers_type[index_banger_selected][ over_banger_event-1]=0;};
 reset_banger_event(index_banger_selected, over_banger_event-1);
 position_mouse_z(0);  //reset
 mouse_level_for_event=mouse_z;
 mouse_level_for_banger=mouse_z;
 editing_banger_family=0;editing_banger_action=0;
 }
 }
 break;
 case 1:
 if(editing_banger_action==1 && over_banger_action>0)
 {
 if(mouse_z>mouse_level_for_banger)
 {
 position_mouse_z(0);//reset
 mouse_level_for_banger=mouse_z;
 mouse_level_for_event=mouse_z;
 editing_banger_family=0;editing_banger_action=0;
 bangers_action[index_banger_selected][over_banger_action-1]++;
 reset_banger_params(index_banger_selected,over_banger_action-1);
 constrain_banger_param(over_banger_action-1);
 }
 if(mouse_z<mouse_level_for_banger)
 {
 position_mouse_z(0);//reset
 mouse_level_for_banger=mouse_z;
 mouse_level_for_event=mouse_z;
 bangers_action[index_banger_selected][over_banger_action-1]--;
 if(bangers_action[index_banger_selected][over_banger_action]<0){bangers_action[index_banger_selected][over_banger_action-1]=0;}
 reset_banger_params(index_banger_selected,over_banger_action-1);
 }
 }
 break;
 default:
 break;
 }
 }
 else
 {
 mouse_level_for_event=mouse_z;
 mouse_level_for_banger=mouse_z;
 editing_banger_family=0;editing_banger_action=0;
 }
 return(0);
}





int do_logical_ChannelScroller( int ScrollX, int ScrollY)
{

if(mouse_x>=ScrollX-10 && mouse_x<ScrollX-10+100 && mouse_y>ScrollY-5 && mouse_y<ScrollY+270
&& index_over_A_window==0 &&  index_over_faderspace==0 && mouse_released==0)
{
if( Midi_Faders_Affectation_Type!=0 )
{
attribute_midi_solo_affectation(1624,Midi_Faders_Affectation_Mode);
mouse_released=1;
}
else
{
scroll_channelspace=mouse_y-ScrollY;
}
}
return(0);
}

int do_logical_Draw_Channel_Preset_View( int xchan, int ychan,   int prst_v)
{

int maxchan_per_ligne=12;
int nbre_lignes=channel_number_of_lines[prst_v];
int num_circ=0;
int ypos_ch=0;



for (int l=0;l<nbre_lignes+1;l++)
{
//if (l==nbre_lignes) {maxchan_per_ligne=(channel_number_in_View[prst_v]%13)+1;} //derniere ligne à 512
for (int c=1; c<=maxchan_per_ligne;c++)
{
ypos_ch=ychan+(l*70);
num_circ=Channel_View_ROUTING[prst_v][c+(l*12)-1];
//Selection circuit
if (index_over_A_window==0 && index_over_faderspace==0 && mouse_x>   ((xchan-10) + (45*c)) && mouse_x< ((xchan+30) + (45*c))
&& mouse_y> ypos_ch && mouse_y< ypos_ch+65
&&  num_circ!=0 && num_circ<513)
{

if(index_level_attribue==1)//pour déselection lors prochain circuit piqué dominique guesdon 10aout 2010
{
for(int y=1;y<512;y++)
{
Selected_Channel[y]=0;
last_ch_selected=0;
if(index_plot_window==1){substract_channel_selection_to_layers_plot();}
}
index_level_attribue=0;
}

if (index_ch_thruth==0)//selection normale
 {
 Selected_Channel[num_circ]=1-(Temp_Selected_Channel[num_circ]); //chainage mouse released sorti
 last_ch_selected=num_circ;
 if(index_plot_window==1){add_channel_selection_to_layers_plot();substract_channel_selection_to_layers_plot();}
 }
 else if (index_ch_thruth==1)//selection thruth
 {
 Channel_select_thruth(last_ch_selected,num_circ);
 index_ch_thruth=0;
 mouse_released=1;
 if(index_plot_window==1){add_channel_selection_to_layers_plot();substract_channel_selection_to_layers_plot();}
 }
 //
}
}

}

return(0);
}


int do_logical_Draw_Channel_Preset_Title(int xchan, int ychan, int prst_v)
{


 if(window_focus_id==0 && mouse_button==1  && mouse_released==0 && prst_v!=0 )
 {
 if(mouse_y>ychan && mouse_y<ychan+20  )
 {
 //TITRE VIEW
 if(mouse_x> xchan+40 && mouse_x<xchan+40+210)
 {
 if(prst_v>0 && index_type==1 )
 {
 sprintf( channel_view_Name[prst_v],numeric);
 reset_numeric_entry();
 numeric_postext=0;
 mouse_released=1;
 }
 }
 // Mem sel
 else if(mouse_x>  xchan+435 && mouse_x<xchan+435+20)
 {
 if(channel_view_type_of_behaviour[prst_v]!=1){channel_view_type_of_behaviour[prst_v]=1;}
 else{channel_view_type_of_behaviour[prst_v]=0;}
 mouse_released=1;
 }
 // fader sel
 else if(mouse_x>  xchan+460 && mouse_x<xchan+460+20 )
 {
 if(channel_view_type_of_behaviour[prst_v]!=2){channel_view_type_of_behaviour[prst_v]=2;}
 else{channel_view_type_of_behaviour[prst_v]=0;}
 mouse_released=1;
 }

 //All sel
  else if(mouse_x>  xchan+485 && mouse_x<xchan+485+30  )
 {
 channel_view_mode_builder[prst_v]=toggle(channel_view_mode_builder[prst_v]);
 mouse_released=1;
 }

 //do build logique sortie car plantage grave à la reconstruction. il faut garder ca dans la boucle affichage
/* else if(mouse_x>  xchan+520 && mouse_x<xchan+520+55 )
 {
 channel_view_is=prst_v;
 if(index_do_dock==1)
 {
 if(channel_view_type_of_behaviour[channel_view_is]==0 &&  channel_view_mode_builder[channel_view_is]==0)
 {index_ask_record_selection_of_view=1 ; index_ask_confirm=1; mouse_released=1;}
 else  {index_ask_build_view=1; index_ask_confirm=1; mouse_released=1;  }
 }

 else if(index_do_modify==1)
 {
 index_ask_modify_selection_of_view=1 ;index_ask_confirm=1;   mouse_released=1;
 }
 else if(index_do_report==1)
 {
 index_ask_report_selection_of_view=1;index_ask_confirm=1;      mouse_released=1;
 }
 else if(index_main_clear==1)
 {
 index_ask_clear_selection_of_view=1;   mouse_released=1;
 }

 } */


 }

 }

 return(0);
}




int do_logical_ClassicalChannelSpace( int xchan, int ychan, int scroll)//les 512 circuits
{

int maxchan_per_ligne=13;
for (int l=0;l<43;l++)
{
if (l==42) {maxchan_per_ligne=9;} //derniere ligne à 512

for (int c=1; c<maxchan_per_ligne;c++)
{

//Selection circuit
if ( mouse_x>   ((xchan-10) + (45*c)) && mouse_x< ((xchan+30) + (45*c))
&& mouse_y> (((ychan*l)+ 40) - (int)((float)(scroll)* Ch_Scroll_Factor) )
&& mouse_y< (((ychan*l)+ 100 - (int)((float)(scroll)* Ch_Scroll_Factor)) )
&& index_over_A_window==0 && index_over_faderspace==0
)
{

if(index_level_attribue==1)//pour déselection lors prochain circuit piqué dominique guesdon 10aout 2010
{
for(int y=1;y<512;y++)
{
Selected_Channel[y]=0;
last_ch_selected=0;
if(index_plot_window==1){substract_channel_selection_to_layers_plot();}
}
index_level_attribue=0;
}

 if (index_ch_thruth==0)//selection normale
 {
 Selected_Channel[c+(l*12)]=1-(Temp_Selected_Channel[c+(l*12)]); //chainage mouse released sorti
 if(Selected_Channel[c+(l*12)]==1){ last_ch_selected=c+(l*12);}
 if(index_plot_window==1){add_channel_selection_to_layers_plot();substract_channel_selection_to_layers_plot();}
 }
 else if (index_ch_thruth==1)//selection thruth
 {
 Channel_select_thruth(last_ch_selected,c+(l*12));
 last_ch_selected=c+(l*12);
 index_ch_thruth=0;
 mouse_released=1;
 if(index_plot_window==1){add_channel_selection_to_layers_plot();substract_channel_selection_to_layers_plot();}
 }
 //
}
}

}

return(0);
}


int do_logical_ChannelsMenuSelection(int chx, int chy)
{

//LOGIQUE
//if(window_focus_id==0 && index_over_A_window==0 && index_over_faderspace==0 && mouse_button==1 && mouse_released==0)
if(window_focus_id==0 && index_over_faderspace==0 && mouse_button==1 && mouse_released==0)
{
if( mouse_y>chy+1 && mouse_y<chy+1+18 && mouse_x>chx+70 && mouse_x<chx+70+60 )
 {
 if(Midi_Faders_Affectation_Type!=0)
{
  attribute_midi_solo_affectation(1644,Midi_Faders_Affectation_Mode);
}
else
{
 ClassicalChannelView=toggle(ClassicalChannelView);
}
 mouse_released=1;
 }
for(int lv=0;lv<2;lv++)
{
for(int i=0;i<8;i++)
{
if(mouse_x>chx+140+(i*15) && mouse_x<chx+140+(i*15)+10 && mouse_y>chy+1+(15*lv) && mouse_y<chy+1+(15*lv)+10)
{
if(Midi_Faders_Affectation_Type!=0)
{
  attribute_midi_to_control(1628+i+(lv*8),Midi_Faders_Affectation_Type,Midi_Faders_Affectation_Mode);
}
else
{
if(index_do_dock==0 && index_do_modify==0 && index_main_clear==0)
{
if(ClassicalChannelView==1){ClassicalChannelView=0;if(Channel_View_MODE[i+(lv*8)]==0){Channel_View_MODE[i+(lv*8)]=1;}}
else
{
Channel_View_MODE[i+(lv*8)]=toggle(Channel_View_MODE[i+(lv*8)]);
ClassicalChannelView=0;
}
}
else if(index_do_dock==1 && i+(lv*8)!=0)//=vue patch
{
channel_view_is=i+(lv*8);
index_ask_record_selection_of_view=i+(lv*8);
index_ask_confirm=1;
}
else if(index_do_modify==1 && i+(lv*8)!=0)//=vue patch
{
channel_view_is=i+(lv*8);

index_ask_modify_selection_of_view=1;
index_ask_confirm=1;
}
else if(index_main_clear==1 && i+(lv*8)!=0)//=vue patch
{
channel_view_is=i+(lv*8);
index_ask_confirm=1;
index_ask_clear_selection_of_view=1;
}
}
mouse_released=1;
}
}
}
command_button_logical(chx+270,chy+1,index_blind,"Blind","Shift-F10",754,110);
command_button_logical(chx+340,chy+1,index_direct_chan,"Direct CH.","",1333,111);
command_button_logical(chx+410,chy+1,index_inspekt,"View","",1334,112);// int x, inty ,bool state, char *textedesc, int midiaffectation
command_button_logical(chx+480,chy+1,index_do_hipass,"HiPass","",1542,113);// int x, inty ,bool state, char *textedesc, int midiaffectation
command_button_logical(chx+550,chy+1,index_do_fgroup,"FGroup","",1592,114);// int x, inty ,bool state, char *textedesc, int midiaffectation

command_button_logical(chx+620,chy+1,Midi_Faders_Affectation_Type,"MidiAffect","",1625,106);// int x, inty ,bool state, char *textedesc, int midiaffectation
command_button_logical(chx+690,chy+1,index_midi_mute,"MidiMute","",1277,107);// int x, inty ,bool state, char *textedesc, int midiaffectation
command_button_logical(chx+760,chy+1,index_global_midi_send_on_faders,"M.Out Fad.","",1593,109);// int x, inty ,bool state, char *textedesc, int midiaffectation

command_button_logical(chx+830,chy+1,index_do_dock,"STORE","F1",743,102);// int x, inty ,bool state, char *textedesc, int midiaffectation
command_button_logical(chx+900,chy+1,index_do_modify,"MODIFY","F2",744,103);
command_button_logical(chx+970,chy+1,index_do_report,"REPORT","F3",745,104);
command_button_logical(chx+1040,chy+1,index_main_clear,"CLEAR","F4",746,105);
command_button_logical(chx+1110,chy+1,index_show_main_menu,"MENUS","RIGHT CLICK",1626,108);
}
//fin windows focus
return(0);
}
