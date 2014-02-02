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

 \file tracker_main14nov.cpp
 \brief {description courte} 
 \author Christoph Guillermet
 \version {numero de version du fichier}
 \date {date description}
 
 White Cat {- catégorie} {- sous catégorie {- sous catégorie}}
 Description détaillée
 
 **/
 
#include <allegro.h>
#include <OpenLayer.hpp>
#include "stdio.h"
#include <loadpng.h>
using namespace ol;

//pour serveur
#include <winsock2.h>
#include <fcntl.h> 
int udp_port=11059;
int UDPMAX_CAR= 512;
char ip[17];
FILE *cfg_file = NULL ;
char write_buff[ 256 ] ;//buffer d ecriture du fichier de conf
char **theip = NULL;


int old_trackerX=0; int old_trackerY=0;
int old_trackerX_fine=0; int old_trackerY_fine=0;

// convert from shorts to bytes and back again
#define short_get_high_byte(x) ((HIGH_BYTE & x) >> 8)
#define short_get_low_byte(x)  (LOW_BYTE & x)
#define bytes_to_short(h,l) ( ((h << 8) & 0xff00) | (l & 0x00FF) );

char rep[255];//repertoire avec arborescence complete du fichier
char mondirectory[200];//endroit de l exe
char working_nameis[120];
char string_infos[128];
//COULEURS
Rgba CouleurFond(0.0,0.0,0.0);
Rgba Gris( 0.0, 0.0, 0.0 );
Rgba CouleurLigne(1.0,1.0,1.0);
Rgba CouleurFader(1.0,0.4,0.0);
Rgba CouleurSurvol(0.8,0.0,0.0);
Rgba CouleurLevel(0.5,0.7,0.6);
Rgba CouleurSelection(0.5,0.0,0.7);
Rgba CouleurNiveau(0.1,0.0,0.6);
Rgba CouleurBlind(1.0,0.0,0.0);
Rgba CouleurLock(0.0,0.4,0.2);
Rgba CouleurConfig(0.1,0.1,0.3);
Rgba CouleurBleuProcedure(0.0,0.2,0.5);

//BOXES
float epaisseur_ligne_fader=2.0;
float rondeur_fader=15;
float rondeur_boutons=10;
//POLICES et mise en page

TextRenderer neuro;
TextRenderer petitchiffre;
TextRenderer circuitlevel;
TextRenderer circuitfaderlevel;
TextRenderer doomblanc;

TextRenderer petitpetitchiffre;
TextRenderer petitchiffrerouge;

//variables globales d'affichage
char string_Last_Order[120];
char string_xy_mouse[12];
char string_display_fps[5];
char string_joystick_state[64];
/////////////////////////////////////////////////////////////////////////////////
int click_mouse_wait=150;
int keyboard_wait=100;
int BPS_RATE=50;

bool index_quit=0;



void Load_Fonts()
{
 

   neuro.Load( "Fonts/neuropol.ttf",20,20, Rgba::WHITE );  
   if( !neuro ) 
   {  allegro_message( "Couldn't load Fonts/europol.ttf!" );
      exit( -1 );
   }  
   petitchiffre.Load( "Fonts/neuropol.ttf",12,12, Rgba::WHITE );  
   if( !neuro ) 
   {  allegro_message( "Couldn't load Fonts/europol.ttf!" );
      exit( -1 );
   }  
   petitpetitchiffre.Load( "Fonts/neuropol.ttf",10,10, Rgba::WHITE );  
   if( !neuro ) 
   {  allegro_message( "Couldn't load Fonts/europol.ttf!" );
      exit( -1 );
   }  
   petitchiffrerouge.Load( "Fonts/neuropol.ttf",12,12, Rgba::RED );  
   if( !neuro ) 
   {  allegro_message( "Couldn't load Fonts/europol.ttf!" );
      exit( -1 );
   }  
   circuitlevel.Load( "Fonts/neuropol.ttf",10,10, CouleurLevel);  
   if( !neuro ) 
   {  allegro_message( "Couldn't load Fonts/europol.ttf!" );
      exit( -1 );
   }  
   circuitfaderlevel.Load( "Fonts/neuropol.ttf",10,10, CouleurFader);  
   if( !neuro ) 
   {  allegro_message( "Couldn't load Fonts/europol.ttf!" );
      exit( -1 );
   }  
    doomblanc.Load( "Fonts/doom.ttf",20,20, Rgba::WHITE);  
   if( !doomblanc ) 
   {  allegro_message( "Couldn't load Fonts/doom.ttf!" );
      exit( -1 );
   }  
  
}


