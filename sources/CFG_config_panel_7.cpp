
int do_keyboard_conf(int cfgnetw_X, int cfgnetw_Y)
{
    
petitchiffre.Print("Keyboard Mapping: " ,cfgnetw_X, cfgnetw_Y);
Line(Vec2D(cfgnetw_X,cfgnetw_Y+5),Vec2D(cfgnetw_X+90,cfgnetw_Y+5)).Draw(CouleurLigne);
sprintf(string_clavier_is,"Asc= %d Allg= %d KEY= %s ",scan_ascii_is,scan_allegro_key_is,string_key_id);
petitchiffre.Print(string_clavier_is,cfgnetw_X,cfgnetw_Y+20);
char str_iskeyfunct[25];
for (int oi=0;oi<nbre_key_persos;oi++)
{
Rect keyParam(Vec2D(cfgnetw_X+60,cfgnetw_Y+25+(oi*30)),Vec2D(50,25));
keyParam.SetRoundness(7.5);
keyParam.SetLineWidth(epaisseur_ligne_fader);  
keyParam.Draw(CouleurFond.WithAlpha(0.5));
switch(oi)
{
case 0:
sprintf(str_iskeyfunct,"AT LEVEL");
break;
case 1:
sprintf(str_iskeyfunct,"CH +");
break;
case 2:
sprintf(str_iskeyfunct,"CH -");
break;
case 3:
sprintf(str_iskeyfunct,"THRU");
break;
case 4:
sprintf(str_iskeyfunct,"CLEAR");
break;          
}
if(mouse_x>cfgnetw_X+60 && mouse_x<cfgnetw_X+60+50 && mouse_y>cfgnetw_Y+25+(oi*30) && mouse_y<cfgnetw_Y+25+(oi*30)+25)
{
keyParam.DrawOutline(CouleurLevel);
if(mouse_button==1 && mouse_released==0)
{
int tmp_valAsc=atoi(numeric);
if((tmp_valAsc>0 && tmp_valAsc<255)|| tmp_valAsc==999)//999 pour eviter de bloquer les commandes en 0 ascii , genre touches de fonctions
{
mapping_temporaire[oi]=tmp_valAsc;
}
reset_numeric_entry();
mouse_released=1;                 
}                        
}
petitpetitchiffre.Print(str_iskeyfunct,cfgnetw_X,cfgnetw_Y+40+(oi*30));
petitpetitchiffre.Print(ol::ToString(mapping_temporaire[oi]),cfgnetw_X+75,cfgnetw_Y+40+(oi*30));
}

 return(0);   
}

int do_core_config( int x_cfg_sc,int y_cfg_sc, int largeur_cfg_sc, int hauteur_cfg_sc)
{
petitchiffre.Print("Max Calculations:", x_cfg_sc+15, y_cfg_sc+40); 
Line(Vec2D( x_cfg_sc+15, y_cfg_sc+50),Vec2D( x_cfg_sc+120, y_cfg_sc+50)).Draw(CouleurLigne);

int def_param=0;
int def_param_min=1;
int def_param_max=1;

for (int l=0;l<7;l++)
{
Rect Param(Vec2D(x_cfg_sc+120,y_cfg_sc+60+(l*30)),Vec2D(50,25));
Param.SetRoundness(7.5);
Param.Draw(CouleurFond.WithAlpha(0.5));    
switch(l)
{
case 0:
petitchiffre.Print("Faders", x_cfg_sc+15,y_cfg_sc+75+(l*30));
petitchiffre.Print(ol::ToString(core_user_define_nb_faders), x_cfg_sc+135,y_cfg_sc+75+(l*30));
def_param_max=48;
def_param_min=1;
break; 
case 1:
petitchiffre.Print("Docks", x_cfg_sc+15,y_cfg_sc+75+(l*30));
petitchiffre.Print(ol::ToString(core_user_define_nb_docks), x_cfg_sc+140,y_cfg_sc+75+(l*30));
def_param_max=6;
break;  
case 2:
petitchiffre.Print("Chasers", x_cfg_sc+15,y_cfg_sc+75+(l*30));
petitchiffre.Print(ol::ToString(core_user_define_nb_chasers), x_cfg_sc+135,y_cfg_sc+75+(l*30));
def_param_max=128;
break; 
case 3:
petitchiffre.Print("Tracks/Chasers", x_cfg_sc+15,y_cfg_sc+75+(l*30));
petitchiffre.Print(ol::ToString(core_user_define_nb_tracks_per_chasers), x_cfg_sc+135,y_cfg_sc+75+(l*30));
def_param_max=24;
break;   
case 4:
petitchiffre.Print("Gridplayers", x_cfg_sc+15,y_cfg_sc+75+(l*30));
petitchiffre.Print(ol::ToString(core_user_define_nb_gridplayers), x_cfg_sc+140,y_cfg_sc+75+(l*30));
def_param_max=4;
break;  
case 5:
petitchiffre.Print("Bangers", x_cfg_sc+15,y_cfg_sc+75+(l*30));  
petitchiffre.Print(ol::ToString(core_user_define_nb_bangers), x_cfg_sc+135,y_cfg_sc+75+(l*30));
def_param_max=128;   
break;     
case 6:
petitchiffre.Print("Echo", x_cfg_sc+15,y_cfg_sc+75+(l*30));  
petitchiffre.Print(ol::ToString(core_user_define_nb_echo), x_cfg_sc+135,y_cfg_sc+75+(l*30));
def_param_max=24;        
break;     
}    
if(window_focus_id==920 && mouse_x>x_cfg_sc+120 && mouse_x<x_cfg_sc+120+50 && mouse_y>y_cfg_sc+60+(l*30) && mouse_y<y_cfg_sc+60+(l*30)+25)
{
Param.DrawOutline(CouleurLigne);
if(mouse_b&1 && mouse_released==0  &&  numeric_postext>0)
{
def_param=atoi(numeric);
if(def_param<=def_param_max && def_param>=def_param_min)
{
switch(l)
{
case 0:
core_user_define_nb_faders=def_param;
break;
case 1:
core_user_define_nb_docks=def_param;
for(int i=0;i<48;i++)//core adapt
{
for(int d=0;d<6;d++)
{
DockIsSelected[i][d]=0;
}

DockIsSelected[i][0]=1;   
}
break;
case 2:
core_user_define_nb_chasers=def_param;
break;    
case 3:
core_user_define_nb_tracks_per_chasers=def_param; 
nbre_track_visualisables=core_user_define_nb_tracks_per_chasers;   
if(nbre_track_visualisables>8){nbre_track_visualisables=8;} 
break;     
case 4:
core_user_define_nb_gridplayers=def_param;
break;
case 5:
core_user_define_nb_bangers=def_param;
break;       
case 6:
core_user_define_nb_echo=def_param;     
break;               
}
mouse_released=1; 
reset_numeric_entry();
}            
}
}
}    
    
petitchiffre.Print("Do Calculations:", x_cfg_sc+200, y_cfg_sc+40); 
Line(Vec2D( x_cfg_sc+200, y_cfg_sc+50),Vec2D( x_cfg_sc+305, y_cfg_sc+50)).Draw(CouleurLigne);
char tmp_state_core[8];   
for (int l=0;l<9;l++)
{
Rect BParam(Vec2D(x_cfg_sc+300,y_cfg_sc+60+(l*25)),Vec2D(50,20));
BParam.SetRoundness(7.5);
BParam.Draw(CouleurFond.WithAlpha(0.5));  
switch(core_do_calculations[l])
{
case 0:
sprintf(tmp_state_core,"/Off");
break;
case 1:
BParam.Draw(CouleurFader);  
sprintf(tmp_state_core,"/On");
break;                               
}  
switch(l)
{
case 0:
petitchiffre.Print("Faders", x_cfg_sc+200,y_cfg_sc+75+(l*25));
break;    
case 1:
petitchiffre.Print("CueList", x_cfg_sc+200,y_cfg_sc+75+(l*25));     
break;
case 2:
petitchiffre.Print("Banger", x_cfg_sc+200,y_cfg_sc+75+(l*25));
break;    
case 3:
petitchiffre.Print("Trichromy", x_cfg_sc+200,y_cfg_sc+75+(l*25));     
break;
case 4:
petitchiffre.Print("VideoTracking", x_cfg_sc+200,y_cfg_sc+75+(l*25));
break;    
case 5:
petitchiffre.Print("Chasers", x_cfg_sc+200,y_cfg_sc+75+(l*25));
break;    
case 6:
petitchiffre.Print("Grid", x_cfg_sc+200,y_cfg_sc+75+(l*25));     
break;  
case 7:
petitchiffre.Print("Ch. Macros", x_cfg_sc+200,y_cfg_sc+75+(l*25));     
break;  
case 8:
petitchiffre.Print("Echo", x_cfg_sc+200,y_cfg_sc+75+(l*25));     
break;  
default:
break;
}
petitchiffre.Print(tmp_state_core,x_cfg_sc+310,y_cfg_sc+75+(l*25));
if(window_focus_id==920 && mouse_x>x_cfg_sc+305 && mouse_x<x_cfg_sc+305+50 && mouse_y>y_cfg_sc+60+(l*25) && mouse_y<y_cfg_sc+60+(l*25)+20
&& mouse_released==0)
{
core_do_calculations[l]=toggle(core_do_calculations[l]);                     
mouse_released=1;                        
}

}  

petitchiffre.Print("FPS RATE:", x_cfg_sc+370, y_cfg_sc+40); 
Line(Vec2D( x_cfg_sc+370, y_cfg_sc+50),Vec2D( x_cfg_sc+485, y_cfg_sc+50)).Draw(CouleurLigne);  
for (int l=0;l<2;l++)
{
Rect BCParam(Vec2D(x_cfg_sc+470,y_cfg_sc+60+(l*30)),Vec2D(50,25));
BCParam.SetRoundness(7.5);
BCParam.Draw(CouleurFond.WithAlpha(0.5));
switch(l)
{
case 0:
petitchiffre.Print("DMX:", x_cfg_sc+370, y_cfg_sc+75+(l*30)); 
petitchiffre.Print(ol::ToString(BPS_RATE), x_cfg_sc+485, y_cfg_sc+75+(l*30)); 
break;
case 1:
petitchiffre.Print("DMX IN:", x_cfg_sc+370, y_cfg_sc+75+(l*30)); 
petitchiffre.Print(ol::ToString(dmxINrate), x_cfg_sc+485, y_cfg_sc+75+(l*30)); 
break;
default:
break;         
}
if(window_focus_id==920 && mouse_x>x_cfg_sc+470 && mouse_x<x_cfg_sc+470+50 && mouse_y>y_cfg_sc+60+(l*30) && mouse_y<y_cfg_sc+60+(l*30)+25)
{
BCParam.DrawOutline(CouleurLevel);
if(mouse_b&1 && mouse_released==0  &&  numeric_postext>0)
{
def_param=atoi(numeric);
switch(l)
{
case 0://BPS RATE
if(def_param>=10 && def_param<=250)
{
BPS_RATE=def_param;
ticker_rate = BPS_TO_TIMER(BPS_RATE);
install_int_ex(ticker , ticker_rate);
reset_numeric_entry();
}
break;   
case 1://DMX IN RATE
if(def_param>=10 && def_param<=50)
{
dmxINrate=def_param;
ticker_dmxIn_rate = BPS_TO_TIMER(dmxINrate);
install_int_ex(ticker_dmxIn , ticker_dmxIn_rate);
reset_numeric_entry();
}
break;
}
mouse_released=1;
}                     
}
}

 return(0);   
}

