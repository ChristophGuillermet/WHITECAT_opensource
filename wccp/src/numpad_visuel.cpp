/*-------------------------------------------------------------------------------------------------------------
                                 |
          CWWWWWWWW              | Copyright (C) 2009-2013  Christoph Guillermet
       WWWWWWWWWWWWWWW           |
     WWWWWWWWWWWWWWWWWWW         | This file is part of White Cat.
    WWWWWWWWWWWWWWWWWCWWWW       |
   WWWWWWWWWWWWWWWWW tWWWWW      | White Cat is free software: you can redistribute it and/or modify
  WWWW   WWWWWWWWWW  tWWWWWW     | it under the terms of the GNU General Public License as published by
 WWWWWt              tWWWWWWa    | the Free Software Foundation, either version 2 of the License, or
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

* \file numpad_visuel.cpp
* \brief {GUI fonction for the numerical pad window}
* \author Christoph Guillermet
* \version {0.8.5.2}
* \date {19/02/2014}

 White Cat {- categorie} {- sous categorie {- sous categorie}}

*   Fonction GUI pour la fen�tre du pad num�rique dans whitecat
*
*   GUI fonction for the numerical pad window in whitecat
*
**/

int rafraichissement_padwheel()
{
angle_snap_pad=(((PIknob*2)+angle_correction_wheel) / 128)*midi_levels[664];
   pad_vx = cos(angle_snap_pad)* rayon_wheel_level;
   pad_vy = sin(angle_snap_pad)* rayon_wheel_level;
   position_curseur_pad_x= xnum_window+10+260+pad_vx;
   position_curseur_pad_y=ynum_window+35+pad_vy ;
return(0);
}


int level_wheel (int xw,int yw, int rayon_k, float angle_correction)
{
ol::Circle (ol::Vec2D(xw,yw), rayon_k+5).DrawOutline(CouleurLigne);
ol::Circle (ol::Vec2D(xw,yw), rayon_k-5).DrawOutline(CouleurLigne);
ol::Circle(ol::Vec2D(position_curseur_pad_x,position_curseur_pad_y),5).Draw(CouleurBlind);

int poucentmidi=(int)((((float)midi_levels[664])/127)*100);
petitpetitchiffrerouge.Print(ol::ToString(poucentmidi),xw-5,yw+2);
petitpetitchiffre.Print(ol::ToString(midi_levels[664]),xw-5,yw+12);

//AFFECTATION MIDI
if( Midi_Faders_Affectation_Type!=0)//config midi
  {
  if(mouse_x>xw-rayon_k-5 && mouse_x<xw+rayon_k+5 && mouse_y>yw-rayon_k-5 && mouse_y<yw+rayon_k+5)
  {
  ol::Circle (ol::Vec2D(xw,yw), rayon_k+5).DrawOutline(CouleurBlind);
  if(mouse_button==1 && mouse_released==0)
  {
  ol::Circle (ol::Vec2D(xw,yw), rayon_k+5).Draw(CouleurBlind);
  }
  }
}

//toggle mode Relativ / absolute
ol::Rect WheelMode(ol::Vec2D(xw+rayon_k+10,yw-rayon_k),ol::Vec2D(15,15));
WheelMode.SetRoundness(3);

switch(wheellevel_absolutemode)
{
case 0:
WheelMode.Draw(CouleurLock);
petitchiffre.Print("R",xw+rayon_k+13,yw-rayon_k+12);
break;
case 1:
WheelMode.Draw(CouleurSurvol);
petitchiffre.Print("A",xw+rayon_k+13,yw-rayon_k+12);
break;
}
WheelMode.DrawOutline(CouleurLigne);

//midi out
ol::Line (ol::Vec2D(xw+rayon_k,yw+rayon_k-3),ol::Vec2D(xw+rayon_k+7,yw+rayon_k+7)).Draw(CouleurLigne);
ol::Circle MidiOutW(ol::Vec2D(xw+rayon_k+10,yw+rayon_k+10),7);
if(midi_send_out[664]==1){MidiOutW.Draw(CouleurBlind);}

MidiOutW.DrawOutline(CouleurLigne);

return(0);
}


