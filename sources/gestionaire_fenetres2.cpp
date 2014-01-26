

int reset_config_indexes()
{
index_config_dmx=0;
index_config_midi=0;
index_config_network=0;
index_setup_gfx=0;
index_config_arduino=0;
index_config_general=0;
index_config_core=0;
 return(0);   
}

int clear_non_desired_values_in_window_list()
{
    
for(int i=0;i<63;i++)
{
if(window_opened[i] >max_window_identity_is+1){window_opened[i]=0;} //+1 règle bug de rwanoux et jaques alpha 0.8.3
if(window_opened[i] <900){window_opened[i]=0;}          
}
 return(0);   
}


int initialisation_tableau_windows()
{
for(int i=0;i<63;i++)
 {
  window_opened[i]=0;
  temp_report_window[i]=0;
}
 return(0);   
}

int check_nbre_opened_windows()
{
int nbre_windows=0;
for(int i=0;i<63;i++)
{
if(window_opened[i]>=900)
{
nbre_windows++;
}        
}
 return(nbre_windows);   
}

int add_a_window(int id)
{

//nettoyage pour éviter doublons perdus
 for(int i=1;i<63;i++)//0 pas concerné pour ne pas cuter la fenetre sur scène bug christoph banger, jacques, batgg // 1er dec 2012
 {
 if(window_opened[i]==id){window_opened[i]=0;}
 }

int temp_report_window[65];
 
 for(int i=0;i<63;i++)
 {
  temp_report_window[i+1]=window_opened[i];  
  window_opened[i]=0;     
 }

 int pos=0;//etait 1 dans precedente version
 for(int i=0;i<63;i++)
 {
 if(temp_report_window[i]!=0)
 {
 window_opened[pos]=temp_report_window[i];  
 pos++;
 }     
 }
 window_opened[0]=id;
 window_focus_id=window_who_is_on_top();

 
 switch(id)
 {
 case W_SAVEREPORT:
      index_show_save_load_report=1;
 break;

 case W_TRICHROMY:
      index_trichro_window=1;index_affect_color_to_dock=0;
 break;
 case W_NUMPAD:
      index_visual_pad=1;
 break;
 case W_TRACKINGVIDEO:
      index_video_window=1;
 break;
 case W_ARTPOLLREPLY:
 break;
 case W_FADERS:
      index_show_faders=1;
 break;
 case W_PATCH:
      index_patch_window=1;index_enable_curve_editing=0;
      index_patch_overide=0; 
      reset_check_grada_overide_dimmers();   
 break;
 case W_TIME:
      index_time=1; 
 break;
 case W_SEQUENCIEL:
      index_window_sequentiel=1;
 break;
 case W_ASKCONFIRM:
       index_ask_confirm=1;
 break;
 case W_PLOT:
      index_plot_window=1;refresh_shape_factors(shape_selected_type);
 break;
 case W_ECHO:
  index_show_echo_window=1;
 break;
 case W_DRAW:
  index_draw_window=1;
 break;
 case W_LIST:
      index_list_projecteurs=1;
 break;
 case W_SAVE:
       index_menu_save=1;
       scan_importfolder("");scan_savesfolder();
       index_save_global_is=1;
       for (int r=0;r<72;r++)
       {
       specify_who_to_save_load[r]=1;    
       } 
 break;
 case W_MAINMENU:
      index_show_main_menu=1;
 break;
 case W_BANGER:
      index_show_banger_window=1;
 break;
 case W_ALARM:
 break;
 case W_AUDIO:
      index_show_audio_window=1;reset_audio_indexs_to_dock(); 
 break;
 case W_CFGMENU:
      reset_config_indexes();

      switch(config_page_is)
      {
        case 1:
             index_config_dmx=1;
        break;
        case 2:
             index_config_midi=1;
        break;
        case 3:
             index_config_network=1;
        break;
        case 4:
             index_setup_gfx=1;
        break;
        case 5:
             index_config_arduino=1;
        break;        
        case 6:
             index_config_general=1;
        break;        
        case 7:
             index_config_core=1;
        break;                
        default:
        break;
      }
      index_show_config_window=1;
 break;
 case W_WIZARD:
      index_show_wizard_window=1;
 break;
 case W_MINIFADERS:
      index_show_minifaders=1;
 break;
 case W_CHASERS:
      index_affect_chaser_to_dock=0;
      index_window_chasers=1;
 break;
 case W_MOVER:
    index_show_mover_window=1; index_mouse_is_tracking=0;
 break;
 case W_iCAT:
      index_window_gui_iCat=1;
 break;
 case W_GRID:
      index_grider_window=1;
      for(int i=0;i<4;i++)
      {grid_affect_to_dock[i]=0;}
 break;    
 case W_MY_WINDOW:
      index_my_window=1;
 break;    
 default:
 break;
 }
clear_non_desired_values_in_window_list();

return(0);   
}


//substract in CORE

int initiate_windows()
{

 for(int i=0;i<63;i++)
 {
  if(window_opened[i]>=900)
  {add_a_window(window_opened[i]);}                   
 }   
 window_focus_id=window_opened[0];

 return(0);
}


int window_bring_to_front(int num_window)
{
int temp_window[64];
for(int i=0;i<63;i++)
{
temp_window[i]=window_opened[i];        
}
int pos=1;
for(int i=0;i<63;i++)
{
if(temp_window[i]>=900 && temp_window[i]!=num_window)
{
 window_opened[pos]=temp_window[i];
 pos++;                  
}  
else{window_opened[pos]=0;}//enlevage des doublons

if( window_opened[pos]==num_window)   {window_opened[pos]=0;}//enlevement des doublons sur vauban
} 
window_opened[0]=num_window;
window_focus_id=num_window;
return(num_window);    
}



int check_save_load_report_window()
{
switch(index_show_save_load_report)
{
case 0:
substract_a_window(W_SAVEREPORT);  
break;
case 1:
add_a_window(W_SAVEREPORT); 
break;
}
return(0); 
}