int Box_artnet_udpport(int macx, int macy)
{
   petitchiffre.Print("Art-Net UDP Ports",macx-5,macy-20);
   petitpetitchiffre.Print("- default 6454 -",macx+10,macy);
   petitpetitchiffre.Print("Out:",macx-5,macy+20);
   petitpetitchiffre.Print("In:",macx+70,macy+20);
   Rect UDPOut(Vec2D(macx+20,macy+5),Vec2D(45,25));
   UDPOut.SetRoundness(7.5);
   UDPOut.Draw(CouleurFond.WithAlpha(0.4));
   Rect UDPIn(Vec2D(macx+95,macy+5),Vec2D(45,25));
   UDPIn.SetRoundness(7.5);
   UDPIn.Draw(CouleurFond.WithAlpha(0.4));
   petitpetitchiffre.Print(ol::ToString(clientport_artnet),macx+25,macy+20);
   petitpetitchiffre.Print(ol::ToString(serveurport_artnet),macx+100,macy+20);
   
   
   if(mouse_y>macy+5 && mouse_y<macy+5+25 && window_focus_id==920)
   {
   if(mouse_x>macx+20 && mouse_x<macx+20+45)
   {
   UDPOut.DrawOutline(CouleurLevel);                   
   if(mouse_button==1 && mouse_released==0)
   {
   clientport_artnet=atoi(numeric);
   reset_numeric_entry();
   index_re_init_client_artnet=1;
   index_ask_confirm=1;
   mouse_released=1;                   
   }                    
   }      
   else    if(mouse_x>macx+95 && mouse_x<macx+95+45)
   {
   UDPIn.DrawOutline(CouleurLevel);                   
   if(mouse_button==1 && mouse_released==0)
   {
   serveurport_artnet=atoi(numeric);
   reset_numeric_entry();
   index_re_init_serveur_artnet=1;
   index_ask_confirm=1;
   mouse_released=1;                   
   }                    
   }                  
   }
   
return(0);                   
}

int do_network_config( int x_cfg_sc,int y_cfg_sc, int largeur_cfg_sc, int hauteur_cfg_sc)
{

Canvas::SetClipping( x_cfg_sc, y_cfg_sc, largeur_cfg_sc, hauteur_cfg_sc);

Box_IP_routing (  x_cfg_sc+20, y_cfg_sc+70);
do_artnet_affect_config(x_cfg_sc+20, y_cfg_sc+250);

Box_artnet_udpport(x_cfg_sc+240, y_cfg_sc+40);
BoxiCat(x_cfg_sc+240, y_cfg_sc+150);



Canvas::DisableClipping();
return(0);
}