/////////////////TIMER POUR DATA ET REFRESH RATE////////////////////////////////
volatile int ticks = 0;
int ticker_rate = BPS_TO_TIMER(BPS_RATE);
int ticks_passed = 0;

volatile int ticks_move = 0;

void ticker() 
{
++ticks;
}
END_OF_FUNCTION(ticker);

volatile float actual_spline_tick=0.0;


#include "chat_dock_tracker_14nov2009.cpp"


void ticker_move() 
{
do_joystick_job();
ReCalculateScale();   
Move_do_crossfade(dock_move_selected);
++ticks_move;
actual_spline_tick+=spline_tick_fraction;
move_current_time=ticks_move;
}
END_OF_FUNCTION(ticker_move);
////////////////////////////////////////////////////////////////////////////////
int On_Open_name_of_directory()
{
    get_executable_name(mondirectory,sizeof(mondirectory));   
    replace_filename(mondirectory,mondirectory,"",sizeof(mondirectory)); 
    return(0);
}


////////////////////////////////////////////////////////////////////////////////

int main() {

   Setup::SetupProgram(KEYBOARD | MOUSE ); 
   Setup::SetupScreen( 1050, 650,WINDOWED, 8 );
   Settings::SetAntialiasing( true );
   
   install_joystick(JOY_TYPE_AUTODETECT);
   calibrate_joystick_name(0);

   install_timer();

   LOCK_VARIABLE(ticks);
   LOCK_FUNCTION(ticker);
   install_int_ex(ticker , ticker_rate);
   LOCK_VARIABLE(ticks_move);
   LOCK_FUNCTION(ticker_move);
   
   if(install_int_ex(ticker_move , ticker_rate)!=0){sprintf(string_joystick_state,"Error Initializing Joystick !");};
   if(!num_joysticks){sprintf(string_joystick_state,"No Joystick detected!");}
   else { sprintf(string_joystick_state,"%d Joysticks detected",num_joysticks);}
      
   Load_Fonts(); 
  
   
    


 if(set_display_switch_mode(SWITCH_BACKGROUND))
 {set_display_switch_mode(SWITCH_BACKAMNESIA);}
 
 
 
Load_Tracker();  
prepare_move_values(dock_move_selected);//prepa  
Prepare_Cross_Spline(dock_move_selected);



char udp_buffer[UDPMAX_CAR];
 
int sinsize;
int nbrbytessended=0;
SOCKET sock;

char broadcast='1';
WSADATA wsa;
WSAStartup(MAKEWORD(2,0),&wsa);
SOCKADDR_IN sin;
sin.sin_family=AF_INET;
				   
sin.sin_addr.s_addr=inet_addr(ip);
sin.sin_port=htons(udp_port);
sock=socket(AF_INET,SOCK_DGRAM,0); //On initialise le socket avec SOCK_DGRAM pour dire qu'on est en UDP
sprintf(string_Last_Order,"INIT socket");
 if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast,
        sizeof broadcast) == -1) {
        perror("\a\nsetsockopt (SO_BROADCAST)");
        exit(1);
    }

bind(sock,(SOCKADDR*)&sin,sizeof(sin)); //Liaison entre la structure et la socket
sprintf(string_Last_Order,"BIDING socket");
sinsize=sizeof(sin);
memset(udp_buffer,0,sizeof(udp_buffer));


