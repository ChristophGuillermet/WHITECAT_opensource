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

 \file help_2.cpp
 \brief {description courte} 
 \author Christoph Guillermet
 \version {numero de version du fichier}
 \date {date description}
 
 White Cat {- catégorie} {- sous catégorie {- sous catégorie}}
 Description détaillée
 
 **/
 
 


int HelpWindow(int xhelp, int yhelp)
{

Rect BackHelp(Vec2D(xhelp,yhelp),Vec2D(700,300));
BackHelp.SetRoundness(rondeur_fader/2);
BackHelp.SetLineWidth(epaisseur_ligne_fader*3); 
BackHelp.Draw(CouleurFond);
BackHelp.Draw(CouleurBleuProcedure.WithAlpha(0.5));
if(window_focus_id==W_HELP){
BackHelp.DrawOutline(CouleurFader);
}
else {BackHelp.DrawOutline(CouleurLigne);}

neuro.Print("HELP",xhelp+100, yhelp+30);
petitchiffre.Print("SITE:      http://www.le-chat-noir-numerique.fr",xhelp+190, yhelp+10);
petitchiffre.Print("WIKI:      http://www.le-chat-noir-numerique.fr/whitecat/dokuwiki/",xhelp+190, yhelp+25);
petitchiffre.Print("FORUM:     http://www.le-chat-noir-numerique.fr/forums/",xhelp+190, yhelp+40);
//////////////////UP DOWN LINE IMPORT/////////////////////
Circle LineUp(Vec2D(xhelp+660,yhelp+40),12);
LineUp.Draw(CouleurFond);
Circle LineDown(Vec2D(xhelp+660,yhelp+260),12);
LineDown.Draw(CouleurFond);
if(mouse_x>xhelp+660-12 && mouse_x<xhelp+660+12)
{
if(mouse_y>yhelp+40-12 && mouse_y<yhelp+40+12)
{                      
LineUp.Draw(CouleurSurvol);
if(mouse_button==1)
{
LineUp.Draw(CouleurFader);
if(line_help>0){line_help--;}
}                 
}
else if(mouse_y>yhelp+260-12 && mouse_y<yhelp+260+12)
{
LineDown.Draw(CouleurSurvol);
if(mouse_button==1)
{
LineDown.Draw(CouleurFader);
if(line_help<(512-15)){line_help++;}
}                  
}                                                
}
petitchiffre.Print("-",xhelp+658,yhelp+43);
petitchiffre.Print("+",xhelp+656,yhelp+262);
LineUp.DrawOutline(CouleurLigne);
LineDown.DrawOutline(CouleurLigne);

///////////TEXTE///////////////////////////////////////////////

Rect TitleUnder(Vec2D(xhelp+5,yhelp+45),Vec2D(620,25)); 
TitleUnder.Draw(CouleurFond); 

petitchiffre.Print("ACTION" ,xhelp+10,yhelp+60);       
petitchiffre.Print("Keyboard ShortCut",xhelp+210,yhelp+60);       
petitchiffre.Print("SYNTAX" ,xhelp+420,yhelp+60);       

for(int tx=0;tx<15;tx++)
{
if (btitle_help[tx+line_help]==1)
{
Rect TitleBar(Vec2D(xhelp+5,yhelp+75+(tx*15)),Vec2D(620,15));
TitleBar.Draw(CouleurFond);  
TitleBar.Draw(CouleurSurvol.WithAlpha(0.5));                                
}
petitchiffre.Print(TblHelp[tx+line_help][0] ,xhelp+10,yhelp+85+(tx*15));       
petitchiffre.Print(TblHelp[tx+line_help][1] ,xhelp+210,yhelp+85+(tx*15));       
petitchiffre.Print(TblHelp[tx+line_help][2] ,xhelp+350,yhelp+85+(tx*15));       
}

Rect Bdoctype(Vec2D(xhelp+570,yhelp+47),Vec2D(50,20));
Bdoctype.SetRoundness(rondeur_fader/2);


if(mouse_x>xhelp+570 && mouse_x<xhelp+570+50 && mouse_y>yhelp+47 && mouse_y<yhelp+47+20)
{

Bdoctype.Draw(CouleurSurvol); 
if(mouse_button==1 && mouse_released==0)
{
help_lang=toggle(help_lang);
if(help_lang==0){generate_help_file_fr();}
else if(help_lang==1){generate_help_file_engl();}
mouse_released=1;                   
}                     
}
if(help_lang==0){petitchiffre.Print("FR",xhelp+585,yhelp+62);}
else if(help_lang==1){petitchiffre.Print("ENGL",xhelp+580,yhelp+62);}

Bdoctype.DrawOutline(CouleurLigne); 
return(0);   
}