int do_main_config(int cfgnetw_X, int cfgnetw_Y, int largeurCFGdmxwindow,int hauteurCFGdmxwindow)
{
//config channels    
for (int oi=0;oi<5;oi++)
{
Rect UnderParam(Vec2D(cfgnetw_X+155,cfgnetw_Y+55+(oi*30)),Vec2D(50,25));
UnderParam.SetRoundness(7.5);
UnderParam.SetLineWidth(epaisseur_ligne_fader);  
UnderParam.Draw(CouleurFond.WithAlpha(0.5));
if(window_focus_id==W_CFGMENU && mouse_x> cfgnetw_X+155 && mouse_x<cfgnetw_X+155+50 && mouse_y>cfgnetw_Y+55+(oi*30) && mouse_y<cfgnetw_Y+55+(oi*30)+25)
{
UnderParam.DrawOutline(CouleurLevel);
if(mouse_button==1 && mouse_released==0)
{
int param_data_in=0;
switch(dmx_view)
{
case 0:
if(oi!=1){param_data_in=(int)(atoi(numeric)*2.55)+1;}
else {param_data_in=atoi(numeric);}
reset_numeric_entry();
break;
case 1:
param_data_in=atoi(numeric);
reset_numeric_entry();
break;
}     
switch(oi)
{
case 0:
dmx_view=toggle(dmx_view);
break;        
case 1:
default_step_level=param_data_in;
break;  
case 2:
check_channel_level=param_data_in;
break; 
case 3:
dimmer_check_level=param_data_in;
break; 
case 4:
index_blink_change_memories=toggle(index_blink_change_memories);
break;  
}
UnderParam.Draw(CouleurBlind);
mouse_released=1;                   
}
}   
}
char string_cfg_main[32];
petitchiffre.Print("Channels: " ,(cfgnetw_X+10), (cfgnetw_Y+40));
Line(Vec2D(cfgnetw_X+10,cfgnetw_Y+45),Vec2D(cfgnetw_X+100,cfgnetw_Y+45)).Draw(CouleurLigne);
petitchiffre.Print("Display mode:", (cfgnetw_X+20), (cfgnetw_Y+70) );
switch(dmx_view)
{
case 0:
sprintf(string_cfg_main,"/100");
break;
case 1:
sprintf(string_cfg_main,"/255");
break;
}
petitchiffre.Print(string_cfg_main, (cfgnetw_X+170), (cfgnetw_Y+70) );

//step level ne varie pas
sprintf(string_cfg_main,"%d", default_step_level);
petitchiffre.Print("Default step level:", (cfgnetw_X+20), (cfgnetw_Y+100) );
petitchiffre.Print(string_cfg_main ,(cfgnetw_X+170), (cfgnetw_Y+100));

switch(dmx_view)
{
case 0:
sprintf(string_cfg_main,"%d", (int)(((float)check_channel_level)/2.55));
break;
case 1:
sprintf(string_cfg_main,"%d", check_channel_level);
break;
}
petitchiffre.Print("Check channels level:", (cfgnetw_X+20), (cfgnetw_Y+130) );
petitchiffre.Print(string_cfg_main ,(cfgnetw_X+170), (cfgnetw_Y+130));

switch(dmx_view)
{
case 0:
sprintf(string_cfg_main,"%d", (int)(((float)dimmer_check_level)/2.55));
break;
case 1:
sprintf(string_cfg_main,"%d", dimmer_check_level);
break;
}
petitchiffre.Print("Check dimmers level:", (cfgnetw_X+20), (cfgnetw_Y+160) );
petitchiffre.Print(string_cfg_main ,(cfgnetw_X+170), (cfgnetw_Y+160));



switch(index_blink_change_memories)
{
case 0:
sprintf(string_cfg_main,"/Off");
break;
case 1:
sprintf(string_cfg_main,"/On");
break;
}
petitchiffre.Print("Show Change/Mem:", (cfgnetw_X+20), (cfgnetw_Y+190) );
petitchiffre.Print(string_cfg_main ,(cfgnetw_X+170), (cfgnetw_Y+190));

/////////////////////////NBRE AUDIO PLAYERS///////////////////////////////////////////////////

Rect UnderParam(Vec2D(cfgnetw_X+155,cfgnetw_Y+235),Vec2D(50,25));
UnderParam.SetRoundness(7.5);
UnderParam.SetLineWidth(epaisseur_ligne_fader);  
UnderParam.Draw(CouleurFond.WithAlpha(0.5));

Rect UnderParam2(Vec2D(cfgnetw_X+155,cfgnetw_Y+265),Vec2D(50,20));
UnderParam2.SetRoundness(7.5);
UnderParam2.SetLineWidth(epaisseur_ligne_fader);  
UnderParam2.Draw(CouleurFond.WithAlpha(0.5));




if(window_focus_id==W_CFGMENU && mouse_x> cfgnetw_X+155 && mouse_x<cfgnetw_X+155+50 )
{
if( mouse_y>cfgnetw_Y+235 && mouse_y<cfgnetw_Y+235+25)//nbre de channels audio
{
UnderParam.DrawOutline(CouleurLevel);
if(mouse_button==1 && mouse_released==0)
{
int param_data_in=0;
param_data_in=atoi(numeric);
index_nbre_players_visibles=param_data_in;
if(index_nbre_players_visibles<1){index_nbre_players_visibles=1;}
else if(index_nbre_players_visibles>4){index_nbre_players_visibles=4;}
reset_numeric_entry();
UnderParam.Draw(CouleurBlind);
mouse_released=1;
}
}
if(window_focus_id==W_CFGMENU &&  mouse_y>cfgnetw_Y+265 && mouse_y<cfgnetw_Y+265+20)//nbre de channels audio
{
UnderParam2.DrawOutline(CouleurLevel);
if(mouse_button==1 && mouse_released==0)
{
index_preloaded_sounds=toggle(index_preloaded_sounds);
UnderParam2.Draw(CouleurBlind);
mouse_released=1;
}
}
}


petitchiffre.Print("Audio: " ,(cfgnetw_X+10), (cfgnetw_Y+220));
petitchiffre.Print(audio_device_name,(cfgnetw_X+120), (cfgnetw_Y+220));
Line(Vec2D(cfgnetw_X+10,cfgnetw_Y+225),Vec2D(cfgnetw_X+100,cfgnetw_Y+225)).Draw(CouleurLigne);
petitchiffre.Print("Number of Players:", (cfgnetw_X+20), (cfgnetw_Y+250) );
sprintf(string_cfg_main,"%d", index_nbre_players_visibles);
petitchiffre.Print(string_cfg_main ,(cfgnetw_X+170), (cfgnetw_Y+250));
petitchiffre.Print("Preload on read:",(cfgnetw_X+20), (cfgnetw_Y+280));
switch(index_preloaded_sounds)
{
case 0:
sprintf(string_cfg_main,"/Off");
break;
case 1:
sprintf(string_cfg_main,"/On");
break;
}
petitchiffre.Print(string_cfg_main,(cfgnetw_X+170), (cfgnetw_Y+280));



///////////////////DEUXIEME COLONNE

for (int oi=0;oi<6;oi++)
{
Rect UnderParam(Vec2D(cfgnetw_X+365,cfgnetw_Y+55+(oi*30)),Vec2D(50,25));
UnderParam.SetRoundness(7.5);
UnderParam.SetLineWidth(epaisseur_ligne_fader);  
UnderParam.Draw(CouleurFond.WithAlpha(0.5));
if(window_focus_id==W_CFGMENU && mouse_x> cfgnetw_X+365 && mouse_x<cfgnetw_X+365+50 && mouse_y>cfgnetw_Y+55+(oi*30) && mouse_y<cfgnetw_Y+55+(oi*30)+25)
{
UnderParam.DrawOutline(CouleurLevel);
if(mouse_button==1 && mouse_released==0)
{
int param_data_in=0;
   
switch(oi)//action souris
{
case 0://Temps par defaut
default_time=atof(numeric);
reset_numeric_entry();
break;
case 1://temps par defaut d un bang
default_time_of_the_bang=atof(numeric);
reset_numeric_entry();
break;
case 2://time digit nbre decimales affichages chasers
index_affichage_digit_time_unit=atoi(numeric)-1;
if(index_affichage_digit_time_unit<=0){index_affichage_digit_time_unit=0;}
else 
{
if (index_affichage_digit_time_unit>3)
{index_affichage_digit_time_unit=3;}
reset_numeric_entry();
}
break;
case 3://nbre tracks visualisables chasers
nbre_track_visualisables=atoi(numeric);
if(nbre_track_visualisables<1){nbre_track_visualisables=1;}
if(nbre_track_visualisables>nbre_tracks_par_chaser){nbre_track_visualisables=nbre_tracks_par_chaser;}
reset_numeric_entry();
break;
case 4://midi force mode pour le play
Midi_Force_Go=toggle(Midi_Force_Go);
break;
case 5://nombre cues dans preset view
nbre_memoires_visualisables_en_preset=atoi(numeric);
if(nbre_memoires_visualisables_en_preset<5){nbre_memoires_visualisables_en_preset=5;}
if(nbre_memoires_visualisables_en_preset>25){nbre_memoires_visualisables_en_preset=25;}
hauteur_globale_sequenciel=180+(35*(nbre_memoires_visualisables_en_preset+1))+35;
reset_numeric_entry();
break;
default:
break;
}
mouse_released=1;
}
}
  
switch(oi)//affichage
{
case 0://Temps par defaut
petitchiffre.Print("CueList / Bangs / Chasers " ,(cfgnetw_X+225), (cfgnetw_Y+40));
Line(Vec2D(cfgnetw_X+225,cfgnetw_Y+45),Vec2D(cfgnetw_X+410,cfgnetw_Y+45)).Draw(CouleurLigne);
petitchiffre.Print("Record Default time:",cfgnetw_X+235,cfgnetw_Y+70+(oi*30));
petitchiffre.Print(ol::ToString(default_time),cfgnetw_X+380,cfgnetw_Y+70+(oi*30));
break;
case 1:
petitchiffre.Print("Bang stay-on time:",cfgnetw_X+235,cfgnetw_Y+70+(oi*30));
char tmpo[10];
sprintf(tmpo,"%.1f",default_time_of_the_bang);
petitchiffre.Print(tmpo,cfgnetw_X+380,cfgnetw_Y+70+(oi*30));    
break;
case 2:
petitchiffre.Print("Time Unit Digits:",cfgnetw_X+235,cfgnetw_Y+70+(oi*30));
petitchiffre.Print(ol::ToString(index_affichage_digit_time_unit+1),cfgnetw_X+380,cfgnetw_Y+70+(oi*30));
break;
case 3:
petitchiffre.Print("Chaser View Tracks:",cfgnetw_X+235,cfgnetw_Y+70+(oi*30));
petitchiffre.Print(ol::ToString(nbre_track_visualisables),cfgnetw_X+380,cfgnetw_Y+70+(oi*30));  
break;  
case 4:
petitchiffre.Print("GO Midi ForceMode:",cfgnetw_X+235,cfgnetw_Y+70+(oi*30));
switch(Midi_Force_Go)
{
case 0:
petitchiffre.Print("/Off",cfgnetw_X+380,cfgnetw_Y+70+(oi*30));
break;
case 1:
petitchiffre.Print("/On",cfgnetw_X+380,cfgnetw_Y+70+(oi*30));
break;                              
}
break;
case 5:
petitchiffre.Print("Cues in Preset:",cfgnetw_X+235,cfgnetw_Y+70+(oi*30));
petitchiffre.Print(ol::ToString(nbre_memoires_visualisables_en_preset),cfgnetw_X+380,cfgnetw_Y+70+(oi*30));  
break;
default:
break;
}

}

//EXPERT MODE confirmations ou non

Rect ExpertB(Vec2D(cfgnetw_X+365,cfgnetw_Y+240),Vec2D(50,20));
ExpertB.SetRoundness(7.5);
ExpertB.SetLineWidth(epaisseur_ligne_fader);  
ExpertB.Draw(CouleurFond.WithAlpha(0.5));
ExpertB.Draw(CouleurFader.WithAlpha(expert_mode*alpha_blinker));

if(window_focus_id==W_CFGMENU && mouse_x> cfgnetw_X+365 && mouse_x<cfgnetw_X+365+50 && mouse_y>cfgnetw_Y+240 && mouse_y<cfgnetw_Y+240+20)
{
ExpertB.DrawOutline(CouleurLevel);
if(mouse_button==1 && mouse_released==0)
{
expert_mode=toggle(expert_mode);
ExpertB.Draw(CouleurBlind);
mouse_released=1;
}
}


petitchiffre.Print("EXPERT MODE: " ,(cfgnetw_X+225), (cfgnetw_Y+255));
Line(Vec2D(cfgnetw_X+225,cfgnetw_Y+260),Vec2D(cfgnetw_X+325,cfgnetw_Y+260)).Draw(CouleurLigne);
switch(expert_mode)
{
case 0:
sprintf(string_cfg_main,"/Off");
break;
case 1:
sprintf(string_cfg_main,"/On");
break;
}
petitchiffre.Print(string_cfg_main,(cfgnetw_X+380), (cfgnetw_Y+255));


//quatrieme bloc

petitchiffre.Print("On Start" ,(cfgnetw_X+430), (cfgnetw_Y+40));
Line(Vec2D(cfgnetw_X+430,cfgnetw_Y+45),Vec2D(cfgnetw_X+530,cfgnetw_Y+45)).Draw(CouleurLigne);
petitchiffre.Print("Open Camera",cfgnetw_X+430,cfgnetw_Y+70);
Rect CamOn(Vec2D(cfgnetw_X+525,cfgnetw_Y+55),Vec2D(50,20));
CamOn.SetRoundness(7.5);
CamOn.SetLineWidth(epaisseur_ligne_fader);  
CamOn.Draw(CouleurFond.WithAlpha(0.5));

if(window_focus_id==W_CFGMENU && mouse_x> cfgnetw_X+525 && mouse_x<cfgnetw_X+525+50 && mouse_y>cfgnetw_Y+55 && mouse_y<cfgnetw_Y+55+20)
{
CamOn.DrawOutline(CouleurLevel);
if(mouse_button==1 && mouse_released==0)
{
load_camera_on_start=toggle(load_camera_on_start);
CamOn.Draw(CouleurBlind);
mouse_released=1;
}
} 
switch(load_camera_on_start)
{
case 0:
sprintf(string_cfg_main,"/Off");
break;
case 1:
sprintf(string_cfg_main,"/On");
break;
}
petitchiffre.Print(string_cfg_main,cfgnetw_X+540,cfgnetw_Y+70);



petitchiffre.Print("Open Arduino",cfgnetw_X+430,cfgnetw_Y+100);
Rect ArduinOn(Vec2D(cfgnetw_X+525,cfgnetw_Y+85),Vec2D(50,20));
ArduinOn.SetRoundness(7.5);
ArduinOn.SetLineWidth(epaisseur_ligne_fader);  
ArduinOn.Draw(CouleurFond.WithAlpha(0.5));

if(window_focus_id==W_CFGMENU && mouse_x> cfgnetw_X+525 && mouse_x<cfgnetw_X+525+50 && mouse_y>cfgnetw_Y+85 && mouse_y<cfgnetw_Y+85+20)
{
ArduinOn.DrawOutline(CouleurLevel);
if(mouse_button==1 && mouse_released==0)
{
open_arduino_on_open=toggle(open_arduino_on_open);
ArduinOn.Draw(CouleurBlind);
mouse_released=1;
}
} 
switch(open_arduino_on_open)
{
case 0:
sprintf(string_cfg_main,"/Off");
break;
case 1:
sprintf(string_cfg_main,"/On");
break;
}
petitchiffre.Print(string_cfg_main,cfgnetw_X+540,cfgnetw_Y+100);

//iCat enable_iCat
petitchiffre.Print("Open iCat",cfgnetw_X+430,cfgnetw_Y+130);
Rect iCatOn(Vec2D(cfgnetw_X+525,cfgnetw_Y+115),Vec2D(50,20));
iCatOn.SetRoundness(7.5);
iCatOn.SetLineWidth(epaisseur_ligne_fader);  
iCatOn.Draw(CouleurFond.WithAlpha(0.5));

if(window_focus_id==W_CFGMENU && mouse_x> cfgnetw_X+525 && mouse_x<cfgnetw_X+525+50 && mouse_y>cfgnetw_Y+115 && mouse_y<cfgnetw_Y+115+20)
{
iCatOn.DrawOutline(CouleurLevel);
if(mouse_button==1 && mouse_released==0)
{
enable_iCat=toggle(enable_iCat);
mouse_released=1;
}
} 
switch(enable_iCat)
{
case 0:
sprintf(string_cfg_main,"/Off");
break;
case 1:
sprintf(string_cfg_main,"/On");
break;
}
petitchiffre.Print(string_cfg_main,cfgnetw_X+540,cfgnetw_Y+130);



//PROCESS ACCESS>>multicore

Rect ProcessB(Vec2D(cfgnetw_X+525,cfgnetw_Y+190),Vec2D(50,20));
ProcessB.SetRoundness(7.5);
ProcessB.SetLineWidth(epaisseur_ligne_fader);  
ProcessB.Draw(CouleurFond.WithAlpha(0.5));

if(window_focus_id==W_CFGMENU && mouse_x> cfgnetw_X+525 && mouse_x<cfgnetw_X+525+50 && mouse_y>cfgnetw_Y+190 && mouse_y<cfgnetw_Y+190+20)
{
ProcessB.DrawOutline(CouleurLevel);
if(mouse_button==1 && mouse_released==0)
{
index_allow_multicore=toggle(index_allow_multicore);
ProcessB.Draw(CouleurBlind);
mouse_released=1;
if(index_allow_multicore==1 && core_to_assign>0 && core_to_assign<9 && index_allow_multicore==1)
{process_assign_to_core(core_to_assign);}
}
}


Rect ProcessCorB(Vec2D(cfgnetw_X+525,cfgnetw_Y+215),Vec2D(50,20));
ProcessCorB.SetRoundness(7.5);
ProcessCorB.SetLineWidth(epaisseur_ligne_fader);  
ProcessCorB.Draw(CouleurFond.WithAlpha(0.5));

if(window_focus_id==W_CFGMENU && mouse_x> cfgnetw_X+525 && mouse_x<cfgnetw_X+525+50 && mouse_y>cfgnetw_Y+215 && mouse_y<cfgnetw_Y+215+20)
{
ProcessCorB.DrawOutline(CouleurLevel);
if(mouse_button==1 && mouse_released==0)
{
core_to_assign=atoi(numeric);
reset_numeric_entry();
if(core_to_assign>0 && core_to_assign<9 && index_allow_multicore==1)
{process_assign_to_core(core_to_assign);}
ProcessCorB.Draw(CouleurBlind);
mouse_released=1;
}
}
petitchiffre.Print("Multicore: " ,(cfgnetw_X+430), (cfgnetw_Y+203));
Line(Vec2D(cfgnetw_X+430,cfgnetw_Y+210),Vec2D(cfgnetw_X+500,cfgnetw_Y+210)).Draw(CouleurLigne);
switch(index_allow_multicore)
{
case 0:
sprintf(string_cfg_main,"/Off");
break;
case 1:
sprintf(string_cfg_main,"/On");
break;
}
petitchiffre.Print(string_cfg_main,(cfgnetw_X+540), (cfgnetw_Y+203));
petitchiffre.Print("Affect to Core:",cfgnetw_X+430,cfgnetw_Y+228);
petitchiffre.Print(ol::ToString(core_to_assign),cfgnetw_X+545,cfgnetw_Y+228);



//suite Fader space
//LargeurEspaceFaderSize

Rect faderSpace(Vec2D(cfgnetw_X+525,cfgnetw_Y+265),Vec2D(50,20));
faderSpace.SetRoundness(7.5);
faderSpace.SetLineWidth(epaisseur_ligne_fader);  
faderSpace.Draw(CouleurFond.WithAlpha(0.5));

if(window_focus_id==W_CFGMENU && mouse_x> cfgnetw_X+525 && mouse_x<cfgnetw_X+525+50 && mouse_y>cfgnetw_Y+265 && mouse_y<cfgnetw_Y+265+20)
{
faderSpace.DrawOutline(CouleurLevel);
if(mouse_button==1 && mouse_released==0)
{
LargeurEspaceFaderSize=atoi(numeric);
reset_numeric_entry();
if(LargeurEspaceFaderSize<1024 ){LargeurEspaceFaderSize=1024;} 
if(LargeurEspaceFaderSize>largeur_ecran){LargeurEspaceFaderSize=largeur_ecran;}
faderSpace.Draw(CouleurBlind);
mouse_released=1;
}
}
petitchiffre.Print("Fader Space Width: " ,(cfgnetw_X+430), (cfgnetw_Y+255));
Line(Vec2D(cfgnetw_X+430,cfgnetw_Y+260),Vec2D(cfgnetw_X+560,cfgnetw_Y+260)).Draw(CouleurLigne);
petitchiffre.Print(ol::ToString(LargeurEspaceFaderSize),(cfgnetw_X+540), (cfgnetw_Y+280));


////////////////////////////////////////////////////



do_keyboard_conf((cfgnetw_X+620), (cfgnetw_Y+40));

return(0);   
}












