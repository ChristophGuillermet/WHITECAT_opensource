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

 \file MAIN_nov_2013_opensource_kbd.cpp
 \brief Main
 \author Christoph Guillermet
 \version {numero de version du fichier}
 \date {date description}

 White Cat root
 Description détaillée

 **/

 #include <allegro.h>
#include <winalleg.h>
#include <OpenLayer.hpp>
#include <stdio.h>
#include <loadpng.h>
#include <assert.h>
#include <Iphlpapi.h>
#include <audiere.h>

#include <jpgalleg.h>


using namespace audiere;
using namespace ol;


int BPS_RATE=50;//devient dmx rate
int dmxINrate=25;
int ARDUINO_RATE=20;


//ticks
volatile int ticks = 0;
int ticker_rate = BPS_TO_TIMER(BPS_RATE);
int ticker_dmxIn_rate = BPS_TO_TIMER(dmxINrate);
int ticks_passed = 0;
volatile int mouse_button;
volatile int mouse_released;
volatile int mouse_maintained;
volatile int mouse_R_button;
volatile int mouse_R_released;

volatile bool calculation_on_faders_done=0;//pour snap des faders depuis Echo
//////////////////////////////////////////////////////////////////////////////
#define PI 3.14116 // pour ok trichro
//#define PI  3.14159265358979323846264338327950288419716939937510
//////////////////////////////////////////////////////////////////////////////

////////////////////REMPLACEMENT DU GCC 3.4 >? //////////////////////////////////
template <class T> const T& Tmax ( const T& a, const T& b ) {
  return (b<a)?a:b;     // or: return comp(b,a)?a:b; for the comp version
}
/* exemple
bufferSaisiesnamp=0;
   for(int j=0;j<48;j++)
   {
   bufferSaisiesnamp= Tmax (bufferSaisiesnamp,(int)bufferSaisie[j]);
   }
*/




#include <hpdf.h>
#include <MidiShare.h>
#include <whitecat.h>
#include <my_window_file_sample.h>//ressources juste après whitecat.h
#include <patch_splines_2.cpp>//spline pour curves


#include <grider_calculs8.cpp>


#include <midi_CORE.cpp>
#include <CORE_6.cpp>




#include <saves_export_pdf2.cpp>
#include <saves_export_import.cpp>
#include <saves_menu_8.cpp>
#include <gestionaire_fenetres2.cpp>

#include <gui_boutons_rebuild1.cpp>
#include <grider_core8.cpp>
#include <wizard_operations.cpp>

#include <faders_operations.cpp>
#include <chasers_core_5.cpp>


#include "mover_spline6.cpp"
#include "mover_2013.cpp"
#include "plot_core9.cpp"
#include "plot9.cpp"

#include <audio_core5.cpp>

#include <save_show_13.cpp>
#include <network_artnet_3.cpp> //artnet functions

#include <logicals_intres.cpp>

#include <channels_9_core.cpp>
#include <wizard.cpp>
#include <grand_master.cpp>
#include <faders_core_24.cpp>

#include <audio_visu4.cpp>
#include <icat_core14.cpp>

#include <arduino_device_core.cpp>
#include <banger_core_8.cpp>
#include <banger_visu_8.cpp>
#include <Call_everybody_5.cpp>

#include <patch_core.cpp>
#include <patch_visu.cpp>
#include <time_core_3.cpp>
#include <time_visu_3.cpp>
#include <trichro_core2.cpp>
#include <trichro_visu2.cpp>
#include <numpad_core.cpp>
#include <numpad_visuel.cpp>
#include <keyboard_functions2.cpp>
#include <keyboard_routines2.cpp>
#include <minifaders_core.cpp>
#include <minifaders_visu.cpp>
#include <faders_visuels_26.cpp>
#include <channels_10_visu.cpp>
#include <video_tracking_core.cpp>
#include <video_tracking_visu.cpp>

#include <list_proj_core.cpp>
#include <list_proj_visu.cpp>//liste projecteurs


#include <sequentiel_6_core.cpp>
#include <iCat14.cpp>//iCat remote
#include <network_MAC_adress_3.cpp>
#include <midi_launchpad.cpp>

#include <grider8.cpp>
#include <sequentiel_7_visu.cpp>
#include <Draw3.cpp>
#include <echo3.cpp>


#include <my_window_file_sample.cpp>//creation de fenetres utilisateurs, doit être avant proc visuels


#include <procs_visuels_rebuild1.cpp>
#include <dmx_functions_13.cpp>

