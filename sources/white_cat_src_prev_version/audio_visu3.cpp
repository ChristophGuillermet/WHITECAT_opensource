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

 \file {nom du fichier.extension}
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

sprintf(string_niveauson,"%0.2f",show_player_niveauson[numero]); 
petitpetitchiffre.Print(string_niveauson,xp+22,yp+10);
petitpetitchiffre.Print(ol::ToString(player_niveauson[numero]),xp+22,yp+20);

if(mouse_x>xp && mouse_x<xp+20 && mouse_y>=yp && mouse_y<=yp+127 && window_focus_id==919)
{
if(Midi_Faders_Affectation_Type!=0)//config midi
{
FaderSon.DrawOutline(CouleurBlind);
}
}

Line (Vec2D(xp+20,yp+107),Vec2D(xp+30,yp+112)).Draw(CouleurLigne);

if(midi_send_out[616+numero]==1){Circle(Vec2D(xp+37,yp+117),7).Draw(CouleurBlind);}
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
if( player_is_playing[numero]==1){Play.Draw(CouleurBlind);} 
Play.DrawOutline(CouleurLigne);
Line(Vec2D(xp+6,yp+34),Vec2D(xp+6,yp+46)).Draw(CouleurLigne);
Line(Vec2D(xp+6,yp+34),Vec2D(xp+16,yp+40)).Draw(CouleurLigne);
Line(Vec2D(xp+6,yp+46),Vec2D(xp+16,yp+40)).Draw(CouleurLigne);

if(mouse_x>xp && mouse_x<xp+20 && mouse_y>yp+30 && mouse_y<yp+30+20)
{
if(Midi_Faders_Affectation_Type!=0)//config midi
{
Play.DrawOutline(CouleurBlind);

}
}


char time_is_for_filePos[64];
char time_is_for_fileTotal[64];

sprintf(time_is_for_filePos,"Pos: %.1f",(((float)position_of_file_in_player[numero])/audio_rate[numero]));
petitpetitchiffre.Print(time_is_for_filePos,xp+120,yp+40);
sprintf(time_is_for_fileTotal,"Total: %.1f",(((float)length_of_file_in_player[numero])/audio_rate[numero]));
petitpetitchiffre.Print(time_is_for_fileTotal,xp+120,yp+50);



//SEEK TO 0
Rect SeekToZero(Vec2D(xp+25,yp+30),Vec2D(20,20));
SeekToZero.SetRoundness(4);
Line(Vec2D(xp+41,yp+34),Vec2D(xp+41,yp+46)).Draw(CouleurLigne);
Line(Vec2D(xp+41,yp+34),Vec2D(xp+31,yp+40)).Draw(CouleurLigne);
Line(Vec2D(xp+31,yp+40),Vec2D(xp+41,yp+46)).Draw(CouleurLigne);
Line(Vec2D(xp+29,yp+34),Vec2D(xp+29,yp+46)).Draw(CouleurLigne);
SeekToZero.DrawOutline(CouleurLigne);

if(midi_show_flash_seektouch[numero]==1){SeekToZero.Draw(CouleurFader);midi_show_flash_seektouch[numero]=0;}

if(mouse_x>xp+25 && mouse_x<xp+45 && mouse_y>yp+30 && mouse_y<yp+30+20)
{
if(Midi_Faders_Affectation_Type!=0)//config midi
{
SeekToZero.DrawOutline(CouleurBlind);
}
}


//GENERAL LOOP
Rect GeneralLoop(Vec2D(xp+50,yp+30),Vec2D(20,20));
GeneralLoop.SetRoundness(4);

if(player_is_onloop[numero]==1)
{Circle(Vec2D(xp+60,yp+40),5).Draw(CouleurFader);}

Circle(Vec2D(xp+60,yp+40),5).DrawOutline(CouleurLigne);

if(mouse_x>xp+50 && mouse_x<xp+70 && mouse_y>yp+30 && mouse_y<yp+30+20)
{
if( Midi_Faders_Affectation_Type!=0)//config midi
{
GeneralLoop.DrawOutline(CouleurBlind);
}
}