////////////////DMX/////////////////////////////////////////////////////////////

int SelectDmxDevice(int interfacedmx)
{
Close_dmx_interface();
myDMXinterfaceis=interfacedmx;
if(interfacedmx!=1)
{   
index_art_polling=0;  
}
  
Init_dmx_interface();
return(0);    
}


int affect_dmx_in(int callb_x, int callb_y)
{
Rect MonCallButton(Vec2D(callb_x,callb_y),Vec2D(100,30));
MonCallButton.SetRoundness(7.5);
MonCallButton.SetLineWidth(demi_epaisseur_ligne_fader);

MonCallButton.DrawOutline(CouleurLigne); 
//affichage boutons

if (index_affect_dmxin==1)
{
MonCallButton.Draw(CouleurFader);
MonCallButton.DrawOutline(CouleurFader); 
}    
petitchiffre.Print("DmxIn>Dock",callb_x+5, callb_y+20);
if(window_focus_id==W_CFGMENU && mouse_x>callb_x && mouse_x<callb_x+100 && mouse_y>callb_y && mouse_y<callb_y+30 && window_focus_id==920)
{
if(mouse_button==1 && mouse_released==0)
{
 if(index_affect_dmxin==0)
{
reset_index_actions();
reset_indexs_confirmation(); 
index_affect_dmxin=1;
}
else 
{
reset_index_actions();     
}
index_do_dock=index_affect_dmxin;     
mouse_released=1;                   
}                   
}

return(0);   
}

