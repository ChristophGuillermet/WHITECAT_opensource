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

* \file sequentiel_7_visu.cpp
* \brief {GUI fonctions for the cuelist}
* \author Christoph Guillermet
* \version {0.8.5.2}
* \date {19/02/2014}

 White Cat {- categorie} {- sous categorie {- sous categorie}}

*   GUI fonctions pour la fenêtre des memoires
*
*   GUI fonctions for the cuelist
*
**/

int refresh_vision_memories( int x_seq, int y_seq)
{

ol::Rect ExclueMem(ol::Vec2D(x_seq,y_seq),ol::Vec2D(10,30));


if(window_focus_id==W_SEQUENCIEL && mouse_x>x_seq && mouse_x<x_seq+450 && mouse_y> y_seq and mouse_y<y_seq+hauteur_globale_sequenciel ) // affichage survol des cases souris
{
ol::Rect DelayOver (ol::Vec2D(x_seq, y_seq ), ol::Vec2D( 40,20));
DelayOver.SetRoundness(7.5);
ol::Rect InOutOver (ol::Vec2D(x_seq, y_seq ), ol::Vec2D( 60,20));
InOutOver.SetRoundness(7.5);
ol::Rect MemOver (ol::Vec2D(x_seq, y_seq), ol::Vec2D( 60,20));
MemOver.SetRoundness(7.5);
ol::Rect LinkOver (ol::Vec2D(x_seq, y_seq ), ol::Vec2D( 35,20));
LinkOver.SetRoundness(7.5);
ol::Rect bangOver (ol::Vec2D(x_seq, y_seq), ol::Vec2D( 40,20));
bangOver.SetRoundness(7.5);
ol::Rect Gpl1Over (ol::Vec2D(x_seq, y_seq), ol::Vec2D( 30,20));
Gpl1Over.SetRoundness(7.5);
ol::Rect TextOver (ol::Vec2D(x_seq, y_seq), ol::Vec2D( 160,15));
TextOver.SetRoundness(5);



//MANIPULATIONS MEM BEFORE ONE
if(mouse_y>y_seq+80 && mouse_y< y_seq+100)
{
if(mouse_x> x_seq+180 && mouse_x<x_seq+215)//LINKS
{
LinkOver.MoveTo (ol::Vec2D(x_seq+180,y_seq+80));
LinkOver.DrawOutline(CouleurLigne);
}
else if(mouse_x> x_seq+225 && mouse_x<x_seq+265)//BANGER
{
bangOver.MoveTo (ol::Vec2D(x_seq+225, y_seq+80));
bangOver.DrawOutline(CouleurLigne);
}
else if(mouse_x>x_seq+270 && mouse_x<x_seq+300)//GRID PLAYER
{
Gpl1Over.MoveTo (ol::Vec2D(x_seq+275, y_seq+80));
Gpl1Over.DrawOutline(CouleurLigne);
}
else if(mouse_x> x_seq+310 && mouse_x<x_seq+470)//TEXT DECRIPTION
{
if(mouse_y> y_seq+80 && mouse_y < y_seq+95)
{
TextOver.MoveTo (ol::Vec2D(x_seq+310, y_seq+80));
}
else
{
TextOver.MoveTo(ol::Vec2D(x_seq+310, y_seq+95));
}
TextOver.DrawOutline(CouleurLigne);
}
}



//Manipulation en stage
else if(mouse_y>y_seq+110 && mouse_y<y_seq+140)
{
if(mouse_x> x_seq+10 && mouse_x<x_seq+50)//DELAYS
{
DelayOver.MoveTo (ol::Vec2D(x_seq+10, y_seq+115 ));
DelayOver.DrawOutline(CouleurLigne);
}
else if(mouse_x> x_seq+50 && mouse_x<x_seq+110)//IN OUT
{
InOutOver.MoveTo(ol::Vec2D(x_seq+50, y_seq+115 ));
InOutOver.DrawOutline(CouleurLigne);
}
else if(mouse_x> x_seq+110 && mouse_x<x_seq+170)//MEMS
{
MemOver.MoveTo (ol::Vec2D(x_seq+110, y_seq+115 ));
MemOver.DrawOutline(CouleurLigne);
}
else if(mouse_x> x_seq+180 && mouse_x<x_seq+215)//LINKS
{
LinkOver.MoveTo (ol::Vec2D(x_seq+180, y_seq+115 ));
LinkOver.DrawOutline(CouleurLigne);
}
else if(mouse_x> x_seq+225 && mouse_x<x_seq+265)//BANGER
{
bangOver.MoveTo (ol::Vec2D(x_seq+225, y_seq+115 ));
bangOver.DrawOutline(CouleurLigne);
}
else if(mouse_x>x_seq+270 && mouse_x<x_seq+300)//GRID PLAYER
{
Gpl1Over.MoveTo (ol::Vec2D(x_seq+275, y_seq+115 ));
Gpl1Over.DrawOutline(CouleurLigne);
}
else if(mouse_x> x_seq+310 && mouse_x<x_seq+470)//TEXT DECRIPTION
{
if(mouse_y> y_seq+110 && mouse_y < y_seq+125)
{
TextOver.MoveTo (ol::Vec2D(x_seq+310, y_seq+110));
}
else
{
TextOver.MoveTo(ol::Vec2D(x_seq+310, y_seq+125));
}
TextOver.DrawOutline(CouleurLigne);
}
}



//Manipulation en preset
else if(mouse_y>y_seq+140 && mouse_y<y_seq+170)
{
if(mouse_x> x_seq+10 && mouse_x<x_seq+50)//DELAYS
{
DelayOver.MoveTo (ol::Vec2D(x_seq+10, y_seq+145 ));
DelayOver.DrawOutline(CouleurLigne);
}
else if(mouse_x> x_seq+50 && mouse_x<x_seq+110)//IN OUT
{
InOutOver.MoveTo  (ol::Vec2D(x_seq+50, y_seq+145));
InOutOver.DrawOutline(CouleurLigne);
}
else if(mouse_x> x_seq+110 && mouse_x<x_seq+170)//MEMS
{
MemOver.MoveTo  (ol::Vec2D(x_seq+110, y_seq+145));
MemOver.DrawOutline(CouleurLigne);
}
else if(mouse_x> x_seq+180 && mouse_x<x_seq+215)//LINKS
{
LinkOver.MoveTo  (ol::Vec2D(x_seq+180, y_seq+145 ));
LinkOver.DrawOutline(CouleurLigne);
}
else if(mouse_x> x_seq+225 && mouse_x<x_seq+265)//BANGER
{
bangOver.MoveTo  (ol::Vec2D(x_seq+225, y_seq+145 ));
bangOver.DrawOutline(CouleurLigne);
}
else if(mouse_x>x_seq+270 && mouse_x<x_seq+300)//GRID PLAYER
{
Gpl1Over.MoveTo  (ol::Vec2D(x_seq+275, y_seq+145 ));
Gpl1Over.DrawOutline(CouleurLigne);
}
else if(mouse_x> x_seq+310 && mouse_x<x_seq+470)//TEXT DECRIPTION
{
if(mouse_y> y_seq+140 && mouse_y < y_seq+155)
{
TextOver.MoveTo  (ol::Vec2D(x_seq+310, y_seq+140));
}
else
{
TextOver.MoveTo(ol::Vec2D(x_seq+310, y_seq+155));
}
TextOver.DrawOutline(CouleurLigne);
}
}




//Next Mems
for(int i=0;i<nbre_memoires_visualisables_en_preset;i++)
{
///ACTIONS
if(mouse_y>y_seq+145+(35*i)  && mouse_y< y_seq+175+(35*i))
{

if(mouse_x> x_seq+110 && mouse_x<x_seq+170)//MEMS
{
MemOver.MoveTo (ol::Vec2D(x_seq+110,y_seq+145+(35*i)));
MemOver.DrawOutline(CouleurLigne);
}
else if(mouse_x> x_seq+180 && mouse_x<x_seq+215)//LINKS
{
LinkOver.MoveTo (ol::Vec2D(x_seq+180,y_seq+145+(35*i)));
LinkOver.DrawOutline(CouleurLigne);
}
else if(mouse_x> x_seq+225 && mouse_x<x_seq+265)//BANGER
{
bangOver.MoveTo (ol::Vec2D(x_seq+225, y_seq+145+(35*i) ));
bangOver.DrawOutline(CouleurLigne);
}
else if(mouse_x>x_seq+270 && mouse_x<x_seq+300)//GRID PLAYER
{
Gpl1Over.MoveTo (ol::Vec2D(x_seq+275, y_seq+145+(35*i) ));
Gpl1Over.DrawOutline(CouleurLigne);
}
else if(mouse_x> x_seq+310 && mouse_x<x_seq+470)//TEXT DECRIPTION
{
if(mouse_y> y_seq+140+(35*i) && mouse_y < y_seq+155+(35*i) )
{
TextOver.MoveTo (ol::Vec2D(x_seq+310, y_seq+140+(35*i)));
}
else
{
TextOver.MoveTo(ol::Vec2D(x_seq+310, y_seq+155+(35*i)));
}
TextOver.DrawOutline(CouleurLigne);
}

}
}

}


//fin si window focus id== seq




///////////////////AFFICHAGES////////////////////////////////////////////////////

//Mem excluded from sequence
/*if(index_is_saving==0 && init_done==1 && index_writing_curve==0 && index_quit==0)
{

ExclueMem.MoveTo(ol::Vec2D(x_seq+170,y_seq+140));
ExclueMem.Draw(CouleurYellow.WithAlpha(alpha_blinker*(MemoiresExclues[position_preset])));
}*/

//mem before one
if (MemoiresExistantes[mem_before_one]==1)
{
//sprintf(string_mem_before_one,"%d.%d",mem_before_one/10, mem_before_one%10);
ExclueMem.MoveTo(ol::Vec2D(x_seq+170,y_seq+80));
ExclueMem.Draw(CouleurYellow.WithAlpha(alpha_blinker*(MemoiresExclues[mem_before_one])));

neuro.Print(string_mem_before_one,  x_seq+115, y_seq+100);
//banger
if(Banger_Memoire[mem_before_one]!=0)//dout
{
petitchiffre.Print(ol::ToString(Banger_Memoire[mem_before_one]),x_seq+235,y_seq+100);
}
//griplayer
if(set_from_seq_gridplayer1_next_step[mem_before_one]!=-1)
{
petitchiffre.Print(ol::ToString(set_from_seq_gridplayer1_next_step[mem_before_one]+1),x_seq+280,y_seq+100);
}
if(Times_Memoires[position_onstage][0]>0.0)
{
petitpetitchiffre.Print(string_time_memonstage[0], x_seq+20,y_seq+90);
}
if(Times_Memoires[position_onstage][2]>0.0)//dIn
{

petitpetitchiffrerouge.Print(string_time_memonstage[2], x_seq+20,y_seq+100);
}
//in
petitpetitchiffre.Print(string_time_memonstage[3], x_seq+70, y_seq+90);
//out
petitpetitchiffrerouge.Print(string_time_memonstage[1], x_seq+70, y_seq+100);
petitpetitchiffre.Print(descriptif_memoires[mem_before_one], x_seq+320, y_seq+90);
petitpetitchiffre.Print(annotation_memoires[mem_before_one], x_seq+320, y_seq+105);
if(Links_Memoires[mem_before_one]==1)
{
ol::Line(ol::Vec2D(x_seq+190,y_seq+85),ol::Vec2D(x_seq+200,y_seq+85)).Draw(CouleurLigne);
ol::Line(ol::Vec2D(x_seq+200,y_seq+85),ol::Vec2D(x_seq+200,y_seq+100)).Draw(CouleurLigne);
ol::Line(ol::Vec2D(x_seq+200,y_seq+100),ol::Vec2D(x_seq+195,y_seq+95)).Draw(CouleurLigne);
ol::Line(ol::Vec2D(x_seq+200,y_seq+100),ol::Vec2D(x_seq+205,y_seq+95)).Draw(CouleurLigne);
}
}

//mem on stage
if (MemoiresExistantes[position_onstage]==1)
{
neuro.Print(string_mem_onstage,  x_seq+115, y_seq+100+ 30);

ExclueMem.MoveTo(ol::Vec2D(x_seq+170,y_seq+110));
ExclueMem.Draw(CouleurYellow.WithAlpha(alpha_blinker*(MemoiresExclues[position_onstage])));

petitpetitchiffre.Print(descriptif_memoires[position_onstage], x_seq+320, y_seq+120);
petitpetitchiffre.Print(annotation_memoires[position_onstage], x_seq+320, y_seq+135);
//banger
if(Banger_Memoire[position_onstage]!=0)
{
petitchiffre.Print(ol::ToString(Banger_Memoire[position_onstage]),x_seq+235,y_seq+130);
}
//griplayer
if(set_from_seq_gridplayer1_next_step[position_onstage]!=-1)
{
petitchiffre.Print(ol::ToString(set_from_seq_gridplayer1_next_step[position_onstage]+1),x_seq+280,y_seq+130);
}
if(Links_Memoires[position_onstage]==1)
{
ol::Line(ol::Vec2D(x_seq+190,y_seq+115+5),ol::Vec2D(x_seq+200,y_seq+120)).Draw(CouleurLigne);
ol::Line(ol::Vec2D(x_seq+200,y_seq+115+5),ol::Vec2D(x_seq+200,y_seq+135)).Draw(CouleurLigne);
ol::Line(ol::Vec2D(x_seq+200,y_seq+130+5),ol::Vec2D(x_seq+195,y_seq+130)).Draw(CouleurLigne);
ol::Line(ol::Vec2D(x_seq+200,y_seq+130+5),ol::Vec2D(x_seq+205,y_seq+130)).Draw(CouleurLigne);
}
}
//mem on preset
if (MemoiresExistantes[position_preset]==1)
{
ExclueMem.MoveTo(ol::Vec2D(x_seq+170,y_seq+140));
ExclueMem.Draw(CouleurYellow.WithAlpha(alpha_blinker*(MemoiresExclues[position_preset])));
neuro.Print(string_mem_preset,  x_seq+115, y_seq+160);
petitpetitchiffre.Print(descriptif_memoires[position_preset], x_seq+320, y_seq+150);
petitpetitchiffre.Print(annotation_memoires[position_preset], x_seq+320, y_seq+165);
petitpetitchiffre.Print(cross_din, x_seq+20, y_seq+160);
petitchiffre.Print(cross_in, x_seq+55, y_seq+160);
petitchiffre.Print(cross_out, x_seq+55, y_seq+130);
petitpetitchiffre.Print(cross_dout, x_seq+20, y_seq+130);
//banger
if(Banger_Memoire[position_preset]!=0)
{
petitchiffre.Print(ol::ToString(Banger_Memoire[position_preset]),x_seq+235,y_seq+160);
}
//griplayer
if(set_from_seq_gridplayer1_next_step[position_preset]!=-1)
{
petitchiffre.Print(ol::ToString(set_from_seq_gridplayer1_next_step[position_preset]+1),x_seq+280,y_seq+160);
}
if(Links_Memoires[position_preset]==1)
{
ol::Line(ol::Vec2D(x_seq+190,y_seq+150),ol::Vec2D(x_seq+200,y_seq+150)).Draw(CouleurLigne);
ol::Line(ol::Vec2D(x_seq+200,y_seq+150),ol::Vec2D(x_seq+200,y_seq+165)).Draw(CouleurLigne);
ol::Line(ol::Vec2D(x_seq+200,y_seq+165),ol::Vec2D(x_seq+195,y_seq+160)).Draw(CouleurLigne);
ol::Line(ol::Vec2D(x_seq+200,y_seq+165),ol::Vec2D(x_seq+205,y_seq+160)).Draw(CouleurLigne);
}
}





//LES MEMOIRES SUIVANTES USER DEFINED
int index_nbre_mem_visues=0;
int last_mem_visue=0;
bool turn=0;
for(int memsearch=position_preset+1; memsearch<10000;memsearch++)
{
if(memsearch>=9999 && turn==0){memsearch=0;turn=1;}
if(MemoiresExistantes[memsearch]==1 && index_nbre_mem_visues<nbre_memoires_visualisables_en_preset)
{
index_nbre_mem_visues++;

//mems
sprintf(string_next_mem,"%d.%d",memsearch/10, memsearch%10);
neuro.Print(string_next_mem,  x_seq+115, y_seq+160+(35*index_nbre_mem_visues));
petitpetitchiffre.Print(descriptif_memoires[memsearch], x_seq+320, y_seq+150+(35*index_nbre_mem_visues));
petitpetitchiffre.Print(annotation_memoires[memsearch], x_seq+320, y_seq+165+(35*index_nbre_mem_visues));

ol::Line(ol::Vec2D(x_seq+10, y_seq+100+70+(35*index_nbre_mem_visues)),ol::Vec2D(x_seq+450, y_seq+170+(35*index_nbre_mem_visues))).Draw(CouleurLigne.WithAlpha(0.5));

ExclueMem.MoveTo(ol::Vec2D(x_seq+170,y_seq+135+(35*index_nbre_mem_visues)));
ExclueMem.Draw(CouleurYellow.WithAlpha(alpha_blinker*(MemoiresExclues[memsearch])));


//din dout
if(Times_Memoires[memsearch][2]>0.0)
{
petitpetitchiffre.Print(string_time_mem8after[index_nbre_mem_visues][2], x_seq+20, y_seq+150+(35*index_nbre_mem_visues));
}
if(Times_Memoires[memsearch][0]>0.0)//dout
{
petitpetitchiffrerouge.Print(string_time_mem8after[index_nbre_mem_visues][0], x_seq+20, y_seq+165+(35*index_nbre_mem_visues));
}
//in out
//in
petitpetitchiffre.Print(string_time_mem8after[index_nbre_mem_visues][3], x_seq+70, y_seq+150+(35*index_nbre_mem_visues));
//out
petitpetitchiffrerouge.Print(string_time_mem8after[index_nbre_mem_visues][1], x_seq+70, y_seq+165+(35*index_nbre_mem_visues));
//banger
if(Banger_Memoire[memsearch]!=0)
{
petitchiffre.Print(ol::ToString(Banger_Memoire[memsearch]),x_seq+235,y_seq+160+(35*index_nbre_mem_visues));
}
//griplayer
if(set_from_seq_gridplayer1_next_step[memsearch]!=-1)
{
petitchiffre.Print(ol::ToString(set_from_seq_gridplayer1_next_step[memsearch]+1),x_seq+280,y_seq+160+(35*index_nbre_mem_visues));
}

if(Links_Memoires[memsearch]==1)
{
ol::Line(ol::Vec2D(x_seq+190,y_seq+145+(35*index_nbre_mem_visues)),ol::Vec2D(x_seq+200,y_seq+145+(35*index_nbre_mem_visues))).Draw(CouleurLigne);
ol::Line(ol::Vec2D(x_seq+200,y_seq+145+(35*index_nbre_mem_visues)),ol::Vec2D(x_seq+200,y_seq+160+(35*index_nbre_mem_visues))).Draw(CouleurLigne);
ol::Line(ol::Vec2D(x_seq+200,y_seq+160+(35*index_nbre_mem_visues)),ol::Vec2D(x_seq+195,y_seq+155+(35*index_nbre_mem_visues))).Draw(CouleurLigne);
ol::Line(ol::Vec2D(x_seq+200,y_seq+160+(35*index_nbre_mem_visues)),ol::Vec2D(x_seq+205,y_seq+155+(35*index_nbre_mem_visues))).Draw(CouleurLigne);
}

if(index_nbre_mem_visues>=nbre_memoires_visualisables_en_preset){break;}
}
}


return(0);
}