//Backward position
Rect Backward(Vec2D(xp,yp+60),Vec2D(28,16));
Backward.SetRoundness(0.5);
Backward.DrawOutline(CouleurLigne);

if(midi_show_flash_backwardtouch[numero]==1){Backward.Draw(CouleurBlind);midi_show_flash_backwardtouch[numero]=0;}


if(mouse_x>xp && mouse_x<xp+28 && mouse_y>yp+60 && mouse_y<yp+60+16)
{
if( Midi_Faders_Affectation_Type!=0)//config midi
{
Backward.DrawOutline(CouleurBlind);
}
}

Line(Vec2D(xp+16,yp+62),Vec2D(xp+16,yp+74)).Draw(CouleurLigne);
Line(Vec2D(xp+16,yp+62),Vec2D(xp+6,yp+68)).Draw(CouleurLigne);
Line(Vec2D(xp+16,yp+74),Vec2D(xp+6,yp+68)).Draw(CouleurLigne);

Line(Vec2D(xp+26,yp+62),Vec2D(xp+26,yp+74)).Draw(CouleurLigne);
Line(Vec2D(xp+26,yp+62),Vec2D(xp+16,yp+68)).Draw(CouleurLigne);
Line(Vec2D(xp+26,yp+74),Vec2D(xp+16,yp+68)).Draw(CouleurLigne);

//Forward position
Rect Forward(Vec2D(xp+35,yp+60),Vec2D(28,16));
Forward.SetRoundness(0.5);
Forward.DrawOutline(CouleurLigne);
if(midi_show_flash_forwardtouch[numero]==1){Forward.Draw(CouleurBlind);midi_show_flash_forwardtouch[numero]=0;}

if(mouse_x>xp+35 && mouse_x<xp+35+28 && mouse_y>yp+60 && mouse_y<yp+60+16)
{
if( Midi_Faders_Affectation_Type!=0)//config midi
{
Forward.DrawOutline(CouleurBlind);
}
}

Line(Vec2D(xp+36,yp+62),Vec2D(xp+36,yp+74)).Draw(CouleurLigne);
Line(Vec2D(xp+36,yp+62),Vec2D(xp+46,yp+68)).Draw(CouleurLigne);
Line(Vec2D(xp+36,yp+74),Vec2D(xp+46,yp+68)).Draw(CouleurLigne);

Line(Vec2D(xp+46,yp+62),Vec2D(xp+46,yp+74)).Draw(CouleurLigne);
Line(Vec2D(xp+46,yp+62),Vec2D(xp+56,yp+68)).Draw(CouleurLigne);
Line(Vec2D(xp+46,yp+74),Vec2D(xp+56,yp+68)).Draw(CouleurLigne);


//autoload cuelist

//audio_auto_load
Rect Autoload(Vec2D(xp,yp+100),Vec2D(28,16));
Autoload.SetRoundness(0.5);
Autoload.Draw(CouleurFond.WithAlpha(0.5));
Autoload.Draw(CouleurBlind.WithAlpha(audio_autoload[numero]));
Autoload.DrawOutline(CouleurLigne.WithAlpha(0.5));
if(mouse_x>xp && mouse_x<xp+28 && mouse_y>yp+100 && mouse_y<yp+100+16)
{
if( Midi_Faders_Affectation_Type!=0)//config midi
{
Autoload.DrawOutline(CouleurBlind);
}
}

Line(Vec2D(xp+6,yp+102),Vec2D(xp+15,yp+102)).Draw(CouleurLigne);
Line(Vec2D(xp+15,yp+102),Vec2D(xp+15,yp+113)).Draw(CouleurLigne);
Line(Vec2D(xp+15,yp+113),Vec2D(xp+22,yp+108)).Draw(CouleurLigne);
Line(Vec2D(xp+15,yp+108),Vec2D(xp+22,yp+108)).Draw(CouleurLigne);

