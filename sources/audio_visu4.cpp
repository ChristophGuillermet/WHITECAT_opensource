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

 \file audio_visu4.cpp
 \brief {description courte} 
 \author Christoph Guillermet
 \version {numero de version du fichier}
 \date {date description}
 
 White Cat {- catégorie} {- sous catégorie {- sous catégorie}}
 Description détaillée
 
 **/
 
 


int fader_niveau_son(int xp, int yp, int numero)
{
Rect FaderSon(Vec2D(xp,yp),Vec2D(20,127));
FaderSon.SetRoundness(5);
Rect FaderSonNiveau(Vec2D(xp,yp+(127-player_niveauson[numero])),Vec2D(20,player_niveauson[numero]));
FaderSonNiveau.Draw(CouleurFader);   
FaderSon.DrawOutline(CouleurLigne.WithAlpha(0.5));   

petitpetitchiffre.Print(string_niveauson[numero],xp+22,yp+10);
petitpetitchiffre.Print(ol::ToString(player_niveauson[numero]),xp+22,yp+20);

if(window_focus_id==919 && Midi_Faders_Affectation_Type!=0 && mouse_x>xp && mouse_x<xp+20 && mouse_y>=yp && mouse_y<=yp+127 )
{
FaderSon.DrawOutline(CouleurBlind);
}

Line (Vec2D(xp+20,yp+107),Vec2D(xp+30,yp+112)).Draw(CouleurLigne);

Circle(Vec2D(xp+37,yp+117),7).Draw(CouleurBlind.WithAlpha(midi_send_out[616+numero]));
Circle(Vec2D(xp+37,yp+117),7).DrawOutline(CouleurLigne);

raccrochage_midi_visuel_vertical(xp,yp,616+numero,20,127);

return(0);    
}

