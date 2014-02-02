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

 \file Draw3.cpp
 \brief {description courte} 
 \author Christoph Guillermet
 \version {numero de version du fichier}
 \date {date description}
 
 White Cat {- catégorie} {- sous catégorie {- sous catégorie}}
 Description détaillée
 
 **/
 
 

int snap_state_of_draw_grid(int pr)
{
for(int i=0;i<500;i++)
{
if(draw_preset_levels[pr][i]>0.0) {draw_snap_preset_levels[pr][i]=1;}
else{draw_snap_preset_levels[pr][i]=0;}      
}    
return(0);   
}



int merge_draw_and_grid_player(int pr)
{    
merging_gpl_in_draw=1;
     //recup des niveaux issus d'un gridplayer
if(draw_point_is_traced[pr]==1)
{
float value_grid;
int coord=0;

int rate=BPS_RATE/10;
// draw_level_to_do[pr]/BPS_RATE car dans la boucle data

if(draw_brush_type[pr]==0)//Point
          { 
          coord=index_case[pr];
           switch(draw_mode[pr])
           {
           case 0://draw 
           draw_preset_levels[pr][coord]+=draw_level_to_do[pr]/rate ;
           if(draw_preset_levels[pr][coord]>1.0){draw_preset_levels[pr][coord]=1.0;}   
           break;  
           case 1://erase
           draw_preset_levels[pr][coord]-=draw_level_to_do[pr]/rate ;
           if(draw_preset_levels[pr][coord]<0.0){draw_preset_levels[pr][coord]=0.0;}
           break;      
           case 2://solo
           for(int i=0;i<500;i++)
           {
           if(i!=coord){draw_preset_levels[pr][i]=0.0;  }     
           }
           draw_preset_levels[pr][coord]+=draw_level_to_do[pr]/rate ;
           if(draw_preset_levels[pr][coord]>1.0){draw_preset_levels[pr][coord]=1.0;}
           break;
           case 3://ghost
           for(int i=0;i<(draw_preset_parameters[pr][0]*draw_preset_parameters[pr][1]);i++)
           {
           if(i!=coord && draw_preset_levels[pr][i]>0.0)
           {
           draw_preset_levels[pr][i]-=draw_ghost_to_do[pr]/(rate);
           if(draw_preset_levels[pr][i]<0.0){draw_preset_levels[pr][i]=0.0;}
           }     
           }
           draw_preset_levels[pr][coord]+=draw_level_to_do[pr]/rate ;
           if(draw_preset_levels[pr][coord]>1.0){draw_preset_levels[pr][coord]=1.0;}                                                                          
          break;
          default:
          break;                 
          }
          }      
                   
else //GPL MODE
{     
if(draw_brush_type[pr]==2)//mode solo, reset
            { 
            for(int i=0;i<500;i++)
            {
            draw_preset_levels[pr][i]=0.0;   
            }
            } 
            
                          
for(int c=-draw_preset_parameters[pr][0]/2;c<=(draw_preset_parameters[pr][0]/2) ;c++)
 {
   for(int l=-draw_preset_parameters[pr][1]/2;l<grider_nb_row;l++)
    {            

          coord=index_case[pr]+c+(l*draw_preset_parameters[pr][0]);
          //restriction au tableau pour éviter les débordements du motif
          if( coord>=0 && coord<=(draw_preset_parameters[pr][1]*draw_preset_parameters[pr][0]))
          {
          
          if (coord%draw_preset_parameters[pr][0]>=draw_centre_x[pr]-(draw_preset_parameters[pr][0]/2)
          && coord%draw_preset_parameters[pr][0]<=draw_centre_x[pr]+(draw_preset_parameters[pr][0]/2) 
         // && coord/draw_preset_parameters[pr][0]>=draw_centre_y[pr]-(draw_preset_parameters[pr][1]/2)
         // && coord/draw_preset_parameters[pr][0]<=draw_centre_y[pr]+(draw_preset_parameters[pr][1]/2)  
         )
          {
          value_grid=((float)(buffer_gridder[draw_get_gpl[pr]-1][draw_offset_gpl[pr]-1 + c+(l*grider_nb_col)])/255 )*draw_level_to_do[pr];
                   
          switch(draw_mode[pr])
          {
          case 0://draw  
          draw_preset_levels[pr][coord]+= value_grid;
          if(draw_preset_levels[pr][coord]>1.0){draw_preset_levels[pr][coord]=1.0;}
          break;
          case 1://erase
          draw_preset_levels[pr][coord]-= value_grid;
          if(draw_preset_levels[pr][coord]<0.0){draw_preset_levels[pr][coord]=0.0;}
          break;
          case 2://solo
          draw_preset_levels[pr][coord]= value_grid;
          break;
          case 3://ghost
          if(value_grid!=0.0)//is a pattern
          {
          draw_preset_levels[pr][coord]+=value_grid;
          if(draw_preset_levels[pr][coord]>1.0){draw_preset_levels[pr][coord]=1.0;}
          }
          else 
          {
          draw_preset_levels[pr][coord]-=draw_ghost_to_do[pr]/(rate);
          if(draw_preset_levels[pr][coord]<0.0){draw_preset_levels[pr][coord]=0.0;
          }                                                                                 
          break;
          default:
          break;
          }
          
          }
          
          
          }
          }
          }
          }                   
                             

}
draw_point_is_traced[pr]=0;//pour ipad reset de l'action
}
merging_gpl_in_draw=0;      
return(0);
}
int Draw_TrackingAera(int xw,int yw)
{
    
Rect Aera(Vec2D(xw,yw),Vec2D(draw_tracking_aera_size_x,draw_tracking_aera_size_y));
Aera.SetLineWidth(2.0);
Aera.SetRoundness(4);
Aera.Draw(CouleurBleuProcedure.WithAlpha(0.1)); 
 
Rect Level(Vec2D(xw,yw),Vec2D(draw_largeur_case[draw_preset_selected],draw_hauteur_case[draw_preset_selected]));

int ind=0;
for(int col=0;col<draw_preset_parameters[draw_preset_selected][0];col++)
{
Line(Vec2D(xw+(col*draw_largeur_case[draw_preset_selected]),yw),Vec2D(xw+(col*draw_largeur_case[draw_preset_selected]),yw+(draw_preset_parameters[draw_preset_selected][1]*draw_hauteur_case[draw_preset_selected]))).Draw(CouleurFond.WithAlpha(0.5));           

for(int lig=0;lig<draw_preset_parameters[draw_preset_selected][1];lig++)
{   
Line(Vec2D(xw,yw+(lig*draw_hauteur_case[draw_preset_selected])),Vec2D(xw+(draw_preset_parameters[draw_preset_selected][0]*draw_largeur_case[draw_preset_selected]),yw+(lig*draw_hauteur_case[draw_preset_selected]))).Draw(CouleurFond.WithAlpha(0.5));         
     
ind=col+(lig*draw_preset_parameters[draw_preset_selected][0]);
Level.MoveTo(Vec2D(xw+(col*draw_largeur_case[draw_preset_selected]),yw+(lig*draw_hauteur_case[draw_preset_selected])));
Level.Draw(CouleurLevel.WithAlpha(draw_preset_levels[draw_preset_selected][ind]));

if(draw_preset_parameters[draw_preset_selected][0]<=12)
{
petitchiffre.Print(ol::ToString(draw_preset_channel_routing[draw_preset_selected][ind]),xw+1+(col*draw_largeur_case[draw_preset_selected]),yw+(lig*draw_hauteur_case[draw_preset_selected])+10);
}
else 
{
minichiffre.Print(ol::ToString(draw_preset_channel_routing[draw_preset_selected][ind]),xw+1+(col*draw_largeur_case[draw_preset_selected]),yw+(lig*draw_hauteur_case[draw_preset_selected])+10); 
}

}
}


Rect Curseur(Vec2D(xw+draw_centre_x[draw_preset_selected]*draw_largeur_case[draw_preset_selected],yw+draw_centre_y[draw_preset_selected]*draw_hauteur_case[draw_preset_selected]),
Vec2D(draw_largeur_case[draw_preset_selected], draw_hauteur_case[draw_preset_selected]  ));   
Curseur.SetLineWidth(3.0);
Curseur.DrawOutline(CouleurBlind); 

return(0);   
}