int visual_numeric_pad (int x_num, int y_num)
{


//background window
ol::Rect NumBackground(ol::Vec2D(x_num, y_num ), ol::Vec2D( 480,300));
NumBackground.SetRoundness(15);
NumBackground.SetLineWidth(epaisseur_ligne_fader*3);
NumBackground.Draw(CouleurFond);
if(window_focus_id==903)
{
NumBackground.DrawOutline(CouleurFader);
}
else
{
NumBackground.DrawOutline(CouleurLigne);
}
level_wheel(x_num+270,y_num+35,rayon_wheel_level,angle_correction_wheel);
//premiere ligne 1 2 3
for (int loi=0;loi<3;loi++)
{
ol::Rect MyNum(ol::Vec2D(x_num+10+(70*loi),y_num+16),ol::Vec2D(60,60));
MyNum.SetRoundness(7.5);
MyNum.SetLineWidth(epaisseur_ligne_fader);
if (show_numpad_midi[1+loi]==1){MyNum.Draw(CouleurSurvol);show_numpad_midi[1+loi]=0;}
MyNum.DrawOutline(CouleurLigne);
if( Midi_Faders_Affectation_Type!=0)//config midi
{
  if(mouse_x>x_num+10+(70*loi) && mouse_x<x_num+70+(70*loi) && mouse_y>y_num+16 && mouse_y<y_num+76)
  {
  MyNum.DrawOutline(CouleurBlind);
  }
}
}

//deuxieme ligne 4 5 6
for (int loi=0;loi<3;loi++)
{
ol::Rect MyNum(ol::Vec2D(x_num+10+(70*loi),y_num+86),ol::Vec2D(60,60));
MyNum.SetRoundness(7.5);
MyNum.SetLineWidth(epaisseur_ligne_fader);
if (show_numpad_midi[4+loi]==1){MyNum.Draw(CouleurSurvol);show_numpad_midi[4+loi]=0;}
MyNum.DrawOutline(CouleurLigne);
if( Midi_Faders_Affectation_Type!=0)//config midi
{
  if(mouse_x>x_num+10+(70*loi) && mouse_x<x_num+70+(70*loi) && mouse_y>y_num+86 && mouse_y<y_num+146)
  {
  MyNum.DrawOutline(CouleurBlind);
  }
}
}

//troisieme ligne 7 8 9
for (int loi=0;loi<3;loi++)
{
ol::Rect MyNum(ol::Vec2D(x_num+10+(70*loi),y_num+156),ol::Vec2D(60,60));
MyNum.SetRoundness(7.5);
MyNum.SetLineWidth(epaisseur_ligne_fader);
if (show_numpad_midi[7+loi]==1){MyNum.Draw(CouleurSurvol);show_numpad_midi[7+loi]=0;}
MyNum.DrawOutline(CouleurLigne);
if(Midi_Faders_Affectation_Type!=0)//config midi
{
  if(mouse_x>x_num+10+(70*loi) && mouse_x<x_num+70+(70*loi) && mouse_y>y_num+156 && mouse_y<y_num+216)
  {
  MyNum.DrawOutline(CouleurBlind);
  }
}
}
//quatrieme ligne 0 point Clear
for (int loi=0;loi<3;loi++)
{
ol::Rect MyNum(ol::Vec2D(x_num+10+(70*loi),y_num+226),ol::Vec2D(60,60));
MyNum.SetRoundness(7.5);
MyNum.SetLineWidth(epaisseur_ligne_fader);
MyNum.DrawOutline(CouleurLigne);
switch(loi)
{
case 0:
if (show_numpad_midi[0]==1){MyNum.Draw(CouleurSurvol);show_numpad_midi[0]=0;}
break;
case 1:
if (show_numpad_midi[10]==1){MyNum.Draw(CouleurSurvol);show_numpad_midi[10]=0;}
break;
if (show_numpad_midi[11]==1){MyNum.Draw(CouleurSurvol);show_numpad_midi[11]=0;}
case 2:
break;
}
if(Midi_Faders_Affectation_Type!=0)//config midi
{
  if(mouse_x>x_num+10+(70*loi) && mouse_x<x_num+70+(70*loi) && mouse_y>y_num+226 && mouse_y<y_num+286)
  {
  MyNum.DrawOutline(CouleurBlind);
  }
}
}


//UP
ol::Rect UPT(ol::Vec2D(x_num+220,y_num+86),ol::Vec2D(100,60));
UPT.SetRoundness(7.5);
UPT.SetLineWidth(epaisseur_ligne_fader);
if (show_numpad_midi[18]==1){UPT.Draw(CouleurSurvol);show_numpad_midi[18]=0;}
UPT.DrawOutline(CouleurLigne);
if( Midi_Faders_Affectation_Type!=0)//config midi
{
  if(mouse_x>x_num+220 && mouse_x<x_num+320 && mouse_y>y_num+86 && mouse_y<y_num+146)
  {
  UPT.DrawOutline(CouleurBlind);
  }
}
//
//DOWN
ol::Rect DOWNT(ol::Vec2D(x_num+220,y_num+156),ol::Vec2D(100,60));
DOWNT.SetRoundness(7.5);
DOWNT.SetLineWidth(epaisseur_ligne_fader);
if (show_numpad_midi[19]==1){DOWNT.Draw(CouleurSurvol);show_numpad_midi[19]=0;}
DOWNT.DrawOutline(CouleurLigne);
if(Midi_Faders_Affectation_Type!=0)//config midi
{
  if(mouse_x>x_num+220 && mouse_x<x_num+320 && mouse_y>y_num+156 && mouse_y<y_num+216)
  {
  DOWNT.DrawOutline(CouleurBlind);
  }
}
//ENTER
ol::Rect ENTERT(ol::Vec2D(x_num+220,y_num+226),ol::Vec2D(100,60));
ENTERT.SetRoundness(7.5);
ENTERT.SetLineWidth(epaisseur_ligne_fader);
if (show_numpad_midi[15]==1){ENTERT.Draw(CouleurSurvol);show_numpad_midi[15]=0;}
ENTERT.DrawOutline(CouleurLigne);
if( Midi_Faders_Affectation_Type!=0)//config midi
{
  if(mouse_x>x_num+220 && mouse_x<x_num+320 && mouse_y>y_num+226 && mouse_y<y_num+286)
  {
  ENTERT.DrawOutline(CouleurBlind);
  }
}
//+
ol::Rect MyPlus(ol::Vec2D(x_num+330,y_num+86),ol::Vec2D(60,60));
MyPlus.SetRoundness(7.5);
MyPlus.SetLineWidth(epaisseur_ligne_fader);
if (show_numpad_midi[16]==1){MyPlus.Draw(CouleurSurvol);show_numpad_midi[16]=0;}
MyPlus.DrawOutline(CouleurLigne);
if( Midi_Faders_Affectation_Type!=0)//config midi
{
  if(mouse_x>x_num+330 && mouse_x<x_num+390 && mouse_y>y_num+86 && mouse_y<y_num+146)
  {
  MyPlus.DrawOutline(CouleurBlind);
  }
}
//-
ol::Rect MyMinus(ol::Vec2D(x_num+330,y_num+156),ol::Vec2D(60,60));
MyMinus.SetRoundness(7.5);
MyMinus.SetLineWidth(epaisseur_ligne_fader);
if (show_numpad_midi[17]==1){MyMinus.Draw(CouleurSurvol);show_numpad_midi[17]=0;}
MyMinus.DrawOutline(CouleurLigne);
if( Midi_Faders_Affectation_Type!=0)//config midi
{
  if(mouse_x>x_num+330 && mouse_x<x_num+390 && mouse_y>y_num+156 && mouse_y<y_num+216)
  {
  MyMinus.DrawOutline(CouleurBlind);
  }
}

doom.Print("1",x_num+30, y_num+55);
doom.Print("2",x_num+100, y_num+55);
doom.Print("3",x_num+170, y_num+55);
doom.Print("4",x_num+30, y_num+125);
doom.Print("5",x_num+100, y_num+125);
doom.Print("6",x_num+170, y_num+125);
doom.Print("7",x_num+30, y_num+195);
doom.Print("8",x_num+100, y_num+195);
doom.Print("9",x_num+170, y_num+195);
doom.Print("0",x_num+30, y_num+265);
doom.Print(".",x_num+105, y_num+265);
doom.Print("C",x_num+165, y_num+265);
doom.Print("up",x_num+250, y_num+125);
doom.Print("down",x_num+230, y_num+195);
doom.Print("AT",x_num+245, y_num+265);
doom.Print("+",x_num+350, y_num+125);
doom.Print("-",x_num+350, y_num+195);
return(0);
}

