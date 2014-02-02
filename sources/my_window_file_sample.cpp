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

 \file my_window_file_sample.cpp
 \brief {description courte} 
 \author Christoph Guillermet
 \version {numero de version du fichier}
 \date {date description}
 
 White Cat {- catégorie} {- sous catégorie {- sous catégorie}}
 Description détaillée
 
 **/
 
 

//LA LOGIQUE////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int do_logical_my_window_Box(int mx, int my)
{
    
if(window_focus_id==W_MY_WINDOW && mouse_released==0)//si focus sur la fenetre my window et que bouton pas enclenché
{
//BOUTON LOOP                                  
if(mouse_x>mx+260 && mouse_x<mx+280 && mouse_y>my+100 && mouse_y<my+120)
{
mouse_released=1; //on encleche le bouton pour pas avoir de double ordre
my_window_variable_boolean=toggle(my_window_variable_boolean);                 
}             
//FADERS
for(int i=0;i<6;i++)//pas de mouse released, c est que pour les boutons
{
if(mouse_x>mx+10+(i*35) && mouse_x<mx+30+(i*35) && mouse_y>my+60 && mouse_y<my+207)//10 pixels en haut et en bas en plus
{
my_window_fader_value[i] = constrain_data_to_midi_range((my+70+127)-mouse_y);//dans core_6.cpp               
}
}
                   
}//fin condition focus
  
 return(0);   
}

///////////////////////////////////////////////////////////////////////////////
//L'AFFICHAGE//////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int my_window_Box(int mx, int my)
{
//la fenêtre en elle même: un rectangle
Rect fond_my_window(Vec2D(mx,my), Vec2D( largeur_my_window,hauteur_my_window));
//facteur d'arrondissement du rectangle
fond_my_window.SetRoundness(15);
fond_my_window.SetLineWidth(triple_epaisseur_ligne_fader);
//fond
fond_my_window.Draw(CouleurFond); 
//pourtour
if(window_focus_id==W_MY_WINDOW)//si elle est sélectionnée bord orange
{
fond_my_window.DrawOutline(CouleurFader); 
}
else
{
fond_my_window.DrawOutline(CouleurLigne);    
} 
neuro.Print( "My Window",(mx+110), (my+30));

//BOUTON LOOP
loop_button_view(mx+260,my+100,my_window_variable_boolean);

//FADER d une valeur de 127 pixel

Rect FaderFrame(Vec2D(mx,my),Vec2D(20,127));
FaderFrame.SetRoundness(3); 

for(int i=0;i<6;i++)
{
petitchiffre.Print(ol::ToString(my_window_fader_value[i] ),mx+12+(i*35),my+58);
FaderFrame.MoveTo(Vec2D(mx+10+(i*35),my+70));
Rect FaderLevel(Vec2D(mx+10+(i*35),my+70+127-my_window_fader_value[i] ), Vec2D(20,my_window_fader_value[i] ));
FaderLevel.SetRoundness(3); 
FaderLevel.Draw(CouleurFader);
FaderFrame.DrawOutline(CouleurLigne); 
}
    
 return(0);   
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////PLUS RAPIDE ET MOINS DIFFICILE POUR ECRIRE D ABORD/////////
/////////////////////LE CODE AVANT SCINDAGE EN LOGIQUE / VISUEL ////////////////
////////////////////////////////////////////////////////////////////////////////

//LA LOGIQUE////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*
int do_logical_my_window_Box(int mx, int my)
{
 return(0);   
}

///////////////////////////////////////////////////////////////////////////////
//L'AFFICHAGE//////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int my_window_Box(int mx, int my)
{
//la fenêtre en elle même: un rectangle
Rect fond_my_window(Vec2D(mx,my), Vec2D( largeur_my_window,hauteur_my_window));
//facteur d'arrondissement du rectangle
fond_my_window.SetRoundness(15);
fond_my_window.SetLineWidth(triple_epaisseur_ligne_fader);
//fond
fond_my_window.Draw(CouleurFond); 
//pourtour
if(window_focus_id==W_MY_WINDOW)//si elle est sélectionnée bord orange
{
fond_my_window.DrawOutline(CouleurFader); 
}
else
{
fond_my_window.DrawOutline(CouleurLigne);    
} 
neuro.Print( "My Window",(mx+110), (my+30));

//BOUTON LOOP
loop_button_view(mx+260,my+100,my_window_variable_boolean);
if(window_focus_id==W_MY_WINDOW && mouse_released==0)//si focus sur la fenetre my window et que bouton pas enclenché
{
//BOUTON LOOP                                  
if(mouse_x>mx+260 && mouse_x<mx+280 && mouse_y>my+100 && mouse_y<my+120)
{
mouse_released=1; //on encleche le bouton pour pas avoir de double ordre
my_window_variable_boolean=toggle(my_window_variable_boolean);                 
}             
                  
}//fin condition focus
  
//FADER d une valeur de 127 pixel

Rect FaderFrame(Vec2D(mx,my),Vec2D(20,127));
FaderFrame.SetRoundness(3); 

for(int i=0;i<6;i++)
{
petitchiffre.Print(ol::ToString(my_window_fader_value[i] ),mx+12+(i*35),my+58);
FaderFrame.MoveTo(Vec2D(mx+10+(i*35),my+70));
Rect FaderLevel(Vec2D(mx+10+(i*35),my+70+127-my_window_fader_value[i] ), Vec2D(20,my_window_fader_value[i] ));
FaderLevel.SetRoundness(3); 
FaderLevel.Draw(CouleurFader);
FaderFrame.DrawOutline(CouleurLigne); 
if(window_focus_id==W_MY_WINDOW && mouse_button==1)//mouse_button n est pas utilisé dans le code scindé,
//car c est lui que l on regarde pour faire toutes les parties logiques
{
if(mouse_x>mx+10+(i*35) && mouse_x<mx+30+(i*35) && mouse_y>my+60 && mouse_y<my+207)//10 pixels en haut et en bas en plus
{
my_window_fader_value[i] = constrain_data_to_midi_range((my+70+127)-mouse_y);//dans core_6.cpp               
}
}
}   
 return(0);   
}*/