int do_dmx_config(int cfgdmx_X, int cfgdmx_Y, int largeurCFGdmxwindow,int hauteurCFGdmxwindow)
{
    
//definition des aires devices
Rect DeviceArtNetDevice( Vec2D((cfgdmx_X+10),(cfgdmx_Y+50)),Vec2D(180,30)); 
DeviceArtNetDevice.SetRoundness(15);

Rect DeviceEnttecOpen( Vec2D((cfgdmx_X+10),(cfgdmx_Y+80)),Vec2D(180,30)); 
DeviceEnttecOpen.SetRoundness(15);

Rect DeviceEnttecPro( Vec2D((cfgdmx_X+10),(cfgdmx_Y+110)),Vec2D(180,30)); 
DeviceEnttecPro.SetRoundness(15);


Rect DeviceSunlite ( Vec2D((cfgdmx_X+10),(cfgdmx_Y+170)),Vec2D(180,30)); 
DeviceSunlite.SetRoundness(15);


//definition des aires univers
Rect ArtNetSendUniverse( Vec2D((cfgdmx_X+220),(cfgdmx_Y+50)),Vec2D(70,30)); 
ArtNetSendUniverse.SetRoundness(15);

//definition des aire d edition d adresse  
Rect ArtNetSendAdress( Vec2D((cfgdmx_X+320),(cfgdmx_Y+50)),Vec2D(170,30)); 
ArtNetSendAdress.SetRoundness(15);

//SPEC. ARTNET: BROADCAST MODE
Rect ArtNetBroadcastMode( Vec2D((cfgdmx_X+520),(cfgdmx_Y+50)),Vec2D(130,30)); 
ArtNetBroadcastMode.SetRoundness(15);
 
//SPEC ARTNET ARTPOLL
Rect ArtNetPollforDevices( Vec2D((cfgdmx_X+670),(cfgdmx_Y+50)),Vec2D(100,30));
ArtNetPollforDevices.SetRoundness(15);
 
//SPEC ARTNET DOUBLE DMX
Rect ArtNetDoubleDMX( Vec2D((cfgdmx_X+770),(cfgdmx_Y+50)),Vec2D(60,30));
ArtNetDoubleDMX.SetRoundness(15);
 


//SPEC ENTTEC PROIN
Rect EnttecProIN( Vec2D((cfgdmx_X+240),(cfgdmx_Y+110)),Vec2D(170,30));
EnttecProIN.SetRoundness(15);



//SPEC SUNLITE
Rect SunliteAllowsIN( Vec2D((cfgdmx_X+240),(cfgdmx_Y+170)),Vec2D(150,30)); 
SunliteAllowsIN.SetRoundness(15);





{

//SELECTION DE L INTERFACE
if(window_focus_id==W_CFGMENU && mouse_x>cfgdmx_X+20 && mouse_x<cfgdmx_X+200 )
{
   if( mouse_y>(cfgdmx_Y+60) && mouse_y< (cfgdmx_Y+80)) //artnet    
   {
   DeviceArtNetDevice.Draw(CouleurFond.WithAlpha(0.5)); 
   Survol_interface_numero=1;
   if(mouse_button==1 && mouse_released==0 )
   {
   SelectDmxDevice(1);           
   }
   }
   else if( mouse_y>(cfgdmx_Y+90) && mouse_y< (cfgdmx_Y+110)) //enttec open
   {
   DeviceEnttecOpen.Draw(CouleurFond.WithAlpha(0.5)); 
   Survol_interface_numero=2;
   if(mouse_button==1 && mouse_released==0  )
   {
   SelectDmxDevice(2);   
      
   }
   }
   else if( mouse_y>(cfgdmx_Y+120) && mouse_y< (cfgdmx_Y+140)) //enttec pro   
   {  
   DeviceEnttecPro.Draw(CouleurFond.WithAlpha(0.5)); 
   Survol_interface_numero=3;
   if(mouse_button==1 && mouse_released==0 )
   {
   SelectDmxDevice(3);            
   }  
   } 
     
    else if( mouse_y>(cfgdmx_Y+170) && mouse_y< (cfgdmx_Y+200)) //sunlite
   {  
   DeviceSunlite.Draw(CouleurFond.WithAlpha(0.5)); 
   Survol_interface_numero=4;
   if(mouse_button==1 && mouse_released==0  )
   {
   SelectDmxDevice(4);            
   }  
   }             
}
   
//SELECTION DE L UNIVERS
if(window_focus_id==W_CFGMENU && mouse_x>cfgdmx_X+230 && mouse_x<cfgdmx_X+300 )
{
   if( mouse_y>(cfgdmx_Y+60) && mouse_y< (cfgdmx_Y+80) && myDMXinterfaceis==1) //artnet    
   {
   ArtNetSendUniverse.DrawOutline(CouleurLigne.WithAlpha(0.5)); 
   if(mouse_button==1 &&  mouse_released==0 )
   {
   Univers=atoi(numeric);  if(Univers>15){Univers=15;}  
   reset_numeric_entry();
   numeric_postext=0;
    
   mouse_released=1;       
   }
   }
         
}

//SELECTION DE L ADRESSE OUT ARTNET OU DU VCOM POUR LES INTERFACES DMX
if(window_focus_id==W_CFGMENU && mouse_x>cfgdmx_X+320 && mouse_x<cfgdmx_X+490 )
{
   if( mouse_y>(cfgdmx_Y+60) && mouse_y< (cfgdmx_Y+80) && myDMXinterfaceis==1) //artnet    
   {
   ArtNetSendAdress.DrawOutline(CouleurLigne.WithAlpha(0.7)); 
   if(mouse_button==1 &&  mouse_released==0  && numeric_postext>0)
   {
   Close_dmx_interface(); 
   sprintf(ip_artnet,numeric);
   reset_numeric_entry();
   Init_dmx_interface();  
   mouse_released=1;       
   }
   }
          
}


//SPECIFIC A CHAQUE INTERFACE
if(window_focus_id==W_CFGMENU && mouse_x>cfgdmx_X+520 && mouse_x<cfgdmx_X+620 )
{
   if( mouse_y>(cfgdmx_Y+60) && mouse_y< (cfgdmx_Y+80) && myDMXinterfaceis==1) //artnet BROADCAST MODE   ON OFF
   {
   ArtNetBroadcastMode.DrawOutline(CouleurLigne.WithAlpha(0.7)); 

   if(mouse_button==1 &&  mouse_released==0 )
   {
   ArtNetBroadcastMode.DrawOutline(CouleurLigne.WithAlpha(0.7)); 
   Close_dmx_interface(); 
   if(index_broadcast==0)
   {   
   index_broadcast=1;  
   Init_dmx_interface(); 
   }
   else if(index_broadcast==1)
   {
   index_broadcast=0;
   Init_dmx_interface(); 
   }  
   mouse_released=1;
   }
   }        
}

//SPECIFIC A CHAQUE INTERFACE <ARTNET SUITE>
if(window_focus_id==W_CFGMENU && mouse_x>cfgdmx_X+670 && mouse_x<cfgdmx_X+770 )
{
   if( mouse_y>(cfgdmx_Y+60) && mouse_y< (cfgdmx_Y+80) ) //artpoll for devices 
   {
   ArtNetPollforDevices.DrawOutline(CouleurLigne.WithAlpha(0.7)); 

   if(mouse_button==1 &&  mouse_released==0 )
   {
   if (myDMXinterfaceis!=1)//dans le cas d un art poll avec interface usb
   {
    initialisation_client_artnet();
    ConstructArtPoll();
    }             
   
   if(index_art_polling==0)
   {    
   reset_poll_list();
   ticks_poll=0; 
   index_art_polling=1;
   index_show_artpoll_reply_content=1;
   nbrbytessended=sendto(sockartnet, ArtPollBuffer,sizeof( ArtPollBuffer),0,(SOCKADDR*)&sinS,sinsize);
   }
  else if(index_art_polling==1)
   {
   index_art_polling=0;
   if (myDMXinterfaceis!=1)//dans le cas d un art poll avec interface usb
   {fermeture_client_artnet();}   
   }
   mouse_released=1;
   }
   
   }        
}
//DOUBLE DMX
if(window_focus_id==W_CFGMENU && mouse_x>cfgdmx_X+770 && mouse_x<cfgdmx_X+830 )
{
   if( mouse_y>(cfgdmx_Y+60) && mouse_y< (cfgdmx_Y+80) ) //double dmx 
   {
   ArtNetDoubleDMX.DrawOutline(CouleurLigne.WithAlpha(0.7)); 

   if(mouse_button==1 &&  mouse_released==0 &&  myDMXinterfaceis!=1)
   {
   index_artnet_doubledmx=toggle(index_artnet_doubledmx);
   mouse_released=1;
   switch(index_artnet_doubledmx)
   {
   case 0:                              
         fermeture_client_artnet();
   break;
   case 1:
         initialisation_client_artnet();
         //ConstructArtPoll();
         ConstructArtPollReply();
         nbrbytessended=sendto(sockartnet, ArtPollBuffer,sizeof( ArtPollBuffer),0,(SOCKADDR*)&sinS,sinsize);
         //ArtNet
         ArtDmx();
         //nbrbytessended=sendto(sockartnet, ArtDmxBuffer,sizeof( ArtDmxBuffer),0,(SOCKADDR*)&sinS,sinsize);  
   break;
                                 
   }
    }
    }
}
//ENTTEC PRO IN MODULE

if (window_focus_id==W_CFGMENU && mouse_x>cfgdmx_X+270 && mouse_x<cfgdmx_X+420 && mouse_y>cfgdmx_Y+120 && mouse_y<cfgdmx_Y+140 )
{
EnttecProIN.Draw(CouleurFond.WithAlpha(0.5)); 

   if(mouse_button==1 &&  mouse_released==0 )
   { 

   if (index_init_EnttecPROIN_ok==0)
   {   Detect_EnttecProIn() ; 
       Open_ProIn();
   }
   else if (index_init_EnttecPROIN_ok==1)
   {   Close_ProIn() ; index_init_EnttecPROIN_ok=0;}
 
   }   
   mouse_released=1;                   
}

//sunlite allows IN
if (window_focus_id==W_CFGMENU && mouse_x>cfgdmx_X+240 && mouse_x<cfgdmx_X+390 && mouse_y>cfgdmx_Y+180 && mouse_y<cfgdmx_Y+210 && myDMXinterfaceis==5)
{
SunliteAllowsIN.Draw(CouleurFond.WithAlpha(0.5)); 
if(mouse_button==1 &&  mouse_released==0 &&  window_focus_id==920)
   {
   index_allow_sunlite_dmxIN=toggle(index_allow_sunlite_dmxIN);
   mouse_released=1;
   }
}



}//fin condition etre 920 fenetre maitre


/////////////////////////////////////////////////////////////////////////////////////
if(myDMXinterfaceis==1){   DeviceArtNetDevice.DrawOutline(CouleurLevel); }
else if(myDMXinterfaceis==2){   DeviceEnttecOpen.DrawOutline(CouleurLevel); }
else if(myDMXinterfaceis==3){   DeviceEnttecPro.DrawOutline(CouleurLevel); }
else if(myDMXinterfaceis==4)
{   
DeviceSunlite.DrawOutline(CouleurLevel);
petitchiffre.Print("Version:",cfgdmx_X+400, cfgdmx_Y+190); 
petitchiffre.Print(ol::ToString(sunlite_version_number),cfgdmx_X+460, cfgdmx_Y+190);
petitchiffre.Print("Serial:",cfgdmx_X+500, cfgdmx_Y+190); 
petitchiffre.Print(ol::ToString(sunlite_serial_number),cfgdmx_X+550, cfgdmx_Y+190);
petitchiffre.Print(string_sunlite_is,cfgdmx_X+670, cfgdmx_Y+190);
}

if(index_art_polling==1)
{ 
  ArtNetPollforDevices.DrawOutline(CouleurLevel);                      
}    

if(index_artnet_doubledmx==1)
{
ArtNetDoubleDMX.Draw(CouleurFader);
}
//////////////////////////////////////////////

if (index_init_EnttecPROIN_ok==1)
{
EnttecProIN.DrawOutline(CouleurLevel); 
}
/////////////////////////////////////////////

/////////////////////////////////////////////

switch(index_allow_sunlite_dmxIN)
{
case 0:
petitchiffre.Print("DMX-IN OFF", cfgdmx_X+250, cfgdmx_Y+190);
break;                                 
case 1:
SunliteAllowsIN.Draw(CouleurSurvol);     
petitchiffre.Print("DMX-IN ON", cfgdmx_X+250, cfgdmx_Y+190);
break;
                                 
}



//affichage
sprintf(string_title_panel_config,"DMX CONFIGURATION PANEL");


petitpetitchiffre.Print("Universe" ,(cfgdmx_X+220), (cfgdmx_Y+50));
petitpetitchiffre.Print("To IP" ,(cfgdmx_X+350), (cfgdmx_Y+50));
petitpetitchiffre.Print("Mode" ,(cfgdmx_X+560), (cfgdmx_Y+50));
petitpetitchiffre.Print("Call" ,(cfgdmx_X+710), (cfgdmx_Y+50));
////////////////////////////////////////////////////////////////////////////////
petitchiffre.Print("ART-NET" ,(cfgdmx_X+20), (cfgdmx_Y+70));
petitchiffre.Print(ol::ToString(Univers),(cfgdmx_X+240), (cfgdmx_Y+70));
petitchiffre.Print(ip_artnet,(cfgdmx_X+340), (cfgdmx_Y+70));
if(index_broadcast==0)
{petitchiffre.Print("UNICAST" ,(cfgdmx_X+540), (cfgdmx_Y+70));}
else if (index_broadcast==1)
{petitchiffre.Print("BROADCAST" ,(cfgdmx_X+540), (cfgdmx_Y+70));}
petitchiffre.Print("ART-POLL",(cfgdmx_X+690), (cfgdmx_Y+70));
petitpetitchiffre.Print("Double",(cfgdmx_X+780), (cfgdmx_Y+65));
petitpetitchiffre.Print("DMX",(cfgdmx_X+790), (cfgdmx_Y+75));
////////////////////////////////////////////////////////////////////////////////
petitchiffre.Print("ENTTEC-OPEN-DMX" ,(cfgdmx_X+20), (cfgdmx_Y+100));
petitchiffre.Print("ENTTEC-PRO OUT /VCOM" ,(cfgdmx_X+20), (cfgdmx_Y+130));
petitchiffre.Print(ol::ToString(istheresomeone_in_enttecpro),(cfgdmx_X+170), (cfgdmx_Y+130));
petitchiffre.Print("ENTTEC-PRO IN /VCOM" ,(cfgdmx_X+250), (cfgdmx_Y+130));
petitchiffre.Print(ol::ToString(vcom_inposition_is),(cfgdmx_X+390), (cfgdmx_Y+130));

////////////////////////////////////////////////////////////////////////////////
petitchiffre.Print("SUNLITE SIUDI " ,(cfgdmx_X+20), (cfgdmx_Y+190));
////////////////////////////////////////////////////////////////////////////////

Rect DmxFeedbackZone(Vec2D((cfgdmx_X+10), (cfgdmx_Y+265)),Vec2D(560,25));
DmxFeedbackZone.SetRoundness(5);
DmxFeedbackZone.Draw(CouleurFond.WithAlpha(0.5));
petitchiffre.Print("DMX feedback: >>",(cfgdmx_X+20), (cfgdmx_Y+280));
petitchiffrerouge.Print(string_display_dmx_params,(cfgdmx_X+130), (cfgdmx_Y+280));

affect_dmx_in(cfgdmx_X+710,cfgdmx_Y+200);

return(0);   
}