#include <midi_13.cpp>
#include <CFG_screen.cpp>


#include <arduino_core_6_UNO_anton3.cpp>
#include <arduino_6_UNO_anton2.cpp>



int time_doing()
{
if (index_play_chrono==1){++actual_tickers_chrono;}
++ticks;
actual_time++;
alpha_blinker+=0.05;
if(alpha_blinker>1){alpha_blinker=0.2;}
return(0);
}
/////////////////TIMER POUR DATA ET REFRESH RATE////////////////////////////////
void ticker_dmxIn() // nettoyage des ticker pour verifier stabilité
{
Receive_DMX_IN();
}
END_OF_FUNCTION(ticker_dmxIn);

/////////////////////////////////////////////



void ticker()
{
time_doing();

if(index_is_saving==0 && init_done==1 && index_writing_curve==0 && index_quit==0)
{
for (int i=0;i<9;i++)
{
if(core_do_calculations[i]==1)
{
switch (i)
{
       case 0:
       detect_actual_master_lock_is();
       do_lfos();
       break;
       case 1:
       do_crossfade();
       if(index_go==1){index_crossfading=1;}else{index_crossfading=0;}
       do_goback();
       break;
       case 2:
      // for (int i=0;i<core_user_define_nb_bangers;i++){do_bang(i);}// dans full loop
       break;
       case 3:
       //trichro_back_buffer(315/2,550/2,125,15);//calcul trichro ( triangle et saturation dans buffer separé)
       break;//obligé dans main loop
       case 4:
       ventilation_video_trackers();
       break;
       case 5:
       do_chaser();
       break;
       case 6:
       do_grid();
       break;
       case 7:
       do_channels_macro();
       break;
       case 8://echo
       do_echo();
       break;
       default:
       break;
}
}
}



for(int pr=0;pr<6;pr++)
{
merge_draw_and_grid_player(pr);
}


//tracker
Move_do_crossfade(dock_move_selected);
ventilation_niveaux_mover();
++ticks_move;
actual_spline_tick+=spline_tick_fraction;
move_current_time=ticks_move;





//
Merger();
SendData_to_interface();
ventilation_midi_sur_crossfade();
emit_midi_out();
refresh_chaser_window_timeline_for_midi_out();
if(enable_launchpad==1 && entered_main==1 && index_quit==0 && index_is_saving==0){launchpad_refresh_buffer_led();}


if( index_re_init_clientserveur_icat==1)
{
if(iCat_serveur_is_initialized==1)
{fermeture_clientserveur_iCat();}
initialisation_clientserveur_iCat();
}
do_autolaunch();//attention contient rafraichissement Faders

}



}

END_OF_FUNCTION(ticker);

/////////////////////////////////////////////////////////////////////////////////////////////

#include <CFG_config_panel_8.cpp>

#include <chasers_visu.cpp>

#include <graphics_rebuild1.cpp>

//////////////////////////////////MOUSE/////////////////////////////////////////

int do_mouse_right_click_menu()
{
 x_mainmenu=mouse_x, y_mainmenu=mouse_y;
 index_show_main_menu=toggle(index_show_main_menu);
 if( index_show_main_menu==1){add_a_window(W_MAINMENU);}
 else {substract_a_window(W_MAINMENU);}
 right_click_for_menu=0;
 return(0);
}