int lecteur_audio( int xp, int yp, int numero)
{
Rect FramePlayer(Vec2D(xp-5,yp-5),Vec2D(340,135));   
FramePlayer.SetRoundness(5);   
FramePlayer.Draw(CouleurBleuProcedure.WithAlpha(0.7));
Rect Player(Vec2D(xp,yp),Vec2D(200,20));
Player.SetRoundness(5);   
Player.Draw(CouleurConfig.WithAlpha(0.8));
Player.DrawOutline(CouleurLigne);

Canvas::SetClipping( xp,yp,200,20);
petitdoomInspekt.Print(ol::ToString(numero+1),xp+5,yp+12);
petitchiffre.Print(sound_files[numero],xp+20,yp+12);
Canvas::DisableClipping();


//PLAY / Pause
Rect Play(Vec2D(xp,yp+30),Vec2D(20,20));
Play.SetRoundness(4);
Play.Draw(CouleurBlind.WithAlpha( player_is_playing[numero]));
Play.Draw(CouleurFond.WithAlpha(0.5));
Play.DrawOutline(CouleurLigne);
Line(Vec2D(xp+6,yp+34),Vec2D(xp+6,yp+46)).Draw(CouleurLigne);
Line(Vec2D(xp+6,yp+34),Vec2D(xp+16,yp+40)).Draw(CouleurLigne);
Line(Vec2D(xp+6,yp+46),Vec2D(xp+16,yp+40)).Draw(CouleurLigne);





petitpetitchiffre.Print(time_is_for_filePos[numero],xp+120,yp+40);

petitpetitchiffre.Print(time_is_for_fileTotal[numero],xp+120,yp+50);



//SEEK TO 0
Rect SeekToZero(Vec2D(xp+25,yp+30),Vec2D(20,20));
SeekToZero.SetRoundness(4);
SeekToZero.Draw(CouleurFond.WithAlpha(0.5));
Line(Vec2D(xp+41,yp+34),Vec2D(xp+41,yp+46)).Draw(CouleurLigne);
Line(Vec2D(xp+41,yp+34),Vec2D(xp+31,yp+40)).Draw(CouleurLigne);
Line(Vec2D(xp+31,yp+40),Vec2D(xp+41,yp+46)).Draw(CouleurLigne);
Line(Vec2D(xp+29,yp+34),Vec2D(xp+29,yp+46)).Draw(CouleurLigne);

SeekToZero.DrawOutline(CouleurLigne);

if(midi_show_flash_seektouch[numero]==1){SeekToZero.Draw(CouleurFader);midi_show_flash_seektouch[numero]=0;}





//GENERAL LOOP
Rect GeneralLoop(Vec2D(xp+50,yp+30),Vec2D(20,20));
GeneralLoop.SetRoundness(4);
GeneralLoop.Draw(CouleurFond.WithAlpha(0.5));
GeneralLoop.DrawOutline(CouleurLigne.WithAlpha(0.5));
Circle(Vec2D(xp+60,yp+40),5).Draw(CouleurFader.WithAlpha(player_is_onloop[numero]));
Circle(Vec2D(xp+60,yp+40),5).DrawOutline(CouleurLigne);




//SEEK TO End
Rect SeekToEnd(Vec2D(xp+75,yp+30),Vec2D(20,20));
SeekToEnd.SetRoundness(4);
SeekToEnd.Draw(CouleurFond.WithAlpha(0.5));
Line(Vec2D(xp+79,yp+34),Vec2D(xp+79,yp+46)).Draw(CouleurLigne);
Line(Vec2D(xp+79,yp+34),Vec2D(xp+89,yp+40)).Draw(CouleurLigne);
Line(Vec2D(xp+89,yp+40),Vec2D(xp+79,yp+46)).Draw(CouleurLigne);
Line(Vec2D(xp+91,yp+34),Vec2D(xp+91,yp+46)).Draw(CouleurLigne);
SeekToEnd.DrawOutline(CouleurLigne);




//Backward position
Rect Backward(Vec2D(xp,yp+60),Vec2D(28,16));
Backward.SetRoundness(0.5);
Backward.Draw(CouleurFond.WithAlpha(0.5));
Backward.DrawOutline(CouleurLigne.WithAlpha(0.5));

if(midi_show_flash_backwardtouch[numero]==1){Backward.Draw(CouleurBlind);midi_show_flash_backwardtouch[numero]=0;}


Line(Vec2D(xp+16,yp+62),Vec2D(xp+16,yp+74)).Draw(CouleurLigne);
Line(Vec2D(xp+16,yp+62),Vec2D(xp+6,yp+68)).Draw(CouleurLigne);
Line(Vec2D(xp+16,yp+74),Vec2D(xp+6,yp+68)).Draw(CouleurLigne);

Line(Vec2D(xp+26,yp+62),Vec2D(xp+26,yp+74)).Draw(CouleurLigne);
Line(Vec2D(xp+26,yp+62),Vec2D(xp+16,yp+68)).Draw(CouleurLigne);
Line(Vec2D(xp+26,yp+74),Vec2D(xp+16,yp+68)).Draw(CouleurLigne);

//Forward position
Rect Forward(Vec2D(xp+35,yp+60),Vec2D(28,16));
Forward.SetRoundness(0.5);
Forward.Draw(CouleurFond.WithAlpha(0.5));
Forward.DrawOutline(CouleurLigne.WithAlpha(0.5));
if(midi_show_flash_forwardtouch[numero]==1){Forward.Draw(CouleurBlind);midi_show_flash_forwardtouch[numero]=0;}


Line(Vec2D(xp+36,yp+62),Vec2D(xp+36,yp+74)).Draw(CouleurLigne);
Line(Vec2D(xp+36,yp+62),Vec2D(xp+46,yp+68)).Draw(CouleurLigne);
Line(Vec2D(xp+36,yp+74),Vec2D(xp+46,yp+68)).Draw(CouleurLigne);

Line(Vec2D(xp+46,yp+62),Vec2D(xp+46,yp+74)).Draw(CouleurLigne);
Line(Vec2D(xp+46,yp+62),Vec2D(xp+56,yp+68)).Draw(CouleurLigne);
Line(Vec2D(xp+46,yp+74),Vec2D(xp+56,yp+68)).Draw(CouleurLigne);



//previous position
Rect Previous(Vec2D(xp,yp+85),Vec2D(45,16));
Previous.SetRoundness(0.5);
Previous.Draw(CouleurFond.WithAlpha(0.5));
Previous.DrawOutline(CouleurLigne.WithAlpha(0.5));
petitchiffre.Print("PREV",xp+6,yp+96);





//nEXT TRACK position
Rect Next(Vec2D(xp+50,yp+85),Vec2D(45,16));
Next.SetRoundness(0.5);
Next.Draw(CouleurFond.WithAlpha(0.5));
Next.DrawOutline(CouleurLigne.WithAlpha(0.5));
petitchiffre.Print("NEXT",xp+56,yp+96);


//NUM FICHIER
Rect NumFichier(Vec2D(xp+70,yp+55),Vec2D(40,25));
NumFichier.SetRoundness(3);
NumFichier.Draw(CouleurFond.WithAlpha(0.5));
neuromoyen.Print(ol::ToString(player_has_file_coming_from_pos[numero]),xp+80,yp+71);
NumFichier.DrawOutline(CouleurLigne.WithAlpha(0.5));




if(window_focus_id==919 && mouse_x>xp+70 && mouse_x<xp+110 && mouse_y>yp+55 && mouse_y<yp+80)
{
NumFichier.DrawOutline(CouleurLigne.WithAlpha(0.5));
}
//autoload cuelist
//audio_auto_load
Rect Autoload(Vec2D(xp,yp+110),Vec2D(28,16));
Autoload.SetRoundness(0.5);
Autoload.Draw(CouleurFond.WithAlpha(0.5));
Autoload.Draw(CouleurBlind.WithAlpha(audio_autoload[numero]));
Autoload.DrawOutline(CouleurLigne.WithAlpha(0.5));


Line(Vec2D(xp+3,yp+112),Vec2D(xp+20,yp+112)).Draw(CouleurLigne);
Line(Vec2D(xp+20,yp+112),Vec2D(xp+20,yp+123)).Draw(CouleurLigne);
Line(Vec2D(xp+20,yp+123),Vec2D(xp+27,yp+118)).Draw(CouleurLigne);
Line(Vec2D(xp+13,yp+118),Vec2D(xp+20,yp+123)).Draw(CouleurLigne);

//autopause
Rect Autostop(Vec2D(xp+35,yp+110),Vec2D(40,16));
Autostop.SetRoundness(0.5);
Autostop.Draw(CouleurFond.WithAlpha(0.5));
Autostop.Draw(CouleurBlind.WithAlpha(audio_autopause[numero]));
Autostop.DrawOutline(CouleurLigne.WithAlpha(0.5));
minichiffre.Print("A.PAUSE",xp+38,yp+120);



//SET cue IN
Rect SetLoopIN(Vec2D(xp+210,yp),Vec2D(20,20));
SetLoopIN.SetRoundness(4);


if(midi_show_flash_cueIntouch[numero]==1){SetLoopIN.Draw(CouleurBlind);midi_show_flash_cueIntouch[numero]=0;}

SetLoopIN.DrawOutline(CouleurLigne);
Line(Vec2D(xp+210,yp),Vec2D(xp+220,yp+10)).Draw(CouleurLigne);
Line(Vec2D(xp+230,yp),Vec2D(xp+220,yp+10)).Draw(CouleurLigne);
petitpetitchiffre.Print("in",xp+215,yp+17);




//SET cue out
Rect SetLoopOut(Vec2D(xp+235,yp),Vec2D(20,20));
SetLoopOut.SetRoundness(4);

if(midi_show_flash_cueOuttouch[numero]==1){SetLoopOut.Draw(CouleurBlind);midi_show_flash_cueOuttouch[numero]=0;}
SetLoopOut.DrawOutline(CouleurLigne);

Line(Vec2D(xp+235,yp),Vec2D(xp+245,yp+10)).Draw(CouleurLigne);
Line(Vec2D(xp+255,yp),Vec2D(xp+245,yp+10)).Draw(CouleurLigne);
petitpetitchiffre.Print("out",xp+235,yp+17);


//CUE ON OFF
Rect CueOn(Vec2D(xp+260,yp),Vec2D(20,20));
CueOn.SetRoundness(4);
CueOn.Draw(CouleurBlind.WithAlpha(player_is_onloopCue[numero]));
petitpetitchiffre.Print("cue",xp+260,yp+12);
CueOn.DrawOutline(CouleurLigne);


petitpetitchiffre.Print(time_is_for_fileCueIn[numero],xp+234,yp+40);
petitpetitchiffre.Print(time_is_for_fileCueOut[numero],xp+234,yp+50);

//Seek cue
Rect SeekCue(Vec2D(xp+210,yp+30),Vec2D(20,20));
SeekCue.SetRoundness(4);
SeekCue.DrawOutline(CouleurLigne);
if(midi_show_flash_cueSeektouch[numero]==1){SeekCue.Draw(CouleurBlind);midi_show_flash_cueSeektouch[numero]=0;}

Line(Vec2D(xp+226,yp+34),Vec2D(xp+226,yp+46)).Draw(CouleurLigne);
Line(Vec2D(xp+226,yp+34),Vec2D(xp+216,yp+40)).Draw(CouleurLigne);
Line(Vec2D(xp+216,yp+40),Vec2D(xp+226,yp+46)).Draw(CouleurLigne);
Line(Vec2D(xp+214,yp+34),Vec2D(xp+214,yp+46)).Draw(CouleurLigne);





//Pitch
Line(Vec2D(xp+130,yp+115),Vec2D(xp+257,yp+115)).Draw(CouleurLigne);//ligne horizontale
Line(Vec2D(xp+194,yp+105),Vec2D(xp+194,yp+125)).Draw(CouleurLigne);//barre 64
Line(Vec2D(xp+257,yp+110),Vec2D(xp+257,yp+115)).Draw(CouleurLigne);//barre 127
Rect PitchPos(Vec2D(xp+120+player_pitch[numero],yp+110),Vec2D(20,10));
PitchPos.SetRoundness(4);
PitchPos.Draw(CouleurBlind);
PitchPos.DrawOutline(CouleurLigne);
Rect PitchMidi(Vec2D(xp+130,yp+110),Vec2D(127,10));
petitpetitchiffre.Print(string_pitch[numero],(xp+130),(yp+107));
petitpetitchiffre.Print(ol::ToString(player_pitch[numero]),(xp+110),(yp+117));




raccrochage_midi_visuel_horizontal_audio (xp+130, yp+100, 620+numero, 127,10);
//Pan 
Line(Vec2D(xp+130,yp+90),Vec2D(xp+257,yp+90)).Draw(CouleurLigne);//ligne horizontale
Line(Vec2D(xp+194,yp+80),Vec2D(xp+194,yp+100)).Draw(CouleurLigne);//barre 64
Line(Vec2D(xp+257,yp+85),Vec2D(xp+257,yp+90)).Draw(CouleurLigne);//barre 127
Rect PanPos(Vec2D(xp+120+player_pan[numero],yp+85),Vec2D(20,10));
PanPos.SetRoundness(4);
PanPos.Draw(CouleurBlind);
PanPos.DrawOutline(CouleurLigne);
Rect PanMidi(Vec2D(xp+130,yp+85),Vec2D(127,10));

petitpetitchiffre.Print(string_pan[numero],(xp+130),(yp+82));
petitpetitchiffre.Print(ol::ToString(player_pan[numero]),(xp+110),(yp+94));




raccrochage_midi_visuel_horizontal_audio (xp+130, yp+80, 624+numero, 127,10);
//les faders son
fader_niveau_son(xp+290,yp,numero);

if(window_focus_id==919 && Midi_Faders_Affectation_Type!=0 )
{
if( mouse_x>xp && mouse_x<xp+20 && mouse_y>yp+30 && mouse_y<yp+50){Play.DrawOutline(CouleurBlind);}
else if(mouse_x>xp+25 && mouse_x<xp+45 && mouse_y>yp+30 && mouse_y<yp+50){SeekToZero.DrawOutline(CouleurBlind);}
else if( mouse_x>xp+50 && mouse_x<xp+70 && mouse_y>yp+30 && mouse_y<yp+50){GeneralLoop.DrawOutline(CouleurBlind);}
else if( mouse_x>xp+75 && mouse_x<xp+95 && mouse_y>yp+30 && mouse_y<yp+50){SeekToEnd.DrawOutline(CouleurBlind);}
else if( mouse_x>xp && mouse_x<xp+28 && mouse_y>yp+60 && mouse_y<yp+76){Backward.DrawOutline(CouleurBlind);}
else if ( mouse_x>xp+35 && mouse_x<xp+63 && mouse_y>yp+60 && mouse_y<yp+76){Forward.DrawOutline(CouleurBlind);}
else  if( mouse_x>xp && mouse_x<xp+45 && mouse_y>yp+85 && mouse_y<yp+101){Previous.DrawOutline(CouleurBlind);}
else if ( mouse_x>xp+50 && mouse_x<xp+95 && mouse_y>yp+85 && mouse_y<yp+101){Next.DrawOutline(CouleurBlind);}
else if ( mouse_x>xp+70 && mouse_x<xp+110 && mouse_y>yp+55 && mouse_y<yp+80) {NumFichier.DrawOutline(CouleurBlind);}
else if (  mouse_x>xp && mouse_x<xp+28 && mouse_y>yp+110 && mouse_y<yp+126){Autoload.DrawOutline(CouleurBlind);}
else if (  mouse_x>xp+35 && mouse_x<xp+75 && mouse_y>yp+110 && mouse_y<yp+126){Autostop.DrawOutline(CouleurBlind);}
else if ( mouse_x>xp+210 && mouse_x<xp+240 && mouse_y>yp && mouse_y<yp+20) {SetLoopIN.DrawOutline(CouleurBlind);}
else if( mouse_x>xp+235 && mouse_x<xp+255 && mouse_y>yp && mouse_y<yp+20) {SetLoopOut.DrawOutline(CouleurBlind);}
else if( mouse_x>xp+260 && mouse_x<xp+280 && mouse_y>yp && mouse_y<yp+20){CueOn.DrawOutline(CouleurBlind);}
else if(  mouse_x>xp+210 && mouse_x<xp+230 && mouse_y>yp+30 && mouse_y<yp+50){SeekCue.DrawOutline(CouleurBlind);}
else if (  mouse_x>xp+120+player_pitch[numero] && mouse_x<xp+150+player_pitch[numero] && mouse_y>yp+110 && mouse_y<yp+120)
{PitchMidi.DrawOutline(CouleurBlind);}
else if (mouse_x>xp+120+player_pan[numero] && mouse_x<xp+150+player_pan[numero] && mouse_y>yp+85 && mouse_y<yp+95)
{PanMidi.DrawOutline(CouleurBlind);}
}

return(0);   
}