int save_network_settings()
{
FILE *fp;
char rep_conf_ntwk[256];
sprintf(rep_conf_ntwk,"%s\\user\\config_network.txt",mondirectory);
if(fp=fopen(rep_conf_ntwk,"w"))
{
                               
fprintf(fp,"#arguments: Receive art-net Socket  \n");
fprintf(fp,"%s\n",IP_artnet_IN);                               
fprintf(fp,"#arguments: Send art-net Socket \n");
fprintf(fp,"%s\n",IP_artnet_OUT);    
fprintf(fp,"#arguments: Art-net UDP PORT IN / UDP PORT OUT \n");
fprintf(fp,"%d / %d /\n",serveurport_artnet, clientport_artnet);      
fprintf(fp,"#arguments: Fantastick-iCat Socket /\n");
fprintf(fp,"%s\n",IP_fantastick);                             
fprintf(fp,"#arguments: Fantastick-iCat  SEND TO adress ( iPhone/iPad) \n");
fprintf(fp,"%s\n",specified_fs_ip);
fprintf(fp,"Fantastick-iCat UDP PORT IN from FS / iCat UDP PORT OUT TO FS / Iterations /\n");
fprintf(fp,"%d / %d / %d /\n",serveurport_iCat,clientport_iCat,nbre_d_envois_de_l_info);

}
fclose(fp);
sprintf(string_Last_Order,">>Saved Network configuration");     
return(0);   
}