void my_callback(int flags) {

    if (flags & MOUSE_FLAG_LEFT_DOWN )
        {
        mouse_button=1;
        original_posx=mouse_x;original_posy=mouse_y;
        //sab 29/05/2013 deb ---------------------------------------------------------------
        //window_focus_id=detection_over_window();
        if (wc_askConfirmWindowIsOpen())
		{
			window_focus_id=W_ASKCONFIRM;
		}
		else
		{
			window_focus_id=detection_over_window();
		}
		//sab 29/05/2013 fin ---------------------------------------------------------------
        if(window_focus_id==0 || window_focus_id==W_LIST ){snap_channels_selection_array(); }
        mouse_released=0;
        }

    else if (flags & MOUSE_FLAG_LEFT_UP )//relevage bouton
        {
        mouse_button=0; mouse_released=1;   //liberation du curseur souris
        index_click_move_faderspace=0; im_moving_a_window=0; index_mouse_is_tracking=0;
        index_moving_fader_space=0;index_moving_x_slide=0;index_moving_y_slide=0;
        index_click_inside_plot=0; plot_facteur_move_x=0;plot_facteur_move_y=0;
        index_click_inside_relativ_xy=0; rlativ_xm=0; rlativ_ym=0;
        index_editing_theatre_plan=0;moving_plan_relativ_x=0;moving_plan_relativ_y=0;
        editing_plan_data_type=0; editing_plot_sizey=0; editing_plot_sizex=0;  moving_size_relativ_x=0; moving_size_relativ_y=0;
        plot_editing_color_line=0; plot_editing_color_background=0; index_adjusting_shape_x=0;index_adjusting_shape_y=0;
        handle_selected_for_line_editing=0;  editing_shape_line_number=0;
        if(dragging_draw==1)
        {draw_point_is_traced[draw_preset_selected]=0;}
        dragging_draw=0; dragging_channel_in_echo=0;

        }

       if (flags & MOUSE_FLAG_RIGHT_DOWN )
        {
        original_posx=mouse_x;original_posy=mouse_y;
        mouse_R_button=1;
        mouse_R_released=0;

        if(window_focus_id==W_PLOT) {
                                    index_move_plot_view_port=1;
                                    reset_symbols_selected(view_plot_calc_number_is);
                                    unselect_all_shapes();
                                    key_unselect_ch();
                                    }
        else {
             if(mouse_R_released==0)
             {
              mouse_R_released=1;
              right_click_for_menu=1; //renvoi vers les procs en 10eme de secondes pour enlever le bug d extinctions fenetres
              }

             }
        }

       else if (flags & MOUSE_FLAG_RIGHT_UP )
        {
        mouse_R_button=0;
        mouse_R_released=1;
        index_move_plot_view_port=0;
        plot_facteur_move_x=0;plot_facteur_move_y=0;
        index_click_inside_plot=0;
        set_mouse_range(0, 0, SCREEN_W-1, SCREEN_H-1);//liberation du curseur souris
        }

        if(mouse_button==0)
        {set_mouse_range(0, 0, SCREEN_W-1, SCREEN_H-1);}

}
END_OF_FUNCTION(my_callback);

////////////checks 10emes et souris/////////////////////////////////////////////////
int ticker_dixiemes_de_secondes_check = BPS_TO_TIMER(10);//10eme de secondes



void dixiemes_de_secondes()
{
ticks_dixieme_for_icat_and_draw++;
if(index_is_saving==0 && init_done==1 && index_writing_curve==0 && index_quit==0)
{
get_current_time();
for(int yr=0;yr<48;yr++)
{
dock_used_by_fader_is[yr]=detect_dock_used(yr);
}

do_sprintf_job();//include time_left calculs
refresh_minifader_state_view_core(position_minifader_selected);//les infos dock temps etc dans fenetre minifaders
match_minifaders();//verif du preset en cours: correspond ou pas à un des 8 preset
nbre_fenetre_actives=check_nbre_opened_windows();

      if(index_do_quick_save==1)
      {

      index_is_saving=1;
      Save_Show();
      sprintf(string_Last_Order,">> Show Saved at %s", tmp_time);
      index_do_quick_save=0;
      index_is_saving=0;
      }

right_click_on_plot();
}



if (index_play_chrono==1)
{
time_minutes=(int)((float)(actual_tickers_chrono)/(BPS_RATE*60))%60;
time_secondes=(int)((float)(actual_tickers_chrono)/BPS_RATE)%60;
time_centiemes=(int)((float)(actual_tickers_chrono)/(((float)BPS_RATE)/100))%100;
//report des angles popur garder la mesure en fin de chrono.
angle_timesnap_min=((float)(time_minutes)*5.980005)/60;//=(int)((angle_timesnap_min/6.280005)*63);
angle_timesnap_sec=((float)(time_secondes)*5.980005)/60;//=(int)((angle_timesnap_sec/((PI*360) / 180))*63);
angle_timesnap_dix=((float)(time_centiemes)*5.980005)/100;//=(int)((angle_timesnap_dix/6.280005)*10);
set_time_cursor_to_time_type(time_wheel_datatype_is);
}



sprintf(string_last_ch,"Last Ch. selected: %d", last_ch_selected);
sprintf(string_last_copy_mem,"Mem to copy: %d.%d", CTRLC_mem_to_copy/10,CTRLC_mem_to_copy%10);
switch(index_patch_window)
{
case 0:
sprintf(string_secondary_feeback,string_last_over_dock);//last over dock: permet de savoir quelle selection orange est allumée
break;
case 1:
sprintf(string_secondary_feeback,string_monitor_patch);
break;
}
sprintf(string_mem_onstage,"%d.%d",position_onstage/10, position_onstage%10);
sprintf(string_mem_preset,"%d.%d",position_preset/10, position_preset%10);

if(index_inspekt==1){indicate_wich_fader_is_the_highest();}

//fenetre confirm always on top
if(index_ask_confirm==1 && previous_index_ask_confirm==0)
{previous_window_focus_id=window_focus_id; window_focus_id=W_ASKCONFIRM;add_a_window(window_focus_id);}
previous_index_ask_confirm=index_ask_confirm;

if (enable_iCat==1 && iCat_serveur_is_initialized==1 && index_quit==0 && index_is_saving==0)
      {
      if(   do_send_icat_init_page==1 && finished_to_send_orders_to_iCat==1)
      {
      do_send_icat_init_page=0;do_refresh_iCat(iCatPageis);
      index_refresh_valeurs_continous=1;
      refresh_check_buttons();
      }
      else
      {
      refresh_continuously_iCat_sliders();
      refresh_continuously_iCat_buttons();
      refresh_continuously_iCat_trackerzones();
      }
      /*for(int nbr=0;nbr<12;nbr++)//sensibilité clavier
      {
      FS_sensibilite_touche[nbr]-=1;
      if( FS_sensibilite_touche[nbr]<0){ FS_sensibilite_touche[nbr]=0;}
      }*/
      }

if(expert_mode==1)
{
index_edit_listproj=1;
index_enable_edit_banger=1;
index_edit_audio=1;
index_enable_edit_chaser=1;
index_enable_edit_Grider=1;
index_edit_light_plot=1;
index_enable_edit_Draw=1;
index_enable_edit_echo=1;
}
if(right_click_for_menu==1)//sortie du call back pour écrire correctement fermeture fenetres
{do_mouse_right_click_menu();}


if(arduino_device_0_is_ignited==1)
{
arduino_do_digital_in_whitecat();arduino_do_analog_in_whitecat();
arduino__send_config();
}

}
END_OF_FUNCTION(dixiemes_de_secondes);