//autopause
Rect Autostop(Vec2D(xp+35,yp+100),Vec2D(40,16));
Autostop.SetRoundness(0.5);
Autostop.Draw(CouleurFond.WithAlpha(0.5));
Autostop.Draw(CouleurBlind.WithAlpha(audio_autopause[numero]));
Autostop.DrawOutline(CouleurLigne.WithAlpha(0.5));
minichiffre.Print("A.PAUSE",xp+38,yp+110);
if(mouse_x>xp+35 && mouse_x<xp+35+40 && mouse_y>yp+100 && mouse_y<yp+100+16)
{
if( Midi_Faders_Affectation_Type!=0)//config midi
{
Autostop.DrawOutline(CouleurBlind);
}
}

//SET cue IN
Rect SetLoopIN(Vec2D(xp+210,yp),Vec2D(20,20));
SetLoopIN.SetRoundness(4);


if(midi_show_flash_cueIntouch[numero]==1){SetLoopIN.Draw(CouleurBlind);midi_show_flash_cueIntouch[numero]=0;}

SetLoopIN.DrawOutline(CouleurLigne);
Line(Vec2D(xp+210,yp),Vec2D(xp+220,yp+10)).Draw(CouleurLigne);
Line(Vec2D(xp+230,yp),Vec2D(xp+220,yp+10)).Draw(CouleurLigne);
petitpetitchiffre.Print("in",xp+215,yp+17);

if(mouse_x>xp+210 && mouse_x<xp+240 && mouse_y>yp && mouse_y<yp+20)
{
if( Midi_Faders_Affectation_Type!=0)//config midi
{
SetLoopIN.DrawOutline(CouleurBlind);
}
}

//SET cue out
Rect SetLoopOut(Vec2D(xp+235,yp),Vec2D(20,20));
SetLoopOut.SetRoundness(4);

if(midi_show_flash_cueOuttouch[numero]==1){SetLoopOut.Draw(CouleurBlind);midi_show_flash_cueOuttouch[numero]=0;}
SetLoopOut.DrawOutline(CouleurLigne);

Line(Vec2D(xp+235,yp),Vec2D(xp+245,yp+10)).Draw(CouleurLigne);
Line(Vec2D(xp+255,yp),Vec2D(xp+245,yp+10)).Draw(CouleurLigne);
petitpetitchiffre.Print("out",xp+235,yp+17);
if(mouse_x>xp+235 && mouse_x<xp+255 && mouse_y>yp && mouse_y<yp+20)
{
if( Midi_Faders_Affectation_Type!=0)//config midi
{
SetLoopOut.DrawOutline(CouleurBlind);
}
}

//CUE ON OFF
Rect CueOn(Vec2D(xp+260,yp),Vec2D(20,20));
CueOn.SetRoundness(4);
if(player_is_onloopCue[numero]==1)
{
CueOn.Draw(CouleurBlind);
}  
petitpetitchiffre.Print("cue",xp+260,yp+12);
CueOn.DrawOutline(CouleurLigne);

if(mouse_x>xp+260 && mouse_x<xp+280 && mouse_y>yp && mouse_y<yp+20)
{

if( Midi_Faders_Affectation_Type!=0)//config midi
{
CueOn.DrawOutline(CouleurBlind);
}
}

char time_is_for_fileCueIn[64];
char time_is_for_fileCueOut[64];
sprintf(time_is_for_fileCueIn,"In: %.1f",(((float)player_seek_position[numero])/audio_rate[numero]));
petitpetitchiffre.Print(time_is_for_fileCueIn,xp+234,yp+40);
sprintf(time_is_for_fileCueOut,"Out: %.1f",(((float)player_loop_out_position[numero])/audio_rate[numero]));
petitpetitchiffre.Print(time_is_for_fileCueOut,xp+234,yp+50);

//Seek cue
Rect SeekCue(Vec2D(xp+210,yp+30),Vec2D(20,20));
SeekCue.SetRoundness(4);
SeekCue.DrawOutline(CouleurLigne);
if(midi_show_flash_cueSeektouch[numero]==1){SeekCue.Draw(CouleurBlind);midi_show_flash_cueSeektouch[numero]=0;}