int Draw_Window(int xw,int yw)
{
Rect DrawBckgd(Vec2D(xw,  yw), Vec2D( draw_window_x_size,draw_window_y_size));
DrawBckgd.SetRoundness(15);
DrawBckgd.SetLineWidth(triple_epaisseur_ligne_fader);  
DrawBckgd.Draw(CouleurFond); 

if(window_focus_id==W_DRAW)
{
DrawBckgd.DrawOutline(CouleurFader); 
}    
else {DrawBckgd.DrawOutline(CouleurLigne); }
neuro.Print("DRAW",xw+100,yw+30);    

neuro.Print(ol::ToString(draw_preset_channel_routing[draw_preset_selected][(index_case[draw_preset_selected])]),xw+40,yw+110);
petitchiffre.Print(ol::ToString(draw_centre_x[draw_preset_selected]),xw+55,yw+125);
petitchiffre.Print(ol::ToString(draw_centre_y[draw_preset_selected]),xw+75,yw+125);
//SHAPE
Rect DrawBrush(Vec2D(xw+180,  yw+12), Vec2D( 50,20));
DrawBrush.SetRoundness(5);
DrawBrush.Draw(CouleurLevel);
DrawBrush.DrawOutline(CouleurLigne);

if(window_focus_id==W_DRAW && Midi_Faders_Affectation_Type!=0 && mouse_x> xw+180 && mouse_x<xw+230 && mouse_y>yw+12 && mouse_y<yw+32)//config midi
{
DrawBrush.DrawOutline(CouleurBlind);
}

Rect Ppoint(Vec2D(xw+202,yw+21),Vec2D(3,3));


switch(draw_brush_type[draw_preset_selected])
{
case 0://single point
  Ppoint.Draw(CouleurFond);  
break;
case 1://ligne
  petitchiffrenoir.Print("GridPl.",xw+185, yw+25);   
break;                                                                             
}

//SELECTION PRESETS
Rect DrawPst(Vec2D(xw,  yw), Vec2D( 50,20));
DrawPst.SetRoundness(5);
for(int pr=0;pr<6;pr++)
{
DrawPst.MoveTo(Vec2D(xw+240+(pr*58),yw+12));
if(draw_preset_selected==pr){DrawPst.Draw(CouleurFader);}
DrawPst.DrawOutline(CouleurLigne.WithAlpha(0.5));
petitchiffre.Print(ol::ToString(pr+1),xw+260+(pr*58),yw+25);
if(window_focus_id==W_DRAW && Midi_Faders_Affectation_Type!=0 && mouse_x> xw+240+(pr*58) && mouse_x<xw+290+(pr*58) && mouse_y>yw+12 && mouse_y<yw+32)//config midi
{
DrawPst.DrawOutline(CouleurBlind);
}

//fin presets
}     


Rect DrawingMode(Vec2D(xw+20,yw+60),Vec2D(60,20));
DrawingMode.SetRoundness(5);
DrawingMode.DrawOutline(CouleurLigne.WithAlpha(0.5));

switch(draw_mode[draw_preset_selected])
{
case 0:
  DrawingMode.Draw(CouleurLevel); 
  petitchiffrenoir.Print("DRAW",xw+33,yw+74);   
break;
case 1:
  DrawingMode.Draw(CouleurBlind); 
  petitchiffre.Print("ERASE",xw+30,yw+74);  
break;     
case 2:
  DrawingMode.Draw(CouleurFader); 
  petitchiffrenoir.Print("SOLO",xw+33,yw+74);  
break;  
case 3:
  DrawingMode.Draw(CouleurLigne);
  petitchiffrenoir.Print("GHOST",xw+33,yw+74);       
break;
default:
break;                                        
}

if(window_focus_id==W_DRAW && Midi_Faders_Affectation_Type!=0 && mouse_x>xw+20 && mouse_x<xw+80 && mouse_y>yw+60 && mouse_y<yw+80)//config midi
{
DrawingMode.SetLineWidth(2.0);
DrawingMode.DrawOutline(CouleurBlind);
}



petitchiffre.Print("Pressure",xw+105,yw+53);
Rect FrameLevelPressure(Vec2D(xw+180,yw+45),Vec2D(127,10));
Rect LevelPressure(Vec2D(xw+180,yw+45),Vec2D(127*draw_level_to_do[draw_preset_selected],10));
LevelPressure.Draw(CouleurFader);
FrameLevelPressure.DrawOutline(CouleurLigne.WithAlpha(0.5));
minichiffrerouge.Print(ol::ToString((int)( draw_level_to_do[draw_preset_selected]*127)),xw+312,yw+53);
switch(dmx_view)
{
case 0:
petitchiffre.Print(ol::ToString((int) (draw_level_to_do[draw_preset_selected]*100)),xw+330,yw+54);
break;
case 1:
petitchiffre.Print(ol::ToString((int) (draw_level_to_do[draw_preset_selected]*255)),xw+330,yw+54);
break;
}

if(window_focus_id==W_DRAW && Midi_Faders_Affectation_Type!=0 && mouse_x>xw+180 && mouse_x<xw+307 && mouse_y>yw+45 && mouse_y<yw+55)//config midi
{
FrameLevelPressure.DrawOutline(CouleurBlind);
}

Circle BMidiOutPressure(xw+316,yw+62, 5);
if(midi_send_out[1653]==1)
{
BMidiOutPressure.Draw(CouleurBlind);
}
BMidiOutPressure.DrawOutline(CouleurGrisMoyen);


petitchiffre.Print("Brush Angle",xw+105,yw+78);
Rect FrameLevelTilt(Vec2D(xw+180,yw+70),Vec2D(127,10));
Rect LevelTilt(Vec2D(xw+180,yw+70),Vec2D(127*draw_tilt_to_do[draw_preset_selected],10));
LevelTilt.Draw(CouleurFader);
FrameLevelTilt.DrawOutline(CouleurLigne.WithAlpha(0.5));
minichiffrerouge.Print(ol::ToString((int)( draw_tilt_to_do[draw_preset_selected]*127)),xw+312,yw+78);
petitchiffre.Print(ol::ToString( draw_tilt_to_do[draw_preset_selected]),xw+330,yw+84);

if(window_focus_id==W_DRAW && Midi_Faders_Affectation_Type!=0 && mouse_x>xw+180 && mouse_x<xw+307 && mouse_y>yw+70 && mouse_y<yw+80)//config midi
{
FrameLevelTilt.DrawOutline(CouleurBlind);
}

Circle BMidiOutTilt(xw+316,yw+87, 5);
if(midi_send_out[1654]==1)
{
BMidiOutTilt.Draw(CouleurBlind);
}
BMidiOutTilt.DrawOutline(CouleurGrisMoyen);



petitchiffre.Print("Ghost Level",xw+105,yw+103);
Rect FrameGhostSize(Vec2D(xw+180,yw+95),Vec2D(127,10));
Rect GhostSize(Vec2D(xw+180,yw+95),Vec2D(127*draw_ghost_to_do[draw_preset_selected],10));
GhostSize.Draw(CouleurFader);
FrameGhostSize.DrawOutline(CouleurLigne.WithAlpha(0.5));
minichiffrerouge.Print(ol::ToString((int)( draw_ghost_to_do[draw_preset_selected]*127)),xw+312,yw+103);
switch(dmx_view)
{
case 0:
petitchiffre.Print(ol::ToString((int) (draw_ghost_to_do[draw_preset_selected]*100)),xw+330,yw+104);
break;
case 1:
petitchiffre.Print(ol::ToString((int) (draw_ghost_to_do[draw_preset_selected]*255)),xw+330,yw+104);
break;
}
if(window_focus_id==W_DRAW && Midi_Faders_Affectation_Type!=0 && mouse_x>xw+180 && mouse_x<xw+307 && mouse_y>yw+95 && mouse_y<yw+105)//config midi
{
FrameGhostSize.DrawOutline(CouleurBlind);
}

Circle BMidiOutSize(xw+316,yw+112, 5);
if(midi_send_out[1655]==1)
{
BMidiOutSize.Draw(CouleurBlind);
}
BMidiOutSize.DrawOutline(CouleurGrisMoyen);

Rect ClearBoard(Vec2D(xw+400,yw+40), Vec2D(110,20));
ClearBoard.SetRoundness(7.5);
petitchiffre.Print("Erase Drawing",xw+417,yw+53);
ClearBoard.DrawOutline(CouleurLigne.WithAlpha(0.5));


if(window_focus_id==W_DRAW &&  Midi_Faders_Affectation_Type!=0 && mouse_x>xw+400 && mouse_x<xw+510 && mouse_y>yw+40 && mouse_y<yw+60)
{  
ClearBoard.DrawOutline(CouleurBlind);    
}

Rect AffectMode(Vec2D(xw+400,yw+75), Vec2D(110,20));
AffectMode.SetRoundness(7.5);

switch(draw_channel_recording_mode) //4 modes: 0 solo - 1 until end of line - 2 until end of col - 3 until end of matrix
{
case 0:                                  
petitchiffre.Print("Rec Ch. Solo",xw+418,yw+88);
break;
case 1:             
AffectMode.Draw(CouleurFader);                       
petitchiffre.Print("Rec Ch.> End Row",xw+405,yw+88);
break;
case 2:     
AffectMode.Draw(CouleurGreen);                               
petitchiffre.Print("Rec Ch.> End Col",xw+410,yw+88);
break;
case 3:             
AffectMode.Draw(CouleurBlind);                       
petitchiffre.Print("Rec Ch.> End Grid",xw+405,yw+88);
break;
default:
break;
}
AffectMode.DrawOutline(CouleurLigne.WithAlpha(0.5));


Rect Affecttodock(Vec2D(xw+400,yw+100), Vec2D(110,20));
Affecttodock.SetRoundness(7.5);
if(index_affect_draw_to_dock==1){Affecttodock.Draw(CouleurFader);}
Affecttodock.DrawOutline(CouleurLigne.WithAlpha(0.5));

petitchiffre.Print( "Affect To Dock",(xw+410), (yw+113));

//PARAMETRES PRESETS

//////////////EDIT MODE///////////////////////////////
Rect GriderEditEnable( Vec2D((xw+530),(yw+40)),Vec2D(50,20));
GriderEditEnable.SetRoundness(5);
if(index_enable_edit_Draw==1)
{
GriderEditEnable.Draw(CouleurFader);                                 
}
GriderEditEnable.DrawOutline(CouleurLigne.WithAlpha(0.5));
petitchiffre.Print("edit",xw+540 ,yw+52);

//def number row and cols
Rect BackGriderCol(Vec2D(xw+550,yw+75),Vec2D(30,20));
BackGriderCol.SetRoundness(4);
BackGriderCol.Draw(CouleurBleuProcedure.WithAlpha(0.5));
petitchiffre.Print("Col.:",xw+525,yw+90);
petitchiffre.Print(ol::ToString(draw_preset_parameters[draw_preset_selected][0]),xw+560,yw+90);

Rect BackGriderRow(Vec2D(xw+550,yw+100),Vec2D(30,20));
BackGriderRow.SetRoundness(4);
BackGriderRow.Draw(CouleurBleuProcedure.WithAlpha(0.5));
petitchiffre.Print("Row:",xw+525,yw+115);
petitchiffre.Print(ol::ToString(draw_preset_parameters[draw_preset_selected][1]),xw+560,yw+115);


if(window_focus_id==W_DRAW  && mouse_x> xw+550 && mouse_x<xw+580 && mouse_y>yw+75 && mouse_y<yw+95 )
{               
BackGriderCol.DrawOutline(CouleurLigne.WithAlpha(0.7));
}
if(window_focus_id==W_DRAW  && mouse_x> xw+550 && mouse_x<xw+580 && mouse_y>yw+100 && mouse_y<yw+120 )
{
BackGriderRow.DrawOutline(CouleurLigne.WithAlpha(0.7));                            
}                            


//tracking aera
Draw_TrackingAera(xw+20,yw+140);

//controls midi xy
petitchiffre.Print("Midi X:",xw+20,yw+588);
Rect MidiX(Vec2D(xw+65,yw+580),Vec2D(127,10));
Rect MidiXLev(Vec2D(xw+65,yw+580),Vec2D(midi_levels[1660],10));
MidiXLev.Draw(CouleurFader);
MidiX.DrawOutline(CouleurLigne.WithAlpha(0.5));
if(window_focus_id==W_DRAW  && Midi_Faders_Affectation_Type!=0  && mouse_x>= xw+65 && mouse_x<=xw+192 && mouse_y>yw+580 && mouse_y<yw+590 )
{
MidiX.DrawOutline(CouleurBlind);                            
}  


petitchiffre.Print("Midi Y:",xw+200,yw+588);
Rect MidiY(Vec2D(xw+245,yw+580),Vec2D(127,10));
Rect MidiYLev(Vec2D(xw+245,yw+580),Vec2D(midi_levels[1661],10));
MidiYLev.Draw(CouleurFader);
MidiY.DrawOutline(CouleurLigne.WithAlpha(0.5));
if(window_focus_id==W_DRAW  && Midi_Faders_Affectation_Type!=0  && mouse_x>= xw+245 && mouse_x<=xw+372 && mouse_y>yw+580 && mouse_y<yw+590 )
{
MidiY.DrawOutline(CouleurBlind);                            
}  


//recup anim grid player gridplayer

petitchiffre.Print("GridPlayer:",xw+385,yw+590);


Rect NumGPL(Vec2D(xw+455,yw+578),Vec2D(30,18));
NumGPL.SetRoundness(4);
NumGPL.DrawOutline(CouleurLigne.WithAlpha(0.4));
petitchiffre.Print(ol::ToString(draw_get_gpl[draw_preset_selected]),xw+465,yw+590);

petitchiffre.Print("Offset:",xw+495,yw+590);

Rect OffsetGPL(Vec2D(xw+550,yw+578),Vec2D(35,18));
OffsetGPL.SetRoundness(4);
OffsetGPL.DrawOutline(CouleurLigne.WithAlpha(0.4));
petitchiffre.Print(ol::ToString(draw_offset_gpl[draw_preset_selected]),xw+560,yw+590);


if(window_focus_id==W_DRAW && mouse_y>yw+578 && mouse_y<yw+596 && index_enable_edit_Draw==1)
{
if(mouse_x>xw+455 && mouse_x<xw+455+30){NumGPL.DrawOutline(CouleurLigne);}       
else if(mouse_x>xw+495 && mouse_x<xw+495+35){OffsetGPL.DrawOutline(CouleurLigne);}                    
}


  return(0);   
}