///////////////FULL LOOP FUNCTION/////////////////////////////////////////////
int ticker_full_loop_rate = BPS_TO_TIMER(10000);
void ticker_full_loop()
{

if(core_do_calculations[2]==1 && starting_wcat==0)
{
for (int i=0;i<core_user_define_nb_bangers;i++){do_bang(i);}
sound_core_processing();
}

if(mouse_button==1 && mouse_released==0)
{
switch(im_moving_a_window)
{
case 0:
check_graphics_mouse_handling();
break;
case 1:
move_window(window_focus_id);
break;
}
}
if(index_quit==0 && index_is_saving==0)
{


 if (enable_iCat==1 && iCat_serveur_is_initialized==1 && do_send_icat_init_page==0)
      {
      bytesreceivediCat=recvfrom(sockRiCat,fantastick_message,sizeof(fantastick_message),0,(SOCKADDR*)&sinServiCat,&sinsizeServiCat);
      if(bytesreceivediCat>0 && (fantastick_message[0]!='I' &&  fantastick_message[1] !='P'))//caractere d arret
      {
      fantastick_message[bytesreceivediCat]='\0';
      }
      ReceiveFantastick();
      DoJobFantastickTouch();
      Fantastick_check_string();

      if(refresh_icatpage_please==1){load_iCat_page ( iCatPageis);do_send_icat_init_page=1; refresh_icatpage_please=0;      }
      }


 if(allow_artnet_in==1 && artnet_serveur_is_initialized==1 )
      {
      if(bytesreceived=recvfrom(sock,artnet_message,sizeof(artnet_message),0,(SOCKADDR*)&sinServ,&sinsizeServ)>0)
      {receiving_bytes=1;ReceiveArtDmx();}
      else {receiving_bytes=0;}
      }
commandes_clavier();
DoMouseLevel();
}
}
END_OF_FUNCTION(ticker_full_loop);

////////////////////////ARTNET VOLATILE ALLEGRO///////////////////////////////////////////////////////////
//artnet envoi toutes les 3 secondes

