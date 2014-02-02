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

 \file trichro_visu2.cpp
 \brief {description courte} 
 \author Christoph Guillermet
 \version {numero de version du fichier}
 \date {date description}
 
 White Cat {- catégorie} {- sous catégorie {- sous catégorie}}
 Description détaillée
 
 **/
 
 
int Interface_Trichromie(int xchroma, int ychroma, int rayon, int largeurchroma)
{

//background window  
Rect TrichroBackground(Vec2D (xchroma-158, ychroma-206 ), Vec2D ( 315,550));
TrichroBackground.SetRoundness(15);
TrichroBackground.SetLineWidth(triple_epaisseur_ligne_fader);
TrichroBackground.Draw(CouleurFond);
if(window_focus_id==902)
{
TrichroBackground.DrawOutline(CouleurFader); 
}
else
{
TrichroBackground.DrawOutline(CouleurLigne);    
}


///////////////////////////////roue de couleur///////////////////////////////////////////
/////////AFFICHEE EN OPENGL CAR PLUS RAPIDE QUE BLIT ECT////////////////////////////////
for (hcl=0; hcl<360; hcl+=0.1) 
{
     xcl = cos(hcl*PI/180.0)*(rayon+16);
	 ycl = sin(hcl*PI/180.0)*(rayon+16);
    
	 hsv_to_rgb(hcl, 1.0, 1.0, &rcl, &gcl, &bcl);
	 Line(Vec2D(xchroma,ychroma),Vec2D(xchroma+xcl,ychroma+ycl)).Draw(Rgba(rcl,gcl,bcl));
}


Circle MasqueNoir(Vec2D(xchroma,ychroma),rayon-16);
MasqueNoir.Draw(CouleurFond);

	V3D_f v1 =
	{
		xchroma+vxd, ychroma+vyd, 0,
		0., 0.,
		makecol(0, 0, 0) // black vertex
	};
	V3D_f v2 =
	{
		xchroma+vxw, ychroma+vyw, 0,
		0., 0.,
		makecol(255, 255, 255) // white vertex
	};
	V3D_f v3 =
	{
		xchroma+vxh, ychroma+vyh, 0,
		0., 0.,
		makecol(r_pick, v_pick, b_pick) // color vertex
	};
	

	triangle3d_f(screen, POLYTYPE_GCOL, NULL, &v1, &v2, &v3);


//
//AFFECTATION MIDI
if(Midi_Faders_Affectation_Type!=0 && window_focus_id==W_TRICHROMY)//config midi
  {
   if(mouse_x>xchroma-140 && mouse_x<xchroma+140 && mouse_y>ychroma-140 && mouse_y<ychroma+140)
  {
   Circle Color( Vec2D(xchroma-1,ychroma), 140 ); 
   Color.SetLineWidth(epaisseur_ligne_fader);
   Color.DrawOutline(CouleurBlind);
   }
 }   

//affichage du pointeur de hue
 Circle monCurseurHue( Vec2D(position_curseur_hue_x, position_curseur_hue_y), 12 ); 
 monCurseurHue.SetLineWidth(epaisseur_ligne_fader);
 monCurseurHue.DrawOutline( CouleurLigne );
   

Circle monCurseurTriangle( Vec2D(xchroma+picker_x,ychroma+picker_y), 6 ); 
   monCurseurTriangle.SetLineWidth(epaisseur_ligne_fader);
   monCurseurTriangle.DrawOutline( CouleurLigne );
   


/////////////////TRICHRO / QUADRI CHOOSE//////////////////////////////////////
Rect ChooseTriQuadri( Vec2D(xchroma-60,(ychroma-190)),Vec2D(130,30)); 
ChooseTriQuadri.SetRoundness(15);
ChooseTriQuadri.SetLineWidth(epaisseur_ligne_fader); 
if(window_focus_id==W_TRICHROMY &&  mouse_x>xchroma-60 && mouse_x<xchroma+80 && mouse_y>ychroma-190 && mouse_y<ychroma-160)
{
ChooseTriQuadri.Draw(CouleurSurvol); 
                    
}
ChooseTriQuadri.DrawOutline(CouleurLigne);
if(index_quadri==0){petitchiffre.Print("TRICHROMY",xchroma-30, ychroma-172);}
else if(index_quadri==1){petitchiffre.Print("QUADRICHROMY",xchroma-45, ychroma-172);}


/////////////////////AFFECTATION ON / OFF AUX DOCKS FADERS///////////////////////////

Rect AffectTriTo( Vec2D(xchroma+20,(ychroma+300)),Vec2D(130,30)); 
AffectTriTo.SetRoundness(15);
AffectTriTo.SetLineWidth(epaisseur_ligne_fader); 
if (index_affect_color_to_dock==1){AffectTriTo.Draw(CouleurFader);}
AffectTriTo.DrawOutline(CouleurLigne);
petitchiffre.Print("AFFECT TO DOCK",xchroma+35, ychroma+319);

//PASTE ON THE FLY MODE
Rect PasteTriTo( Vec2D(xchroma-150,(ychroma+300)),Vec2D(130,30)); 
PasteTriTo.SetRoundness(15);
PasteTriTo.SetLineWidth(epaisseur_ligne_fader); 
if (index_paste_on_the_fly==1){PasteTriTo.Draw(CouleurBlind);}
PasteTriTo.DrawOutline(CouleurLigne);
petitpetitchiffre.Print("PASTE ON THE FLY",xchroma-135, ychroma+319);



/////////////////////DOCKING DES COULEURS////////////////////////////////////////
//emplacement des box pour actions de storage RVB sur 6 presets  
Rect RedBox(Vec2D(xchroma-130,ychroma+165),Vec2D(50,20));
RedBox.SetRoundness(10);
RedBox.SetLineWidth(epaisseur_ligne_fader);
RedBox.Draw(Rgba::RED);
RedBox.DrawOutline(CouleurLigne);

Rect GreenBox(Vec2D(xchroma-65,ychroma+165),Vec2D(50,20));
GreenBox.SetRoundness(10);
GreenBox.SetLineWidth(epaisseur_ligne_fader);
GreenBox.Draw(Rgba::GREEN.WithAlpha(0.7));
GreenBox.DrawOutline(CouleurLigne);

Rect BlueBox(Vec2D(xchroma,ychroma+165),Vec2D(50,20));
BlueBox.SetRoundness(10);
BlueBox.SetLineWidth(epaisseur_ligne_fader);
BlueBox.Draw(Rgba::BLUE);
BlueBox.DrawOutline(CouleurLigne);

Rect YellowBox(Vec2D(xchroma+65,ychroma+165),Vec2D(50,20));
YellowBox.SetRoundness(10);
YellowBox.SetLineWidth(epaisseur_ligne_fader);
if (index_quadri==1){YellowBox.Draw(Rgba::YELLOW);}
YellowBox.DrawOutline(CouleurLigne);

petitchiffre.Print(ol::ToString(my_red),xchroma-115, ychroma+179);
petitchiffre.Print(ol::ToString(my_green),xchroma-55, ychroma+179);
petitchiffre.Print(ol::ToString(my_blue ),xchroma+10, ychroma+179);
petitchiffrerouge.Print(ol::ToString(my_yellow ),xchroma+75, ychroma+179);



//midi out de la roue
Circle BMidiOut( xchroma+140,ychroma+110, 10);//box du fader
BMidiOut.SetLineWidth(epaisseur_ligne_fader);
if(midi_send_out[497]==1)
{
BMidiOut.Draw(CouleurBlind);
}
BMidiOut.DrawOutline(CouleurLigne);

raccrochage_midi_visuel_circulaire (xchroma, ychroma, 497, rayon, rayon);

//liaison visuelle au fader du cercle midi out
Line (Vec2D( xchroma+97,ychroma+100),Vec2D(xchroma+125,ychroma+100)).Draw(CouleurLigne);
Line (Vec2D(xchroma+125,ychroma+100),Vec2D(xchroma+125+8,ychroma+106)).Draw(CouleurLigne);


petitchiffre.Print("COLOR PRESETS",xchroma-65, ychroma+205);

//premiere rangee de dock colors
char string_allocation_col[7];
for (int dock_color=0;dock_color<4;dock_color++)
{
Rect DockBoxColor(Vec2D(xchroma-130+(65*dock_color),ychroma+215),Vec2D(50,20));
DockBoxColor.SetRoundness(10);
DockBoxColor.SetLineWidth(epaisseur_ligne_fader);
if(dock_color==dock_color_selected){DockBoxColor.Draw(CouleurFader); }
sprintf(string_allocation_col,"%d / %d",(colorpreset_linked_to_dock[dock_color][0]+1),(colorpreset_linked_to_dock[dock_color][1]+1));

//////////////////////////////////////////////////////////////////////////////////
petitchiffre.Print(string_allocation_col,xchroma-120+(65*dock_color),ychroma+230);

DockBoxColor.DrawOutline(CouleurLigne); 
if(window_focus_id==W_TRICHROMY && mouse_x>xchroma-130+(65*dock_color) && mouse_x<xchroma-80+(65*dock_color) && mouse_y>ychroma+215 && mouse_y<ychroma+235)
{
petitchiffre.Print(string_allocation_col,xchroma-120+(65*dock_color),ychroma+230);
if( Midi_Faders_Affectation_Type!=0)
{DockBoxColor.DrawOutline(CouleurBlind); }
} 


}
//deuxième rangee de dockcolors
for (int dock_colortwo=0;dock_colortwo<4;dock_colortwo++)
{
Rect DockBoxColor2(Vec2D(xchroma-130+(65*dock_colortwo),ychroma+255),Vec2D(50,20));
DockBoxColor2.SetRoundness(10);
DockBoxColor2.SetLineWidth(epaisseur_ligne_fader);
if((dock_colortwo+4)==dock_color_selected){DockBoxColor2.Draw(CouleurFader); }
sprintf(string_allocation_col,"%d / %d",(colorpreset_linked_to_dock[dock_colortwo+4][0]+1),(colorpreset_linked_to_dock[dock_colortwo+4][1]+1));
petitchiffre.Print(string_allocation_col,xchroma-120+(65*dock_colortwo),ychroma+270);

DockBoxColor2.DrawOutline(CouleurLigne);  
if(window_focus_id==W_TRICHROMY && mouse_x>xchroma-130+(65*dock_colortwo) && mouse_x<xchroma-80+(65*dock_colortwo) && mouse_y>ychroma+255 && mouse_y<ychroma+275)
{
petitchiffre.Print(string_allocation_col,xchroma-120+(65*dock_colortwo),ychroma+270);
if(Midi_Faders_Affectation_Type!=0)
{DockBoxColor2.DrawOutline(CouleurBlind); } 
} 

}   

return(0);   
}
/////////////////////////////////////////////////////////////////