int load_network_conf()
{
FILE *cfg_file = NULL ;
char read_buff[ 512 ] ;

    char motcleinterfaceis[24];
    int it=0;
	cfg_file = fopen("user\\config_network.txt", "rt" );
	if( !cfg_file )
	{
	 printf("\nPb à ouverture de config_network.txt\n");
     return 1;
	}	
	
    //premiere ligne les args
	if( !fgets( read_buff , sizeof( read_buff ) ,cfg_file ) )
	{
     sprintf(string_save_load_report[idf],"! config_network.txt");
     return 1;
	}
	fscanf( cfg_file , "%s\n" ,  &IP_artnet_IN );
	
	fgets( read_buff , sizeof( read_buff ) ,cfg_file );
	
	fscanf( cfg_file , "%s\n" ,  &IP_artnet_OUT );
	
	fgets( read_buff , sizeof( read_buff ) ,cfg_file );	
	
	fscanf( cfg_file , "%d / %d /\n" ,  &serveurport_artnet, &clientport_artnet );
	
	fgets( read_buff , sizeof( read_buff ) ,cfg_file );
	
	fscanf( cfg_file , "%s\n" ,  &IP_fantastick );
	
	fgets( read_buff , sizeof( read_buff ) ,cfg_file );
	
	fscanf( cfg_file , "%s\n" ,  &specified_fs_ip );
	
	fgets( read_buff , sizeof( read_buff ) ,cfg_file );
	
	fscanf( cfg_file , "%d / %d / %d /\n" ,  &serveurport_iCat, &clientport_iCat, &nbre_d_envois_de_l_info );
	
	fclose( cfg_file );	
	
    
return(0);  
}