int x1_x2(int x_seq,int y_seq)
{
ol::Rect FaderX1( ol::Vec2D( x_seq+480,y_seq+80), ol::Vec2D( 45,255));//box du fader
FaderX1.SetRoundness(15);
FaderX1.SetLineWidth(epaisseur_ligne_fader);
ol::Rect FaderNiveauX1( ol::Vec2D(  x_seq+480,((y_seq+335) - niveauX1)), ol::Vec2D( 45,niveauX1+2));//niveau fader
FaderNiveauX1.SetRoundness(15);
ol::Rect FaderX2( ol::Vec2D( x_seq+580,y_seq+80), ol::Vec2D( 45,255));//box du fader
FaderX2.SetRoundness(15);
FaderX2.SetLineWidth(epaisseur_ligne_fader);
ol::Rect FaderNiveauX2( ol::Vec2D(  x_seq+580,((y_seq+335-niveauX2))), ol::Vec2D( 45,niveauX2));//niveau fader
FaderNiveauX2.SetRoundness(15);
FaderNiveauX1.Draw(CouleurNiveau.WithAlpha(0.5));
FaderX1.DrawOutline(CouleurLigne);
FaderNiveauX2.Draw(CouleurSurvol);
FaderX2.DrawOutline(CouleurLigne);

raccrochage_midi_visuel_vertical_dmx ( x_seq+480, (y_seq+80), 491, 45, 255);
raccrochage_midi_visuel_vertical_dmx_inverted ( x_seq+580, (y_seq+80), 492, 45, 255);
/////////////////////////////////////CROSSFADE MANUEL/////////////////////////////

/////////////////////////////////////////////////////////////////
if(index_go==1 || index_pause==1)
{
if(crossfade_time_delay_out>(actual_time-crossfade_start_time) )
{FaderX1.Draw(CouleurBlind.WithAlpha(alpha_blinker));}
if(crossfade_time_delay_in>(actual_time-crossfade_start_time) )
{FaderX2.Draw(CouleurBlind.WithAlpha(alpha_blinker));}
}
if(dmx_view==1){
neuro.Print(ol::ToString(niveauX1),x_seq+485,y_seq+70);
neuro.Print(ol::ToString(niveauX2),x_seq+590,y_seq+70);
}
else
{
neuro.Print(ol::ToString((int)((float)niveauX1/2.55)),x_seq+485,y_seq+70);
neuro.Print(ol::ToString((int)((float)niveauX2/2.55)),x_seq+590,y_seq+70);
}

ol::Rect X1X2Together( ol::Vec2D( x_seq+530,y_seq+50), ol::Vec2D( 50,20));//box du fader
X1X2Together.SetRoundness(7.5);
X1X2Together.SetLineWidth(demi_epaisseur_ligne_fader);
X1X2Together.Draw(CouleurFader.WithAlpha(index_x1_x2_together));
X1X2Together.DrawOutline(CouleurLigne);

ol::Line(ol::Vec2D( x_seq+540,y_seq+55),ol::Vec2D(x_seq+550,y_seq+55)).Draw(CouleurLigne);
ol::Line(ol::Vec2D( x_seq+550,y_seq+55),ol::Vec2D(x_seq+560,y_seq+65)).Draw(CouleurLigne);
ol::Line(ol::Vec2D( x_seq+560,y_seq+65),ol::Vec2D(x_seq+570,y_seq+65)).Draw(CouleurLigne);
ol::Line(ol::Vec2D( x_seq+540,y_seq+65),ol::Vec2D(x_seq+550,y_seq+65)).Draw(CouleurLigne);
ol::Line(ol::Vec2D( x_seq+550,y_seq+65),ol::Vec2D(x_seq+560,y_seq+55)).Draw(CouleurLigne);
ol::Line(ol::Vec2D( x_seq+560,y_seq+55),ol::Vec2D(x_seq+570,y_seq+55)).Draw(CouleurLigne);
ol::Rect index_together(ol::Vec2D( x_seq+535,y_seq+60+ratio_X1X2_together+255),ol::Vec2D(40,20));
index_together.SetRoundness(4);
index_together.SetLineWidth(epaisseur_ligne_fader);


ol::Line(ol::Vec2D(x_seq+615,y_seq+311),ol::Vec2D(x_seq+625,y_seq+311)).Draw(CouleurLigne);//1.1
petitpetitchiffrerouge.Print("1.1",x_seq+635,y_seq+311);
ol::Line(ol::Vec2D(x_seq+615,y_seq+292),ol::Vec2D(x_seq+625,y_seq+292)).Draw(CouleurLigne);//1.2
petitpetitchiffrerouge.Print("1.2",x_seq+635,y_seq+295);
ol::Line(ol::Vec2D(x_seq+615,y_seq+275),ol::Vec2D(x_seq+625,y_seq+275)).Draw(CouleurLigne);//1.3
petitpetitchiffrerouge.Print("1.3",x_seq+635,y_seq+278);
ol::Line(ol::Vec2D(x_seq+615,y_seq+261),ol::Vec2D(x_seq+625,y_seq+261)).Draw(CouleurLigne);//1.4
petitpetitchiffrerouge.Print("1.4",x_seq+635,y_seq+264);
ol::Line(ol::Vec2D(x_seq+615,y_seq+249),ol::Vec2D(x_seq+625,y_seq+249)).Draw(CouleurLigne);//1.5
petitpetitchiffrerouge.Print("1.5",x_seq+635,y_seq+252);
ol::Line(ol::Vec2D(x_seq+615,y_seq+239),ol::Vec2D(x_seq+625,y_seq+239)).Draw(CouleurLigne);//1.6
petitpetitchiffrerouge.Print("1.6",x_seq+635,y_seq+242);
ol::Line(ol::Vec2D(x_seq+615,y_seq+229),ol::Vec2D(x_seq+625,y_seq+229)).Draw(CouleurLigne);//1.7
petitpetitchiffrerouge.Print("1.7",x_seq+635,y_seq+232);
ol::Line(ol::Vec2D(x_seq+615,y_seq+221),ol::Vec2D(x_seq+625,y_seq+221)).Draw(CouleurLigne);//1.8
petitpetitchiffrerouge.Print("1.8",x_seq+635,y_seq+224);
ol::Line(ol::Vec2D(x_seq+615,y_seq+213),ol::Vec2D(x_seq+625,y_seq+213)).Draw(CouleurLigne);//1.9
petitpetitchiffrerouge.Print("1.9",x_seq+635,y_seq+216);
ol::Line(ol::Vec2D(x_seq+615,y_seq+207),ol::Vec2D(x_seq+625,y_seq+207)).Draw(CouleurLigne);//2.0
petitpetitchiffrerouge.Print("2",x_seq+635,y_seq+207);

ol::Line(ol::Vec2D(x_seq+555,y_seq+70),ol::Vec2D(x_seq+555,y_seq+70+255)).Draw(CouleurLigne);//gorge
ol::Line(ol::Vec2D(x_seq+525,y_seq+90),ol::Vec2D(x_seq+555,y_seq+90)).Draw(CouleurLigne);//vers stage
ol::Line(ol::Vec2D(x_seq+555,y_seq+70+255),ol::Vec2D(x_seq+580,y_seq+70+255)).Draw(CouleurLigne);//vers stage
index_together.Draw(CouleurFond);
index_together.DrawOutline(CouleurLigne);


petitchiffre.Print(string_ratio_x1x2,x_seq+540,y_seq+330+ratio_X1X2_together);


ol::Circle GoTouch(ol::Vec2D( x_seq+540+14,y_seq+375),20);
GoTouch.SetLineWidth(epaisseur_ligne_fader);
GoTouch.DrawOutline(CouleurLigne);
GoTouch.DrawOutline(CouleurGreen.WithAlpha(Midi_Force_Go));


GoTouch.Draw(CouleurFader.WithAlpha(index_go)); GoTouch.DrawOutline(CouleurLigne);
GoTouch.Draw(CouleurFond.WithAlpha(alpha_blinker*index_pause)); GoTouch.DrawOutline(CouleurLigne);


ol::Line(ol::Vec2D(x_seq+546, y_seq+365), ol::Vec2D(x_seq+566, y_seq+375)).Draw(CouleurLigne);
ol::Line(ol::Vec2D(x_seq+546, y_seq+385), ol::Vec2D(x_seq+566, y_seq+375)).Draw(CouleurLigne);
ol::Line(ol::Vec2D(x_seq+546, y_seq+365), ol::Vec2D(x_seq+546, y_seq+385)).Draw(CouleurLigne);


//////////////go back
ol::Circle GoBackTouch(ol::Vec2D( x_seq+504,y_seq+375),20);
GoBackTouch.SetLineWidth(epaisseur_ligne_fader);
GoBackTouch.DrawOutline(CouleurLigne);
GoBackTouch.Draw(CouleurFader.WithAlpha(index_go_back));GoBackTouch.DrawOutline(CouleurLigne);
GoBackTouch.Draw(CouleurFond.WithAlpha(alpha_blinker*index_pause));GoBackTouch.DrawOutline(CouleurLigne);

ol::Line(ol::Vec2D(x_seq+512, y_seq+365), ol::Vec2D(x_seq+492, y_seq+375)).Draw(CouleurLigne);
ol::Line(ol::Vec2D(x_seq+512, y_seq+385), ol::Vec2D(x_seq+492, y_seq+375)).Draw(CouleurLigne);
ol::Line(ol::Vec2D(x_seq+512, y_seq+365), ol::Vec2D(x_seq+512, y_seq+385)).Draw(CouleurLigne);


//////////////double go
ol::Circle DoubleGoTouch(ol::Vec2D( x_seq+604,y_seq+375),20);
DoubleGoTouch.SetLineWidth(epaisseur_ligne_fader);
DoubleGoTouch.DrawOutline(CouleurLigne);


ol::Line(ol::Vec2D(x_seq+593, y_seq+365), ol::Vec2D(x_seq+613, y_seq+375)).Draw(CouleurLigne);
ol::Line(ol::Vec2D(x_seq+593, y_seq+385), ol::Vec2D(x_seq+613, y_seq+375)).Draw(CouleurLigne);
ol::Line(ol::Vec2D(x_seq+593, y_seq+365), ol::Vec2D(x_seq+593, y_seq+385)).Draw(CouleurLigne);
ol::Line(ol::Vec2D(x_seq+602, y_seq+365), ol::Vec2D(x_seq+620, y_seq+375)).Draw(CouleurLigne);
ol::Line(ol::Vec2D(x_seq+602, y_seq+385), ol::Vec2D(x_seq+620, y_seq+375)).Draw(CouleurLigne);
ol::Line(ol::Vec2D(x_seq+602, y_seq+365), ol::Vec2D(x_seq+602, y_seq+368)).Draw(CouleurLigne);
ol::Line(ol::Vec2D(x_seq+602, y_seq+385), ol::Vec2D(x_seq+602, y_seq+382)).Draw(CouleurLigne);


/////////////////////ACCELEROMETRE

//Cadre bouton speed
ol::Rect SequencielSpeedFrame(ol::Vec2D(x_seq+485, y_seq+405),ol::Vec2D( 127+10, 22) );
SequencielSpeedFrame.SetLineWidth(epaisseur_ligne_fader);
SequencielSpeedFrame.SetRoundness(5);
SequencielSpeedFrame.DrawOutline(CouleurLigne);
ol::Rect SequencielSpeed(ol::Vec2D(x_seq+485+crossfade_speed, y_seq+405),ol::Vec2D( 10, 22) );
SequencielSpeed.SetLineWidth(tiers_epaisseur_ligne_fader);
SequencielSpeed.SetRoundness(5);

ol::Rect OnLinkReset(ol::Vec2D(x_seq+548, y_seq+435),ol::Vec2D( 15, 15) );
OnLinkReset.Draw(CouleurBlind.WithAlpha(auto_reset_crossfade_speed_on_link));
OnLinkReset.DrawOutline(CouleurLigne);



ol::Line (ol::Vec2D(x_seq+554,y_seq+405),ol::Vec2D(x_seq+554,y_seq+427)).Draw(CouleurLigne);

petitchiffre.Print(str_crossfade_speed_is,x_seq+465,y_seq+430);
SequencielSpeed.Draw(CouleurFader);

//midi out des x1 et x2
ol::Circle BMidiOut( x_seq+655,y_seq+340, 10);//box du fader
BMidiOut.SetLineWidth(epaisseur_ligne_fader);
BMidiOut.Draw(CouleurBlind.WithAlpha(midi_send_out[491]));
BMidiOut.DrawOutline(CouleurLigne);
//liaison visuelle au fader du cercle midi out
ol::Line (ol::Vec2D( x_seq+620,y_seq+330),ol::Vec2D(x_seq+640,y_seq+330)).Draw(CouleurLigne);
ol::Line (ol::Vec2D(x_seq+640,y_seq+330),ol::Vec2D(x_seq+650,y_seq+335)).Draw(CouleurLigne);



//MIDI OUT DU SPEED
ol::Circle BSpeedMidiOut( x_seq+655,y_seq+425, 10);//box du fader
BSpeedMidiOut.SetLineWidth(epaisseur_ligne_fader);
BSpeedMidiOut.Draw(CouleurBlind.WithAlpha(midi_send_out[493]));
BSpeedMidiOut.DrawOutline(CouleurLigne);
//liaison visuelle au fader du cercle midi out
ol::Line (ol::Vec2D( x_seq+620,y_seq+415),ol::Vec2D(x_seq+640,y_seq+415)).Draw(CouleurLigne);
ol::Line (ol::Vec2D(x_seq+620,y_seq+415),ol::Vec2D(x_seq+650,y_seq+415)).Draw(CouleurLigne);

raccrochage_midi_visuel_horizontal ( x_seq+490, y_seq+405, 493,127,13);


//OVER MOUSE

if(window_focus_id==W_SEQUENCIEL)
{

if( Midi_Faders_Affectation_Type!=0 )
{
//X1 X2
if(mouse_y>=y_seq+60 && mouse_y<=y_seq+355)//-20 +20
{
if(mouse_x> x_seq+480 && mouse_x< x_seq+525)
{
FaderX1.DrawOutline(CouleurBlind);
}
else if(mouse_x> x_seq+580 && mouse_x< x_seq+625)
{
FaderX2.DrawOutline(CouleurBlind);
}
}
else if(mouse_x>x_seq+534 && mouse_x<x_seq+574 && mouse_y>y_seq+355 && mouse_y<y_seq+395)
{
GoTouch.DrawOutline(CouleurBlind);
}
else if( mouse_x>x_seq+484 && mouse_x<x_seq+524 && mouse_y>y_seq+355 && mouse_y<y_seq+395)
{
GoBackTouch.DrawOutline(CouleurBlind);
}
else if( mouse_x>x_seq+584 && mouse_x<x_seq+624 && mouse_y>y_seq+355 && mouse_y<y_seq+395)
{
DoubleGoTouch.DrawOutline(CouleurBlind);
}
else if( mouse_x> x_seq+548 && mouse_x<  x_seq+563 && mouse_y>y_seq+435 && mouse_y<y_seq+450 && Midi_Faders_Affectation_Type!=0)
{
char sttmp[24];
sprintf(sttmp,"AutoReset Crossfade Speed");
show_type_midi(1591,sttmp );
OnLinkReset.DrawOutline(CouleurBlind);
}
//Accelerometre
else if(mouse_x>=x_seq+490 && mouse_x<=x_seq+617 && mouse_y>y_seq+405 && mouse_y<y_seq+427)
{
//midi report
switch(miditable[0][493])
{
//midi report
case 0:
sprintf(thetypinfo,"Note");
break;
case 1:
sprintf(thetypinfo,"Key On");
break;
case 2:
sprintf(thetypinfo,"Key Off");
break;
case 4:
sprintf(thetypinfo,"Ctrl Change");
break;
}
sprintf(string_last_midi_id,"Sequenciel SPEED is Ch: %d Pitch: %d Typ: %s" , miditable[1][493],miditable[2][493],thetypinfo);
SequencielSpeedFrame.DrawOutline(CouleurBlind);

}

}


//fin focus window
}
return(0);
}