int old_iris=0;
int old_focus=0;
int old_zoom=0;
int old_iris_fine=0;
int old_focus_fine=0;
int old_zoom_fine=0;

     sprintf(string_infos,"Compilation 14nov 09 / Pan= %d Tilt= %d Iris= %d Focus= %d Zoom= %d/ IP: %s / Port: %d",xadress,yadress,irisadress,focusadress,zoomadress,ip, udp_port);


 while(index_quit!=1)
 {    
      
   
      while(ticks == 0) {rest(1);}
      ticks_passed = ticks;
      ticks = 0;     
      poll_joystick();
      

      
       Canvas::Fill(CouleurFond); 
           
       petitchiffre.Print(string_infos,10,10);
       
  
       tracker_box(xtrack_window, ytrack_window);  
      
 
     
      
            

       if(Thetracker_X!=old_trackerX || Thetracker_Y!=old_trackerY || 
       Thetracker_X_fine!=old_trackerX_fine || Thetracker_Y_fine!=old_trackerY_fine 
       || iris_state!= old_iris   ||  focus_state!=old_focus  || zoom_state!=old_zoom
       )
      {
     
      if(index_rescale_signal==0)
      {
      udp_buffer[xadress-1]=buffer_tracker[xadress]+position_defaut[0];  udp_buffer[yadress-1]=buffer_tracker[yadress]+position_defaut[1];
      
      if (buffer_tracker[xadress]+position_defaut[0]<0) {udp_buffer[xadress-1]=0;}                         
      if (buffer_tracker[xadress]+position_defaut[0]>255) {udp_buffer[xadress-1]=255;}   
      if (buffer_tracker[yadress]+position_defaut[1]<0) {udp_buffer[yadress-1]=0;}                         
      if (buffer_tracker[yadress]+position_defaut[1]>255) {udp_buffer[yadress-1]=255;}       
      }
      
      else if(index_rescale_signal==1)
      {udp_buffer[xadress-1]=buffer_tracker_rescaled[xadress];  udp_buffer[yadress-1]=buffer_tracker_rescaled[yadress];}
     
      if(tracker_mode==1){udp_buffer[xadress]=buffer_tracker[xadress+1];  udp_buffer[yadress]=buffer_tracker[yadress+1];} 
      
      udp_buffer[irisadress-1]=iris_state;
      udp_buffer[focusadress-1]=focus_state;
      udp_buffer[zoomadress-1]=zoom_state;
     
            
      nbrbytessended=sendto(sock,udp_buffer,UDPMAX_CAR,0,(SOCKADDR*)&sin,sinsize);   
      
      old_trackerX=Thetracker_X; old_trackerY=Thetracker_Y;
      old_trackerX_fine=Thetracker_X_fine; old_trackerY_fine=Thetracker_Y_fine; 
      old_iris=iris_state;
      old_focus=focus_state;
      old_zoom=zoom_state;
      old_iris_fine=iris_state_fine; 
      old_focus_fine=focus_state_fine; 
      old_zoom_fine=zoom_state_fine;

      }

     Line( Vec2D( mouse_x-1,mouse_y-1), Vec2D( mouse_x+20,mouse_y+30),3.0).Draw( CouleurFader );
     Line( Vec2D( mouse_x+20,mouse_y+30), Vec2D( mouse_x+30,mouse_y+10),3.0).Draw( CouleurFader );
     Line( Vec2D( mouse_x+30,mouse_y+10), Vec2D( mouse_x-1,mouse_y-1),3.0).Draw( CouleurFader ); 
     triangle(screen,mouse_x,mouse_y, mouse_x+20,mouse_y+30,mouse_x+30,mouse_y+10,makecol(255,255,255));
     if(mouse_b)
     {triangle(screen,mouse_x,mouse_y, mouse_x+20,mouse_y+30,mouse_x+30,mouse_y+10,makecol(0,0,0)); }
     

      Canvas::Refresh(); 
     
}


remove_timer();
shutdown(sock,2);
closesocket(sock);
allegro_exit();
return 0;
}
END_OF_MAIN()