int do_panel_config(int cfg_X,int cfg_Y)
{

Rect ConfigPanel(Vec2D(cfg_X,  cfg_Y), Vec2D( largeurCFGwindow,hauteurCFGwindow));
ConfigPanel.SetRoundness(15);
ConfigPanel.Draw(CouleurConfig); 
ConfigPanel.DrawOutline(CouleurLigne); 

if(index_config_dmx==1)
{
do_dmx_config(cfg_X,cfg_Y,largeurCFGwindow,hauteurCFGwindow);
petitchiffre.Print( string_title_panel_config,cfg_X+20, cfg_Y+20);
} 
else if(index_config_midi==1)
{
do_midi_config(cfg_X,cfg_Y,largeurCFGwindow,hauteurCFGwindow);
//pas de titre car onglets  
}
else if(index_config_arduino==1)
{
//deviendra Arduino CFG
do_arduino_config(cfg_X,cfg_Y);
sprintf(string_title_panel_config,"ARDUINO CONFIGURATION");
petitchiffre.Print( string_title_panel_config,cfg_X+20, cfg_Y+20);  
} 
else if(index_setup_gfx==1)
{
do_screen_config(cfg_X,cfg_Y,largeurCFGwindow,hauteurCFGwindow);
sprintf(string_title_panel_config,"SCREEN CONFIGURATION");
petitchiffre.Print( string_title_panel_config,cfg_X+20, cfg_Y+20);      
}
else if(index_config_network==1)
{
do_network_config(cfg_X,cfg_Y,largeurCFGwindow,hauteurCFGwindow);
sprintf(string_title_panel_config,"NETWORK");
petitchiffre.Print( string_title_panel_config,cfg_X+20, cfg_Y+20);          
}
else if (index_config_general==1)
{
do_main_config(cfg_X,cfg_Y,largeurCFGwindow,hauteurCFGwindow);
sprintf(string_title_panel_config,"MAIN CONFIGURATION");
petitchiffre.Print( string_title_panel_config,cfg_X+20, cfg_Y+20);      
}

else if(index_config_core==1)
{
do_core_config(cfg_X,cfg_Y,largeurCFGwindow,hauteurCFGwindow);
sprintf(string_title_panel_config,"CORE CONFIGURATION");
petitchiffre.Print( string_title_panel_config,cfg_X+20, cfg_Y+20);     
}
///BOUTON DE SAUVEGARDE DE CONF
if( index_config_dmx==1 || index_config_network==1 || index_config_general==1 ||  index_config_core==1)
{
Rect SaveCFG( Vec2D((cfg_X+largeurCFGwindow-110),(cfg_Y+hauteurCFGwindow-40)),Vec2D(90,30)); 
SaveCFG.SetRoundness(15);
SaveCFG.Draw(CouleurFond.WithAlpha(0.5));
if( window_focus_id==920)
{
if(mouse_x>(cfg_X+largeurCFGwindow-110) && mouse_x<(cfg_X+largeurCFGwindow-20)
&& mouse_y>(cfg_Y+hauteurCFGwindow-40)&& mouse_y<(cfg_Y+hauteurCFGwindow-10) )
{
SaveCFG.DrawOutline(CouleurLevel);
if(mouse_button==1 &&  mouse_released==0  )
{
SaveCFG.Draw(CouleurLevel); 
if( index_config_dmx==1)
{
Save_my_dmx_conf();
save_artnet_conf();
}
else if(index_config_network==1)
{
save_network_settings();
}
else if( index_config_general==1)
{
Save_onstart_Config();
}
else if( index_config_core==1)
{
Save_Core_Config();     
}
mouse_released=1;           
}                                         
}
}
petitchiffre.Print( "SAVE CFG",(cfg_X+largeurCFGwindow-100), (cfg_Y+hauteurCFGwindow-20));
}
return(0);   
}




int config_general_menu()
{
Rect CadreGeneralConfig(Vec2D(window_cfgX,window_cfgY),Vec2D(largeurCFGwindow,hauteurCFGwindow+40));
CadreGeneralConfig.SetRoundness(15);
CadreGeneralConfig.SetLineWidth(triple_epaisseur_ligne_fader); 
CadreGeneralConfig.Draw(CouleurConfig);
if(window_focus_id==920)
{CadreGeneralConfig.DrawOutline(CouleurBlind);}
else{CadreGeneralConfig.DrawOutline(CouleurLigne);}
int mysetupx=window_cfgX+100;
for (int choix=0;choix<7;choix++)
{
Rect MySetup(Vec2D( mysetupx+(choix*100), window_cfgY+5),Vec2D(90,40));
MySetup.SetRoundness(7.5);
MySetup.Draw(CouleurBleuProcedure.WithAlpha(0.8));
switch(choix)
{
case 0:
if(config_page_is==1){MySetup.Draw(CouleurSurvol);}
neuromoyen.Print("   dmx cfg",mysetupx+(choix*100)+5,window_cfgY+30);
if(window_focus_id==W_CFGMENU && mouse_x> mysetupx+(choix*100) && mouse_x< mysetupx+(choix*100)+90 && mouse_y>window_cfgY+5 && mouse_y<window_cfgY+5+40)
{
if(mouse_button==1 && mouse_released==0 && window_focus_id==920)
{
reset_indexes_conf();
index_config_dmx=1;
config_page_is=1;
mouse_released=1;                   
}            
}
break;
case 1:
if(config_page_is==2){MySetup.Draw(CouleurSurvol);}
neuromoyen.Print("  midi cfg",mysetupx+(choix*100)+5,window_cfgY+30);
if(window_focus_id==W_CFGMENU && mouse_x> mysetupx+(choix*100) && mouse_x< mysetupx+(choix*100)+90 && mouse_y>window_cfgY+5 && mouse_y<window_cfgY+5+40)
{
if(mouse_button==1 && mouse_released==0 && window_focus_id==920)
{
reset_indexes_conf();
index_config_midi=1;
config_page_is=2;
mouse_released=1;                   
}            
}
break;
case 2:
if(config_page_is==3){MySetup.Draw(CouleurSurvol);}
neuromoyen.Print("network cfg",mysetupx+(choix*100)+5,window_cfgY+30);
if(window_focus_id==W_CFGMENU && mouse_x> mysetupx+(choix*100) && mouse_x< mysetupx+(choix*100)+90 && mouse_y>window_cfgY+5 && mouse_y<window_cfgY+5+40)
{
if(mouse_button==1 && mouse_released==0 && window_focus_id==920)
{
reset_indexes_conf();
index_config_network=1;
config_page_is=3;
mouse_released=1;                   
}            
}
break;
case 3:
if(config_page_is==4){MySetup.Draw(CouleurSurvol);}
neuromoyen.Print("screen cfg",mysetupx+(choix*100)+5,window_cfgY+30);
if(window_focus_id==W_CFGMENU && mouse_x> mysetupx+(choix*100) && mouse_x< mysetupx+(choix*100)+90 && mouse_y>window_cfgY+5 && mouse_y<window_cfgY+5+40)
{
if(mouse_button==1 && mouse_released==0 && window_focus_id==920)
{
reset_indexes_conf();
index_setup_gfx=1;
config_page_is=4;
mouse_released=1;                   
}            
}
break; 
case 4:
if(config_page_is==5){MySetup.Draw(CouleurSurvol);}
neuromoyen.Print("arduino cfg",mysetupx+(choix*100)+5,window_cfgY+30);
if(window_focus_id==W_CFGMENU && mouse_x> mysetupx+(choix*100) && mouse_x< mysetupx+(choix*100)+90 && mouse_y>window_cfgY+5 && mouse_y<window_cfgY+5+40)
{
if(mouse_button==1 && mouse_released==0 && window_focus_id==920)
{
reset_indexes_conf();
index_config_arduino=1;
config_page_is=5;
mouse_released=1;                   
}            
}
break;            
case 5:
if(config_page_is==6){MySetup.Draw(CouleurSurvol);}
neuromoyen.Print(" GENERAL",mysetupx+(choix*100)+5,window_cfgY+30);
if(window_focus_id==W_CFGMENU && mouse_x> mysetupx+(choix*100) && mouse_x< mysetupx+(choix*100)+90 && mouse_y>window_cfgY+5 && mouse_y<window_cfgY+5+40)
{
if(mouse_button==1 && mouse_released==0 && window_focus_id==920)
{
reset_indexes_conf();
index_config_general=1;
config_page_is=6;
mouse_released=1;                   
}            
}
break;   
case 6:
if(config_page_is==7){MySetup.Draw(CouleurSurvol);}
neuromoyen.Print("  core cfg",mysetupx+(choix*100)+5,window_cfgY+30);
if(window_focus_id==W_CFGMENU && mouse_x> mysetupx+(choix*100) && mouse_x< mysetupx+(choix*100)+90 && mouse_y>window_cfgY+5 && mouse_y<window_cfgY+5+40)
{
if(mouse_button==1 && mouse_released==0 && window_focus_id==920)
{
reset_indexes_conf();
index_config_core=1;
config_page_is=7;
mouse_released=1;                   
}            
}
break; 
}  
}
do_panel_config(window_cfgX,window_cfgY+40);

return(0);   
}