Line(Vec2D(xp+226,yp+34),Vec2D(xp+226,yp+46)).Draw(CouleurLigne);
Line(Vec2D(xp+226,yp+34),Vec2D(xp+216,yp+40)).Draw(CouleurLigne);
Line(Vec2D(xp+216,yp+40),Vec2D(xp+226,yp+46)).Draw(CouleurLigne);
Line(Vec2D(xp+214,yp+34),Vec2D(xp+214,yp+46)).Draw(CouleurLigne);

if(mouse_x>xp+210 && mouse_x<xp+230 && mouse_y>yp+30 && mouse_y<yp+50)
{
if(Midi_Faders_Affectation_Type!=0)//config midi
{
SeekCue.DrawOutline(CouleurBlind);
}
}




//Pitch
Line(Vec2D(xp+130,yp+115),Vec2D(xp+257,yp+115)).Draw(CouleurLigne);//ligne horizontale
Line(Vec2D(xp+194,yp+105),Vec2D(xp+194,yp+125)).Draw(CouleurLigne);//barre 64
Line(Vec2D(xp+257,yp+110),Vec2D(xp+257,yp+115)).Draw(CouleurLigne);//barre 127
Rect PitchPos(Vec2D(xp+120+player_pitch[numero],yp+110),Vec2D(20,10));
PitchPos.SetRoundness(4);
PitchPos.Draw(CouleurBlind);
PitchPos.DrawOutline(CouleurLigne);
Rect PitchMidi(Vec2D(xp+130,yp+110),Vec2D(127,10));
sprintf(string_pitch,"Pitch %0.2f",show_pitch_value[numero]);
petitpetitchiffre.Print(string_pitch,(xp+90),(yp+107));
petitpetitchiffre.Print(ol::ToString(player_pitch[numero]),(xp+110),(yp+117));

if(mouse_x>xp+120+player_pitch[numero] && mouse_x<xp+150+player_pitch[numero] && mouse_y>yp+110 && mouse_y<yp+110+10)
{
if(Midi_Faders_Affectation_Type!=0)//config midi
{
PitchMidi.DrawOutline(CouleurBlind);
}
}

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

sprintf(string_pan,"Pan %0.2f",show_pan_value[numero]);

petitpetitchiffre.Print(string_pan,(xp+90),(yp+82));
petitpetitchiffre.Print(ol::ToString(player_pan[numero]),(xp+110),(yp+94));

if(mouse_x>xp+120+player_pan[numero] && mouse_x<xp+150+player_pan[numero] && mouse_y>yp+85 && mouse_y<yp+85+10)
{
if(Midi_Faders_Affectation_Type!=0)//config midi
{
PanMidi.DrawOutline(CouleurBlind);
}
}
raccrochage_midi_visuel_horizontal_audio (xp+130, yp+80, 624+numero, 127,10);
//les faders son
fader_niveau_son(xp+290,yp,numero);

return(0);   
}