volatile int ticks_for_artnet=0;
int ticker_artnet_rate = BPS_TO_TIMER(1);
void ticker_artnet()
{
 ticks_for_artnet++;
 if(ticks_for_artnet==3)
 {
 for(int bup=0;bup<512;bup++)
 {artnet_backup[bup]=0;}   //reset du backup pour enclencher l envoi de data automatiquement
 ticks_for_artnet=0;
 index_art_polling=0;
 /*if (myDMXinterfaceis!=1 && client_artnet_is_closed==0 && index_artnet_doubledmx==0)
 {fermeture_client_artnet();}//dans le cas d un art poll avec interface usb*/
 }
}
END_OF_FUNCTION(ticker_artnet);




void Load_Fonts()
{
   doom.Load( "Fonts/doom.ttf",25,25, CouleurLigne );
   if( !doom )
   {  allegro_message( "Couldn't load Fonts/doom.ttf!" );
      exit( -1 );
   }
   doomblanc.Load( "Fonts/doom.ttf",20,20, CouleurLigne);
   if( !doomblanc )
   {  allegro_message( "Couldn't load Fonts/doom.ttf!" );
      exit( -1 );
   }
   doomrouge.Load( "Fonts/doom.ttf",25,25, CouleurBlind);
   if( !doomrouge )
   {  allegro_message( "Couldn't load Fonts/doom.ttf!" );
      exit( -1 );
   }
   petitdoomblanc.Load( "Fonts/doom.ttf",18,18, CouleurLigne);
   if( !doomrouge )
   {  allegro_message( "Couldn't load Fonts/doom.ttf!" );
      exit( -1 );
   }
   petitdoomInspekt.Load( "Fonts/doom.ttf",10,10, CouleurFader );
   if( !petitdoomInspekt )
   {  allegro_message( "Couldn't load Fonts/doom.ttf!" );
      exit( -1 );
   }

   petitdoomrouge.Load( "Fonts/doom.ttf",10,10, CouleurBlind );
   if( !petitdoomrouge )
   {  allegro_message( "Couldn't load Fonts/doom.ttf!" );
      exit( -1 );
   }

   minidoomblanc.Load( "Fonts/doom.ttf",8,8, CouleurLigne );
   if( !minidoomblanc )
   {  allegro_message( "Couldn't load Fonts/doom.ttf!" );
      exit( -1 );
   }

   axaxax12.Load( "Fonts/axaxax.ttf",12,12, CouleurLigne );
   if( !axaxax12 )
   {  allegro_message( "Couldn't load Fonts/doom.ttf!" );
      exit( -1 );
   }


   neuro.Load( "Fonts/prototype.ttf",20,20, CouleurLigne );
   if( !neuro )
   {  allegro_message( "Couldn't load Fonts/prototype.ttf!" );
      exit( -1 );
   }
    neuromoyen.Load( "Fonts/prototype.ttf",14,14, CouleurLigne );
   if( !neuromoyen )
   {  allegro_message( "Couldn't load Fonts/prototype.ttf!" );
      exit( -1 );
   }
   petitchiffre.Load( "Fonts/prototype.ttf",12,12, CouleurLigne );
   if( !petitchiffre )
   {  allegro_message( "Couldn't load Fonts/prototype.ttf!" );
      exit( -1 );
   }
   petitpetitchiffre.Load( "Fonts/prototype.ttf",10,10, CouleurLigne );
   if( !petitpetitchiffre )
   {  allegro_message( "Couldn't load Fonts/prototype.ttf!" );
      exit( -1 );
   }
   minichiffre.Load( "Fonts/prototype.ttf",8,8, CouleurLigne );
   if( !minichiffre )
   {  allegro_message( "Couldn't load Fonts/prototype.ttf!" );
      exit( -1 );
   }
    petitpetitchiffregris.Load( "Fonts/prototype.ttf",9,9, CouleurLigne.WithAlpha(0.7) );
   if( !petitpetitchiffre )
   {  allegro_message( "Couldn't load Fonts/prototype.ttf!" );
      exit( -1 );
   }
   minichiffregris.Load( "Fonts/prototype.ttf",8,8, CouleurLigne.WithAlpha(0.7) );
   if( !minichiffre )
   {  allegro_message( "Couldn't load Fonts/prototype.ttf!" );
      exit( -1 );
   }
    minichiffrenoir.Load( "Fonts/prototype.ttf",8,8, CouleurNoir );
   if( !minichiffrenoir )
   {  allegro_message( "Couldn't load Fonts/prototype.ttf!" );
      exit( -1 );
   }
    petitpetitchiffrerouge.Load( "Fonts/prototype.ttf",10,10, CouleurBlind );
   if( !petitpetitchiffrerouge )
   {  allegro_message( "Couldn't load Fonts/prototype.ttf!" );
      exit( -1 );
   }
   petitchiffrerouge.Load( "Fonts/prototype.ttf",12,12, CouleurBlind );
   if( !petitchiffrerouge)
   {  allegro_message( "Couldn't load Fonts/prototype.ttf!" );
      exit( -1 );
   }
    minichiffrerouge.Load( "Fonts/prototype.ttf",8,8, CouleurBlind );
   if( !minichiffrerouge )
   {  allegro_message( "Couldn't load Fonts/prototype.ttf!" );
      exit( -1 );
   }
   petitchiffrenoir.Load( "Fonts/prototype.ttf",12,12, CouleurFond );
   if( !petitchiffrenoir )
   {  allegro_message( "Couldn't load Fonts/prototype.ttf!" );
      exit( -1 );
   }
   circuitlevel.Load( "Fonts/prototype.ttf",10,10, CouleurLevel);
   if( !circuitlevel )
   {  allegro_message( "Couldn't load Fonts/prototype.ttf!" );
      exit( -1 );
   }
   circuitfaderlevel.Load( "Fonts/prototype.ttf",10,10, CouleurFader);
   if( !circuitfaderlevel )
   {  allegro_message( "Couldn't load Fonts/prototype.ttf!" );
      exit( -1 );
   }
   circuitblindlevel.Load( "Fonts/prototype.ttf",10,10, CouleurBlind);
   if( !circuitblindlevel )
   {  allegro_message( "Couldn't load Fonts/prototype.ttf!" );
      exit( -1 );
   }
   neuroTitle.Load( "Fonts/neuropol.ttf",70,70, CouleurLigne);
   if( !neuroTitle )
   {  allegro_message( "Couldn't load Fonts/neuropol.ttf!" );
      exit( -1 );
   }


}