int do_logical_Draw_TrackingAera(int xw,int yw)
{
if(window_focus_id==W_DRAW && mouse_x>=xw && mouse_x<=xw+draw_tracking_aera_size_x && mouse_y>=yw && mouse_y<=yw+draw_tracking_aera_size_y && mouse_button==1)
{
if( dragging_draw==0){snap_state_of_draw_grid(draw_preset_selected); dragging_draw=1;}   
draw_point_is_traced[draw_preset_selected]=1;
Draw_point_and_perform_level_on_area_NEW(draw_preset_selected,mouse_x-xw,mouse_y-yw);    
}
return(0);   
}


int do_logical_Draw_Window(int xw,int yw)
{
//SHAPE SELECTION
//SHAPE
Rect DrawBrush(Vec2D(xw+180,  yw+12), Vec2D( 50,20));    
if(window_focus_id==W_DRAW && dragging_draw==0 && mouse_button==1 && mouse_released==0 && mouse_x> xw+180 && mouse_x<xw+180+50 && mouse_y>yw+12 && mouse_y<yw+12+20)
{
show_type_midi(1658, "Draw Brush Type");
if( Midi_Faders_Affectation_Type!=0)//config midi
{  
attribute_midi_solo_affectation(1658,Midi_Faders_Affectation_Mode);
}
else
{
draw_brush_type[draw_preset_selected]=toggle(draw_brush_type[draw_preset_selected]);

switch(draw_brush_type[draw_preset_selected])
{
case 0:
sprintf(string_Last_Order,"DOT brush selected");
break;  
case 1:
sprintf(string_Last_Order,"GridPlayer selected");
break;    
default:
break;                               
}
}
mouse_released=1;                           
}    
//selection presets
for(int pr=0;pr<6;pr++)
{
if(window_focus_id==W_DRAW && dragging_draw==0 && mouse_button==1 && mouse_released==0 && mouse_x> xw+240+(pr*58) && mouse_x<xw+240+(pr*58)+50 && mouse_y>yw+12 && mouse_y<yw+12+20)
{
char temp[24];
sprintf(temp,"Draw Preset %d ",pr+1);
show_type_midi(1646+pr, temp);
if( Midi_Faders_Affectation_Type!=0)//config midi
{  
attribute_midi_solo_affectation(1646+pr,Midi_Faders_Affectation_Mode);
}                  
else
{
if(index_main_clear==0)
{
draw_preset_selected=pr; 
recalculate_draw_sizes(draw_preset_selected);   
}
else
{
if(index_enable_edit_Draw==1)
{
draw_prst_to_clear=pr;
index_ask_confirm=1;
ask_clear_draw_preset=1;
}  
}
}
mouse_released=1;
}
//fin presets
}     

//Drawing mode
if(window_focus_id==W_DRAW && mouse_button==1 && mouse_released==0 && mouse_x>xw+20 && mouse_x<xw+20+60 && mouse_y>yw+60 && mouse_y<yw+60+20)
{
show_type_midi(1652, "Draw Mode");
if( Midi_Faders_Affectation_Type!=0)//config midi
{  
attribute_midi_solo_affectation(1652,Midi_Faders_Affectation_Mode);
}                  
else
{
draw_mode[draw_preset_selected]++;
if(draw_mode[draw_preset_selected]>3){draw_mode[draw_preset_selected]=0;}
mouse_released=1;                          
}
}
//pressures etc
if(window_focus_id==W_DRAW && mouse_button==1 && dragging_draw==0)
{
for(int i=0;i<3;i++)
{
        
if(mouse_released==0 && mouse_x>=xw+311 && mouse_x<=xw+321 && mouse_y>=yw+57+(i*25) && mouse_y<=yw+67+(i*25))
{
midi_send_out[1653+i]=toggle(midi_send_out[1653+i]);
mouse_released=1;
}     
        
if(mouse_x>=xw+175 && mouse_x<=xw+315 && mouse_y>=yw+45+(i*25) && mouse_y<=yw+45+(i*25)+10)
{
char temp[24];
sprintf(temp,"Draw Param %d ",i+1);
show_type_midi(1653+i, temp);
if( Midi_Faders_Affectation_Type!=0)//config midi
{  
attribute_midi_solo_affectation(1653+i,Midi_Faders_Affectation_Mode);
}                  
else
{
float po=constrain_data_to_midi_range(mouse_x-(xw+175));
 switch(i)
 {
 case 0:
 draw_level_to_do[draw_preset_selected]=po/127;
 break;
 case 1:
 draw_tilt_to_do[draw_preset_selected]=po/127;
 break;
 case 2:
 draw_ghost_to_do[draw_preset_selected]=po/127;
 break;
 default:
 break;         
 }                  
}                   
}      
}

}

//erase draw
if(window_focus_id==W_DRAW  && mouse_button==1 && mouse_released==0 && mouse_x> xw+400 && mouse_x<xw+400+110 && mouse_y>yw+40 && mouse_y<yw+40+20 )
{
show_type_midi(1657, "Erase Drawing");
if( Midi_Faders_Affectation_Type!=0)//config midi
{  
attribute_midi_solo_affectation(1657,Midi_Faders_Affectation_Mode);
}                  
else
{
draw_erase_drawing(draw_preset_selected);   
}   
mouse_released=1;                
}
//affectation mode
if(window_focus_id==W_DRAW  && mouse_button==1 && mouse_released==0 && mouse_x> xw+400 && mouse_x<xw+400+110 && mouse_y>yw+75 && mouse_y<yw+75+20 )
{               
draw_channel_recording_mode++;
if(draw_channel_recording_mode>3){draw_channel_recording_mode=0;}
mouse_released=1;                
}
////AFFECT TO DOCK
if(window_focus_id==W_DRAW && mouse_button==1 && mouse_released==0 && mouse_x>xw+400 && mouse_x<xw+400+110 && mouse_y>yw+100 && mouse_y<yw+100+20  )
{
if(index_affect_draw_to_dock==0)
{
reset_index_actions();
reset_indexs_confirmation(); 
index_affect_draw_to_dock=1;
}
else 
{
reset_index_actions();     
}
index_do_dock=index_affect_draw_to_dock;
mouse_released=1;                   
}



//////////////EDIT MODE///////////////////////////////

if(window_focus_id==W_DRAW && mouse_button==1 && mouse_released==0 )
{
if( mouse_x> xw+530 && mouse_x<xw+530+50 && mouse_y>yw+40 && mouse_y<yw+40+20)
{
index_enable_edit_Draw=toggle(index_enable_edit_Draw);
mouse_released=1;    
} 
//nbre de col
else if(  index_enable_edit_Draw==1 && mouse_x> xw+550 && mouse_x<xw+550+30 && mouse_y>yw+75 && mouse_y<yw+75+20 )
{               
 draw_preset_parameters[draw_preset_selected][0]=atoi(numeric);
 if(draw_preset_parameters[draw_preset_selected][0]<1) 
 {draw_preset_parameters[draw_preset_selected][0]=1;}
 else if(draw_preset_parameters[draw_preset_selected][0]>25)
 { draw_preset_parameters[draw_preset_selected][0]=25;}
 recalculate_draw_sizes(draw_preset_selected);
 reset_numeric_entry();                         
 mouse_released=1;
}
//nbre de rows
else if( index_enable_edit_Draw==1 && mouse_x> xw+550 && mouse_x<xw+550+30 && mouse_y>yw+100 && mouse_y<yw+100+20 )
{               
 draw_preset_parameters[draw_preset_selected][1]=atoi(numeric);
 if(draw_preset_parameters[draw_preset_selected][1]<1) 
 {draw_preset_parameters[draw_preset_selected][1]=1;}
 else if(draw_preset_parameters[draw_preset_selected][1]>20)
 { draw_preset_parameters[draw_preset_selected][1]=20;}
 recalculate_draw_sizes(draw_preset_selected);
 reset_numeric_entry();                         
 mouse_released=1;
}                            
}


//tracking aera
do_logical_Draw_TrackingAera(xw+20,yw+140);

//controls midi xy
if(window_focus_id==W_DRAW   && mouse_released==0 && mouse_x>= xw+65 && mouse_x<=xw+65+127 && mouse_y>yw+580 && mouse_y<yw+580+10 )
{
show_type_midi(1660, "Sel. TrackingZone X");
if( Midi_Faders_Affectation_Type!=0)//config midi
{  
attribute_midi_solo_affectation(1660,Midi_Faders_Affectation_Mode);
mouse_released=1;
}                  
else
{        
 midi_levels[1660]=constrain_data_to_midi_range(mouse_x- (xw+65));            
}            
}  

if(window_focus_id==W_DRAW  && mouse_released==0   && mouse_x>= xw+245 && mouse_x<=xw+245+127 && mouse_y>yw+580 && mouse_y<yw+580+10 )
{
show_type_midi(1661, "Sel. TrackingZone Y");
if( Midi_Faders_Affectation_Type!=0)//config midi
{  
attribute_midi_solo_affectation(1661,Midi_Faders_Affectation_Mode);
mouse_released=1;
}                  
else
{        
 midi_levels[1661]=constrain_data_to_midi_range(mouse_x- (xw+245));                       
}                                   
}  

//capture Grid players et offset GPL

//get GPL ON OFF
if(window_focus_id==W_DRAW  && mouse_released==0  && mouse_x>= xw+455 && mouse_x<=xw+455+30 && mouse_y>yw+578 && mouse_y<yw+578+18 )
{
draw_get_gpl[draw_preset_selected]=constrain_int_data_to_this_range(atoi(numeric),1,4);
reset_numeric_entry();
numeric_postext=0;
mouse_released=1;                          
}


//offset de GPL
if(window_focus_id==W_DRAW  && mouse_released==0   && index_enable_edit_Draw==1  && mouse_x>= xw+550 && mouse_x<=xw+550+35 && mouse_y>yw+578 && mouse_y<yw+578+18 && numeric_postext>0)
{
draw_offset_gpl[draw_preset_selected]= constrain_int_data_to_this_range(atoi(numeric),1,512);
reset_numeric_entry();
numeric_postext=0;
mouse_released=1;                          
}



  return(0);   
}