int fenetre_audio(int xb,int yb)
{

Rect audio_backg(Vec2D(xb,yb), Vec2D( 600,150+(index_nbre_players_visibles*6*20)));
audio_backg.SetRoundness(15);
audio_backg.Draw(CouleurFond); 
audio_backg.SetLineWidth(epaisseur_ligne_fader*3);
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
if(mouse_x>xb+350 && mouse_x<xb+590 && mouse_y>yb+10 && mouse_y<yb+40 && window_focus_id==919)
{
AudioFolderis.DrawOutline(CouleurLigne);           
}

//////////////////////LISTE sons///////////////////////////////////////

Rect BackDeroule(Vec2D(xb+350,yb+45),Vec2D(240,(index_nbre_players_visibles*6*20)));
BackDeroule.SetRoundness(15);
BackDeroule.Draw(CouleurConfig.WithAlpha(0.7));

for (int y=1;y<(index_nbre_players_visibles*6);y++)
{
Rect OverAudioFile(Vec2D(xb+355,(yb+45+(y*20)-10)),Vec2D(200,20));
OverAudioFile.SetRoundness(7.5);
//affichage qui est selectionné
if(strcmp(audiofile_name,list_audio_files[audiofile_selected])==0 && (audiofile_selected==(y+line_audio)) )
{OverAudioFile.Draw(CouleurBlind);}

if(mouse_x>xb+355 && mouse_x<xb+355+150 && mouse_y>(yb+45+(y*20)-10) && mouse_y<(yb+45+(y*20)+5)&& window_focus_id==919)
{
OverAudioFile.DrawOutline(CouleurLigne);                  
}


petitpetitchiffre.Print(ol::ToString(line_audio+y),xb+360,yb+45+(y*20)); 
Canvas::SetClipping( xb+355,yb+45+(y*20)-10,230,20);
petitpetitchiffre.Print(list_audio_files[line_audio+y],xb+370,yb+45+(y*20));   
Canvas::DisableClipping();
}

//////////////////UP DOWN LINE IMPORT/////////////////////
Circle LineUp(Vec2D(xb+570,yb+65),12);
LineUp.Draw(CouleurFond);   
Circle LineDown(Vec2D(xb+570,yb+(index_nbre_players_visibles*6* 20)),12);
LineDown.Draw(CouleurFond);

if(mouse_x>xb+570-12 && mouse_x<xb+570+12 && window_focus_id==919 && window_focus_id==919)
{
if(mouse_y>yb+65-12 && mouse_y<yb+65+12)
{                      
LineUp.Draw(CouleurSurvol);           
}
if(mouse_y>yb+(index_nbre_players_visibles*6* 20)-7 && mouse_y<yb+(index_nbre_players_visibles*6* 20)+5)
{ 
LineDown.Draw(CouleurSurvol);             
}                                                
}
petitchiffre.Print("-",xb+566,yb+70);
petitchiffre.Print("+",xb+566,yb+(index_nbre_players_visibles*6* 20)+5);
LineUp.DrawOutline(CouleurLigne);
LineDown.DrawOutline(CouleurLigne);

///RESCAN FOLDER
    Rect AudioRescanDriver( Vec2D(xb+230,yb+10), Vec2D ( 50,20));
    AudioRescanDriver.SetRoundness(7.5); 
    AudioRescanDriver.DrawOutline(CouleurLigne);

    if(mouse_x>xb+230 && mouse_x<xb+230+50 && mouse_y>yb+10 && mouse_y<yb+10+20 && window_focus_id==919)
    {
    AudioRescanDriver.Draw(CouleurSurvol);            
    }
    petitpetitchiffre.Print("rescan!",xb+235,yb+23);
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
Rect AffectA(Vec2D(xb+440,yb+55+(index_nbre_players_visibles*6*20)),Vec2D(140,20));
AffectA.SetRoundness(5);
if(index_affect_audio_to_dock==1){AffectA.Draw(CouleurFader);}
AffectA.DrawOutline(CouleurLigne);  
petitchiffre.Print("AFFECT TO DOCK",xb+460,yb+55+15+(index_nbre_players_visibles*6*20));


//petitchiffre.Print("Volume",xb+400,yb+575);
petitchiffre.Print("Volume",xb+400,yb+100+(index_nbre_players_visibles*6*20));
//petitchiffre.Print("Pan",xb+400,yb+595);
petitchiffre.Print("Pan",xb+400,yb+120+(index_nbre_players_visibles*6*20));
//petitchiffre.Print("Pitch",xb+400,yb+615);
petitchiffre.Print("Pitch",xb+400,yb+140+(index_nbre_players_visibles*6*20));

for(int pl=0;pl<index_nbre_players_visibles;pl++)
{
for (int co=0;co<3;co++)
{
 Rect AffectAudioType(Vec2D(xb+460+(pl*30),yb+90+(index_nbre_players_visibles*6*20)+(co*20)),Vec2D(20,15));
 AffectAudioType.SetRoundness(3);
 if(player_to_affect_to_dock==pl && audio_type_for_dock_affectation_is==co){AffectAudioType.Draw(CouleurFader);}
 AffectAudioType.DrawOutline(CouleurLigne);  
}
 petitpetitchiffre.Print(ol::ToString((pl+1)),xb+467+(pl*30),yb+55+30+(index_nbre_players_visibles*6*20));       
}

return(0);    
}