int Sequenciel_Window(int xseq, int yseq,int largeur_seq,int hauteur_seq)
{
//background window
ol::Rect SeqWindow(ol::Vec2D(xseq, yseq ), ol::Vec2D( largeur_seq,hauteur_seq));
SeqWindow.SetRoundness(15);
SeqWindow.SetLineWidth(triple_epaisseur_ligne_fader);
SeqWindow.Draw(CouleurFond);
if(window_focus_id==W_SEQUENCIEL )
{
SeqWindow.DrawOutline(CouleurFader);
}
else
{
SeqWindow.DrawOutline(CouleurLigne);
}
neuro.Print("CUELIST",xseq+90, yseq+30);

/////////////////////////////TITRES////////////////////////////////////////////

petitpetitchiffre.Print("Delay", xseq+20, yseq+60);
petitpetitchiffre.Print("In/Out", xseq+60, yseq+60);
neuromoyen.Print("Mem", xseq+115, yseq+60);

ol::Rect InPositionStage(ol::Vec2D(xseq+10, yseq+80), ol::Vec2D( 100,40));
InPositionStage.SetRoundness(4);
InPositionStage.Draw(CouleurNiveau.WithAlpha(0.5));


ol::Rect PositionStage(ol::Vec2D(xseq+10, yseq+110), ol::Vec2D( 460,40));
PositionStage.Draw(CouleurNiveau.WithAlpha(0.5));

ol::Rect OutPositionStage(ol::Vec2D(xseq+10, yseq+110), ol::Vec2D( 100,40));
OutPositionStage.SetRoundness(4);
OutPositionStage.Draw(CouleurSurvol);


ol::Rect PositionPreset(ol::Vec2D(xseq+10, yseq+140), ol::Vec2D( 460,30));
PositionPreset.SetRoundness(7.5);
PositionPreset.Draw(CouleurSurvol);


///////////////////////////////////////////////////////////////////////////////

refresh_vision_memories(xseq,yseq);
x1_x2(xseq,yseq-25);

////////////////////MENUS///////////////////////////////////////////////////////
//options hautes
//LINK
ol::Rect SeqModL(ol::Vec2D(xseq+185, yseq+50), ol::Vec2D(35,15));
SeqModL.Draw(CouleurFader.WithAlpha(index_link_is_on));
SeqModL.DrawOutline(CouleurLigne.WithAlpha(0.5));
petitchiffre.Print("Link",xseq+187,yseq+60);
//BANGER
ol::Rect SeqMod(ol::Vec2D(xseq+223, yseq+50), ol::Vec2D(45,15));
SeqMod.Draw(CouleurFader.WithAlpha(index_banger_is_on));
SeqMod.DrawOutline(CouleurLigne.WithAlpha(0.5));
petitchiffre.Print("Banger",xseq+225,yseq+60);
//GRID PLAYER EMBEDED
ol::Rect GplMod(ol::Vec2D(xseq+275, yseq+50), ol::Vec2D(30,15));
GplMod.Draw(CouleurFader.WithAlpha(show_gridplayer_in_seq));
GplMod.DrawOutline(CouleurLigne.WithAlpha(0.5));
petitchiffre.Print("Gpl.1",xseq+277,yseq+60);

/////menus bas
char string_seq_function[5];
ol::Rect SeqAction(ol::Vec2D(xseq,yseq), ol::Vec2D( 50,20));
SeqAction.SetRoundness(7.5);

for (int op=0;op<7;op++)
{
//ol::Rect SeqAction(ol::Vec2D(xseq+20+(op*60), yseq+395 ), ol::Vec2D( 50,20));
int rangee_y= yseq+180+(35*(nbre_memoires_visualisables_en_preset+1));

SeqAction.MoveTo(ol::Vec2D(xseq+20+(op*60),rangee_y));
//affichage
SeqAction.DrawOutline(CouleurLigne);
if(window_focus_id==W_SEQUENCIEL &&  Midi_Faders_Affectation_Type!=0 && mouse_x>xseq+20+(op*60) && mouse_x<xseq+70+(op*60) && mouse_y>  rangee_y && mouse_y<  rangee_y+20 )//config midi
{
SeqAction.DrawOutline(CouleurBlind);
}
switch(op)
{
case 0:
petitchiffre.Print("Create",xseq+23+(op*60),  rangee_y+15);
if(  Midi_Faders_Affectation_Type!=0) show_type_midi(768,"CREATE MEM");
break;
case 1:
petitchiffre.Print("Delete",xseq+23+(op*60),  rangee_y+15);
if(  Midi_Faders_Affectation_Type!=0) show_type_midi(769,"DELETE MEM");
break;
case 2:
petitchiffre.Print("Stage-",xseq+23+(op*60),rangee_y+15);
if(  Midi_Faders_Affectation_Type!=0) show_type_midi(770,"STAGE MINUS");
break;
case 3:
petitchiffre.Print("Stage+",xseq+23+(op*60),  rangee_y+15);
if(  Midi_Faders_Affectation_Type!=0) show_type_midi(771,"STAGE PLUS");
break;
case 4:
petitchiffre.Print("Preset-",xseq+23+(op*60),  rangee_y+15);
if(  Midi_Faders_Affectation_Type!=0) show_type_midi(772,"PRESET MINUS");
break;
case 5:
petitchiffre.Print("Preset+",xseq+23+(op*60), rangee_y+15);
if(  Midi_Faders_Affectation_Type!=0) show_type_midi(773,"PRESET PLUS");
break;
case 6:
petitchiffre.Print("GOTO",xseq+27+(op*60), rangee_y+15);
if(  Midi_Faders_Affectation_Type!=0) show_type_midi(1645,"GOTO");
break;
default:
break;
}
}


/////////////////AVERTISSEMENT GET BACK MIDI FADERS
if (index_get_back_faders_need_to_be_done==1)
{
ol::Rect FondClignotant(ol::Vec2D(xseq+450,yseq+185),ol::Vec2D(200,30));
FondClignotant.SetRoundness(15);
FondClignotant.Draw(CouleurFond);
FondClignotant.Draw(CouleurBlind.WithAlpha(alpha_blinker));

petitdoomInspekt.Print("GET UP MIDI FADERS !",xseq+470,yseq+205);
}


//TIME LEFT

neuro.Print(string_time_left_is,xseq+380,yseq+30);
neuromoyen.Print("---",xseq+430,yseq+40);
neuro.Print(string_total_time_is,xseq+380,yseq+60);
//colonnages
ol::Line(ol::Vec2D(xseq+170,yseq+80),ol::Vec2D(xseq+170,yseq+hauteur_seq-75)).Draw(CouleurLigne.WithAlpha(0.5));//après mem
ol::Line(ol::Vec2D(xseq+180,yseq+80),ol::Vec2D(xseq+180,yseq+hauteur_seq-75)).Draw(CouleurLigne.WithAlpha(0.5));//après exclude mem
ol::Line(ol::Vec2D(xseq+220,yseq+80),ol::Vec2D(xseq+220,yseq+hauteur_seq-75)).Draw(CouleurLigne.WithAlpha(0.5));//apres link
ol::Line(ol::Vec2D(xseq+270,yseq+80),ol::Vec2D(xseq+270,yseq+hauteur_seq-75)).Draw(CouleurLigne.WithAlpha(0.5));//apres banger
ol::Line(ol::Vec2D(xseq+310,yseq+80),ol::Vec2D(xseq+310,yseq+hauteur_seq-75)).Draw(CouleurLigne.WithAlpha(0.5));//apres banger


//Grid Player 1 enchassé
if(show_gridplayer_in_seq==1)
{
neuromoyen.Print("GridPlayer 1 /",xseq+660,yseq+40);
neuromoyen.Print("Grid ",xseq+760,yseq+40);
neuromoyen.Print(ol::ToString(index_grider_selected[0]+1),xseq+810,yseq+40);
ol::Line(ol::Vec2D(xseq+660,yseq+50),ol::Vec2D(xseq+860,yseq+50)).Draw(CouleurLigne);

//UP DOWN griders number selected
ol::Circle GriderPlus(ol::Vec2D(xseq+910,yseq+40),12);
ol::Circle GriderMinus(ol::Vec2D(xseq+880,yseq+40),12);

petitchiffre.Print("-",xseq+877,yseq+44);
petitchiffre.Print("+",xseq+907,yseq+44);

GriderPlus.DrawOutline(CouleurLigne);
GriderMinus.DrawOutline(CouleurLigne);

if(window_focus_id==W_SEQUENCIEL  )
{

if(mouse_x>xseq+880-12 && mouse_x<xseq+892 && mouse_y>yseq+28 && mouse_y<yseq+52)
{
if( Midi_Faders_Affectation_Type!=0)
{
char sttmp[24];
sprintf(sttmp,"Minus Grid in GridPl.1");
show_type_midi(1476,sttmp );
GriderMinus.DrawOutline(CouleurBlind);
}
}

if( mouse_x>xseq+898 && mouse_x<xseq+922 && mouse_y>yseq+28 && mouse_y<yseq+52)
{
if(Midi_Faders_Affectation_Type!=0)
{
char sttmp[24];
sprintf(sttmp,"Plus Grid in GridPl.1");
show_type_midi(1480,sttmp );
GriderPlus.DrawOutline(CouleurBlind);
}
}
}
petitchiffre.Print(grider_name[index_grider_selected[0]],xseq+660,yseq+70);
TheStepBox(xseq+660,yseq+90,0);
ThePlayCommands(xseq+760,yseq+90,0);
TheGrid_commands(xseq+660,yseq+130,0);//pos x, posy y, num player


ol::Rect BoxLink(ol::Vec2D(xseq+690, yseq+295),ol::Vec2D(105,15));
BoxLink.Draw(CouleurFader.WithAlpha(index_link_speed_crossfade_to_gpl1));
BoxLink.DrawOutline(CouleurLigne.WithAlpha(0.5));
petitchiffre.Print("Speed is CueList",xseq+693,yseq+305);
if(window_focus_id==W_SEQUENCIEL  &&  mouse_x>xseq+690 && mouse_x<xseq+795 && mouse_y>yseq+295 && mouse_y<yseq+310)
 {
 if( Midi_Faders_Affectation_Type!=0)
 {
 char sttmp[24];
 sprintf(sttmp,"Speed is CueList GPl.1");
 show_type_midi(1540,sttmp );
 BoxLink.DrawOutline(CouleurBlind);
 }

 }
}

return(0);
}