int fenetre_audio(int xb,int yb)
{

Rect audio_backg(Vec2D(xb,yb), Vec2D( 600,150+(index_nbre_players_visibles*120)));
audio_backg.SetRoundness(15);
audio_backg.Draw(CouleurFond); 
audio_backg.SetLineWidth(triple_epaisseur_ligne_fader);
if(window_focus_id==919)
{
audio_backg.DrawOutline(CouleurFader); 
}
else
{
audio_backg.DrawOutline(CouleurLigne);    
}
 
neuro.Print( "AudioPlayers",(xb+80), (yb+30));

//FOLDER
Rect AudioFolderis(Vec2D(xb+350,yb+10),Vec2D(240,30));
AudioFolderis.SetRoundness(7.5);
AudioFolderis.Draw(CouleurSurvol);
neuromoyen.Print(audio_folder,xb+365,yb+25);


//////////////////////LISTE sons///////////////////////////////////////

Rect BackDeroule(Vec2D(xb+350,yb+45),Vec2D(240,(index_nbre_players_visibles*120)));
BackDeroule.SetRoundness(15);
BackDeroule.Draw(CouleurConfig.WithAlpha(0.7));
bool ind=0;

for (int y=1;y<(index_nbre_players_visibles*6);y++)
{
Rect OverAudioFile(Vec2D(xb+355,(yb+33+(y*20))),Vec2D(200,18));
OverAudioFile.SetRoundness(7.5);

OverAudioFile.Draw(CouleurFond.WithAlpha(0.5*ind));
ind=toggle(ind);
//affichage qui est selectionné
if(strcmp(audiofile_name,list_audio_files[audiofile_selected])==0 && (audiofile_selected==(y+line_audio)) )
{OverAudioFile.Draw(CouleurBlind);}

if(window_focus_id==919 && mouse_x>xb+355 && mouse_x<xb+505 && mouse_y>(yb+33+(y*20)) && mouse_y<(yb+48+(y*20)))
{
OverAudioFile.DrawOutline(CouleurLigne);                  
}


petitpetitchiffre.Print(ol::ToString(line_audio+y),xb+360,yb+45+(y*20)); 
Canvas::SetClipping( xb+355,yb+35+(y*20),230,20);
petitpetitchiffre.Print(list_audio_files[line_audio+y],xb+375,yb+45+(y*20));   
Canvas::DisableClipping();
}

//////////////////UP DOWN LINE IMPORT/////////////////////
Circle LineUp(Vec2D(xb+570,yb+65),12);
LineUp.Draw(CouleurFond);   
Circle LineDown(Vec2D(xb+570,yb+(index_nbre_players_visibles*120)),12);
LineDown.Draw(CouleurFond);


petitchiffre.Print("-",xb+566,yb+70);
petitchiffre.Print("+",xb+566,yb+(index_nbre_players_visibles*120)+5);
LineUp.DrawOutline(CouleurLigne);
LineDown.DrawOutline(CouleurLigne);

///RESCAN FOLDER
    Rect AudioRescanDriver( Vec2D(xb+230,yb+10), Vec2D ( 50,20));
    AudioRescanDriver.SetRoundness(7.5); 
    AudioRescanDriver.DrawOutline(CouleurLigne);

////////////////////////////////////////////////////////////////////////////////
//EDIT ON OFF
   Rect EditSound( Vec2D(xb+230,yb+40), Vec2D ( 50,20));
    EditSound.SetRoundness(7.5); 
    if(index_edit_audio==1){EditSound.Draw(CouleurFader);}
    EditSound.DrawOutline(CouleurLigne);
    petitpetitchiffre.Print("EDIT",xb+240,yb+53);


//////LES LECTEURS//////////////////////////////////////////////////////////////
for(int lop=0;lop<index_nbre_players_visibles;lop++)
{
lecteur_audio(xb+5,yb+70+(lop*140),lop);
}

//////L AFFECTATION AUX FADERS//////////////////////////////////////////////////
//Rect AffectA(Vec2D(xb+440,yb+450),Vec2D(140,20));
Rect AffectA(Vec2D(xb+440,yb+55+(index_nbre_players_visibles*120)),Vec2D(140,20));
AffectA.SetRoundness(5);
AffectA.Draw(CouleurFader.WithAlpha(index_affect_audio_to_dock));
AffectA.DrawOutline(CouleurLigne);  
petitchiffre.Print("AFFECT TO DOCK",xb+460,yb+70+(index_nbre_players_visibles*120));

petitchiffre.Print("Volume",xb+400,yb+100+(index_nbre_players_visibles*120));
petitchiffre.Print("Pan",xb+400,yb+120+(index_nbre_players_visibles*120));
petitchiffre.Print("Pitch",xb+400,yb+140+(index_nbre_players_visibles*120));

for(int pl=0;pl<index_nbre_players_visibles;pl++)
{
for (int co=0;co<3;co++)
{
 Rect AffectAudioType(Vec2D(xb+460+(pl*30),yb+90+(index_nbre_players_visibles*120)+(co*20)),Vec2D(20,15));
 AffectAudioType.SetRoundness(3);
 if(player_to_affect_to_dock==pl && audio_type_for_dock_affectation_is==co){AffectAudioType.Draw(CouleurFader);}
 AffectAudioType.DrawOutline(CouleurLigne);  
}
 petitpetitchiffre.Print(ol::ToString((pl+1)),xb+467+(pl*30),yb+85+(index_nbre_players_visibles*120));       
}


if(window_focus_id==919  )
{
if(mouse_x>xb+350 && mouse_x<xb+590 && mouse_y>yb+10 && mouse_y<yb+40)
{
AudioFolderis.DrawOutline(CouleurLigne);   
}        
else if( mouse_x>xb+230 && mouse_x<xb+280 && mouse_y>yb+10 && mouse_y<yb+30 )
    {
    AudioRescanDriver.Draw(CouleurSurvol);            
    }
else if( mouse_x>xb+558 && mouse_x<xb+582  )
{
if(mouse_y>yb+53 && mouse_y<yb+72)
{                      
LineUp.Draw(CouleurSurvol);           
}
if(mouse_y>yb+(index_nbre_players_visibles*120)-7 && mouse_y<yb+(index_nbre_players_visibles*120)+5)
{ 
LineDown.Draw(CouleurSurvol);             
}                                                
}
}


petitpetitchiffre.Print("rescan!",xb+235,yb+23);
return(0);    
}