////////////////////////////////////////////////////////////////////////////////
int main_actions_on_screen()
{
      Canvas::Fill(CouleurFond);
      if(index_writing_curve==0){Boxes();}
      if(core_do_calculations[3]==1)
      {
      trichro_back_buffer(315/2,550/2,125,15);//calcul trichro ( triangle et saturation dans buffer separé)
      do_colors();//ventilation des niveaux pickés ainsi que distrib dans faders et docks
      }
      DoMouse();
      previous_ch_selected=last_ch_selected;
 return(0);
}


int main() {

load_screen_config();
if(index_multiplescreen==1)
  {
    largeur_ecran= GetSystemMetrics(SM_CXVIRTUALSCREEN);
    hauteur_ecran = GetSystemMetrics(SM_CYVIRTUALSCREEN);
  }

//if(index_borderwindow==0)
//{Settings::SetWindowBorder(false);}
//else
//{Settings::SetWindowBorder(true);}

Setup::SetupProgram(KEYBOARD | MOUSE);

if(index_fullscreen==0)
{Setup::SetupScreen( largeur_ecran, hauteur_ecran,WINDOWED, desktop_color_depth() );}
else {Setup::SetupScreen( largeur_ecran, hauteur_ecran,FULLSCREEN, desktop_color_depth() );}

   HWND hwnd = win_get_window();
    if (hwnd != NULL)
        MoveWindow(hwnd, posX_mainwindow, posY_mainwindow, SCREEN_W, SCREEN_H, true);


   install_joystick(JOY_TYPE_AUTODETECT);
   calibrate_joystick_name(0);


   install_timer();

   jpgalleg_init();

   LOCK_FUNCTION(ticker_dmxIn);
   install_int_ex(ticker_dmxIn , ticker_dmxIn_rate);


   LOCK_FUNCTION(ticker_artnet);
   LOCK_VARIABLE(ticks_for_artnet);
   install_int_ex(ticker_artnet , ticker_artnet_rate);

   rest(100);

   LOCK_VARIABLE(ticks);
   LOCK_FUNCTION(ticker);
   install_int_ex(ticker , ticker_rate);

   LOCK_VARIABLE(ticks_arduino);
   LOCK_FUNCTION(ticker_arduino);
   install_int_ex(ticker_arduino ,BPS_TO_TIMER(ARDUINO_RATE));


    LOCK_FUNCTION(my_callback);
    mouse_callback = my_callback;


   LOCK_FUNCTION(dixiemes_de_secondes);
   install_int_ex(dixiemes_de_secondes , ticker_dixiemes_de_secondes_check);


   LOCK_FUNCTION( ticker_full_loop);
   install_int_ex(ticker_full_loop,ticker_full_loop_rate );




   load_indexes();
   LoadWhiteCatColorProfil();

   Canvas::Fill(CouleurFond);
   Canvas::Refresh();



   Load_Fonts();
   save_load_print_to_screen("Loaded Fonts");


   logo.Load( "gfx/logo.png" );//ne s'affiche que en 32 bits !!!

   if( logo.Load( "gfx/logo.png" ) != true )
      {
      allegro_message( "Couldn't load gfx/logo.png" );
      exit( -1 );
      }



    Canvas::Fill(CouleurFond);
    Canvas::Refresh();

   save_load_print_to_screen("Loaded Gfx");


   sprintf(string_last_ch,"Last Ch. selected: %d", last_ch_selected);
   sprintf(string_Last_Order,">> This is Last Order");


  save_load_print_to_screen("Loading setup conf");
 Load_setup_conf(); //avant tout sinon, le cfg ecrit ailleurs et ca fout la zone
 save_load_print_to_screen("Init Arrays");

//sauvegarde chargement, en tout dernier
 for (int r=0;r<72;r++)
 {
 specify_who_to_save_load[r]=1;
 }
 GlobInit();//rajout version 0.8.2.3
//reset des bangs
reset_all_bangers();
 generation_Tableau_noms_clavier_FR() ;
// generation_Tableau_noms_fonctions() ;
 save_load_print_to_screen("Init Midi");
 InitMidi();//init avant les appels de fichiers
 midi_init_sepecial_case_key_on();//pour régler pb de cle flashs et key on key off
 ////////////////////////////////////////////////////////////

 load_onstart_config();
 load_core_config();
  ///////////////////////////////////////////////////////
 save_load_print_to_screen("Loading Dmx conf");
 load_dmx_conf();
 save_load_print_to_screen("Loading Art-net conf");
 load_artnet_conf();

 detection_mise_en_place_carte_reseaux();
 //opening double dmx conf
 if(index_artnet_doubledmx==1)
 {
 initialisation_client_artnet();
         //ConstructArtPoll();
         ConstructArtPollReply();
         nbrbytessended=sendto(sockartnet, ArtPollBuffer,sizeof( ArtPollBuffer),0,(SOCKADDR*)&sinS,sinsize);
         //ArtNet
         ArtDmx();
 save_load_print_to_screen("Double DMX Art-net ON");
 }
 load_network_conf();//icat
 save_load_print_to_screen("Loading Art-netnetwork conf");


 save_load_print_to_screen("Loading Video conf");
 Load_Video_Conf();
 sprintf(tmp_ip_artnet,ip_artnet);

 load_show_coming_from();
 idf++;

 On_Open_name_of_directory();
 Canvas::Fill(CouleurFond);
 Canvas::Refresh();
 save_load_print_to_screen("Init Sound");
 InitSound();
 Load_Show();

 init_kbd_custom();
 save_load_print_to_screen("Init Keyboard");
 Show_report_save_load();

 save_load_print_to_screen("Init Dmx");
 Init_dmx_interface();

 if(load_camera_on_start==1)
 {
 save_load_print_to_screen("Init Camera");
 InitVideo();
 }





scan_importfolder("");
scan_savesfolder();


if( open_arduino_on_open==1)
{
save_load_print_to_screen("Init Arduino");
arduino_init(0);
index_send_digital_data=1;
index_send_pwm_data=1;
read_arduino_data(0);
}


prepare_move_values(dock_move_selected);//prepa
Prepare_Cross_Spline(dock_move_selected);

save_load_print_to_screen("Init Backamnesia");
 if(set_display_switch_mode(SWITCH_BACKGROUND))
 {set_display_switch_mode(SWITCH_BACKAMNESIA);}


init_done=1;
if(there_is_an_error_on_save_load==1){index_show_save_load_report=1;there_is_change_on_show_save_state=1;    }

 mouse_released=0;
 entered_main=1;
//launchpad séparé
if(enable_launchpad==1)
{reset_launchpad();}

 init_artnet_variables();

//serveur
    if(allow_artnet_in==1 && artnet_serveur_is_initialized==0)
      {
	initialisation_serveur_artnet();
     }

 init_iphone_fonts();
 if (enable_iCat==1)
 {
 initialisation_clientserveur_iCat();

 nbrbytessendediCat=sendto(sockiCat, "opengl 1",sizeof("opengl 1"),0,(SOCKADDR*)&siniCat,sinsizeiCat);
 init_iCat_data();//varibales de stockage
 someone_changed_in_sequences=1;//icat
 do_send_icat_init_page=1;
 do_refresh_iCat(iCatPageis);

 }


bmp_buffer_trichro= create_bitmap(315,550);
clear_bitmap(bmp_buffer_trichro);


rafraichissement_padwheel();

recalculate_draw_sizes(draw_preset_selected);

//init du flash de bang en cours
//bang_is_sended[index_banger_selected]=1;




   rest(100);

starting_wcat=1;
for(int i=0;i<4;i++)
{
 audiofile_selected=player_has_file_coming_from_pos[i];
 sprintf(audiofile_name,list_audio_files[audiofile_selected]);
 if(strcmp (audiofile_name,"")==1)
 {
 AffectSoundFile(i);
 }
rest(10);
}


if(index_loading_a_sound_file==0)
{
for(int i=0;i<4;i++)
{
//position player
  if(player_ignited[i]==1 && player_position_on_save[i]<=length_of_file_in_player[i] )
  {
  switch(i)
  {
  case 0:
  player1->setPosition(player_position_on_save[i]);
  break;
  case 1:
  player2->setPosition(player_position_on_save[i]);
  break;
  case 2:
  player3->setPosition(player_position_on_save[i]);
  break;
  case 3:
  player4->setPosition(player_position_on_save[i]);
  break;
  default:
  break;
  }
 position_of_file_in_player[i]=player_position_on_save[i];
}
}
}



starting_wcat=0;



while(index_quit!=1)
{


MemoiresExistantes[0]=1;
show_im_recording_a_time=0;// met à zéro l'affichage du stock visuel du time
//remis dans boucle pour bug freeze V8.3 et 8.4
if(old_ticks_arduino!=ticks_arduino && index_is_saving==0 && init_done==1 && index_writing_curve==0 && arduino_device_0_is_ignited==1 && index_quit==0)//procedures de communication
{
    arduino_read();
    arduino_do_digital_out_whitecat(); //ecriture
    arduino_do_pwm_out_whitecat();
    serial0.Flush();
}

 switch(index_art_polling)
 {
    case 0:
    main_actions_on_screen();
    break;
    case 1:
      if(bytesreceived=recvfrom(sock,artpollreply_message,sizeof(artpollreply_message),0,(SOCKADDR*)&sinS,&sinsize)!=0)
      {      AnalyseArtPollReply();      }
      Procedure("Art-Net Polling","Please wait 3 seconds, polling network ...");
    break;
   }
//DEBUG
sprintf(string_debug,"%d",channel_macro_val[23][0][0]);

if(there_is_change_on_show_save_state==1)
{
check_save_load_report_window();
there_is_change_on_show_save_state=0;
}

Canvas::Refresh();
if(index_do_a_screen_capture==1){do_a_screen_capture();index_do_a_screen_capture=0;}
if(index_do_a_plot_screen_capture==1 ){do_plot_screen_capture(plot_name_of_capture);index_do_a_plot_screen_capture=0;}

}

entered_main=0;
substract_a_window(W_ASKCONFIRM);
window_focus_id=window_who_is_on_top();

init_done=0;//evite envois data pendant save de fermeture


Canvas::Refresh();
Canvas::Fill(CouleurFond);


save_load_print_to_screen("Quitting");
Show_report_save_load();
save_load_print_to_screen("Closing dmx");
Close_dmx_interface();
save_load_print_to_screen("Closing Arduino");
arduino_close(0);

save_load_print_to_screen("Closing video");
CloseVideo();
save_load_print_to_screen("Closing midi");

 if (enable_iCat==1 && iCat_serveur_is_initialized==1)
 {
 fermeture_clientserveur_iCat();
 //if(client_icat_is_closed==1)  fermeture_client_iCat();
 }


QuitMidi() ;
save_load_print_to_screen("Saving Show");
if(index_please_do_not_save==0)//a garder sinon plante sur le please not save
{
Save_Show();
save_load_print_to_screen("Saving Video Conf");
Save_Video_Conf();
save_load_print_to_screen("Saving Screen Conf");
Save_Screen_Config();
save_load_print_to_screen("Saving Setup Conf");
Save_setup_conf();

}
destroy_bitmap(bmp_buffer_trichro);
//destroy_bitmap(Draw0);
remove_timer();
WSACleanup ();//liberation librairie socket
return 0;
}
END_OF_MAIN()

