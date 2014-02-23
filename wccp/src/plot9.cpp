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

* \file plot9.cpp
* \brief {Plot GUI fonctions}
* \author Christoph Guillermet
* \version {0.8.5.2}
* \date {19/02/2014}

 White Cat {- categorie} {- sous categorie {- sous categorie}}

*   Fonctions GUI de dessin de la fenêtre plot
*
*   GUI fonctions for the plot window
*
**/


int draw_channel_of_symbol( int calc, int symb, int plotx, int ploty)
{



 ol::Circle ChannelProp(ol::Vec2D(plotx,ploty),12);
 switch( symbol_is_selected[calc][symb])
 {
 case 0:
 ChannelProp.Draw(CouleurPlotFill);
 break;
 case 1:
 ChannelProp.Draw(CouleurFader);
 break;
 }
 ChannelProp.DrawOutline(CouleurPlotLine.WithAlpha(0.5));
 if(symbol_channel_is[calc][symb]<10)
 {
 petitchiffrenoir.Print(ol::ToString(symbol_channel_is[calc][symb] ), (int)(plotx-5),(int)(ploty+3));
 }
 else if(symbol_channel_is[calc][symb]>=10 && symbol_channel_is[calc][symb]<100)
 {
 petitchiffrenoir.Print(ol::ToString(symbol_channel_is[calc][symb] ), (int)(plotx-6),(int)(ploty+3));
 }
 else
 {
  petitchiffrenoir.Print(ol::ToString(symbol_channel_is[calc][symb] ), plotx-11,ploty+3);
 }

 return(0);
}

int draw_gels_of_a_symbol(int calc, int symb, int plotx, int ploty)
{
for(int i=0;i<3;i++)//3 slots de gelat par symbol
{
if(plot_show_options[i+2]==1 && gelat[calc][symb][i]!=0 )
{
switch(gelat_family[calc][symb][i])
{
case 0:
sprintf(temp_plot_info,"L.%d",gelat[calc][symb][i]);
break;
case 1:
sprintf(temp_plot_info,"R.%d",gelat[calc][symb][i]);
break;
case 2:
sprintf(temp_plot_info,"G.%d",gelat[calc][symb][i]);
break;
case 3:
sprintf(temp_plot_info,"A.%d",gelat[calc][symb][i]);
break;
default:
sprintf(temp_plot_info,"");
break;
}
petitchiffrenoir.Print(temp_plot_info,plotx+10,(int)(ploty+(i*10)));
}
}
 return(0);
}

int draw_line_to_channel_of_symbol(int x1, int y1, int x2, int y2, int way)//way= 0 left to right 1= Right to left 2= up to down 3= down to up
{

switch(way)
{
case 0:
ol::Line(ol::Vec2D(x1,y1),ol::Vec2D(x1,y2)).Draw(CouleurPlotLine.WithAlpha(0.5));
ol::Line(ol::Vec2D(x1,y2),ol::Vec2D(x2,y2)).Draw(CouleurPlotLine.WithAlpha(0.5));
break;
case 1:
ol::Line(ol::Vec2D(x2,y2),ol::Vec2D(x1,y2)).Draw(CouleurPlotLine.WithAlpha(0.5));
ol::Line(ol::Vec2D(x1,y2),ol::Vec2D(x1,y1)).Draw(CouleurPlotLine.WithAlpha(0.5));
break;
case 2:
ol::Line(ol::Vec2D(x1,y1),ol::Vec2D(x2,y1)).Draw(CouleurPlotLine.WithAlpha(0.5));
ol::Line(ol::Vec2D(x2,y1),ol::Vec2D(x2,y2)).Draw(CouleurPlotLine.WithAlpha(0.5));
break;
case 3:
ol::Line(ol::Vec2D(x2,y2),ol::Vec2D(x2,y1)).Draw(CouleurPlotLine.WithAlpha(0.5));
ol::Line(ol::Vec2D(x2,y1),ol::Vec2D(x1,y1)).Draw(CouleurPlotLine.WithAlpha(0.5));
break;
default:
break;
}
 return(0);
}


int plot_draw_symbol_pc( int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=40.0*size_symbol;
float taille_y=40.0*size_symbol;

ol::Circle lentille(ol::Vec2D(plotx,ploty),taille_x);
lentille.RotateBy(-angle_pc*6.5);
lentille.SetRadius(taille_x/2);
lentille.DrawSlice(CouleurPlotLine,0, PI);

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));
carcasse.Draw(CouleurPlotFill);
carcasse.DrawOutline(CouleurPlotLine);
lentille.SetRadius((taille_x/2)-(4*size_symbol));
lentille.Draw(CouleurPlotFill);

if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
carcasse.DrawOutline(CouleurPlotLine);
lentille.Draw(CouleurFader);
}


 return(0);
}

int plot_draw_symbol_fresnel( int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol , int plot_calc_number_is)
{
float taille_x=40.0*size_symbol;
float taille_y=40.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2)-(5.0*size_symbol),ploty-(5*size_symbol)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2)-(10.0*size_symbol),ploty));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2)-(15.0*size_symbol),ploty-(5*size_symbol)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2)-(20.0*size_symbol),ploty));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2)-(25.0*size_symbol),ploty-(5*size_symbol)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2)-(30.0*size_symbol),ploty));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2)-(35.0*size_symbol),ploty-(5*size_symbol)));

carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));
carcasse.Draw(CouleurPlotFill);
carcasse.DrawOutline(CouleurPlotLine);

if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
}

return(0);
}



int plot_draw_symbol_decoupe_etc_1(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=40.0*size_symbol;
float taille_y=80.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));


ol::Circle lentille(ol::Vec2D(plotx,ploty),taille_x);
lentille.RotateBy(-angle_pc*6.5);
lentille.SetRadius(taille_x/2);


carcasse.Draw(CouleurPlotFill);




if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
}
lentille.DrawSlice(CouleurPlotLine, PI,PI);

carcasse.DrawOutline(CouleurPlotLine);

return(0);
}


int plot_draw_symbol_decoupe_etc_2(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=40.0*size_symbol;
float taille_y=80.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));


ol::Circle lentille(ol::Vec2D(plotx,ploty),taille_x);
lentille.RotateBy(-angle_pc*6.5);
lentille.SetRadius(taille_x/2);



ol::Poly foc;
foc.Add(ol::Vec2D(plotx-(taille_x/2)+(10*size_symbol),ploty-(10*size_symbol)));
foc.Add(ol::Vec2D(plotx+(taille_x/2)-(10*size_symbol),ploty-(10*size_symbol)));
foc.Add(ol::Vec2D(plotx,ploty-(taille_x/2)-(10*size_symbol)));


foc.SetPivot(ol::Vec2D(plotx,ploty));
foc.RotateBy(angle_pc*(6.5));

carcasse.Draw(CouleurPlotFill);

if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
}
lentille.DrawSlice(CouleurPlotLine, PI,PI);

carcasse.DrawOutline(CouleurPlotLine);

foc.DrawOutline(CouleurPlotLine);

 return(0);
}


int plot_draw_symbol_decoupe_etc_3(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=40.0*size_symbol;
float taille_y=80.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));

ol::Circle lentille(ol::Vec2D(plotx,ploty),taille_x);
lentille.RotateBy(-angle_pc*6.5);
lentille.SetRadius(taille_x/2);

ol::Poly foc;
foc.Add(ol::Vec2D(plotx-(taille_x/2)+(10*size_symbol),ploty-(25*size_symbol)));
foc.Add(ol::Vec2D(plotx+(taille_x/2)-(10*size_symbol),ploty-(25*size_symbol)));
foc.Add(ol::Vec2D(plotx+(taille_x/2)-(10*size_symbol),ploty-(10*size_symbol)));
foc.Add(ol::Vec2D(plotx-(taille_x/2)+(10*size_symbol),ploty-(10*size_symbol)));
foc.SetPivot(ol::Vec2D(plotx,ploty));
foc.RotateBy(angle_pc*(6.5));

carcasse.Draw(CouleurPlotFill);

if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
}
lentille.DrawSlice(CouleurPlotLine, PI,PI);
carcasse.DrawOutline(CouleurPlotLine);
foc.DrawOutline(CouleurPlotLine);

 return(0);
}

int plot_draw_symbol_decoupe_1(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=40.0*size_symbol;
float taille_y=80.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));

ol::Poly fenetre;
fenetre.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(12*size_symbol)));
fenetre.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(17*size_symbol)));
fenetre.SetPivot(ol::Vec2D(plotx,ploty));
fenetre.RotateBy(angle_pc*(6.5));

ol::Poly poignee;
poignee.Add(ol::Vec2D(plotx-(12*size_symbol),ploty+(taille_y/2)));
poignee.Add(ol::Vec2D(plotx-(12*size_symbol),ploty+(taille_y/2)+(8*size_symbol)));
poignee.Add(ol::Vec2D(plotx+(12*size_symbol),ploty+(taille_y/2)+(8*size_symbol)));
poignee.Add(ol::Vec2D(plotx+(12*size_symbol),ploty+(taille_y/2)));
poignee.SetPivot(ol::Vec2D(plotx,ploty));
poignee.RotateBy(angle_pc*(6.5));


carcasse.Draw(CouleurPlotFill);
poignee.Draw(CouleurPlotFill);
if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
poignee.Draw(CouleurFader);
}
poignee.DrawOutline(CouleurPlotLine);
carcasse.DrawOutline(CouleurPlotLine);
fenetre.DrawOutline(CouleurPlotLine);

return(0);
}

int plot_draw_symbol_decoupe_2(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=40.0*size_symbol;
float taille_y=80.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));

ol::Poly fenetre;
fenetre.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(12*size_symbol)));
fenetre.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(17*size_symbol)));
fenetre.SetPivot(ol::Vec2D(plotx,ploty));
fenetre.RotateBy(angle_pc*(6.5));

ol::Poly poignee;
poignee.Add(ol::Vec2D(plotx-(12*size_symbol),ploty+(taille_y/2)));
poignee.Add(ol::Vec2D(plotx-(12*size_symbol),ploty+(taille_y/2)+(8*size_symbol)));
poignee.Add(ol::Vec2D(plotx+(12*size_symbol),ploty+(taille_y/2)+(8*size_symbol)));
poignee.Add(ol::Vec2D(plotx+(12*size_symbol),ploty+(taille_y/2)));
poignee.SetPivot(ol::Vec2D(plotx,ploty));
poignee.RotateBy(angle_pc*(6.5));

ol::Poly foc;
foc.Add(ol::Vec2D(plotx-(taille_x/2)+(10*size_symbol),ploty-(10*size_symbol)));
foc.Add(ol::Vec2D(plotx+(taille_x/2)-(10*size_symbol),ploty-(10*size_symbol)));
foc.Add(ol::Vec2D(plotx,ploty-(taille_x/2)-(10*size_symbol)));

foc.SetPivot(ol::Vec2D(plotx,ploty));
foc.RotateBy(angle_pc*(6.5));

carcasse.Draw(CouleurPlotFill);
poignee.Draw(CouleurPlotFill);

if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
poignee.Draw(CouleurFader);
}
carcasse.DrawOutline(CouleurPlotLine);
poignee.DrawOutline(CouleurPlotLine);
fenetre.DrawOutline(CouleurPlotLine);

foc.DrawOutline(CouleurPlotLine);

return(0);
}


int plot_draw_symbol_decoupe_3(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=40.0*size_symbol;
float taille_y=80.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));

ol::Poly fenetre;
fenetre.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(12*size_symbol)));
fenetre.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(17*size_symbol)));
fenetre.SetPivot(ol::Vec2D(plotx,ploty));
fenetre.RotateBy(angle_pc*(6.5));

ol::Poly poignee;
poignee.Add(ol::Vec2D(plotx-(12*size_symbol),ploty+(taille_y/2)));
poignee.Add(ol::Vec2D(plotx-(12*size_symbol),ploty+(taille_y/2)+(8*size_symbol)));
poignee.Add(ol::Vec2D(plotx+(12*size_symbol),ploty+(taille_y/2)+(8*size_symbol)));
poignee.Add(ol::Vec2D(plotx+(12*size_symbol),ploty+(taille_y/2)));
poignee.SetPivot(ol::Vec2D(plotx,ploty));
poignee.RotateBy(angle_pc*(6.5));


ol::Poly foc;
foc.Add(ol::Vec2D(plotx-(taille_x/2)+(10*size_symbol),ploty-(25*size_symbol)));
foc.Add(ol::Vec2D(plotx+(taille_x/2)-(10*size_symbol),ploty-(25*size_symbol)));
foc.Add(ol::Vec2D(plotx+(taille_x/2)-(10*size_symbol),ploty-(10*size_symbol)));
foc.Add(ol::Vec2D(plotx-(taille_x/2)+(10*size_symbol),ploty-(10*size_symbol)));
foc.SetPivot(ol::Vec2D(plotx,ploty));
foc.RotateBy(angle_pc*(6.5));


carcasse.Draw(CouleurPlotFill);
poignee.Draw(CouleurPlotFill);
if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
poignee.Draw(CouleurFader);
}
carcasse.DrawOutline(CouleurPlotLine);
poignee.DrawOutline(CouleurPlotLine);
fenetre.DrawOutline(CouleurPlotLine);
foc.DrawOutline(CouleurPlotLine);

return(0);
}

int plot_draw_symbol_Par_1( int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=40.0*size_symbol;
float taille_y=40.0*size_symbol;

ol::Circle lentille(ol::Vec2D(plotx,ploty),taille_x);
lentille.RotateBy(-angle_pc*6.5);
lentille.SetRadius(taille_x/2);
lentille.DrawSlice(CouleurPlotLine,0, PI);

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty));
carcasse.Add(ol::Vec2D(plotx,ploty+(taille_y)));

carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));

carcasse.Draw(CouleurPlotFill);

carcasse.DrawOutline(CouleurPlotLine);
lentille.SetRadius((taille_x/2)-(4*size_symbol));
lentille.Draw(CouleurPlotFill);

if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
lentille.Draw(CouleurFader);
}

 return(0);
}



int plot_draw_symbol_Par_2( int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=40.0*size_symbol;
float taille_y=40.0*size_symbol;

ol::Circle lentille(ol::Vec2D(plotx,ploty),taille_x);
lentille.RotateBy(-angle_pc*6.5);
lentille.SetRadius(taille_x/2);
lentille.DrawSlice(CouleurPlotLine,0, PI);

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty));
carcasse.Add(ol::Vec2D(plotx,ploty+(taille_y)));

carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));
carcasse.DrawOutline(CouleurPlotLine);
carcasse.Draw(CouleurPlotFill);

lentille.SetRadius((taille_x/2)-(8*size_symbol));
lentille.Draw(CouleurPlotFill);

if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
lentille.Draw(CouleurFader);
}

 return(0);
}


int plot_draw_symbol_Par_3( int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=40.0*size_symbol;
float taille_y=40.0*size_symbol;

ol::Circle lentille(ol::Vec2D(plotx,ploty),taille_x);
lentille.RotateBy(-angle_pc*6.5);
lentille.SetRadius(taille_x/2);
lentille.DrawSlice(CouleurPlotLine,0, PI);

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty));
carcasse.Add(ol::Vec2D(plotx,ploty+(taille_y)));

carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));
carcasse.DrawOutline(CouleurPlotLine);
carcasse.Draw(CouleurPlotFill);

if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
}

 return(0);
}


int plot_draw_symbol_Par_4( int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=40.0*size_symbol;
float taille_y=40.0*size_symbol;

ol::Circle lentille(ol::Vec2D(plotx,ploty),taille_x);
lentille.RotateBy(-angle_pc*6.5);
lentille.SetRadius(taille_x/2);
lentille.DrawSlice(CouleurPlotLine,0, PI);

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty));
carcasse.Add(ol::Vec2D(plotx,ploty+(taille_y)));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));
carcasse.Draw(CouleurPlotLine);

if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
lentille.DrawSlice(CouleurFader,0, PI);
}

 return(0);
}



int plot_draw_symbol_Horiziode_assym( int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=60.0*size_symbol;
float taille_y=40.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));
carcasse.Draw(CouleurPlotFill);
if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
}
carcasse.DrawOutline(CouleurPlotLine);

ol::Poly lampe;
lampe.Add(ol::Vec2D(plotx-(taille_x/2)+(10*size_symbol),ploty+(taille_y/2)-(10*size_symbol)));
lampe.Add(ol::Vec2D(plotx+(taille_x/2)-(10*size_symbol),ploty+(taille_y/2)-(10*size_symbol)));
lampe.Add(ol::Vec2D(plotx+(taille_x/2)-(10*size_symbol),ploty+(taille_y/2)-(8*size_symbol)));
lampe.Add(ol::Vec2D(plotx-(taille_x/2)+(10*size_symbol),ploty+(taille_y/2)-(8*size_symbol)));
lampe.SetPivot(ol::Vec2D(plotx,ploty));
lampe.RotateBy(angle_pc*(6.5));
lampe.Draw(CouleurPlotLine);

 return(0);
}

int plot_draw_symbol_Horiziode_sym( int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=60.0*size_symbol;
float taille_y=40.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));
carcasse.Draw(CouleurPlotFill);
if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
}
carcasse.DrawOutline(CouleurPlotLine);

ol::Poly lampe;
lampe.Add(ol::Vec2D(plotx-(taille_x/2)+(10*size_symbol),ploty));
lampe.Add(ol::Vec2D(plotx+(taille_x/2)-(10*size_symbol),ploty));

lampe.Add(ol::Vec2D(plotx+(taille_x/2)-(10*size_symbol),ploty+(2*size_symbol)));
lampe.Add(ol::Vec2D(plotx-(taille_x/2)+(10*size_symbol),ploty+(2*size_symbol)));
lampe.SetPivot(ol::Vec2D(plotx,ploty));
lampe.RotateBy(angle_pc*(6.5));
lampe.Draw(CouleurPlotLine);

 return(0);
}

int plot_draw_symbol_BT( int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=40.0*size_symbol;
float taille_y=40.0*size_symbol;


ol::Circle lentille(ol::Vec2D(plotx,ploty),taille_x);
lentille.RotateBy(-angle_pc*6.5);
lentille.SetRadius(taille_x);
lentille.DrawSlice(CouleurPlotLine,0, PI);

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));

carcasse.Draw(CouleurPlotLine);
lentille.SetRadius((taille_x)-(4*size_symbol));
lentille.DrawSlice(CouleurPlotLine,0,PI);

if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
carcasse.DrawOutline(CouleurFader);
lentille.DrawSlice(CouleurFader,0,PI);
}

 return(0);
}

int plot_draw_symbol_T8( int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=120.0*size_symbol;
float taille_y=20.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));
carcasse.Draw(CouleurPlotFill);
carcasse.DrawOutline(CouleurPlotLine);

if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
}

double txcl=0;
double tycl=0;
float rayon=taille_x/16;
for (int r=-3;r<5;r++)
{
 txcl= cos(angle_pc*-6.5)*(rayon+(18*size_symbol));
 tycl = sin(angle_pc*-6.5)*(rayon+(18*size_symbol));
ol::Circle ourlet(0, 0, rayon);
ourlet.MoveTo(ol::Vec2D(plotx+(txcl*r/2),ploty+(tycl*r/2)));
ourlet.DrawOutline(CouleurPlotLine);
}


return(0);
}



int plot_draw_symbol_Blinder( int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=80.0*size_symbol;
float taille_y=40.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));
carcasse.Draw(CouleurPlotLine);


if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
}

double txcl=0;
double tycl=0;
float rayon=taille_x/8;

txcl= cos(angle_pc*-6.5)*(rayon);
tycl = sin(angle_pc*-6.5)*(rayon);
ol::Circle ourlet(0, 0, rayon);

ourlet.MoveTo(ol::Vec2D(plotx+(txcl),ploty+(tycl)));
ourlet.Draw(CouleurPlotFill);
ourlet.MoveTo(ol::Vec2D(plotx-(txcl),ploty-(tycl)));
ourlet.Draw(CouleurPlotFill);

return(0);
}

int plot_draw_symbol_Svoboda( int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=120.0*size_symbol;
float taille_y=60.0*size_symbol;

ol::Poly svobe;
svobe.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)));
svobe.Add(ol::Vec2D(plotx-(taille_x/2),ploty));
svobe.Add(ol::Vec2D(plotx-(taille_x/2.5),ploty+(taille_y/5)));
svobe.Add(ol::Vec2D(plotx+(taille_x/2.5),ploty+(taille_y/5)));
svobe.Add(ol::Vec2D(plotx+(taille_x/2),ploty));
svobe.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)));

svobe.SetPivot(ol::Vec2D(plotx,ploty));
svobe.RotateBy(angle_pc*(6.5));
svobe.Draw(CouleurPlotLine);

ol::Poly svobe2;

svobe2.Add(ol::Vec2D(plotx-(taille_x/2.5),ploty));
svobe2.Add(ol::Vec2D(plotx-(taille_x/2.5),ploty+(taille_y/2)));
svobe2.Add(ol::Vec2D(plotx+(taille_x/2.5),ploty+(taille_y/2)));
svobe2.Add(ol::Vec2D(plotx+(taille_x/2.5),ploty));
svobe2.SetPivot(ol::Vec2D(plotx,ploty));
svobe2.RotateBy(angle_pc*(6.5));
svobe2.Draw(CouleurPlotLine);

if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
svobe.DrawOutline(CouleurPlotLine);
svobe2.DrawOutline(CouleurPlotLine);
svobe.Draw(CouleurFader);
svobe2.Draw(CouleurFader);
}



return(0);
}

int plot_draw_symbol_ACL( int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=120.0*size_symbol;


double txcl=0;
double tycl=0;
float rayon=taille_x/3;
for (int r=-2;r<2;r++)
{
txcl= cos(angle_pc*-6.5)*(rayon+(18*size_symbol));
tycl = sin(angle_pc*-6.5)*(rayon+(18*size_symbol));
plot_draw_symbol_Par_4((int)(plotx+(txcl*r/2)), (int)(ploty+(tycl*r/2)),size_symbol,  angle_pc, num_symbol, plot_calc_number_is);
}

return(0);
}


int plot_draw_symbol_fluo_little(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=60.0*size_symbol;
float taille_y=15.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));

ol::Poly Side1;
Side1.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)));
Side1.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));
Side1.Add(ol::Vec2D(plotx-(taille_x/2)-(10*size_symbol),ploty+(taille_y/2)));
Side1.Add(ol::Vec2D(plotx-(taille_x/2)-(10*size_symbol),ploty-(taille_y/2)));
Side1.SetPivot(ol::Vec2D(plotx,ploty));
Side1.RotateBy(angle_pc*(6.5));

ol::Poly Side2;
Side2.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)));
Side2.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
Side2.Add(ol::Vec2D(plotx+(taille_x/2)+(10*size_symbol),ploty+(taille_y/2)));
Side2.Add(ol::Vec2D(plotx+(taille_x/2)+(10*size_symbol),ploty-(taille_y/2)));
Side2.SetPivot(ol::Vec2D(plotx,ploty));
Side2.RotateBy(angle_pc*(6.5));


carcasse.Draw(CouleurPlotFill);
if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
}
carcasse.DrawOutline(CouleurPlotLine);

Side1.Draw(CouleurPlotLine);

Side2.Draw(CouleurPlotLine);

 return(0);
}



int plot_draw_symbol_fluo_big(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=120.0*size_symbol;
float taille_y=15.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));

ol::Poly Side1;
Side1.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)));
Side1.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));
Side1.Add(ol::Vec2D(plotx-(taille_x/2)-(10*size_symbol),ploty+(taille_y/2)));
Side1.Add(ol::Vec2D(plotx-(taille_x/2)-(10*size_symbol),ploty-(taille_y/2)));
Side1.SetPivot(ol::Vec2D(plotx,ploty));
Side1.RotateBy(angle_pc*(6.5));

ol::Poly Side2;
Side2.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)));
Side2.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
Side2.Add(ol::Vec2D(plotx+(taille_x/2)+(10*size_symbol),ploty+(taille_y/2)));
Side2.Add(ol::Vec2D(plotx+(taille_x/2)+(10*size_symbol),ploty-(taille_y/2)));
Side2.SetPivot(ol::Vec2D(plotx,ploty));
Side2.RotateBy(angle_pc*(6.5));


carcasse.Draw(CouleurPlotFill);
if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
}
carcasse.DrawOutline(CouleurPlotLine);

Side1.Draw(CouleurPlotLine);

Side2.Draw(CouleurPlotLine);

 return(0);
}


int plot_draw_symbol_FollowSpot1(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=35.0*size_symbol;
float taille_y=90.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));

ol::Poly fenetre;
fenetre.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)+(18*size_symbol)));
fenetre.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)+(18*size_symbol)));
fenetre.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)+(22*size_symbol)));
fenetre.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)+(22*size_symbol)));

fenetre.SetPivot(ol::Vec2D(plotx,ploty));
fenetre.RotateBy(angle_pc*(6.5));

ol::Poly poignee;
poignee.Add(ol::Vec2D(plotx-(12*size_symbol),ploty+(taille_y/2)));
poignee.Add(ol::Vec2D(plotx-(12*size_symbol),ploty+(taille_y/2)+(8*size_symbol)));
poignee.Add(ol::Vec2D(plotx+(12*size_symbol),ploty+(taille_y/2)+(8*size_symbol)));
poignee.Add(ol::Vec2D(plotx+(12*size_symbol),ploty+(taille_y/2)));
poignee.SetPivot(ol::Vec2D(plotx,ploty));
poignee.RotateBy(angle_pc*(6.5));


carcasse.Draw(CouleurPlotFill);
poignee.Draw(CouleurPlotFill);
if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
poignee.Draw(CouleurFader);
}
poignee.DrawOutline(CouleurPlotLine);
carcasse.DrawOutline(CouleurPlotLine);
fenetre.DrawOutline(CouleurPlotLine);

return(0);
}


int plot_draw_symbol_FollowSpot2(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=45.0*size_symbol;
float taille_y=110.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));

ol::Poly fenetre;
fenetre.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)+(18*size_symbol)));
fenetre.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)+(18*size_symbol)));
fenetre.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)+(22*size_symbol)));
fenetre.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)+(22*size_symbol)));
fenetre.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)+(24*size_symbol)));
fenetre.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)+(24*size_symbol)));
fenetre.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)+(18*size_symbol)));
fenetre.SetPivot(ol::Vec2D(plotx,ploty));
fenetre.RotateBy(angle_pc*(6.5));

ol::Poly poignee;
poignee.Add(ol::Vec2D(plotx-(12*size_symbol),ploty+(taille_y/2)));
poignee.Add(ol::Vec2D(plotx-(12*size_symbol),ploty+(taille_y/2)+(8*size_symbol)));
poignee.Add(ol::Vec2D(plotx+(12*size_symbol),ploty+(taille_y/2)+(8*size_symbol)));
poignee.Add(ol::Vec2D(plotx+(12*size_symbol),ploty+(taille_y/2)));
poignee.SetPivot(ol::Vec2D(plotx,ploty));
poignee.RotateBy(angle_pc*(6.5));


carcasse.Draw(CouleurPlotFill);
poignee.Draw(CouleurPlotFill);
if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
poignee.Draw(CouleurFader);
}
poignee.DrawOutline(CouleurPlotLine);
carcasse.DrawOutline(CouleurPlotLine);
fenetre.DrawOutline(CouleurPlotLine);

return(0);
}



int plot_draw_symbol_VP1(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=60.0*size_symbol;
float taille_y=40.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));



ol::Poly objectif;
objectif.Add(ol::Vec2D(plotx,ploty-(taille_y/2)));
objectif.Add(ol::Vec2D(plotx+(20*size_symbol),ploty-(taille_y/2)));
objectif.Add(ol::Vec2D(plotx+(20*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
objectif.Add(ol::Vec2D(plotx,ploty-(taille_y/2)-(4*size_symbol)));

objectif.SetPivot(ol::Vec2D(plotx,ploty));
objectif.RotateBy(angle_pc*(6.5));


carcasse.Draw(CouleurPlotFill);
objectif.Draw(CouleurPlotFill);
if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
objectif.Draw(CouleurFader);
}
objectif.DrawOutline(CouleurPlotLine);
carcasse.DrawOutline(CouleurPlotLine);


return(0);
}


int plot_draw_symbol_VP2(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{

float taille_x=60.0*size_symbol;
float taille_y=75.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));

ol::Poly tirroir;
tirroir.Add(ol::Vec2D(plotx-(taille_x/4),ploty-(taille_y/2)));
tirroir.Add(ol::Vec2D(plotx+(taille_x/4),ploty-(taille_y/2)));
tirroir.Add(ol::Vec2D(plotx+(taille_x/4),ploty-(taille_y/2)+(15*size_symbol)));
tirroir.Add(ol::Vec2D(plotx-(taille_x/4),ploty-(taille_y/2)+(15*size_symbol)));

tirroir.SetPivot(ol::Vec2D(plotx,ploty));
tirroir.RotateBy(angle_pc*(6.5));


ol::Poly objectif;
objectif.Add(ol::Vec2D(plotx-(10*size_symbol),ploty-(taille_y/2)));
objectif.Add(ol::Vec2D(plotx+(10*size_symbol),ploty-(taille_y/2)));
objectif.Add(ol::Vec2D(plotx+(10*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
objectif.Add(ol::Vec2D(plotx-(10*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));

objectif.SetPivot(ol::Vec2D(plotx,ploty));
objectif.RotateBy(angle_pc*(6.5));


carcasse.Draw(CouleurPlotFill);
objectif.Draw(CouleurPlotFill);
if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
objectif.Draw(CouleurFader);
}
objectif.DrawOutline(CouleurPlotLine);
carcasse.DrawOutline(CouleurPlotLine);
tirroir.DrawOutline(CouleurPlotLine);

return(0);
}

int plot_draw_symbol_slideprojector(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{

float taille_x=40.0*size_symbol;
float taille_y=40.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));


ol::Poly objectif;
objectif.Add(ol::Vec2D(plotx+(5*size_symbol),ploty-(taille_y/2)));
objectif.Add(ol::Vec2D(plotx+(15*size_symbol),ploty-(taille_y/2)));
objectif.Add(ol::Vec2D(plotx+(15*size_symbol),ploty-(taille_y/2)-(8*size_symbol)));
objectif.Add(ol::Vec2D(plotx+(5*size_symbol),ploty-(taille_y/2)-(8*size_symbol)));

objectif.SetPivot(ol::Vec2D(plotx,ploty));
objectif.RotateBy(angle_pc*(6.5));

ol::Circle Panier(ol::Vec2D(plotx,ploty),(taille_x/2)-(5*size_symbol));

carcasse.Draw(CouleurPlotFill);
objectif.Draw(CouleurPlotFill);
if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
objectif.Draw(CouleurFader);
}
objectif.DrawOutline(CouleurPlotLine);
carcasse.DrawOutline(CouleurPlotLine);
Panier.DrawOutline(CouleurPlotLine);


return(0);
}


int plot_draw_symbol_retroprojector(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{

float taille_x=60.0*size_symbol;
float taille_y=60.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));


ol::Poly mirroir;
mirroir.Add(ol::Vec2D(plotx,ploty));
mirroir.Add(ol::Vec2D(plotx+(25*size_symbol),ploty));
mirroir.Add(ol::Vec2D(plotx+(25*size_symbol),ploty-(25*size_symbol)));
mirroir.Add(ol::Vec2D(plotx,ploty-(25*size_symbol)));

mirroir.SetPivot(ol::Vec2D(plotx,ploty));
mirroir.RotateBy(angle_pc*(6.5));

ol::Poly bras;
bras.Add(ol::Vec2D(plotx,ploty+(5*size_symbol)));
bras.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(5*size_symbol)));
bras.Add(ol::Vec2D(plotx+(taille_x/2),ploty));
bras.Add(ol::Vec2D(plotx,ploty));
bras.SetPivot(ol::Vec2D(plotx,ploty));
bras.RotateBy(angle_pc*(6.5));


angle_pc+=0.125;

double txcl= cos(angle_pc*-6.5)*(taille_x/3);
double tycl = sin(angle_pc*-6.5)*(taille_x/3);


ol::Circle Lentille(ol::Vec2D(plotx+txcl,ploty+tycl),(8*size_symbol));


carcasse.Draw(CouleurPlotFill);
mirroir.Draw(CouleurPlotFill);
bras.Draw(CouleurPlotFill);
if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
mirroir.Draw(CouleurFader);
bras.Draw(CouleurFader);
}
mirroir.DrawOutline(CouleurPlotLine);
carcasse.DrawOutline(CouleurPlotLine);
bras.DrawOutline(CouleurPlotLine);
Lentille.DrawOutline(CouleurPlotLine);


return(0);
}


int plot_draw_symbol_barndoors(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{

float taille_x=60.0*size_symbol;
float taille_y=30.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/3),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/3),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));

ol::Poly Side1;
Side1.Add(ol::Vec2D(plotx-(taille_x/1.6),ploty-(taille_y/2)));
Side1.Add(ol::Vec2D(plotx-(taille_x/3),ploty+(taille_y/2)));
Side1.Add(ol::Vec2D(plotx-(taille_x/3)-(5*size_symbol),ploty+(taille_y/2)));
Side1.Add(ol::Vec2D(plotx-(taille_x/1.6)-(5*size_symbol),ploty-(taille_y/2)));
Side1.SetPivot(ol::Vec2D(plotx,ploty));
Side1.RotateBy(angle_pc*(6.5));

ol::Poly Side2;
Side2.Add(ol::Vec2D(plotx+(taille_x/1.6),ploty-(taille_y/2)));
Side2.Add(ol::Vec2D(plotx+(taille_x/3),ploty+(taille_y/2)));
Side2.Add(ol::Vec2D(plotx+(taille_x/3)+(5*size_symbol),ploty+(taille_y/2)));
Side2.Add(ol::Vec2D(plotx+(taille_x/1.6)+(5*size_symbol),ploty-(taille_y/2)));
Side2.SetPivot(ol::Vec2D(plotx,ploty));
Side2.RotateBy(angle_pc*(6.5));

carcasse.Draw(CouleurPlotFill);
if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
}
carcasse.DrawOutline(CouleurPlotLine);
Side1.Draw(CouleurPlotLine);
Side2.Draw(CouleurPlotLine);

return(0);
}


int plot_draw_symbol_top_hat(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{

float taille_x=40.0*size_symbol;
float taille_y=20.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/4),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/4),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/4),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/4),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));

ol::Poly carcasse2;
carcasse2.Add(ol::Vec2D(plotx-(taille_x/4),ploty+(taille_y/2)));
carcasse2.Add(ol::Vec2D(plotx-(taille_x/4),ploty-(taille_y/2)));
carcasse2.Add(ol::Vec2D(plotx+(taille_x/4),ploty-(taille_y/2)));
carcasse2.Add(ol::Vec2D(plotx+(taille_x/4),ploty+(taille_y/2)));
carcasse2.SetPivot(ol::Vec2D(plotx,ploty));
carcasse2.RotateBy(angle_pc*(6.5));

carcasse2.Draw(CouleurPlotFill);

if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse2.Draw(CouleurFader);
}
carcasse.DrawOutline(CouleurPlotLine);

 return(0);
}


int plot_draw_symbol_color_extender(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{

float taille_x=40.0*size_symbol;
float taille_y=30.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/4),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/4),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/4),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/4),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));

ol::Poly carcasse2;
carcasse2.Add(ol::Vec2D(plotx-(taille_x/4),ploty+(taille_y/2)));
carcasse2.Add(ol::Vec2D(plotx-(taille_x/4),ploty-(taille_y/2)));
carcasse2.Add(ol::Vec2D(plotx+(taille_x/4),ploty-(taille_y/2)));
carcasse2.Add(ol::Vec2D(plotx+(taille_x/4),ploty+(taille_y/2)));
carcasse2.SetPivot(ol::Vec2D(plotx,ploty));
carcasse2.RotateBy(angle_pc*(6.5));


ol::Poly carcasse3;
carcasse3.Add(ol::Vec2D(plotx-(taille_x/4),ploty-(taille_y/2)));
carcasse3.Add(ol::Vec2D(plotx-(taille_x/4),ploty-(taille_y/3)));
carcasse3.Add(ol::Vec2D(plotx+(taille_x/4),ploty-(taille_y/3)));
carcasse3.Add(ol::Vec2D(plotx+(taille_x/4),ploty-(taille_y/2)));
carcasse3.SetPivot(ol::Vec2D(plotx,ploty));
carcasse3.RotateBy(angle_pc*(6.5));

carcasse2.Draw(CouleurPlotFill);



if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse2.Draw(CouleurFader);
}
carcasse.DrawOutline(CouleurPlotLine);

carcasse3.DrawOutline(CouleurPlotLine);

 return(0);
}

int plot_draw_symbol_colorchanger(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{

float taille_x=60.0*size_symbol;
float taille_y=15.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));

ol::Poly Side1;
Side1.Add(ol::Vec2D(plotx-(taille_x/3),ploty-(taille_y/2)));
Side1.Add(ol::Vec2D(plotx-(taille_x/3),ploty+(taille_y/2)));
Side1.Add(ol::Vec2D(plotx+(taille_x/3),ploty+(taille_y/2)));
Side1.Add(ol::Vec2D(plotx+(taille_x/3),ploty-(taille_y/2)));
Side1.SetPivot(ol::Vec2D(plotx,ploty));
Side1.RotateBy(angle_pc*(6.5));

carcasse.Draw(CouleurPlotFill);
if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
}
carcasse.DrawOutline(CouleurPlotLine);
Side1.Draw(CouleurPlotLine);
return(0);
}

int plot_draw_symbol_jalousie(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{


float taille_x=60.0*size_symbol;
float taille_y=15.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));

ol::Poly Side1;
for(int i=0;i<3;i++)
{
Side1.Add(ol::Vec2D(plotx-((taille_x/4)*i),ploty+(taille_y/2)));
Side1.Add(ol::Vec2D(plotx-((taille_x/4)*i),ploty-(taille_y/2)));
Side1.Add(ol::Vec2D(plotx-((taille_x/4)*i)+(size_symbol*4),ploty-(taille_y/2)));
Side1.Add(ol::Vec2D(plotx-((taille_x/4)*i)+(size_symbol*4),ploty+(taille_y/2)));
}
ol::Poly Side2;
for(int i=0;i<3;i++)
{
Side2.Add(ol::Vec2D(plotx+((taille_x/4)*i),ploty+(taille_y/2)));
Side2.Add(ol::Vec2D(plotx+((taille_x/4)*i),ploty-(taille_y/2)));
Side2.Add(ol::Vec2D(plotx+((taille_x/4)*i)-(size_symbol*4),ploty-(taille_y/2)));
Side2.Add(ol::Vec2D(plotx+((taille_x/4)*i)-(size_symbol*4),ploty+(taille_y/2)));
}

Side1.SetPivot(ol::Vec2D(plotx,ploty));
Side1.RotateBy(angle_pc*(6.5));
Side2.SetPivot(ol::Vec2D(plotx,ploty));
Side2.RotateBy(angle_pc*(6.5));

carcasse.Draw(CouleurPlotFill);
if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
}
carcasse.DrawOutline(CouleurPlotLine);
Side1.DrawOutline(CouleurPlotLine);
Side2.DrawOutline(CouleurPlotLine);


return(0);
}


int plot_draw_symbol_iris(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=60.0*size_symbol;
ol::Circle Fenetre(plotx,ploty, size_symbol*(taille_x/3));

Fenetre.Draw(CouleurPlotLine);

ol::Poly iris;
iris.Add(ol::Vec2D(plotx,ploty));
iris.Add(ol::Vec2D(plotx+(6*size_symbol),ploty));
iris.Add(ol::Vec2D(plotx+(taille_x/3)+(6*size_symbol),ploty-(taille_x/3)));
iris.Add(ol::Vec2D(plotx+(taille_x/3),ploty-(taille_x/3)));
iris.SetPivot(ol::Vec2D(plotx,ploty));
iris.RotateBy(angle_pc*(6.5));
iris.Draw(CouleurPlotLine);

if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
Fenetre.Draw(CouleurFader);
Fenetre.DrawOutline(CouleurPlotLine);
iris.Draw(CouleurFader);
iris.DrawOutline(CouleurPlotLine);
}

Fenetre.SetRadius(taille_x/6);
Fenetre.Draw(CouleurPlotFill);
Fenetre.DrawOutline(CouleurPlotLine);

return(0);
}

int plot_draw_symbol_goboholder(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{

float taille_x=60.0*size_symbol;
float taille_y=70.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/4),ploty-(taille_y/2)-(taille_y/3)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/4),ploty-(taille_y/2)-(taille_y/3)));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));

carcasse.Draw(CouleurPlotLine);

ol::Circle Fenetre(plotx,ploty, taille_x/3);


if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
carcasse.DrawOutline(CouleurPlotLine);
}

Fenetre.Draw(CouleurPlotFill);
Fenetre.DrawOutline(CouleurPlotLine);
return(0);
}



int plot_draw_symbol_shutter(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{

float taille_x=40.0*size_symbol;
float taille_y=50.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/6),ploty-(taille_y/2)-(taille_y/3)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/6),ploty-(taille_y/2)-(taille_y/3)));

carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));

carcasse.Draw(CouleurPlotLine);

ol::Poly carcasse2;
carcasse2.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)-(taille_y/3)));
carcasse2.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)-(taille_y/3)));
carcasse2.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)-(taille_y/4)));
carcasse2.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)-(taille_y/4)));

carcasse2.SetPivot(ol::Vec2D(plotx,ploty));
carcasse2.RotateBy(angle_pc*(6.5));

carcasse2.Draw(CouleurPlotLine);

if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
carcasse.DrawOutline(CouleurPlotLine);
carcasse2.Draw(CouleurFader);
carcasse2.DrawOutline(CouleurPlotLine);
}


return(0);
}


int plot_draw_symbol_motorized_mirror(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{

float taille_x=40.0*size_symbol;
float taille_y=50.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));

carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));


ol::Poly carcasse2;
carcasse2.Add(ol::Vec2D(plotx-(taille_x/4),ploty-(taille_y/3)));
carcasse2.Add(ol::Vec2D(plotx+(taille_x/4),ploty-(taille_y/3)));
carcasse2.Add(ol::Vec2D(plotx+(taille_x/3)-(size_symbol*6),ploty));
carcasse2.Add(ol::Vec2D(plotx-(taille_x/3)+(size_symbol*6),ploty));

carcasse2.SetPivot(ol::Vec2D(plotx,ploty));
carcasse2.RotateBy(angle_pc*(6.5));


ol::Poly carcasse3;
carcasse3.Add(ol::Vec2D(plotx-(taille_x/2)-(size_symbol*6),ploty+(taille_y/2)+(size_symbol*4)));
carcasse3.Add(ol::Vec2D(plotx+(taille_x/2)+(size_symbol*6),ploty+(taille_y/2)+(size_symbol*4)));
carcasse3.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
carcasse3.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));

carcasse3.SetPivot(ol::Vec2D(plotx,ploty));
carcasse3.RotateBy(angle_pc*(6.5));



carcasse.Draw(CouleurPlotLine);
carcasse2.Draw(CouleurPlotFill);
carcasse3.Draw(CouleurPlotFill);

if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
carcasse2.Draw(CouleurFader);
carcasse3.Draw(CouleurFader);
}
carcasse.DrawOutline(CouleurPlotLine);
carcasse2.DrawOutline(CouleurPlotLine);
carcasse3.DrawOutline(CouleurPlotLine);
 return(0);
}

int plot_draw_symbol_platine_de_sol(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{

float taille_x=40.0*size_symbol;
float taille_y=60.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));

carcasse.Draw(CouleurPlotFill);

if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
}
carcasse.DrawOutline(CouleurPlotLine);

 return(0);
}

int plot_draw_symbol_littlestand(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=200.0*size_symbol;
float taille_y=200.0*size_symbol;

ol::Poly tourelle;
tourelle.Add(ol::Vec2D(plotx-(12*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
tourelle.Add(ol::Vec2D(plotx+(12*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
tourelle.Add(ol::Vec2D(plotx+(12*size_symbol),ploty-(taille_y/2)));
tourelle.Add(ol::Vec2D(plotx-(12*size_symbol),ploty-(taille_y/2)));
tourelle.SetPivot(ol::Vec2D(plotx,ploty));
tourelle.RotateBy(angle_pc*(6.5));

ol::Poly carcasse1;
carcasse1.Add(ol::Vec2D(plotx-(4*size_symbol),ploty-(taille_y/2)));
carcasse1.Add(ol::Vec2D(plotx+(4*size_symbol),ploty-(taille_y/2)));
carcasse1.Add(ol::Vec2D(plotx+(4*size_symbol),ploty));
carcasse1.Add(ol::Vec2D(plotx-(4*size_symbol),ploty));
carcasse1.SetPivot(ol::Vec2D(plotx,ploty));
carcasse1.RotateBy(angle_pc*(6.5));

ol::Poly carcasse2;
carcasse2.Add(ol::Vec2D(plotx-(taille_x/4)-(4*size_symbol),ploty+(taille_y/3)));
carcasse2.Add(ol::Vec2D(plotx-(taille_x/4)+(4*size_symbol),ploty+(taille_y/3)));
carcasse2.Add(ol::Vec2D(plotx+(4*size_symbol),ploty));
carcasse2.Add(ol::Vec2D(plotx-(4*size_symbol),ploty));
carcasse2.SetPivot(ol::Vec2D(plotx,ploty));
carcasse2.RotateBy(angle_pc*(6.5));

ol::Poly carcasse3;
carcasse3.Add(ol::Vec2D(plotx+(taille_x/4)-(4*size_symbol),ploty+(taille_y/3)));
carcasse3.Add(ol::Vec2D(plotx+(taille_x/4)+(4*size_symbol),ploty+(taille_y/3)));
carcasse3.Add(ol::Vec2D(plotx+(4*size_symbol),ploty));
carcasse3.Add(ol::Vec2D(plotx-(4*size_symbol),ploty));
carcasse3.SetPivot(ol::Vec2D(plotx,ploty));
carcasse3.RotateBy(angle_pc*(6.5));

tourelle.Draw(CouleurPlotLine);
carcasse1.Draw(CouleurPlotLine);
carcasse2.Draw(CouleurPlotLine);
carcasse3.Draw(CouleurPlotLine);

if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
tourelle.Draw(CouleurFader);
carcasse1.Draw(CouleurFader);
carcasse2.Draw(CouleurFader);
carcasse3.Draw(CouleurFader);
}


 return(0);
}

int plot_draw_symbol_bigstand(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=200.0*size_symbol;
float taille_y=200.0*size_symbol;

ol::Poly tourelle;
tourelle.Add(ol::Vec2D(plotx-(18*size_symbol),ploty-(taille_y)-(4*size_symbol)));
tourelle.Add(ol::Vec2D(plotx+(18*size_symbol),ploty-(taille_y)-(4*size_symbol)));
tourelle.Add(ol::Vec2D(plotx+(18*size_symbol),ploty-(taille_y)));
tourelle.Add(ol::Vec2D(plotx-(18*size_symbol),ploty-(taille_y)));
tourelle.SetPivot(ol::Vec2D(plotx,ploty));
tourelle.RotateBy(angle_pc*(6.5));

ol::Poly carcasse1;
carcasse1.Add(ol::Vec2D(plotx-(4*size_symbol),ploty-(taille_y)));
carcasse1.Add(ol::Vec2D(plotx+(4*size_symbol),ploty-(taille_y)));
carcasse1.Add(ol::Vec2D(plotx+(4*size_symbol),ploty));
carcasse1.Add(ol::Vec2D(plotx-(4*size_symbol),ploty));
carcasse1.SetPivot(ol::Vec2D(plotx,ploty));
carcasse1.RotateBy(angle_pc*(6.5));

ol::Poly carcasse2;
carcasse2.Add(ol::Vec2D(plotx-(taille_x/3)-(4*size_symbol),ploty+(taille_y/2)));
carcasse2.Add(ol::Vec2D(plotx-(taille_x/3)+(4*size_symbol),ploty+(taille_y/2)));
carcasse2.Add(ol::Vec2D(plotx+(4*size_symbol),ploty));
carcasse2.Add(ol::Vec2D(plotx-(4*size_symbol),ploty));
carcasse2.SetPivot(ol::Vec2D(plotx,ploty));
carcasse2.RotateBy(angle_pc*(6.5));

ol::Poly carcasse3;
carcasse3.Add(ol::Vec2D(plotx+(taille_x/3)-(4*size_symbol),ploty+(taille_y/2)));
carcasse3.Add(ol::Vec2D(plotx+(taille_x/3)+(4*size_symbol),ploty+(taille_y/2)));
carcasse3.Add(ol::Vec2D(plotx+(4*size_symbol),ploty));
carcasse3.Add(ol::Vec2D(plotx-(4*size_symbol),ploty));
carcasse3.SetPivot(ol::Vec2D(plotx,ploty));
carcasse3.RotateBy(angle_pc*(6.5));

tourelle.Draw(CouleurPlotLine);
carcasse1.Draw(CouleurPlotLine);
carcasse2.Draw(CouleurPlotLine);
carcasse3.Draw(CouleurPlotLine);

if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
tourelle.Draw(CouleurFader);
carcasse1.Draw(CouleurFader);
carcasse2.Draw(CouleurFader);
carcasse3.Draw(CouleurFader);
}

 return(0);
}

int  plot_draw_symbol_barre_de_couplage(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=200.0*size_symbol;
float taille_y=200.0*size_symbol;
ol::Poly tourelle;
tourelle.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(10*size_symbol)));
tourelle.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(10*size_symbol)));
tourelle.Add(ol::Vec2D(plotx+(taille_x/2),ploty));
tourelle.Add(ol::Vec2D(plotx-(taille_x/2),ploty));
tourelle.SetPivot(ol::Vec2D(plotx,ploty));
tourelle.RotateBy(angle_pc*(6.5));

tourelle.Draw(CouleurPlotLine);

if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
tourelle.Draw(CouleurFader);
tourelle.DrawOutline(CouleurPlotLine);
}


 return(0);
}



int plot_draw_symbol_echelle(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=60.0*size_symbol;
float taille_y=240.0*size_symbol;
//barreau haut
ol::Poly tourelle;
tourelle.Add(ol::Vec2D(plotx-(taille_x/2)-(4*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
tourelle.Add(ol::Vec2D(plotx+(taille_x/2)+(4*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
tourelle.Add(ol::Vec2D(plotx+(taille_x/2)+(4*size_symbol),ploty-(taille_y/2)+(4*size_symbol)));
tourelle.Add(ol::Vec2D(plotx-(taille_x/2)-(4*size_symbol),ploty-(taille_y/2)+(4*size_symbol)));
tourelle.SetPivot(ol::Vec2D(plotx,ploty));
tourelle.RotateBy(angle_pc*(6.5));
//pieds
ol::Poly carcasse1;
carcasse1.Add(ol::Vec2D(plotx-(4*size_symbol)+(taille_x/2),ploty-(taille_y/2)));
carcasse1.Add(ol::Vec2D(plotx+(4*size_symbol)+(taille_x/2),ploty-(taille_y/2)));
carcasse1.Add(ol::Vec2D(plotx+(4*size_symbol)+(taille_x/2),ploty+(taille_y/2)));
carcasse1.Add(ol::Vec2D(plotx-(4*size_symbol)+(taille_x/2),ploty+(taille_y/2)));
carcasse1.SetPivot(ol::Vec2D(plotx,ploty));
carcasse1.RotateBy(angle_pc*(6.5));

ol::Poly carcasse2;
carcasse2.Add(ol::Vec2D(plotx-(4*size_symbol)-(taille_x/2),ploty-(taille_y/2)));
carcasse2.Add(ol::Vec2D(plotx+(4*size_symbol)-(taille_x/2),ploty-(taille_y/2)));
carcasse2.Add(ol::Vec2D(plotx+(4*size_symbol)-(taille_x/2),ploty+(taille_y/2)));
carcasse2.Add(ol::Vec2D(plotx-(4*size_symbol)-(taille_x/2),ploty+(taille_y/2)));
carcasse2.SetPivot(ol::Vec2D(plotx,ploty));
carcasse2.RotateBy(angle_pc*(6.5));

//barreaux
ol::Poly carcasse3;
carcasse3.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/6)-(4*size_symbol)));
carcasse3.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/6)-(4*size_symbol)));
carcasse3.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/6)+(4*size_symbol)));
carcasse3.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/6)+(4*size_symbol)));
carcasse3.SetPivot(ol::Vec2D(plotx,ploty));
carcasse3.RotateBy(angle_pc*(6.5));

ol::Poly carcasse4;
carcasse4.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/5)-(4*size_symbol)));
carcasse4.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/5)-(4*size_symbol)));
carcasse4.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/5)+(4*size_symbol)));
carcasse4.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/5)+(4*size_symbol)));
carcasse4.SetPivot(ol::Vec2D(plotx,ploty));
carcasse4.RotateBy(angle_pc*(6.5));

tourelle.Draw(CouleurPlotLine);
carcasse1.Draw(CouleurPlotLine);
carcasse2.Draw(CouleurPlotLine);
carcasse3.Draw(CouleurPlotLine);
carcasse4.Draw(CouleurPlotLine);

if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
tourelle.Draw(CouleurFader);
carcasse1.Draw(CouleurFader);
carcasse2.Draw(CouleurFader);
carcasse3.Draw(CouleurFader);
carcasse4.Draw(CouleurFader);
}

 return(0);
}


int plot_draw_symbol_pont50_1m(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=50.0*size_symbol;
float taille_y=92.0*size_symbol;

//side
ol::Poly side1;
side1.Add(ol::Vec2D(plotx-(taille_x/2)-(4*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
side1.Add(ol::Vec2D(plotx-(taille_x/2)+(4*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
side1.Add(ol::Vec2D(plotx-(taille_x/2)+(4*size_symbol),ploty+(taille_y/2)+(4*size_symbol)));
side1.Add(ol::Vec2D(plotx-(taille_x/2)-(4*size_symbol),ploty+(taille_y/2)+(4*size_symbol)));
side1.SetPivot(ol::Vec2D(plotx,ploty));
side1.RotateBy(angle_pc*(6.5));

ol::Poly side2;
side2.Add(ol::Vec2D(plotx+(taille_x/2)-(4*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
side2.Add(ol::Vec2D(plotx+(taille_x/2)+(4*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
side2.Add(ol::Vec2D(plotx+(taille_x/2)+(4*size_symbol),ploty+(taille_y/2)+(4*size_symbol)));
side2.Add(ol::Vec2D(plotx+(taille_x/2)-(4*size_symbol),ploty+(taille_y/2)+(4*size_symbol)));
side2.SetPivot(ol::Vec2D(plotx,ploty));
side2.RotateBy(angle_pc*(6.5));

//barreau haut
ol::Poly barreauhaut;
barreauhaut.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)+(4*size_symbol)));
barreauhaut.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)+(8*size_symbol)));
barreauhaut.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)+(8*size_symbol)));
barreauhaut.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)+(4*size_symbol)));
barreauhaut.SetPivot(ol::Vec2D(plotx,ploty));
barreauhaut.RotateBy(angle_pc*(6.5));

//barreau bas
ol::Poly barreaubas;
barreaubas.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)-(4*size_symbol)));
barreaubas.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)-(8*size_symbol)));
barreaubas.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)-(8*size_symbol)));
barreaubas.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)-(4*size_symbol)));
barreaubas.SetPivot(ol::Vec2D(plotx,ploty));
barreaubas.RotateBy(angle_pc*(6.5));

//traverses
ol::Poly traverse;
traverse.Add(ol::Vec2D(plotx-(taille_x/2)-(3*size_symbol),ploty-(taille_y/2)+(4*size_symbol)));
traverse.Add(ol::Vec2D(plotx-(taille_x/2)+(3*size_symbol),ploty-(taille_y/2)+(4*size_symbol)));
traverse.Add(ol::Vec2D(plotx+(taille_x/2)+(3*size_symbol),ploty+(taille_y/2)-(4*size_symbol)));
traverse.Add(ol::Vec2D(plotx+(taille_x/2)-(3*size_symbol),ploty+(taille_y/2)-(4*size_symbol)));
traverse.SetPivot(ol::Vec2D(plotx,ploty));
traverse.RotateBy(angle_pc*(6.5));

barreauhaut.Draw(CouleurPlotLine);
side1.Draw(CouleurPlotLine);
side2.Draw(CouleurPlotLine);
barreaubas.Draw(CouleurPlotLine);
traverse.Draw(CouleurPlotLine);

if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
side1.Draw(CouleurFader);
side2.Draw(CouleurFader);
barreauhaut.Draw(CouleurFader);
barreaubas.Draw(CouleurFader);
traverse.Draw(CouleurFader);
}

 return(0);
}



int plot_draw_symbol_pont50_3m(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=50.0*size_symbol;
float taille_y=292.0*size_symbol;

//side
ol::Poly side1;
side1.Add(ol::Vec2D(plotx-(taille_x/2)-(4*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
side1.Add(ol::Vec2D(plotx-(taille_x/2)+(4*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
side1.Add(ol::Vec2D(plotx-(taille_x/2)+(4*size_symbol),ploty+(taille_y/2)+(4*size_symbol)));
side1.Add(ol::Vec2D(plotx-(taille_x/2)-(4*size_symbol),ploty+(taille_y/2)+(4*size_symbol)));
side1.SetPivot(ol::Vec2D(plotx,ploty));
side1.RotateBy(angle_pc*(6.5));

ol::Poly side2;
side2.Add(ol::Vec2D(plotx+(taille_x/2)-(4*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
side2.Add(ol::Vec2D(plotx+(taille_x/2)+(4*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
side2.Add(ol::Vec2D(plotx+(taille_x/2)+(4*size_symbol),ploty+(taille_y/2)+(4*size_symbol)));
side2.Add(ol::Vec2D(plotx+(taille_x/2)-(4*size_symbol),ploty+(taille_y/2)+(4*size_symbol)));
side2.SetPivot(ol::Vec2D(plotx,ploty));
side2.RotateBy(angle_pc*(6.5));

//barreau haut
ol::Poly barreauhaut;
barreauhaut.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)+(4*size_symbol)));
barreauhaut.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)+(8*size_symbol)));
barreauhaut.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)+(8*size_symbol)));
barreauhaut.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)+(4*size_symbol)));
barreauhaut.SetPivot(ol::Vec2D(plotx,ploty));
barreauhaut.RotateBy(angle_pc*(6.5));

//barreau bas
ol::Poly barreaubas;
barreaubas.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)-(4*size_symbol)));
barreaubas.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)-(8*size_symbol)));
barreaubas.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)-(8*size_symbol)));
barreaubas.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)-(4*size_symbol)));
barreaubas.SetPivot(ol::Vec2D(plotx,ploty));
barreaubas.RotateBy(angle_pc*(6.5));

//traverses
ol::Poly traverse;
traverse.Add(ol::Vec2D(plotx-(taille_x/2)-(3*size_symbol),ploty-(taille_y/2)+(4*size_symbol)));
traverse.Add(ol::Vec2D(plotx-(taille_x/2)+(3*size_symbol),ploty-(taille_y/2)+(4*size_symbol)));
traverse.Add(ol::Vec2D(plotx+(taille_x/2)+(3*size_symbol),ploty-(taille_y/2)+((taille_y/3))-(4*size_symbol)));
traverse.Add(ol::Vec2D(plotx+(taille_x/2)-(3*size_symbol),ploty-(taille_y/2)+((taille_y/3))-(4*size_symbol)));
traverse.SetPivot(ol::Vec2D(plotx,ploty));
traverse.RotateBy(angle_pc*(6.5));

ol::Poly traverse1;
traverse1.Add(ol::Vec2D(plotx+(taille_x/2)-(3*size_symbol),ploty-(taille_y/2)+((taille_y/3)*2)+(4*size_symbol)));
traverse1.Add(ol::Vec2D(plotx+(taille_x/2)+(3*size_symbol),ploty-(taille_y/2)+((taille_y/3)*2)+(4*size_symbol)));
traverse1.Add(ol::Vec2D(plotx-(taille_x/2)+(3*size_symbol),ploty-(taille_y/2)+((taille_y/3))-(4*size_symbol)));
traverse1.Add(ol::Vec2D(plotx-(taille_x/2)-(3*size_symbol),ploty-(taille_y/2)+((taille_y/3))-(4*size_symbol)));
traverse1.SetPivot(ol::Vec2D(plotx,ploty));
traverse1.RotateBy(angle_pc*(6.5));


ol::Poly traverse2;
traverse2.Add(ol::Vec2D(plotx-(taille_x/2)-(3*size_symbol),ploty-(taille_y/2)+((taille_y/3)*2)+(4*size_symbol)));
traverse2.Add(ol::Vec2D(plotx-(taille_x/2)+(3*size_symbol),ploty-(taille_y/2)+((taille_y/3)*2)+(4*size_symbol)));
traverse2.Add(ol::Vec2D(plotx+(taille_x/2)+(3*size_symbol),ploty-(taille_y/2)+((taille_y/3)*3)-(4*size_symbol)));
traverse2.Add(ol::Vec2D(plotx+(taille_x/2)-(3*size_symbol),ploty-(taille_y/2)+((taille_y/3)*3)-(4*size_symbol)));
traverse2.SetPivot(ol::Vec2D(plotx,ploty));
traverse2.RotateBy(angle_pc*(6.5));


ol::Poly barreau1;
barreau1.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)+(taille_y/3)-(4*size_symbol)));
barreau1.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)+(taille_y/3)-(8*size_symbol)));
barreau1.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)+(taille_y/3)-(8*size_symbol)));
barreau1.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)+(taille_y/3)-(4*size_symbol)));
barreau1.SetPivot(ol::Vec2D(plotx,ploty));
barreau1.RotateBy(angle_pc*(6.5));


ol::Poly barreau2;
barreau2.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)+((taille_y/3)*2)-(4*size_symbol)));
barreau2.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)+((taille_y/3)*2)-(8*size_symbol)));
barreau2.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)+((taille_y/3)*2)-(8*size_symbol)));
barreau2.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)+((taille_y/3)*2)-(4*size_symbol)));
barreau2.SetPivot(ol::Vec2D(plotx,ploty));
barreau2.RotateBy(angle_pc*(6.5));

barreauhaut.Draw(CouleurPlotLine);
side1.Draw(CouleurPlotLine);
side2.Draw(CouleurPlotLine);
barreaubas.Draw(CouleurPlotLine);
traverse.Draw(CouleurPlotLine);
traverse1.Draw(CouleurPlotLine);
traverse2.Draw(CouleurPlotLine);
barreau1.Draw(CouleurPlotLine);
barreau2.Draw(CouleurPlotLine);
if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
side1.Draw(CouleurFader);
side2.Draw(CouleurFader);
barreauhaut.Draw(CouleurFader);
barreaubas.Draw(CouleurFader);
traverse.Draw(CouleurFader);
traverse1.Draw(CouleurFader);
traverse2.Draw(CouleurFader);
barreau1.Draw(CouleurFader);
barreau2.Draw(CouleurFader);
}

return(0);
}


int plot_draw_symbol_pont50_jonction(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=42.0*size_symbol;
float taille_y=42.0*size_symbol;

//side
ol::Poly side1;
side1.Add(ol::Vec2D(plotx-(taille_x/2)-(4*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
side1.Add(ol::Vec2D(plotx-(taille_x/2)+(4*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
side1.Add(ol::Vec2D(plotx-(taille_x/2)+(4*size_symbol),ploty+(taille_y/2)+(4*size_symbol)));
side1.Add(ol::Vec2D(plotx-(taille_x/2)-(4*size_symbol),ploty+(taille_y/2)+(4*size_symbol)));
side1.SetPivot(ol::Vec2D(plotx,ploty));
side1.RotateBy(angle_pc*(6.5));

ol::Poly side2;
side2.Add(ol::Vec2D(plotx+(taille_x/2)-(4*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
side2.Add(ol::Vec2D(plotx+(taille_x/2)+(4*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
side2.Add(ol::Vec2D(plotx+(taille_x/2)+(4*size_symbol),ploty+(taille_y/2)+(4*size_symbol)));
side2.Add(ol::Vec2D(plotx+(taille_x/2)-(4*size_symbol),ploty+(taille_y/2)+(4*size_symbol)));
side2.SetPivot(ol::Vec2D(plotx,ploty));
side2.RotateBy(angle_pc*(6.5));

//barreau haut
ol::Poly barreauhaut;
barreauhaut.Add(ol::Vec2D(plotx-(taille_x/2)-(8*size_symbol),ploty-(taille_y/2)+(4*size_symbol)));
barreauhaut.Add(ol::Vec2D(plotx-(taille_x/2)-(8*size_symbol),ploty-(taille_y/2)+(8*size_symbol)));
barreauhaut.Add(ol::Vec2D(plotx+(taille_x/2)+(8*size_symbol),ploty-(taille_y/2)+(8*size_symbol)));
barreauhaut.Add(ol::Vec2D(plotx+(taille_x/2)+(8*size_symbol),ploty-(taille_y/2)+(4*size_symbol)));
barreauhaut.SetPivot(ol::Vec2D(plotx,ploty));
barreauhaut.RotateBy(angle_pc*(6.5));

//barreau bas
ol::Poly barreaubas;
barreaubas.Add(ol::Vec2D(plotx-(taille_x/2)-(8*size_symbol),ploty+(taille_y/2)-(4*size_symbol)));
barreaubas.Add(ol::Vec2D(plotx-(taille_x/2)-(8*size_symbol),ploty+(taille_y/2)-(8*size_symbol)));
barreaubas.Add(ol::Vec2D(plotx+(taille_x/2)+(8*size_symbol),ploty+(taille_y/2)-(8*size_symbol)));
barreaubas.Add(ol::Vec2D(plotx+(taille_x/2)+(8*size_symbol),ploty+(taille_y/2)-(4*size_symbol)));
barreaubas.SetPivot(ol::Vec2D(plotx,ploty));
barreaubas.RotateBy(angle_pc*(6.5));

barreauhaut.Draw(CouleurPlotLine);
side1.Draw(CouleurPlotLine);
side2.Draw(CouleurPlotLine);
barreaubas.Draw(CouleurPlotLine);

if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
side1.Draw(CouleurFader);
side2.Draw(CouleurFader);
barreauhaut.Draw(CouleurFader);
barreaubas.Draw(CouleurFader);
}

 return(0);
}


int plot_draw_symbol_pont30_1m(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=30.0*size_symbol;
float taille_y=92.0*size_symbol;

//side
ol::Poly side1;
side1.Add(ol::Vec2D(plotx-(taille_x/2)-(4*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
side1.Add(ol::Vec2D(plotx-(taille_x/2)+(4*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
side1.Add(ol::Vec2D(plotx-(taille_x/2)+(4*size_symbol),ploty+(taille_y/2)+(4*size_symbol)));
side1.Add(ol::Vec2D(plotx-(taille_x/2)-(4*size_symbol),ploty+(taille_y/2)+(4*size_symbol)));
side1.SetPivot(ol::Vec2D(plotx,ploty));
side1.RotateBy(angle_pc*(6.5));

ol::Poly side2;
side2.Add(ol::Vec2D(plotx+(taille_x/2)-(4*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
side2.Add(ol::Vec2D(plotx+(taille_x/2)+(4*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
side2.Add(ol::Vec2D(plotx+(taille_x/2)+(4*size_symbol),ploty+(taille_y/2)+(4*size_symbol)));
side2.Add(ol::Vec2D(plotx+(taille_x/2)-(4*size_symbol),ploty+(taille_y/2)+(4*size_symbol)));
side2.SetPivot(ol::Vec2D(plotx,ploty));
side2.RotateBy(angle_pc*(6.5));

//barreau haut
ol::Poly barreauhaut;
barreauhaut.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)+(4*size_symbol)));
barreauhaut.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)+(8*size_symbol)));
barreauhaut.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)+(8*size_symbol)));
barreauhaut.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)+(4*size_symbol)));
barreauhaut.SetPivot(ol::Vec2D(plotx,ploty));
barreauhaut.RotateBy(angle_pc*(6.5));

//barreau bas
ol::Poly barreaubas;
barreaubas.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)-(4*size_symbol)));
barreaubas.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)-(8*size_symbol)));
barreaubas.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)-(8*size_symbol)));
barreaubas.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)-(4*size_symbol)));
barreaubas.SetPivot(ol::Vec2D(plotx,ploty));
barreaubas.RotateBy(angle_pc*(6.5));

//traverses
ol::Poly traverse;
traverse.Add(ol::Vec2D(plotx-(taille_x/2)-(3*size_symbol),ploty-(taille_y/2)+(4*size_symbol)));
traverse.Add(ol::Vec2D(plotx-(taille_x/2)+(3*size_symbol),ploty-(taille_y/2)+(4*size_symbol)));
traverse.Add(ol::Vec2D(plotx+(taille_x/2)+(3*size_symbol),ploty+(taille_y/2)-(4*size_symbol)));
traverse.Add(ol::Vec2D(plotx+(taille_x/2)-(3*size_symbol),ploty+(taille_y/2)-(4*size_symbol)));
traverse.SetPivot(ol::Vec2D(plotx,ploty));
traverse.RotateBy(angle_pc*(6.5));

barreauhaut.Draw(CouleurPlotLine);
side1.Draw(CouleurPlotLine);
side2.Draw(CouleurPlotLine);
barreaubas.Draw(CouleurPlotLine);
traverse.Draw(CouleurPlotLine);

if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
side1.Draw(CouleurFader);
side2.Draw(CouleurFader);
barreauhaut.Draw(CouleurFader);
barreaubas.Draw(CouleurFader);
traverse.Draw(CouleurFader);
}

 return(0);
}



int plot_draw_symbol_pont30_3m(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=30.0*size_symbol;
float taille_y=292.0*size_symbol;

//side
ol::Poly side1;
side1.Add(ol::Vec2D(plotx-(taille_x/2)-(4*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
side1.Add(ol::Vec2D(plotx-(taille_x/2)+(4*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
side1.Add(ol::Vec2D(plotx-(taille_x/2)+(4*size_symbol),ploty+(taille_y/2)+(4*size_symbol)));
side1.Add(ol::Vec2D(plotx-(taille_x/2)-(4*size_symbol),ploty+(taille_y/2)+(4*size_symbol)));
side1.SetPivot(ol::Vec2D(plotx,ploty));
side1.RotateBy(angle_pc*(6.5));

ol::Poly side2;
side2.Add(ol::Vec2D(plotx+(taille_x/2)-(4*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
side2.Add(ol::Vec2D(plotx+(taille_x/2)+(4*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
side2.Add(ol::Vec2D(plotx+(taille_x/2)+(4*size_symbol),ploty+(taille_y/2)+(4*size_symbol)));
side2.Add(ol::Vec2D(plotx+(taille_x/2)-(4*size_symbol),ploty+(taille_y/2)+(4*size_symbol)));
side2.SetPivot(ol::Vec2D(plotx,ploty));
side2.RotateBy(angle_pc*(6.5));

//barreau haut
ol::Poly barreauhaut;
barreauhaut.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)+(4*size_symbol)));
barreauhaut.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)+(8*size_symbol)));
barreauhaut.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)+(8*size_symbol)));
barreauhaut.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)+(4*size_symbol)));
barreauhaut.SetPivot(ol::Vec2D(plotx,ploty));
barreauhaut.RotateBy(angle_pc*(6.5));

//barreau bas
ol::Poly barreaubas;
barreaubas.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)-(4*size_symbol)));
barreaubas.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y/2)-(8*size_symbol)));
barreaubas.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)-(8*size_symbol)));
barreaubas.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y/2)-(4*size_symbol)));
barreaubas.SetPivot(ol::Vec2D(plotx,ploty));
barreaubas.RotateBy(angle_pc*(6.5));

//traverses
ol::Poly traverse;
traverse.Add(ol::Vec2D(plotx-(taille_x/2)-(3*size_symbol),ploty-(taille_y/2)+(4*size_symbol)));
traverse.Add(ol::Vec2D(plotx-(taille_x/2)+(3*size_symbol),ploty-(taille_y/2)+(4*size_symbol)));
traverse.Add(ol::Vec2D(plotx+(taille_x/2)+(3*size_symbol),ploty-(taille_y/2)+((taille_y/3))-(4*size_symbol)));
traverse.Add(ol::Vec2D(plotx+(taille_x/2)-(3*size_symbol),ploty-(taille_y/2)+((taille_y/3))-(4*size_symbol)));
traverse.SetPivot(ol::Vec2D(plotx,ploty));
traverse.RotateBy(angle_pc*(6.5));

ol::Poly traverse1;
traverse1.Add(ol::Vec2D(plotx+(taille_x/2)-(3*size_symbol),ploty-(taille_y/2)+((taille_y/3)*2)+(4*size_symbol)));
traverse1.Add(ol::Vec2D(plotx+(taille_x/2)+(3*size_symbol),ploty-(taille_y/2)+((taille_y/3)*2)+(4*size_symbol)));
traverse1.Add(ol::Vec2D(plotx-(taille_x/2)+(3*size_symbol),ploty-(taille_y/2)+((taille_y/3))-(4*size_symbol)));
traverse1.Add(ol::Vec2D(plotx-(taille_x/2)-(3*size_symbol),ploty-(taille_y/2)+((taille_y/3))-(4*size_symbol)));
traverse1.SetPivot(ol::Vec2D(plotx,ploty));
traverse1.RotateBy(angle_pc*(6.5));


ol::Poly traverse2;
traverse2.Add(ol::Vec2D(plotx-(taille_x/2)-(3*size_symbol),ploty-(taille_y/2)+((taille_y/3)*2)+(4*size_symbol)));
traverse2.Add(ol::Vec2D(plotx-(taille_x/2)+(3*size_symbol),ploty-(taille_y/2)+((taille_y/3)*2)+(4*size_symbol)));
traverse2.Add(ol::Vec2D(plotx+(taille_x/2)+(3*size_symbol),ploty-(taille_y/2)+((taille_y/3)*3)-(4*size_symbol)));
traverse2.Add(ol::Vec2D(plotx+(taille_x/2)-(3*size_symbol),ploty-(taille_y/2)+((taille_y/3)*3)-(4*size_symbol)));
traverse2.SetPivot(ol::Vec2D(plotx,ploty));
traverse2.RotateBy(angle_pc*(6.5));


ol::Poly barreau1;
barreau1.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)+(taille_y/3)-(4*size_symbol)));
barreau1.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)+(taille_y/3)-(8*size_symbol)));
barreau1.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)+(taille_y/3)-(8*size_symbol)));
barreau1.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)+(taille_y/3)-(4*size_symbol)));
barreau1.SetPivot(ol::Vec2D(plotx,ploty));
barreau1.RotateBy(angle_pc*(6.5));


ol::Poly barreau2;
barreau2.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)+((taille_y/3)*2)-(4*size_symbol)));
barreau2.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y/2)+((taille_y/3)*2)-(8*size_symbol)));
barreau2.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)+((taille_y/3)*2)-(8*size_symbol)));
barreau2.Add(ol::Vec2D(plotx+(taille_x/2),ploty-(taille_y/2)+((taille_y/3)*2)-(4*size_symbol)));
barreau2.SetPivot(ol::Vec2D(plotx,ploty));
barreau2.RotateBy(angle_pc*(6.5));

barreauhaut.Draw(CouleurPlotLine);
side1.Draw(CouleurPlotLine);
side2.Draw(CouleurPlotLine);
barreaubas.Draw(CouleurPlotLine);
traverse.Draw(CouleurPlotLine);
traverse1.Draw(CouleurPlotLine);
traverse2.Draw(CouleurPlotLine);
barreau1.Draw(CouleurPlotLine);
barreau2.Draw(CouleurPlotLine);
if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
side1.Draw(CouleurFader);
side2.Draw(CouleurFader);
barreauhaut.Draw(CouleurFader);
barreaubas.Draw(CouleurFader);
traverse.Draw(CouleurFader);
traverse1.Draw(CouleurFader);
traverse2.Draw(CouleurFader);
barreau1.Draw(CouleurFader);
barreau2.Draw(CouleurFader);
}

return(0);
}


int plot_draw_symbol_pont30_jonction(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{
float taille_x=22.0*size_symbol;
float taille_y=22.0*size_symbol;

//side
ol::Poly side1;
side1.Add(ol::Vec2D(plotx-(taille_x/2)-(4*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
side1.Add(ol::Vec2D(plotx-(taille_x/2)+(4*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
side1.Add(ol::Vec2D(plotx-(taille_x/2)+(4*size_symbol),ploty+(taille_y/2)+(4*size_symbol)));
side1.Add(ol::Vec2D(plotx-(taille_x/2)-(4*size_symbol),ploty+(taille_y/2)+(4*size_symbol)));
side1.SetPivot(ol::Vec2D(plotx,ploty));
side1.RotateBy(angle_pc*(6.5));

ol::Poly side2;
side2.Add(ol::Vec2D(plotx+(taille_x/2)-(4*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
side2.Add(ol::Vec2D(plotx+(taille_x/2)+(4*size_symbol),ploty-(taille_y/2)-(4*size_symbol)));
side2.Add(ol::Vec2D(plotx+(taille_x/2)+(4*size_symbol),ploty+(taille_y/2)+(4*size_symbol)));
side2.Add(ol::Vec2D(plotx+(taille_x/2)-(4*size_symbol),ploty+(taille_y/2)+(4*size_symbol)));
side2.SetPivot(ol::Vec2D(plotx,ploty));
side2.RotateBy(angle_pc*(6.5));

//barreau haut
ol::Poly barreauhaut;
barreauhaut.Add(ol::Vec2D(plotx-(taille_x/2)-(8*size_symbol),ploty-(taille_y/2)+(4*size_symbol)));
barreauhaut.Add(ol::Vec2D(plotx-(taille_x/2)-(8*size_symbol),ploty-(taille_y/2)+(8*size_symbol)));
barreauhaut.Add(ol::Vec2D(plotx+(taille_x/2)+(8*size_symbol),ploty-(taille_y/2)+(8*size_symbol)));
barreauhaut.Add(ol::Vec2D(plotx+(taille_x/2)+(8*size_symbol),ploty-(taille_y/2)+(4*size_symbol)));
barreauhaut.SetPivot(ol::Vec2D(plotx,ploty));
barreauhaut.RotateBy(angle_pc*(6.5));

//barreau bas
ol::Poly barreaubas;
barreaubas.Add(ol::Vec2D(plotx-(taille_x/2)-(8*size_symbol),ploty+(taille_y/2)-(4*size_symbol)));
barreaubas.Add(ol::Vec2D(plotx-(taille_x/2)-(8*size_symbol),ploty+(taille_y/2)-(8*size_symbol)));
barreaubas.Add(ol::Vec2D(plotx+(taille_x/2)+(8*size_symbol),ploty+(taille_y/2)-(8*size_symbol)));
barreaubas.Add(ol::Vec2D(plotx+(taille_x/2)+(8*size_symbol),ploty+(taille_y/2)-(4*size_symbol)));
barreaubas.SetPivot(ol::Vec2D(plotx,ploty));
barreaubas.RotateBy(angle_pc*(6.5));

barreauhaut.Draw(CouleurPlotLine);
side1.Draw(CouleurPlotLine);
side2.Draw(CouleurPlotLine);
barreaubas.Draw(CouleurPlotLine);

if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
side1.Draw(CouleurFader);
side2.Draw(CouleurFader);
barreauhaut.Draw(CouleurFader);
barreaubas.Draw(CouleurFader);
}

 return(0);
}



int plot_draw_symbol_smokemachine(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{

float taille_x=60.0*size_symbol;
float taille_y=30.0*size_symbol;

ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty-(taille_y)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2)+(15*size_symbol),ploty-(taille_y)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2)+(15*size_symbol),ploty-(taille_y)+(10*size_symbol)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2)+(30*size_symbol),ploty-(taille_y)+(10*size_symbol)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2)+(30*size_symbol),ploty-(taille_y)+(20*size_symbol)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2)+(20*size_symbol),ploty));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2)+(15*size_symbol),ploty));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2)+(15*size_symbol),ploty+(taille_y)));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y)));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));


carcasse.Draw(CouleurPlotFill);
carcasse.DrawOutline(CouleurPlotLine);

if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
carcasse.DrawOutline(CouleurPlotLine);
}


return(0);
}

int plot_draw_symbol_fogmachine(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{

float taille_x=40.0*size_symbol;
float taille_y=20.0*size_symbol;

ol::Circle lentille(ol::Vec2D(plotx,ploty),taille_x);
lentille.RotateBy(-angle_pc*6.5);
lentille.SetRadius(taille_x/2);
lentille.DrawSlice(CouleurPlotLine,0, PI);



ol::Poly carcasse;
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty));
carcasse.Add(ol::Vec2D(plotx-(taille_x/2),ploty+(taille_y)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty+(taille_y)));
carcasse.Add(ol::Vec2D(plotx+(taille_x/2),ploty));
carcasse.SetPivot(ol::Vec2D(plotx,ploty));
carcasse.RotateBy(angle_pc*(6.5));
carcasse.Draw(CouleurPlotFill);
carcasse.DrawOutline(CouleurPlotLine);
lentille.SetRadius((taille_x/2)-(4*size_symbol));
lentille.Draw(CouleurPlotFill);

ol::Poly Carre;
Carre.Add(ol::Vec2D(plotx-(taille_x/4),ploty+(taille_y)));
Carre.Add(ol::Vec2D(plotx+(taille_x/4),ploty+(taille_y)));
Carre.Add(ol::Vec2D(plotx+(taille_x/4),ploty+(taille_y)-( size_symbol*10)));
Carre.Add(ol::Vec2D(plotx-(taille_x/4),ploty+(taille_y)-( size_symbol*10)));
Carre.SetPivot(ol::Vec2D(plotx,ploty));
Carre.RotateBy(angle_pc*(6.5));

if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
carcasse.Draw(CouleurFader);
carcasse.DrawOutline(CouleurPlotLine);
lentille.Draw(CouleurFader);

}
Carre.DrawOutline(CouleurPlotLine);

return(0);
}

int plot_draw_symbol_direct(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{

float taille_x=40.0*size_symbol;
float taille_y=20.0*size_symbol;

ol::Circle lentille(ol::Vec2D(plotx,ploty),taille_x);
lentille.Draw(CouleurPlotLine);
if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
lentille.Draw(CouleurFader);

}
lentille.DrawOutline(CouleurPlotLine);
neuromoyen.Print("D",plotx-5,ploty+5);

return(0);
}

int plot_draw_symbol_serviceligth(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{

float taille_x=40.0*size_symbol;
float taille_y=20.0*size_symbol;

ol::Circle lentille(ol::Vec2D(plotx,ploty),taille_x);
lentille.Draw(CouleurPlotFill);
if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
lentille.Draw(CouleurFader);

}
lentille.DrawOutline(CouleurPlotLine);
neuromoyen.Print("S",plotx-5,ploty+5);

return(0);
}

int plot_draw_symbol_dimmerline(int plotx, int ploty, float size_symbol, float  angle_pc, int num_symbol, int plot_calc_number_is)
{

float taille_x=40.0*size_symbol;
float taille_y=20.0*size_symbol;

ol::Circle lentille(ol::Vec2D(plotx,ploty),taille_x);
lentille.Draw(CouleurPlotLine);
if( symbol_is_selected[plot_calc_number_is][num_symbol]==1)
{
lentille.Draw(CouleurFader);

}
ol::Line(ol::Vec2D(plotx-(taille_x/2.5),ploty-(taille_x/2.5)),ol::Vec2D(plotx+(taille_x/2.5),ploty+(taille_x/2.5))).Draw(CouleurPlotFill);
ol::Line(ol::Vec2D(plotx+(taille_x/2.5),ploty-(taille_x/2.5)),ol::Vec2D(plotx-(taille_x/2.5),ploty+(taille_x/2.5))).Draw(CouleurPlotFill);
lentille.DrawOutline(CouleurPlotLine);


return(0);
}


int plot_print_neutral_symbol(int s, int plotx, int ploty)
{

//affichage symbole
switch(s)
{
case 0:     //PC 500 ou 650
plot_draw_symbol_pc(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 1:     //PC 1kw
plot_draw_symbol_pc(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 2:     //PC2kw
plot_draw_symbol_pc(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 3:    //fresnel 1kw
plot_draw_symbol_fresnel(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 4:    //fresnel 2kw
plot_draw_symbol_fresnel(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 5:    //fresnel 5kw
plot_draw_symbol_fresnel(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 6: //dec 30°
plot_draw_symbol_decoupe_etc_1(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 7: //dec 40°
plot_draw_symbol_decoupe_etc_2(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 8: //dec 50°
plot_draw_symbol_decoupe_etc_3(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 9: //dec 1kw longue
plot_draw_symbol_decoupe_1(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 10: //dec 1kw mid
plot_draw_symbol_decoupe_2(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 11: //dec 1kw wide
plot_draw_symbol_decoupe_3(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 12: //dec 2kw
plot_draw_symbol_decoupe_1(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 13: //dec 2kw
plot_draw_symbol_decoupe_2(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 14: //dec 2kw
plot_draw_symbol_decoupe_3(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 15: //PAR CP60
plot_draw_symbol_Par_1(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 16: //PAR CP61
plot_draw_symbol_Par_2(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 17: //PAR CP62
plot_draw_symbol_Par_3(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 18: //PAR CP95
plot_draw_symbol_Par_4(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 19://par 56 CP60
plot_draw_symbol_Par_1(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 20://par 56 CP61
plot_draw_symbol_Par_2(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 21://par 56 CP62
 plot_draw_symbol_Par_3(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 22://Par 36
plot_draw_symbol_Par_4(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 23://par 20
plot_draw_symbol_Par_3(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 24://par 16
plot_draw_symbol_Par_1(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 25://horiziode assymetrique
plot_draw_symbol_Horiziode_assym(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 26://horiziode symetrique
plot_draw_symbol_Horiziode_sym(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 27://hallogene de chantier
plot_draw_symbol_Horiziode_sym(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 28://BT250w
plot_draw_symbol_BT(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 29://BT500W
plot_draw_symbol_BT(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 30://rampe brabo
plot_draw_symbol_T8(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 31://blinder
plot_draw_symbol_Blinder(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 32://Svob
plot_draw_symbol_Svoboda(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 33://ACL
plot_draw_symbol_ACL(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 34://FLUO
plot_draw_symbol_fluo_little(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 35://FLUO
plot_draw_symbol_fluo_big(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 36://Follow Spot
plot_draw_symbol_FollowSpot1(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 37://Follow Spot
plot_draw_symbol_FollowSpot2(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 38://VP1
plot_draw_symbol_VP1(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 39://VP2
plot_draw_symbol_VP2(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 40://Proj diapo
plot_draw_symbol_slideprojector(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 41://Rétro Proj
plot_draw_symbol_retroprojector(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 42://volets
plot_draw_symbol_barndoors(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 43://tophat
plot_draw_symbol_top_hat(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 44://color extender
plot_draw_symbol_color_extender(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 45://color changer
plot_draw_symbol_colorchanger(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 46://jalousie
plot_draw_symbol_jalousie(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 47://iris
plot_draw_symbol_iris(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 48://gobo
plot_draw_symbol_goboholder(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 49://shutter
plot_draw_symbol_shutter(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 50://MOTORIZED MIRROR
plot_draw_symbol_motorized_mirror(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 51://smoke machine
plot_draw_symbol_smokemachine(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 52://fog machine
plot_draw_symbol_fogmachine(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 53:
plot_draw_symbol_dimmerline(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
break;
case 54://direct
plot_draw_symbol_direct(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 55://service
plot_draw_symbol_serviceligth(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 56://platine
plot_draw_symbol_platine_de_sol(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 57://pied
plot_draw_symbol_littlestand(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 58://grand pied
plot_draw_symbol_bigstand(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 59://barre de couplage
plot_draw_symbol_barre_de_couplage(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 60://Echelle
plot_draw_symbol_echelle(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 61://pont 50 1m
plot_draw_symbol_pont50_1m(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 62://pont 50 3m
plot_draw_symbol_pont50_3m(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 63://pont 50 jonction
plot_draw_symbol_pont50_jonction(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 64://pont 30 1m
plot_draw_symbol_pont30_1m(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 65://pont 30 3m
plot_draw_symbol_pont30_3m(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
case 66://pont 30 jonction
plot_draw_symbol_pont30_jonction(plotx, ploty , size_symbol[s], 0, 0, 0);
break;
default:
break;
}

 return(0);
}

//////////////////////MENUS/////////////////////////////////////////////////////

int plot_symbol_list(int plotx,int ploty)
{

ol::Rect PlotSymbList(ol::Vec2D(plotx,ploty),ol::Vec2D(205,140));
PlotSymbList.SetRoundness(15);
PlotSymbList.Draw(CouleurBleuProcedure.WithAlpha(0.3));
PlotSymbList.DrawOutline(CouleurPlotLine.WithAlpha(0.4));

ol::Rect UnderName(ol::Vec2D(plotx+10,ploty+5),ol::Vec2D(185,20));
UnderName.SetRoundness(5);
switch(index_edit_symbol_plot)
{
case 0:
UnderName.Draw(CouleurPlotLine.WithAlpha(0.4));
break;
case 1:
UnderName.Draw(CouleurSurvol);
break;
}
UnderName.DrawOutline(CouleurPlotLine.WithAlpha(0.4));
petitchiffrenoir.Print(symbol_nickname[symbol_selected_type],plotx+12,ploty+18);
//up
ol::Line(ol::Vec2D(plotx+12,ploty+50),ol::Vec2D(plotx+28,ploty+50)).Draw(CouleurPlotLine);
ol::Line(ol::Vec2D(plotx+12,ploty+50),ol::Vec2D(plotx+20,ploty+36)).Draw(CouleurPlotLine);
ol::Line(ol::Vec2D(plotx+28,ploty+50),ol::Vec2D(plotx+20,ploty+36)).Draw(CouleurPlotLine);
if(window_focus_id==W_PLOT && mouse_x>plotx+12 && mouse_x<plotx+28 && mouse_y>ploty+36 && mouse_y<ploty+50
&& Midi_Faders_Affectation_Type!=0)
{
ol::Line(ol::Vec2D(plotx+12,ploty+50),ol::Vec2D(plotx+28,ploty+50)).Draw(CouleurBlind);
ol::Line(ol::Vec2D(plotx+12,ploty+50),ol::Vec2D(plotx+20,ploty+36)).Draw(CouleurBlind);
ol::Line(ol::Vec2D(plotx+28,ploty+50),ol::Vec2D(plotx+20,ploty+36)).Draw(CouleurBlind);
show_type_midi(1596,"Plot List Up");
}
//down
ol::Line(ol::Vec2D(plotx+12,ploty+69),ol::Vec2D(plotx+28,ploty+69)).Draw(CouleurPlotLine);
ol::Line(ol::Vec2D(plotx+12,ploty+69),ol::Vec2D(plotx+20,ploty+83)).Draw(CouleurPlotLine);
ol::Line(ol::Vec2D(plotx+28,ploty+69),ol::Vec2D(plotx+20,ploty+83)).Draw(CouleurPlotLine);

if(window_focus_id==W_PLOT && mouse_x>plotx+12 && mouse_x<plotx+28 && mouse_y>ploty+69 && mouse_y<ploty+83
&&  Midi_Faders_Affectation_Type!=0)
{
ol::Line(ol::Vec2D(plotx+12,ploty+69),ol::Vec2D(plotx+28,ploty+69)).Draw(CouleurBlind);
ol::Line(ol::Vec2D(plotx+12,ploty+69),ol::Vec2D(plotx+20,ploty+83)).Draw(CouleurBlind);
ol::Line(ol::Vec2D(plotx+28,ploty+69),ol::Vec2D(plotx+20,ploty+83)).Draw(CouleurBlind);
show_type_midi(1597,"Plot List Up");
}
ol::Rect SliderSel(ol::Vec2D(plotx+3,ploty+30),ol::Vec2D(5,64));
ol::Rect SliderVel(ol::Vec2D(plotx+3,ploty+30+(64-(symbol_selected_type/2))),ol::Vec2D(5,(symbol_selected_type/2)));
SliderSel.DrawOutline(CouleurPlotLine.WithAlpha(0.5));
SliderVel.Draw(CouleurFader);
if(window_focus_id==W_PLOT && mouse_x>plotx+3 && mouse_x<plotx+8 && mouse_y>ploty+30 && mouse_y<ploty+94
&&  Midi_Faders_Affectation_Type!=0)
{
SliderSel.DrawOutline(CouleurBlind);
show_type_midi(1598,"Plot List CC");
}

petitchiffrenoir.Print(ol::ToString(symbol_selected_type),plotx+13,ploty+62);

//affichage du symbol
plot_print_neutral_symbol(symbol_selected_type, plotx+165, ploty+75 );


//Global library size
petitchiffrenoir.Print("Global size",plotx+45,ploty+37);
petitchiffrenoir.Print(ol::ToString(global_symbol_size),plotx+108,ploty+48);
ol::Rect global_sizer_frame(ol::Vec2D(plotx+50,ploty+40),ol::Vec2D(50,10));
ol::Rect global_sizer_level(ol::Vec2D(plotx+50,ploty+40),ol::Vec2D(25.0*global_symbol_size,10));
switch(index_edit_symbol_plot)
{
case 0:
global_sizer_level.Draw(CouleurPlotLine.WithAlpha(0.4));
break;
case 1:
global_sizer_level.Draw(CouleurBlind);
break;
}
global_sizer_frame.DrawOutline(CouleurPlotLine);


//slider size of symbol
petitchiffrenoir.Print("Symbol size",plotx+45,ploty+62);
petitchiffrenoir.Print(ol::ToString(size_symbol[symbol_selected_type]),plotx+108,ploty+73);
ol::Rect symbol_sizer_frame(ol::Vec2D(plotx+50,ploty+65),ol::Vec2D(50,10));
ol::Rect symbol_sizer_level(ol::Vec2D(plotx+50,ploty+65),ol::Vec2D(50.0*size_symbol[symbol_selected_type],10));
switch(index_edit_symbol_plot)
{
case 0:
symbol_sizer_level.Draw(CouleurPlotLine.WithAlpha(0.4));
break;
case 1:
symbol_sizer_level.Draw(CouleurFader);
break;
}
symbol_sizer_frame.DrawOutline(CouleurPlotLine);

ol::Rect edit_symbol_frame(ol::Vec2D(plotx+40,ploty+80),ol::Vec2D(105,20));
edit_symbol_frame.SetRoundness(5);
edit_symbol_frame.Draw(CouleurPlotFill);
edit_symbol_frame.Draw(CouleurBleuProcedure.WithAlpha(0.3));
edit_symbol_frame.Draw(CouleurBlind.WithAlpha(index_edit_symbol_plot));
edit_symbol_frame.DrawOutline(CouleurPlotLine);
petitchiffrenoir.Print("Edit the Symbol",plotx+45,ploty+92);

ol::Rect add_symbol_frame(ol::Vec2D(plotx+40,ploty+110),ol::Vec2D(80,20));
add_symbol_frame.SetRoundness(5);
add_symbol_frame.Draw(CouleurPlotFill);
add_symbol_frame.Draw(CouleurBleuProcedure.WithAlpha(0.3));
add_symbol_frame.DrawOutline(CouleurPlotLine);
petitchiffrenoir.Print("Add To Plot",plotx+45,ploty+122);
petitchiffrenoir.Print(ol::ToString(nbre_symbols_on_plot[view_plot_calc_number_is]),plotx+15,ploty+122);

if(window_focus_id==W_PLOT && mouse_x>plotx+40 && mouse_x<plotx+40+80 && mouse_y>ploty+110 && mouse_y<ploty+110+20
&& Midi_Faders_Affectation_Type!=0)
{
add_symbol_frame.DrawOutline(CouleurBlind);
show_type_midi(1600,"Plot Add Symbol");
}




ol::Rect BSel(ol::Vec2D(plotx+130, ploty+105),ol::Vec2D(65,28));
BSel.SetRoundness(4);
BSel.Draw(CouleurPlotFill);
BSel.Draw(CouleurBleuProcedure.WithAlpha(0.3));
BSel.DrawOutline(CouleurPlotLine);
if(window_focus_id==W_PLOT && mouse_x>plotx+130 && mouse_x<plotx+130+65 && mouse_y>ploty+105 && mouse_y<ploty+105+28)
{
BSel.DrawOutline(CouleurPlotFill.WithAlpha(0.5));
}
petitchiffrenoir.Print("Select ID",plotx+135,ploty+115);
petitchiffrenoir.Print(ol::ToString(symbol_id_to_select),plotx+150,ploty+127);




 return(0);
}


int plot_draw_shape_line(int plotx1, int ploty1, int plotx2, int ploty2, float sizeshape,  float alphashape, int colorpattern, bool isselected)
{
ol::Rgba TmpColor;

switch(isselected)
{
case 0:
switch(colorpattern)
{
case 1://jaune
TmpColor=CouleurYellowFgroup;
break;
case 2: //Rouge
TmpColor=CouleurRed;
break;
case 3: //Vert
TmpColor=CouleurGreen;
break;
case 4: //blue
TmpColor=CouleurBleuProcedure;
break;
default:
TmpColor=CouleurPlotLine;
break;
}

ol::Line(ol::Vec2D(plotx1,ploty1),ol::Vec2D(plotx2,ploty2),sizeshape).Draw(TmpColor.WithAlpha(alphashape));

break;
case 1:
ol::Line(ol::Vec2D(plotx1,ploty1),ol::Vec2D(plotx2,ploty2),sizeshape).Draw(CouleurFader);
break;
}
if(index_edit_light_plot==1 && index_menus_lighting_plot==1)//visualition du centre projo
{
ol::Line(ol::Vec2D(plotx1-5,ploty1-5),ol::Vec2D(plotx1+5,ploty1+5)).Draw(CouleurBlind);
ol::Line(ol::Vec2D(plotx1+5,ploty1-5),ol::Vec2D(plotx1-5,ploty1+5)).Draw(CouleurBlind);
ol::Rect(ol::Vec2D(plotx2-5,ploty2-5),ol::Vec2D(10,10)).Draw(CouleurPlotLine.WithAlpha(handle_selected_for_line_editing*isselected));
ol::Rect(ol::Vec2D(plotx2-5,ploty2-5),ol::Vec2D(10,10)).DrawOutline(CouleurPlotLine);
ol::Line(ol::Vec2D(plotx2-5,ploty2-5),ol::Vec2D(plotx2+5,ploty2+5)).Draw(CouleurBlind);
ol::Line(ol::Vec2D(plotx2+5,ploty2-5),ol::Vec2D(plotx2-5,ploty2+5)).Draw(CouleurBlind);
}



 return(0);
}




int plot_draw_shape_stripline(float plotx1, float ploty1, float plotx2, float ploty2, float sizeshape,  float alphag,int typeofline,int colorpattern, bool isselected, bool show_handle)
{


glLineWidth (sizeshape);
switch(typeofline)
{
case 0:
glLineStipple (1, 0x0101);  /*  dotted  */
break;
case 1:
glLineStipple (4, 0x0101);
break;
case 2:
glLineStipple (3, 0x1F03);
break;
case 3:
glLineStipple (3, 0x1C47);  /*  dash/dot/dash  */
break;
case 4:
glLineStipple (3, 0x00FF);  /*  dashed  */
break;
default:
break;
}

switch(isselected)
{
case 0:
switch(colorpattern)
{
case 1://jaune
glColor4f(1.0,0.7,0.0,alphag);
break;
case 2: //Rouge
glColor4f(0.6,0.0,0.0,alphag);
break;
case 3: //Vert
glColor4f(0.0,0.6,0.0,alphag);
break;
case 4: //Bleu
glColor4f(0.0,0.2,0.5,alphag);
break;
default:
glColor4f(Color_plotline,Color_plotline,Color_plotline,alphag);
break;
}
break;
case 1:
glColor4f(1.0,0.4,0.0,1.0);
break;
}

glEnable (GL_LINE_STIPPLE);
drawStippleLine(plotx1,ploty1,plotx2,ploty2);
glDisable (GL_LINE_STIPPLE);

glColor4f(0.0,0.0, 0.0, 1.0);
if(index_edit_light_plot==1 && index_menus_lighting_plot==1 && show_handle==1)//visualition du centre projo
{

ol::Line(ol::Vec2D(plotx1-5,ploty1-5),ol::Vec2D(plotx1+5,ploty1+5)).Draw(CouleurBlind);
ol::Line(ol::Vec2D(plotx1+5,ploty1-5),ol::Vec2D(plotx1-5,ploty1+5)).Draw(CouleurBlind);
ol::Rect(ol::Vec2D(plotx2-5,ploty2-5),ol::Vec2D(10,10)).Draw(CouleurPlotLine.WithAlpha(isselected));
ol::Rect(ol::Vec2D(plotx2-5,ploty2-5),ol::Vec2D(10,10)).DrawOutline(CouleurPlotLine);
ol::Line(ol::Vec2D(plotx2-5,ploty2-5),ol::Vec2D(plotx2+5,ploty2+5)).Draw(CouleurBlind);
ol::Line(ol::Vec2D(plotx2+5,ploty2-5),ol::Vec2D(plotx2-5,ploty2+5)).Draw(CouleurBlind);
}

return(0);
}

int plot_draw_rectangle(int plotx1, int ploty1, float shapesizex, float shapesizey, float shaperotation, float alphashape, int colorpattern, bool isselected)
{
ol::Poly ShapeRect;
ShapeRect.Add(ol::Vec2D(plotx1-(shapesizex/2),ploty1-(shapesizey/2)));
ShapeRect.Add(ol::Vec2D(plotx1-(shapesizex/2),ploty1+(shapesizey/2)));
ShapeRect.Add(ol::Vec2D(plotx1+(shapesizex/2),ploty1+(shapesizey/2)));
ShapeRect.Add(ol::Vec2D(plotx1+(shapesizex/2),ploty1-(shapesizey/2)));

ShapeRect.SetPivot(ol::Vec2D(plotx1,ploty1));
ShapeRect.RotateBy(shaperotation);

if(isselected==1)
{
ShapeRect.DrawOutline(CouleurFader);
}
else
{
switch(colorpattern)
{
case 0://Outlined + Fill
ShapeRect.Draw(CouleurPlotFill.WithAlpha(alphashape));
ShapeRect.DrawOutline(CouleurPlotLine);
break;
case 1://Outlined only
ShapeRect.DrawOutline(CouleurPlotLine.WithAlpha(alphashape));
break;
case 2: //filled Outline color
ShapeRect.Draw(CouleurPlotLine.WithAlpha(alphashape));
break;
case 3: //filled PlotFill color
ShapeRect.Draw(CouleurPlotFill.WithAlpha(alphashape));
break;
default:
break;
}
}
if(index_edit_light_plot==1)//visualition du centre projo
{
ol::Line(ol::Vec2D(plotx1-5,ploty1-5),ol::Vec2D(plotx1+5,ploty1+5)).Draw(CouleurBlind);
ol::Line(ol::Vec2D(plotx1+5,ploty1-5),ol::Vec2D(plotx1-5,ploty1+5)).Draw(CouleurBlind);
}

return(0);
}

int plot_draw_shape_curtain(int plotx1, int ploty1, int plotx2, int ploty2, float sizeshape,  float alphashape,int colorpattern, bool isselected)
{
float distance_x=plotx2-plotx1;
float distance_y=ploty2-ploty1;
int nbre_de_cercles=(int)(sqrt(distance_x*distance_x+distance_y*distance_y)/(sizeshape*2)); //get diagonal distance and divide by the diameter of a circle.
float correctionx=(distance_x/nbre_de_cercles);
float correctiony=(distance_y/nbre_de_cercles);
double angle_ligne= atan2(ploty2 - ploty1, plotx2 - plotx1);
bool inversion=0;

for(int i=0;i<nbre_de_cercles;i++)
{
inversion=toggle(inversion);
ol::Circle ourlet(0, 0, sizeshape);
ourlet.MoveTo(ol::Vec2D(plotx1+cos(angle_ligne)*sizeshape+(correctionx*i),ploty1+sin(angle_ligne)*sizeshape+(correctiony*i)));
ourlet.RotateBy(angle_ligne + PI*inversion);
ol::Rgba TmpColor;

switch(isselected)
{
case 0:
switch(colorpattern)
{
case 1://jaune
TmpColor=CouleurYellowFgroup;
break;
case 2: //Rouge
TmpColor=CouleurRed;
break;
case 3: //Vert
TmpColor=CouleurGreen;
break;
case 4: //blue
TmpColor=CouleurBleuProcedure;
break;
default:
TmpColor=CouleurPlotLine;
break;
}
ourlet.DrawSlice(TmpColor.WithAlpha(general_alpha_for_shape),0, PI);
break;
case 1:
ourlet.DrawSlice(CouleurFader,0, PI);
break;
}
}
if( index_edit_light_plot==1 && index_menus_lighting_plot==1)//visualition du centre projo
{
ol::Line(ol::Vec2D(plotx1-5,ploty1-5),ol::Vec2D(plotx1+5,ploty1+5)).Draw(CouleurBlind);
ol::Line(ol::Vec2D(plotx1+5,ploty1-5),ol::Vec2D(plotx1-5,ploty1+5)).Draw(CouleurBlind);
ol::Rect(ol::Vec2D(plotx2-5,ploty2-5),ol::Vec2D(10,10)).Draw(CouleurPlotLine.WithAlpha(handle_selected_for_line_editing*isselected));
ol::Rect(ol::Vec2D(plotx2-5,ploty2-5),ol::Vec2D(10,10)).DrawOutline(CouleurPlotLine);
ol::Line(ol::Vec2D(plotx2-5,ploty2-5),ol::Vec2D(plotx2+5,ploty2+5)).Draw(CouleurBlind);
ol::Line(ol::Vec2D(plotx2+5,ploty2-5),ol::Vec2D(plotx2-5,ploty2+5)).Draw(CouleurBlind);
}
return(0);
}


int plot_draw_circle(int plotx1, int ploty1, float sizeshape,  float alphashape,   int colorpattern, bool isselected)
{
ol::Circle ShapeCirc(plotx1, ploty1,sizeshape);

if(isselected==1)
{
ShapeCirc.DrawOutline(CouleurFader);
}
else
{

switch(colorpattern)
{
case 0://Outlined + Fill
ShapeCirc.Draw(CouleurPlotFill.WithAlpha(alphashape));
ShapeCirc.DrawOutline(CouleurPlotLine.WithAlpha(alphashape));
break;
case 1://Outlined only
ShapeCirc.DrawOutline(CouleurPlotLine.WithAlpha(alphashape));
break;
case 2: //filled Outline color
ShapeCirc.Draw(CouleurPlotLine.WithAlpha(alphashape));
break;
case 3: //filled PlotFill color
ShapeCirc.Draw(CouleurPlotFill.WithAlpha(alphashape));
break;
}
}
if( index_edit_light_plot==1 && index_menus_lighting_plot==1)//visualition du centre projo
{
ol::Line(ol::Vec2D(plotx1-5,ploty1-5),ol::Vec2D(plotx1+5,ploty1+5)).Draw(CouleurBlind);
ol::Line(ol::Vec2D(plotx1+5,ploty1-5),ol::Vec2D(plotx1-5,ploty1+5)).Draw(CouleurBlind);
}
 return(0);
}

int plot_draw_slice(int plotx1, int ploty1, float sizeshape, float opening_angle, float shaperotation, float alphashape,   int colorpattern, bool isselected)

{

ol::Circle arcdecercle(ol::Vec2D(plotx1,ploty1),sizeshape);
arcdecercle.RotateBy(shaperotation*6.5);
arcdecercle.SetRadius(sizeshape/2);

if(isselected==1)
{
arcdecercle.DrawSlice(CouleurFader,0,opening_angle);
}
else
{

switch(colorpattern)
{
case 0://Outlined + Fill
arcdecercle.DrawSlice(CouleurPlotFill.WithAlpha(alphashape),CouleurPlotLine.WithAlpha(alphashape),0,opening_angle);
//arcdecercle.DrawSlice(CouleurPlotLine,0,opening_angle);
break;
case 1://Outlined only
arcdecercle.DrawSlice(CouleurPlotLine.WithAlpha(alphashape),CouleurPlotFill.WithAlpha(alphashape),0,opening_angle);
break;
case 2: //filled Outline color
arcdecercle.DrawSlice(CouleurPlotLine.WithAlpha(alphashape),0,opening_angle);
break;
case 3: //filled PlotFill color
arcdecercle.DrawSlice(CouleurPlotFill.WithAlpha(alphashape),0,opening_angle);
break;
}
}
if( index_edit_light_plot==1 && index_menus_lighting_plot==1)//visualition du centre projo
{
ol::Line(ol::Vec2D(plotx1-5,ploty1-5),ol::Vec2D(plotx1+5,ploty1+5)).Draw(CouleurBlind);
ol::Line(ol::Vec2D(plotx1+5,ploty1-5),ol::Vec2D(plotx1-5,ploty1+5)).Draw(CouleurBlind);
}
 return(0);
}




int plot_draw_polygon(int plotx1, int ploty1, float sizeshape, float shaperotation,int numPoints,  float alphashape,   int colorpattern, bool isselected)
{
float ployx ;
float ployy ;
ol::Poly mypoly;

for(int i=0;i<numPoints;i++)
{
ployx = plotx1 + sizeshape * sin(2.0*PI*i/numPoints);
ployy = ploty1 + sizeshape * cos(2.0*PI*i/numPoints);
mypoly.Add(ol::Vec2D(ployx,ployy));
}
mypoly.SetPivot(ol::Vec2D(plotx1,ploty1));
mypoly.RotateBy(shaperotation*6.5);

if(isselected==1)
{
mypoly.DrawOutline(CouleurFader);
}
else
{

switch(colorpattern)
{
case 0://Outlined + Fill
mypoly.Draw(CouleurPlotFill.WithAlpha(alphashape));
mypoly.DrawOutline(CouleurPlotLine.WithAlpha(alphashape));
break;
case 1://Outlined only
mypoly.DrawOutline(CouleurPlotLine.WithAlpha(alphashape));
break;
case 2: //filled Outline color
mypoly.Draw(CouleurPlotLine.WithAlpha(alphashape));
break;
case 3: //filled PlotFill color
mypoly.Draw(CouleurPlotFill.WithAlpha(alphashape));
break;
}
}
if( index_edit_light_plot==1 && index_menus_lighting_plot==1)//visualition du centre projo
{
ol::Line(ol::Vec2D(plotx1-5,ploty1-5),ol::Vec2D(plotx1+5,ploty1+5)).Draw(CouleurBlind);
ol::Line(ol::Vec2D(plotx1+5,ploty1-5),ol::Vec2D(plotx1-5,ploty1+5)).Draw(CouleurBlind);
}
return(0);
}


int plot_draw_text(int plotx,int ploty, int fontsize,int fonttype,  char text[25],float alphatext, bool isselected)
{


int xsi=0;
int ysi=0;
switch(fonttype)
{
case 0:
xsi=minichiffrenoir.FirstLineWidth( text );
ysi=minichiffrenoir.FirstLineHeight( text );
minichiffrenoir.Print(text,plotx,ploty);
break;
case 1:
xsi=minichiffre.FirstLineWidth( text );
ysi=minichiffre.FirstLineHeight( text );
minichiffre.Print(text,plotx,ploty);
break;
case 2:
xsi=minichiffrerouge.FirstLineWidth( text );
ysi=minichiffrerouge.FirstLineHeight( text );
minichiffrerouge.Print(text,plotx,ploty);
break;
case 3:
xsi=petitchiffrenoir.FirstLineWidth( text );
ysi=petitchiffrenoir.FirstLineHeight( text );
petitchiffrenoir.Print(text,plotx,ploty);
break;
case 4:
xsi=petitchiffre.FirstLineWidth( text );
ysi=petitchiffre.FirstLineHeight( text );
petitchiffre.Print(text,plotx,ploty);
break;
case 5:
xsi=petitchiffrerouge.FirstLineWidth( text );
ysi=petitchiffrerouge.FirstLineHeight( text );
petitchiffrerouge.Print(text,plotx,ploty);
break;
case 6:
xsi=petitdoomInspekt.FirstLineWidth( text );
ysi=petitdoomInspekt.FirstLineHeight( text );
petitdoomInspekt.Print(text,plotx,ploty);
break;
case 7:
xsi=neuromoyen.FirstLineWidth( text );
ysi=neuromoyen.FirstLineHeight( text );
neuromoyen.Print(text,plotx,ploty);
break;
case 8:
xsi=neuro.FirstLineWidth( text );
ysi=neuro.FirstLineHeight( text );
neuro.Print(text,plotx,ploty);
break;
case 9:
xsi=petitdoomblanc.FirstLineWidth( text );
ysi=petitdoomblanc.FirstLineHeight( text );
petitdoomblanc.Print(text,plotx,ploty);
break;
case 10:
xsi=doomblanc.FirstLineWidth( text );
ysi=doomblanc.FirstLineHeight( text );
doomblanc.Print(text,plotx,ploty);
break;
case 11:
xsi=doom.FirstLineWidth( text );
ysi=doom.FirstLineHeight( text );
doom.Print(text,plotx,ploty);
break;
case 12:
xsi=doomrouge.FirstLineWidth( text );
ysi=doomrouge.FirstLineHeight( text );
doomrouge.Print(text,plotx,ploty);
break;
case 13:
xsi=neuroTitle.FirstLineWidth( text );
ysi=neuroTitle.FirstLineHeight( text );
neuroTitle.Print(text,plotx,ploty);
break;
default:
break;
}

if(isselected==1)
{
ol::Rect BoxB(ol::Vec2D(plotx,ploty-ysi),ol::Vec2D(xsi,ysi));
BoxB.DrawOutline(CouleurBlind);
}
if( index_edit_light_plot==1 && index_menus_lighting_plot==1)//visualition du centre projo
{
ol::Line(ol::Vec2D(plotx-5,ploty-5),ol::Vec2D(plotx+5,ploty+5)).Draw(CouleurBlind);
ol::Line(ol::Vec2D(plotx+5,ploty-5),ol::Vec2D(plotx-5,ploty+5)).Draw(CouleurBlind);
}


return(0);
}

int plot_shape_list(int plotx,int ploty)
{

ol::Rect PlotSymbList(ol::Vec2D(plotx,ploty),ol::Vec2D(205,140));
PlotSymbList.SetRoundness(15);
PlotSymbList.Draw(CouleurBleuProcedure.WithAlpha(0.3));
PlotSymbList.DrawOutline(CouleurPlotLine.WithAlpha(0.4));

ol::Rect UnderName(ol::Vec2D(plotx+10,ploty+5),ol::Vec2D(185,20));
UnderName.SetRoundness(5);
switch(index_edit_symbol_plot)
{
case 0:
UnderName.Draw(CouleurPlotLine.WithAlpha(0.4));
break;
case 1:
UnderName.Draw(CouleurSurvol);
break;
}
UnderName.DrawOutline(CouleurPlotLine.WithAlpha(0.4));
petitchiffrenoir.Print(shape_nickname[shape_selected_type],plotx+12,ploty+18);
//up
ol::Line(ol::Vec2D(plotx+12,ploty+50),ol::Vec2D(plotx+28,ploty+50)).Draw(CouleurPlotLine);
ol::Line(ol::Vec2D(plotx+12,ploty+50),ol::Vec2D(plotx+20,ploty+36)).Draw(CouleurPlotLine);
ol::Line(ol::Vec2D(plotx+28,ploty+50),ol::Vec2D(plotx+20,ploty+36)).Draw(CouleurPlotLine);

if(window_focus_id==W_PLOT && mouse_x>plotx+12 && mouse_x<plotx+28 && mouse_y>ploty+36 && mouse_y<ploty+50
&& Midi_Faders_Affectation_Type!=0)
{
ol::Line(ol::Vec2D(plotx+12,ploty+50),ol::Vec2D(plotx+28,ploty+50)).Draw(CouleurBlind);
ol::Line(ol::Vec2D(plotx+12,ploty+50),ol::Vec2D(plotx+20,ploty+36)).Draw(CouleurBlind);
ol::Line(ol::Vec2D(plotx+28,ploty+50),ol::Vec2D(plotx+20,ploty+36)).Draw(CouleurBlind);
show_type_midi(1596,"Plot List Up");
}


//down
ol::Line(ol::Vec2D(plotx+12,ploty+69),ol::Vec2D(plotx+28,ploty+69)).Draw(CouleurPlotLine);
ol::Line(ol::Vec2D(plotx+12,ploty+69),ol::Vec2D(plotx+20,ploty+83)).Draw(CouleurPlotLine);
ol::Line(ol::Vec2D(plotx+28,ploty+69),ol::Vec2D(plotx+20,ploty+83)).Draw(CouleurPlotLine);
petitchiffrenoir.Print(ol::ToString(shape_selected_type),plotx+13,ploty+62);

if(window_focus_id==W_PLOT && mouse_x>plotx+12 && mouse_x<plotx+28 && mouse_y>ploty+69 && mouse_y<ploty+83
&& Midi_Faders_Affectation_Type!=0)
{
ol::Line(ol::Vec2D(plotx+12,ploty+69),ol::Vec2D(plotx+28,ploty+69)).Draw(CouleurBlind);
ol::Line(ol::Vec2D(plotx+12,ploty+69),ol::Vec2D(plotx+20,ploty+83)).Draw(CouleurBlind);
ol::Line(ol::Vec2D(plotx+28,ploty+69),ol::Vec2D(plotx+20,ploty+83)).Draw(CouleurBlind);
show_type_midi(1597,"Plot List Down");
}


ol::Rect SliderSel(ol::Vec2D(plotx+3,ploty+30),ol::Vec2D(5,64));
ol::Rect SliderVel(ol::Vec2D(plotx+3,ploty+30+(64-(shape_selected_type*6))),ol::Vec2D(5,(shape_selected_type*6)));
SliderSel.DrawOutline(CouleurPlotLine.WithAlpha(0.5));
SliderVel.Draw(CouleurBleuProcedure);

if(window_focus_id==W_PLOT && mouse_x>plotx+3 && mouse_x<plotx+8 && mouse_y>ploty+30 && mouse_y<ploty+94
&&  Midi_Faders_Affectation_Type!=0)
{
SliderSel.DrawOutline(CouleurBlind);
show_type_midi(1598,"Plot List CC");
}

ol::Rect add_symbol_frame(ol::Vec2D(plotx+40,ploty+110),ol::Vec2D(80,20));
add_symbol_frame.SetRoundness(5);
add_symbol_frame.DrawOutline(CouleurPlotLine);
petitchiffrenoir.Print("Add To Plot",plotx+45,ploty+122);
petitchiffrenoir.Print(ol::ToString(nbre_shapes_on_plot),plotx+15,ploty+122);
if(window_focus_id==W_PLOT && mouse_x>plotx+40 && mouse_x<plotx+40+80 && mouse_y>ploty+110 && mouse_y<ploty+110+20
&&  Midi_Faders_Affectation_Type!=0)
{
add_symbol_frame.DrawOutline(CouleurBlind);
show_type_midi(1600,"Plot Add Symbol");
}

//affichage shape
switch(shape_selected_type)
{
case 0:    //curtain
plot_draw_shape_curtain(plotx+50, ploty+55 , plotx+200, ploty+55, general_shape_size_to_apply/5 ,  1.0,color_pattern_selected, 0);
break;
case 1:     //ligne  plain
plot_draw_shape_line(plotx+50, ploty+55 , plotx+200, ploty+55, general_shape_size_to_apply/10 ,  1.0, color_pattern_selected , 0);
break;
case 2:     //stripline 1               int plotx1, int ploty1, int plotx2, int ploty2, float sizeshape,  float alphag, short typeofline,col pattern, bool isselected, bool edit extrem)
plot_draw_shape_stripline(plotx+50, ploty+55 , plotx+200, ploty+55, general_shape_size_to_apply/10 ,1.0, 0,  color_pattern_selected ,0, 1);
break;
case 3:     //stripline 2
plot_draw_shape_stripline(plotx+50, ploty+55 , plotx+200, ploty+55, general_shape_size_to_apply/10 ,1.0, 1,  color_pattern_selected ,0, 1);
break;
case 4:     //stripline 3
plot_draw_shape_stripline(plotx+50, ploty+55 , plotx+200, ploty+55, general_shape_size_to_apply/10 ,1.0, 2,  color_pattern_selected ,0, 1);
break;
case 5:     //stripline 4
plot_draw_shape_stripline(plotx+50, ploty+55 , plotx+200, ploty+55, general_shape_size_to_apply/10 ,1.0, 3, color_pattern_selected , 0, 1);
break;
case 6:     //stripline 5
plot_draw_shape_stripline(plotx+50, ploty+55 , plotx+200, ploty+55, general_shape_size_to_apply/10 ,1.0, 4,  color_pattern_selected ,0, 1);
break;
case 7://plot_draw_rectangle(int plotx1, int ploty1, int shapesizex, int shapesizey, float shaperotation, float alphashape,  int colorpattern, bool isselected)
plot_draw_rectangle(plotx+120,ploty+60, 40, 20, angle_shape_selected, 1.0, color_pattern_selected , 0);
break;
case 8://plot_draw_circle(int plotx1, int ploty1, float sizeshape,  float alphashape,   int colorpattern, bool isselected)
plot_draw_circle(plotx+120,ploty+60, 20, 1.0, color_pattern_selected , 0);
break;
case 9://plot_draw_slice(int plotx1, int ploty1, float sizeshape, float opening_angle, float shaperotation, float alphashape,   int colorpattern, bool isselected)
plot_draw_slice(plotx+120,ploty+60, 40,opening_angle_selected*6.5, angle_shape_selected,1.0, color_pattern_selected,0);
break;
case 10://polygon int plotx1, int ploty1, float sizeshape, float shaperotation,int numPoints,  float alphashape,   int colorpattern, bool isselected)
plot_draw_polygon(plotx+120,ploty+60, 40,angle_shape_selected,nbre_branches_polygon, 1.0, color_pattern_selected,0);
break;
case 11://(int plotx,int ploty, float fontsize,int fonttype,  char *text,float alphatext, bool isselected)
plot_draw_text(plotx+120,ploty+60,(int)general_shape_size_to_apply,fonttype_selected,"TEXT",1.0, 0);
break;
default:
break;
}

ol::Rect BSel(ol::Vec2D(plotx+130, ploty+105),ol::Vec2D(65,28));
BSel.SetRoundness(4);
BSel.DrawOutline(CouleurPlotLine);
if(window_focus_id==W_PLOT && mouse_x>plotx+130 && mouse_x<plotx+195 && mouse_y>ploty+105 && mouse_y<ploty+133)
{
BSel.DrawOutline(CouleurPlotFill.WithAlpha(0.5));
}
petitchiffrenoir.Print("Select ID",plotx+135,ploty+115);
petitchiffrenoir.Print(ol::ToString(shape_id_to_select),plotx+150,ploty+127);

return(0);
}



int shape_edition(int plotx, int ploty)
{
ol::Rect PlotShEd(ol::Vec2D(plotx,ploty),ol::Vec2D(200,400));
PlotShEd.SetRoundness(15);
PlotShEd.Draw(CouleurBleuProcedure.WithAlpha(0.4));
PlotShEd.DrawOutline(CouleurPlotLine.WithAlpha(0.4));
petitchiffrenoir.Print("SELECTED SHAPES:",plotx+10,ploty+15);
ol::Line(ol::Vec2D(plotx+10,ploty+20),ol::Vec2D(plotx+130,ploty+20)).Draw(CouleurPlotLine);

//rotation
petitchiffrenoir.Print("Rotate",plotx+10,ploty+33);
petitchiffrenoir.Print(ol::ToString(angle_shape_selected),plotx+80,ploty+33);
ol::Rect RotateFrame(ol::Vec2D(plotx+10,ploty+40),ol::Vec2D(100,10));
ol::Rect RotateLevel(ol::Vec2D(plotx+10,ploty+40),ol::Vec2D((angle_shape_selected*100),10));
RotateLevel.Draw(CouleurGreen);
RotateFrame.DrawOutline(CouleurPlotLine);

if(window_focus_id==W_PLOT && mouse_x>plotx+10 && mouse_x<plotx+110 && mouse_y>ploty+40 && mouse_y<ploty+50
&& Midi_Faders_Affectation_Type!=0)
{
RotateFrame.DrawOutline(CouleurBlind);
show_type_midi(1601,"Plot Rotate");
}

ol::Rect OverRot(ol::Vec2D(plotx+76,ploty+22),ol::Vec2D(60,15));
OverRot.SetRoundness(4);
OverRot.DrawOutline(CouleurPlotLine.WithAlpha(0.5));


ol::Rect AnglePos(ol::Vec2D(-10,-10),ol::Vec2D(10,10));

for(int l=0;l<3;l++)
{
for(int c=0;c<3;c++)
{
int pos=(l*3) + c;
AnglePos.MoveTo(ol::Vec2D(plotx+140+(c*15),ploty+10+(l*15)));
AnglePos.DrawOutline(CouleurPlotLine.WithAlpha(0.5));
if(window_focus_id==W_PLOT && mouse_x>plotx+140+(c*15) && mouse_x<plotx+150+(c*15) && mouse_y>ploty+10+(l*15) && mouse_y<ploty+20+(l*15) )
{
AnglePos.Draw(CouleurFader);
}
}
}
AnglePos.MoveTo(ol::Vec2D(plotx+155,ploty+25));
AnglePos.Draw(CouleurPlotFill);
AnglePos.Draw(CouleurBleuProcedure.WithAlpha(0.4));
AnglePos.DrawOutline(CouleurPlotFill);
AnglePos.DrawOutline(CouleurBleuProcedure.WithAlpha(0.4));


ol::Rect MySymbAction(ol::Vec2D(plotx, ploty+60),ol::Vec2D(60,18));
MySymbAction.SetRoundness(5);

for(int c=0;c<3;c++)
{

for(int l=0;l<2;l++)
{
MySymbAction.MoveTo(ol::Vec2D(plotx+5+(c*65),ploty+60+(l*20)));
MySymbAction.Draw(CouleurPlotLine.WithAlpha(0.5));
if(window_focus_id==W_PLOT && index_edit_light_plot==1 && index_click_inside_relativ_xy==0
&& mouse_x>plotx+5+(c*65) && mouse_x<plotx+65+(c*65) && mouse_y>ploty+60+(l*20) && mouse_y<ploty+78+(l*20))
{
MySymbAction.DrawOutline(CouleurLigne.WithAlpha(0.5));
}
switch(c+(l*3))
{
case 0:
if(window_focus_id==W_PLOT && mouse_x>plotx+5+(c*65) && mouse_x<plotx+65+(c*65) && mouse_y>ploty+60+(l*20) && mouse_y<ploty+78+(l*20)
&& Midi_Faders_Affectation_Type!=0)
{
MySymbAction.DrawOutline(CouleurBlind);
show_type_midi(1602,"Plot Dub");
}
petitchiffre.Print("Dub",plotx+20+(c*65),ploty+72+(l*20));
break;
case 1:
if(window_focus_id==W_PLOT && mouse_x>plotx+5+(c*65) && mouse_x<plotx+65+(c*65) && mouse_y>ploty+60+(l*20) && mouse_y<ploty+78+(l*20)
&& Midi_Faders_Affectation_Type!=0)
{
MySymbAction.DrawOutline(CouleurBlind);
show_type_midi(1603,"Plot Delete");
}
petitchiffre.Print("Delete",plotx+10+(c*65),ploty+72+(l*20));
break;
case 2:
if(window_focus_id==W_PLOT && mouse_x>plotx+5+(c*65) && mouse_x<plotx+65+(c*65) && mouse_y>ploty+60+(l*20) && mouse_y<ploty+(l*20)+78
&& Midi_Faders_Affectation_Type!=0)
{
MySymbAction.DrawOutline(CouleurBlind);
show_type_midi(1604,"Plot Unselect");
}
petitchiffre.Print("UnSelect",plotx+7+(c*65),ploty+72+(l*20));
break;
case 3:
petitchiffre.Print("Down",plotx+15+(c*65),ploty+72+(l*20));
break;
case 4:
petitchiffre.Print("Top",plotx+22+(c*65),ploty+72+(l*20));
break;
case 5:
petitchiffre.Print("Grp/unGr.",plotx+7+(c*65),ploty+72+(l*20));
break;
default:
break;
}
}
}

//alignements
ol::Rect MySymbAlign(ol::Vec2D(plotx+5,ploty+80),ol::Vec2D(40,18));
MySymbAlign.SetRoundness(5);
for(int c=0;c<4;c++)
{
 MySymbAlign.MoveTo(ol::Vec2D(plotx+10+(c*45),ploty+110));
 MySymbAlign.Draw(CouleurBleuProcedure.WithAlpha(0.5));
 switch(c)
{
case 0:
petitchiffre.Print("AlignX",plotx+10+(c*45),ploty+122);
break;
case 1:
petitchiffre.Print("AlignY",plotx+10+(c*45),ploty+122);
break;
case 2:
petitchiffre.Print("<-X->",plotx+15+(c*45),ploty+122);
break;
case 3:
petitchiffre.Print("<-Y->",plotx+15+(c*45),ploty+122);
break;
default:
break;
}
}

petitchiffrenoir.Print("Shape General Size",plotx+10,ploty+147);
petitchiffrenoir.Print(ol::ToString( general_shape_size_to_apply ),plotx+130,ploty+147);
ol::Rect symbol_sizer_frame(ol::Vec2D(plotx+10,ploty+155),ol::Vec2D(100,10));
ol::Rect symbol_sizer_level(ol::Vec2D(plotx+10,ploty+155),ol::Vec2D(general_shape_size_to_apply,10));

ol::Rect OverSize(ol::Vec2D(plotx+126,ploty+136),ol::Vec2D(70,15));
OverSize.SetRoundness(4);
OverSize.DrawOutline(CouleurPlotLine.WithAlpha(0.5));

ol::Rect OpeningFrame(ol::Vec2D(plotx+10,ploty+240),ol::Vec2D(180,10));
ol::Rect OpeningLevel(ol::Vec2D(plotx+10,ploty+240),ol::Vec2D(( opening_angle_selected*180),10));


switch(index_edit_symbol_plot)
{
case 0:
symbol_sizer_level.Draw(CouleurPlotLine.WithAlpha(0.4));
break;
case 1:
symbol_sizer_level.Draw(CouleurFader);
break;
}
symbol_sizer_frame.DrawOutline(CouleurPlotLine);

ol::Rect MySHBox(ol::Vec2D(0,0),ol::Vec2D(45,20));
MySHBox.SetRoundness(5);
if(shape_selected_type>=0 && shape_selected_type<7)
{
petitchiffrenoir.Print("Color Pattern",plotx+10,ploty+185);
petitchiffrenoir.Print(ol::ToString(color_pattern_selected),plotx+100,ploty+185);
MySHBox.MoveTo(ol::Vec2D(plotx+10,ploty+190));
switch(color_pattern_selected)
{
case 0:
MySHBox.Draw(CouleurPlotLine);
break;
case 1://yellow
MySHBox.Draw(CouleurYellowFgroup);
break;
case 2://red
MySHBox.Draw(CouleurRed);
break;
case 3: //green
MySHBox.Draw(CouleurGreen);
break;
case 4: //blue
MySHBox.Draw(CouleurBleuProcedure);
break;
default:
break;
}
}
else
{
switch(shape_selected_type)
{
case 7://Rectangle
petitchiffrenoir.Print("Color Pattern",plotx+10,ploty+185);
petitchiffrenoir.Print(ol::ToString(color_pattern_selected),plotx+100,ploty+185);
MySHBox.MoveTo(ol::Vec2D(plotx+10,ploty+190));
switch(color_pattern_selected)
{
case 0://Outlined + Fill
MySHBox.Draw(CouleurPlotFill);
MySHBox.DrawOutline(CouleurPlotLine);
break;
case 1://Outlined only
MySHBox.DrawOutline(CouleurPlotLine);
break;
case 2: //filled Outline color
MySHBox.Draw(CouleurPlotLine);
break;
case 3: //filled PlotFill color
MySHBox.Draw(CouleurPlotFill);
break;
default:
break;
}
petitchiffrenoir.Print("Size X",plotx+10,ploty+225);
MySHBox.MoveTo(ol::Vec2D(plotx+10,ploty+230));
MySHBox.Draw(CouleurPlotLine.WithAlpha(0.4));
MySHBox.Draw(CouleurFader.WithAlpha(index_adjusting_shape_x));
petitchiffrenoir.Print(ol::ToString(shape_rect_size_x),plotx+15,ploty+242);
petitchiffrenoir.Print("Size Y",plotx+100,ploty+225);
MySHBox.MoveTo(ol::Vec2D(plotx+100,ploty+230));
MySHBox.Draw(CouleurPlotLine.WithAlpha(0.4));
MySHBox.Draw(CouleurFader.WithAlpha(index_adjusting_shape_y));
petitchiffrenoir.Print(ol::ToString(shape_rect_size_y),plotx+105,ploty+242);
break;
case 8://circle
petitchiffrenoir.Print("Color Pattern",plotx+10,ploty+185);
petitchiffrenoir.Print(ol::ToString(color_pattern_selected),plotx+100,ploty+185);
MySHBox.MoveTo(ol::Vec2D(plotx+10,ploty+190));
switch(color_pattern_selected)
{
case 0://Outlined + Fill
MySHBox.Draw(CouleurPlotFill);
MySHBox.DrawOutline(CouleurPlotLine);
break;
case 1://Outlined only
MySHBox.DrawOutline(CouleurPlotLine);
break;
case 2: //filled Outline color
MySHBox.Draw(CouleurPlotLine);
break;
case 3: //filled PlotFill color
MySHBox.Draw(CouleurPlotFill);
break;
default:
break;
}
break;
case 9://arc
petitchiffrenoir.Print("Color Pattern",plotx+10,ploty+185);
petitchiffrenoir.Print(ol::ToString(color_pattern_selected),plotx+100,ploty+185);
MySHBox.MoveTo(ol::Vec2D(plotx+10,ploty+190));
switch(color_pattern_selected)
{
case 0://Outlined + Fill
MySHBox.Draw(CouleurPlotFill);
MySHBox.DrawOutline(CouleurPlotLine);
break;
case 1://Outlined only
MySHBox.DrawOutline(CouleurPlotLine);
break;
case 2: //filled Outline color
MySHBox.Draw(CouleurPlotLine);
MySHBox.DrawOutline(CouleurPlotFill);
break;
case 3: //filled PlotFill color
MySHBox.Draw(CouleurPlotFill);
break;
default:
break;
}
petitchiffrenoir.Print("Opening angle:",plotx+10,ploty+225);
petitchiffrenoir.Print(ol::ToString( opening_angle_selected),plotx+120,ploty+225);
OpeningLevel.Draw(CouleurGreen);
OpeningFrame.DrawOutline(CouleurPlotLine);
break;
case 10://polygon
petitchiffrenoir.Print("Color Pattern",plotx+10,ploty+185);
petitchiffrenoir.Print(ol::ToString(color_pattern_selected),plotx+100,ploty+185);
MySHBox.MoveTo(ol::Vec2D(plotx+10,ploty+190));
switch(color_pattern_selected)
{
case 0://Outlined + Fill
MySHBox.Draw(CouleurPlotFill);
MySHBox.DrawOutline(CouleurPlotLine);
break;
case 1://Outlined only
MySHBox.DrawOutline(CouleurPlotLine);
break;
case 2: //filled Outline color
MySHBox.Draw(CouleurPlotLine);
MySHBox.DrawOutline(CouleurPlotFill);
break;
case 3: //filled PlotFill color
MySHBox.Draw(CouleurPlotFill);
break;
default:
break;
}
petitchiffrenoir.Print("Polygon Number of Points",plotx+10,ploty+225);
MySHBox.MoveTo(ol::Vec2D(plotx+10,ploty+230));
MySHBox.Draw(CouleurPlotLine.WithAlpha(0.4));
petitchiffrenoir.Print(ol::ToString(nbre_branches_polygon),plotx+15,ploty+242);
break;
case 11:
petitchiffrenoir.Print("Font Number",plotx+10,ploty+225);
MySHBox.MoveTo(ol::Vec2D(plotx+10,ploty+230));
MySHBox.Draw(CouleurPlotLine.WithAlpha(0.4));
petitchiffrenoir.Print(ol::ToString(fonttype_selected),plotx+15,ploty+242);
break;
default:
break;
}
}
ol::Rect PlotOptionView(ol::Vec2D(plotx+5,ploty+260),ol::Vec2D(10,10));
PlotOptionView.DrawOutline(CouleurPlotLine);
PlotOptionView.Draw(CouleurPlotLine.WithAlpha(index_show_shape_id));
petitchiffrenoir.Print("SHOW SYMBOL ID",plotx+25,ploty+268);
petitchiffrenoir.Print(ol::ToString(index_last_shape_selected),plotx+135,ploty+268);

ol::Rect PlotOptionsEdit_Long(ol::Vec2D(plotx+10,ploty+275),ol::Vec2D(175,18));
PlotOptionsEdit_Long.SetRoundness(3);
PlotOptionsEdit_Long.Draw(CouleurPlotFill.WithAlpha(0.3));
PlotOptionsEdit_Long.DrawOutline(CouleurPlotLine);

if(window_focus_id==W_PLOT && index_edit_light_plot==1 && mouse_x>plotx+10 && mouse_x<plotx+185 && mouse_y>ploty+275 && mouse_y<ploty+293)
{
PlotOptionsEdit_Long.DrawOutline(CouleurPlotFill.WithAlpha(0.9));
}
petitchiffrenoir.Print(shape_legend_name[index_last_shape_selected],plotx+15,ploty+287);

char TextPlot[25];
ol::Rect AeraDrawAdjust(ol::Vec2D(plotx+110,ploty+300),ol::Vec2D(70,70));
AeraDrawAdjust.DrawOutline(CouleurPlotLine.WithAlpha(0.3));

if(index_click_inside_relativ_xy==1 )
{
AeraDrawAdjust.SetLineWidth(2.0);
AeraDrawAdjust.DrawOutline(CouleurFader);
AeraDrawAdjust.Draw(CouleurBlind.WithAlpha(0.3));
sprintf(TextPlot,"%d . %d",rlativ_xm,rlativ_ym);
petitchiffrenoir.Print(TextPlot,plotx+130,ploty+340);
}


//alpha shape
petitchiffrenoir.Print("ALPHA SHAPES",plotx+5,ploty+370);
petitchiffrenoir.Print(ol::ToString(general_alpha_for_shape),plotx+115,ploty+390);
ol::Rect ShapeFrame(ol::Vec2D(plotx+5,ploty+380),ol::Vec2D(100,10));
ol::Rect ShapeLevel(ol::Vec2D(plotx+5,ploty+380),ol::Vec2D((general_alpha_for_shape*100),10));
ShapeLevel.Draw(CouleurPlotLine);
ShapeFrame.DrawOutline(CouleurPlotLine);

 return(0);
}

int symbol_edition_options(int plotx, int ploty)
{
char TextPlot[25];

ol::Rect PlotOptionsEdit_Little(ol::Vec2D(plotx,ploty),ol::Vec2D(50,18));
PlotOptionsEdit_Little.SetRoundness(3);
ol::Rect PlotOptionsEdit_Middle(ol::Vec2D(plotx,ploty),ol::Vec2D(100,18));
PlotOptionsEdit_Middle.SetRoundness(3);
ol::Rect PlotOptionsEdit_Long(ol::Vec2D(plotx,ploty),ol::Vec2D(175,18));
PlotOptionsEdit_Long.SetRoundness(3);
ol::Rect PlotOptionView(ol::Vec2D(plotx,ploty+70), ol::Vec2D(10,10));

char gelat_marque[25];

//properties
for (int l=0;l<8;l++)
{
PlotOptionView.MoveTo(ol::Vec2D(plotx+5,ploty+70+(20*l)));
PlotOptionView.DrawOutline(CouleurPlotLine);
switch(l)
{
case 0:
sprintf(TextPlot,"CHANNEL");
PlotOptionsEdit_Little.MoveTo(ol::Vec2D(plotx+100,ploty+65+(20*l)));
PlotOptionsEdit_Little.Draw(CouleurPlotLine.WithAlpha(0.2));
petitchiffrenoir.Print(ol::ToString(symbol_channel_is[view_plot_calc_number_is][last_selected_symbol_is]),plotx+105,ploty+78+(20*l));

if(window_focus_id==W_PLOT && index_edit_light_plot==1 && mouse_x>plotx+100 && mouse_x<plotx+150 && mouse_y>ploty+65+(20*l) && mouse_y<ploty+83+(20*l) )
{
PlotOptionsEdit_Little.DrawOutline(CouleurLigne.WithAlpha(0.5));
}
break;
case 1:
sprintf(TextPlot,"");
PlotOptionsEdit_Long.MoveTo(ol::Vec2D(plotx+20,ploty+65+(20*l)));
PlotOptionsEdit_Long.Draw(CouleurPlotLine.WithAlpha(0.2));
petitchiffrenoir.Print( descriptif_projecteurs[(symbol_channel_is[view_plot_calc_number_is][last_selected_symbol_is])],plotx+25,ploty+78+(20*l));
if(window_focus_id==W_PLOT && index_edit_light_plot==1 && mouse_x>plotx+20 && mouse_x<plotx+195 && mouse_y>ploty+65+(20*l) && mouse_y<ploty+83+(20*l) )
{
PlotOptionsEdit_Long.DrawOutline(CouleurLigne.WithAlpha(0.5));
}
break;
case 2:
sprintf(TextPlot,"GEL 1:");
PlotOptionsEdit_Little.MoveTo(ol::Vec2D(plotx+70,ploty+65+(20*l)));
PlotOptionsEdit_Little.Draw(CouleurPlotLine.WithAlpha(0.2));
petitchiffrenoir.Print(ol::ToString(gelat[view_plot_calc_number_is][last_selected_symbol_is][0]),plotx+75,ploty+78+(20*l));
if(window_focus_id==W_PLOT && index_edit_light_plot==1 && mouse_x>plotx+70 && mouse_x<plotx+120 && mouse_y>ploty+65+(20*l) && mouse_y<ploty+83+(20*l) )
{
PlotOptionsEdit_Little.DrawOutline(CouleurLigne.WithAlpha(0.5));
}
PlotOptionsEdit_Little.MoveTo(ol::Vec2D(plotx+130,ploty+65+(20*l)));
switch(gelat_family[view_plot_calc_number_is][last_selected_symbol_is][0])
{
case 0:
sprintf(gelat_marque,"Lee");
PlotOptionsEdit_Little.Draw(CouleurBleuProcedure.WithAlpha(0.5));
break;
case 1:
sprintf(gelat_marque,"Rosco");
PlotOptionsEdit_Little.Draw(CouleurBlind.WithAlpha(0.5));
break;
case 2:
sprintf(gelat_marque,"GamCol.");
PlotOptionsEdit_Little.Draw(CouleurGreen.WithAlpha(0.5));
break;
case 3:
sprintf(gelat_marque,"Apollo");
PlotOptionsEdit_Little.Draw(CouleurSelection.WithAlpha(0.5));
break;
default:
break;
}
petitchiffrenoir.Print(gelat_marque,plotx+135,ploty+78+(20*l));
break;
case 3:
sprintf(TextPlot,"GEL 2:");
PlotOptionsEdit_Little.MoveTo(ol::Vec2D(plotx+70,ploty+65+(20*l)));
PlotOptionsEdit_Little.Draw(CouleurPlotLine.WithAlpha(0.2));
petitchiffrenoir.Print(ol::ToString(gelat[view_plot_calc_number_is][last_selected_symbol_is][1]),plotx+75,ploty+78+(20*l));
if(window_focus_id==W_PLOT && index_edit_light_plot==1 && mouse_x>plotx+70 && mouse_x<plotx+120 && mouse_y>ploty+65+(20*l) && mouse_y<ploty+83+(20*l) )
{
PlotOptionsEdit_Little.DrawOutline(CouleurLigne.WithAlpha(0.5));
}
PlotOptionsEdit_Little.MoveTo(ol::Vec2D(plotx+130,ploty+65+(20*l)));
switch(gelat_family[view_plot_calc_number_is][last_selected_symbol_is][1])
{
case 0:
sprintf(gelat_marque,"Lee");
PlotOptionsEdit_Little.Draw(CouleurBleuProcedure.WithAlpha(0.5));
break;
case 1:
sprintf(gelat_marque,"Rosco");
PlotOptionsEdit_Little.Draw(CouleurBlind.WithAlpha(0.5));
break;
case 2:
sprintf(gelat_marque,"GamCol.");
PlotOptionsEdit_Little.Draw(CouleurGreen.WithAlpha(0.5));
break;
case 3:
sprintf(gelat_marque,"Apollo");
PlotOptionsEdit_Little.Draw(CouleurSelection.WithAlpha(0.5));
break;
default:
break;
}
PlotOptionsEdit_Little.MoveTo(ol::Vec2D(plotx+130,ploty+65+(20*l)));
petitchiffrenoir.Print(gelat_marque,plotx+135,ploty+78+(20*l));
break;
case 4:
sprintf(TextPlot,"GEL 3:");
PlotOptionsEdit_Little.MoveTo(ol::Vec2D(plotx+70,ploty+65+(20*l)));
PlotOptionsEdit_Little.Draw(CouleurPlotLine.WithAlpha(0.2));
petitchiffrenoir.Print(ol::ToString(gelat[view_plot_calc_number_is][last_selected_symbol_is][2]),plotx+75,ploty+78+(20*l));
if(window_focus_id==W_PLOT && index_edit_light_plot==1 && mouse_x>plotx+70 && mouse_x<plotx+120 && mouse_y>ploty+65+(20*l) && mouse_y<ploty+83+(20*l) )
{
PlotOptionsEdit_Little.DrawOutline(CouleurLigne.WithAlpha(0.5));
}
PlotOptionsEdit_Little.MoveTo(ol::Vec2D(plotx+130,ploty+65+(20*l)));
switch(gelat_family[view_plot_calc_number_is][last_selected_symbol_is][2])
{
case 0:
sprintf(gelat_marque,"Lee");
PlotOptionsEdit_Little.Draw(CouleurBleuProcedure.WithAlpha(0.5));
break;
case 1:
sprintf(gelat_marque,"Rosco");
PlotOptionsEdit_Little.Draw(CouleurBlind.WithAlpha(0.5));
break;
case 2:
sprintf(gelat_marque,"GamCol.");
PlotOptionsEdit_Little.Draw(CouleurGreen.WithAlpha(0.5));
break;
case 3:
sprintf(gelat_marque,"Apollo");
PlotOptionsEdit_Little.Draw(CouleurSelection.WithAlpha(0.5));
break;
default:
break;
}
petitchiffrenoir.Print(gelat_marque,plotx+135,ploty+78+(20*l));
break;
case 5:
sprintf(TextPlot,"DIMMER:");
PlotOptionsEdit_Little.MoveTo(ol::Vec2D(plotx+100,ploty+65+(20*l)));
PlotOptionsEdit_Little.Draw(CouleurPlotLine.WithAlpha(0.2));
if(window_focus_id==W_PLOT && index_edit_light_plot==1 && mouse_x>plotx+100 && mouse_x<plotx+150 && mouse_y>ploty+65+(20*l) && mouse_y<ploty+83+(20*l))
{
PlotOptionsEdit_Little.DrawOutline(CouleurLigne.WithAlpha(0.5));
}
petitchiffrenoir.Print(ol::ToString(symbol_dimmer_is[view_plot_calc_number_is][last_selected_symbol_is]),plotx+105,ploty+78+(20*l));
break;
case 6:
sprintf(TextPlot,"SHOW SYMBOL ID");
break;
case 7:
sprintf(TextPlot,"NOTES");
break;
default:
sprintf(TextPlot,"-");
break;
}
PlotOptionView.Draw(CouleurPlotLine.WithAlpha(plot_show_options[l]));
petitchiffrenoir.Print(TextPlot,plotx+25,ploty+78+(20*l));
}


//notes
for(int n=0;n<4;n++)
{
ol::Rect Not(ol::Vec2D(plotx+5,ploty+225+(n*20)),ol::Vec2D(195,18));
Not.SetRoundness(4);
Not.Draw(CouleurPlotLine.WithAlpha(0.3));
petitchiffrenoir.Print(symbol_note[view_plot_calc_number_is][last_selected_symbol_is][n],plotx+8,ploty+238+(n*20));
if(window_focus_id==W_PLOT && mouse_x>plotx+5 && mouse_x<plotx+200 && mouse_y>ploty+225+(n*20) && mouse_y<ploty+243+(n*20))
{
Not.DrawOutline(CouleurPlotLine.WithAlpha(0.6));
}
}

ol::Rect SizeNot(ol::Vec2D(plotx+150,ploty+210),ol::Vec2D(30,10));
SizeNot.Draw(CouleurPlotLine.WithAlpha(plot_name_text_size));
SizeNot.DrawOutline(CouleurPlotLine);

petitchiffrenoir.Print("Size",plotx+115,ploty+218);

//logique
index_edit_relativ_xy=0;
for(int i=0;i<5;i++)
{
if(adjust_xy_type[i]==1)
{
index_edit_relativ_xy=1;
break;
}
}
//////////// ADJUST origigne y + 215 pour adjust xy

ol::Rect SoloMode(ol::Vec2D(plotx+75,ploty+350),ol::Vec2D(10,10));
SoloMode.Draw(CouleurBlind.WithAlpha(mode_relatif_xy_solo));
SoloMode.DrawOutline(CouleurPlotLine.WithAlpha(0.5));

//int relatifs xy
petitchiffrenoir.Print("Adjust XY",plotx+115,ploty+315);
for(int i=0;i<5;i++)
{
PlotOptionView.MoveTo(ol::Vec2D(plotx+5,ploty+320+(20*i)));
PlotOptionView.Draw(CouleurPlotLine.WithAlpha(adjust_xy_type[i]));

PlotOptionView.DrawOutline(CouleurPlotLine.WithAlpha(0.5));
switch(i)
{
case 0:
sprintf(TextPlot,"Channel");
break;
case 1:
sprintf(TextPlot,"Name");
break;
case 2:
sprintf(TextPlot,"Gels");
break;
case 3:
sprintf(TextPlot,"Dimmer");
break;
case 4:
sprintf(TextPlot,"Notes");
break;
default:
sprintf(TextPlot,"-");
break;
}
if( window_focus_id==W_PLOT && mouse_x>plotx+5 && mouse_x<plotx+15 && mouse_y>ploty+320+(20*i) && mouse_y<ploty+330+(20*i) &&
 Midi_Faders_Affectation_Type!=0)
{
PlotOptionView.DrawOutline(CouleurBlind);

show_type_midi(1610+i, TextPlot);
}
petitchiffrenoir.Print(TextPlot,plotx+25,ploty+328+(20*i));
}

ol::Rect AeraDrawAdjust(ol::Vec2D(plotx+110,ploty+320),ol::Vec2D(70,70));
AeraDrawAdjust.DrawOutline(CouleurPlotLine.WithAlpha(0.3));

if(index_click_inside_relativ_xy==1 )
{
AeraDrawAdjust.SetLineWidth(2.0);
AeraDrawAdjust.DrawOutline(CouleurFader);

}
if(index_edit_relativ_xy==1)
{
AeraDrawAdjust.Draw(CouleurBlind.WithAlpha(0.3));
sprintf(TextPlot,"%d . %d",rlativ_xm,rlativ_ym);
petitchiffrenoir.Print(TextPlot,plotx+130,ploty+360);
}

//presets de positions
petitchiffrenoir.Print("AdjustXY Presets",plotx+45,ploty+425);
ol::Rect PresetBox(ol::Vec2D(0,0),ol::Vec2D(10,10));
for(int pr=0;pr<8;pr++)
{
PresetBox.MoveTo(ol::Vec2D(plotx+30+(20*pr),ploty+430));
PresetBox.Draw(CouleurFader.WithAlpha(plot_light_preset_on_click[pr]));     //visu de la sélection du preset
if(last_xyrelativ_preset==pr){if(plot_light_preset_on_click[pr]>0.0){plot_light_preset_on_click[pr]-=0.2;}}
PresetBox.DrawOutline(CouleurPlotLine.WithAlpha(0.5));

if(window_focus_id==W_PLOT &&  mouse_x>plotx+30+(20*pr) && mouse_x<plotx+40+(20*pr) && mouse_y>ploty+430 && mouse_y<ploty+440
 && Midi_Faders_Affectation_Type!=0)
{
PresetBox.DrawOutline(CouleurBlind);
char tmppp[24];
sprintf(tmppp,"Plot Preset %d",pr+1);
show_type_midi(1615+pr,tmppp);
}


}


//repatch directly
PlotOptionsEdit_Middle.MoveTo(ol::Vec2D(plotx+5,ploty+450));
PlotOptionsEdit_Middle.Draw(CouleurPlotLine.WithAlpha(0.5));
PlotOptionsEdit_Middle.Draw(CouleurBlind.WithAlpha(index_build_patch_from_plot));
petitchiffrenoir.Print("Link To Patch",plotx+15,ploty+463);

ol::Rect PlotOptionsEdit_LittleM(ol::Vec2D(plotx+110,ploty+450),ol::Vec2D(85,18));
PlotOptionsEdit_LittleM.SetRoundness(3);
PlotOptionsEdit_LittleM.Draw(CouleurPlotLine.WithAlpha(0.5));
petitchiffrenoir.Print("Clear Patch !",plotx+115,ploty+463);

 return(0);
}











int plot_symbol_edition(int plotx,int ploty)
{
ol::Rect PlotSymbEd(ol::Vec2D(plotx,ploty),ol::Vec2D(200,520));
PlotSymbEd.SetRoundness(15);
PlotSymbEd.DrawOutline(CouleurPlotLine.WithAlpha(0.4));
petitchiffrenoir.Print("SELECTED SYMBOLS:",plotx+10,ploty+15);
ol::Line(ol::Vec2D(plotx+10,ploty+20),ol::Vec2D(plotx+130,ploty+20)).Draw(CouleurPlotLine);

//rotation
petitchiffrenoir.Print("Rotate",plotx+10,ploty+33);
petitchiffrenoir.Print(ol::ToString(angle_projo_selectionne),plotx+80,ploty+33);
ol::Rect RotateFrame(ol::Vec2D(plotx+10,ploty+40),ol::Vec2D(100,10));
ol::Rect RotateLevel(ol::Vec2D(plotx+10,ploty+40),ol::Vec2D((angle_projo_selectionne*100),10));
RotateLevel.Draw(CouleurGreen);
RotateFrame.DrawOutline(CouleurPlotLine);

if(window_focus_id==W_PLOT && mouse_x>plotx+10 && mouse_x<plotx+110 && mouse_y>ploty+40 && mouse_y<ploty+50
&& Midi_Faders_Affectation_Type!=0)
{
RotateFrame.DrawOutline(CouleurBlind);
show_type_midi(1601,"Plot Rotate");
}



ol::Rect OverRot(ol::Vec2D(plotx+76,ploty+22),ol::Vec2D(60,15));
OverRot.SetRoundness(4);
OverRot.DrawOutline(CouleurPlotLine.WithAlpha(0.5));

ol::Rect AnglePos(ol::Vec2D(-10,-10),ol::Vec2D(10,10));

for(int l=0;l<3;l++)
{
for(int c=0;c<3;c++)
{
int pos=(l*3) + c;
AnglePos.MoveTo(ol::Vec2D(plotx+140+(c*15),ploty+10+(l*15)));
AnglePos.DrawOutline(CouleurPlotLine.WithAlpha(0.5));
if( window_focus_id==W_PLOT && mouse_x>plotx+140+(c*15) && mouse_x<plotx+150+(c*15) && mouse_y>ploty+10+(l*15) && mouse_y<ploty+20+(l*15) )
{
AnglePos.Draw(CouleurFader);
}
}
}
AnglePos.MoveTo(ol::Vec2D(plotx+155,ploty+25));
AnglePos.Draw(CouleurPlotFill);
AnglePos.DrawOutline(CouleurPlotFill);


ol::Rect MySymbAction(ol::Vec2D(plotx, ploty+60),ol::Vec2D(60,18));
MySymbAction.SetRoundness(5);

int ido=0;
for(int c=0;c<3;c++)
{
for(int l=0;l<2;l++)
{
MySymbAction.MoveTo(ol::Vec2D(plotx+5+(c*65),ploty+60+(l*20)));
MySymbAction.Draw(CouleurPlotLine.WithAlpha(0.5));

if(window_focus_id==W_PLOT && index_edit_light_plot==1 && index_click_inside_relativ_xy==0
&& mouse_x>plotx+5+(c*65) && mouse_x<plotx+65+(c*65) && mouse_y>ploty+60+(l*20) && mouse_y<ploty+78)
{
MySymbAction.DrawOutline(CouleurLigne.WithAlpha(0.5));
}


switch(c+(l*3))
{
case 0:
if(window_focus_id==W_PLOT && mouse_x>plotx+5+(c*65) && mouse_x<plotx+65+(c*65) && mouse_y>ploty+60 && mouse_y<ploty+78
&&  Midi_Faders_Affectation_Type!=0)
{
MySymbAction.DrawOutline(CouleurBlind);
show_type_midi(1602,"Plot Dub");
}
petitchiffre.Print("Dub",plotx+20+(c*65),ploty+72+(l*20));
break;
case 1:
if(window_focus_id==W_PLOT && mouse_x>plotx+5+(c*65) && mouse_x<plotx+65+(c*65) && mouse_y>ploty+60 && mouse_y<ploty+78
&&  Midi_Faders_Affectation_Type!=0)
{
MySymbAction.DrawOutline(CouleurBlind);
show_type_midi(1603,"Plot Delete");
}
petitchiffre.Print("Delete",plotx+10+(c*65),ploty+72+(l*20));
break;
case 2:
if(window_focus_id==W_PLOT && mouse_x>plotx+5+(c*65) && mouse_x<plotx+65+(c*65) && mouse_y>ploty+60 && mouse_y<ploty+78
&& Midi_Faders_Affectation_Type!=0)
{
MySymbAction.DrawOutline(CouleurBlind);
show_type_midi(1604,"Plot SendTo");
}
MySymbAction.Draw(CouleurFader.WithAlpha(index_plot_send_to_mode));
petitchiffre.Print("SendTo",plotx+10+(c*65),ploty+72+(l*20));
break;
case 3:
petitchiffre.Print("Down",plotx+15+(c*65),ploty+72+(l*20));
break;
case 4:
petitchiffre.Print("Top",plotx+22+(c*65),ploty+72+(l*20));
break;
case 5:
petitchiffre.Print("Grp/unGrp",plotx+7+(c*65),ploty+72+(l*20));
break;
default:
break;
}
}
}

//alignements
ol::Rect MySymbAlign(ol::Vec2D(plotx+5,ploty+80),ol::Vec2D(40,18));
 MySymbAlign.SetRoundness(5);
for(int c=0;c<4;c++)
{
 MySymbAlign.MoveTo(ol::Vec2D(plotx+10+(c*45),ploty+110));
 MySymbAlign.Draw(CouleurBleuProcedure.WithAlpha(0.5));
 switch(c)
{
case 0:
petitchiffre.Print("AlignX",plotx+10+(c*45),ploty+122);
break;
case 1:
petitchiffre.Print("AlignY",plotx+10+(c*45),ploty+122);
break;
case 2:
petitchiffre.Print("<-X->",plotx+15+(c*45),ploty+122);
break;
case 3:
petitchiffre.Print("<-Y->",plotx+15+(c*45),ploty+122);
break;
default:
break;
}
}
return(0);
}

int plot_menu_bar(int plotx, int ploty)
{
ol::Rect edit_frame(ol::Vec2D(plotx,ploty),ol::Vec2D(35,20));
edit_frame.SetRoundness(5);
edit_frame.Draw(CouleurBlind.WithAlpha(index_edit_light_plot));
edit_frame.DrawOutline(CouleurPlotLine);
petitchiffrenoir.Print("Edit",plotx+5,ploty+12);

ol::Rect LayerNum(ol::Vec2D(plotx+45,ploty),ol::Vec2D(10,10));
for(int i=0;i<4;i++)
{
LayerNum.MoveTo(ol::Vec2D(plotx+45+(i*15),ploty-5));
LayerNum.DrawOutline(CouleurPlotLine);
if(show_calc_number[i]==1)
{LayerNum.Draw(CouleurFader);}

if( window_focus_id==W_PLOT && mouse_x>plotx+45+(i*15) && mouse_x<plotx+55+(i*15) && mouse_y>ploty-5 && mouse_y<ploty+5 &&
 Midi_Faders_Affectation_Type!=0)
{
LayerNum.DrawOutline(CouleurBlind);
char tmmppp[24];
sprintf(tmmppp,"Plot Layer %d", i+1);
show_type_midi(1605+i,  tmmppp);
}
LayerNum.MoveTo(ol::Vec2D(plotx+45+(i*15),ploty+10));
if(view_plot_calc_number_is==i)
{LayerNum.Draw(CouleurPlotLine);}
LayerNum.DrawOutline(CouleurLigne.WithAlpha(0.3));

}

LayerNum.MoveTo(ol::Vec2D(plotx+110,ploty+2));
LayerNum.Draw(CouleurBlind.WithAlpha(plot_layer_mode));
LayerNum.DrawOutline(CouleurPlotLine);

if( window_focus_id==W_PLOT && mouse_x>plotx+110 && mouse_x<plotx+120 && mouse_y>ploty+2 && mouse_y<ploty+12 &&
Midi_Faders_Affectation_Type!=0)
{
 LayerNum.DrawOutline(CouleurBlind);
 show_type_midi(1609, "Plot Layer mode");
}


ol::Rect MenuBarIs(ol::Vec2D(plotx+130,ploty-5),ol::Vec2D(100,25));
MenuBarIs.SetRoundness(7.5);

MenuBarIs.Draw(CouleurNoir);
MenuBarIs.Draw(CouleurBleuProcedure.WithAlpha(0.5));

switch(index_menus_lighting_plot)
{
case 0:
     petitchiffre.Print("BACKGROUND",plotx+140,ploty+10);
break;
case 1:
     petitchiffre.Print("SHAPES",plotx+157,ploty+10);
break;
case 2:
     petitchiffre.Print("SYMBOLS",plotx+150,ploty+10);
break;
case 3:
     petitchiffre.Print("LEGEND",plotx+160,ploty+10);
break;
case 4:
     petitchiffre.Print("THE PLOT",plotx+155,ploty+10);
break;
default:
break;
}


ol::Rect OverViewPort(ol::Vec2D(plotx+240,ploty-5),ol::Vec2D(120,25));
OverViewPort.SetRoundness(7.5);
OverViewPort.Draw(CouleurFader.WithAlpha(index_move_plot_view_port));
OverViewPort.DrawOutline(CouleurPlotLine.WithAlpha(0.5));


char temp_h[16];
petitchiffrenoir.Print("ViewPort:", plotx+250,ploty+5);
sprintf(temp_h,"X:%d",plot_view_port_x);
petitchiffrenoir.Print(temp_h, plotx+250,ploty+15);
sprintf(temp_h,"Y:%d",plot_view_port_y);
petitchiffrenoir.Print(temp_h, plotx+300,ploty+15);



command_button_view(plotx+380,ploty-5,plot_index_show_levels,"SeeLevels","",1595);// int x, inty ,bool state, char *textedesc, int midiaffectation

command_button_view(plotx+460,ploty-5,plot_index_show_levels_from_faders,"SeeFaders","",1596);// int x, inty ,bool state, char *textedesc, int midiaffectation


command_button_view(plotx+540,ploty-5,index_blind,"Blind","",754);// int x, inty ,bool state, char *textedesc, int midiaffectation


if(index_menus_lighting_plot!=4)
{
ol::Canvas::SetClipping(plotx,ploty-10,plot_window_x_size-position_plan_x,ploty+50);
ol::Rect Export(ol::Vec2D(plotx+620,ploty-5),ol::Vec2D(100,20));
Export.SetRoundness(5);
if(index_show_button_export>0.0){index_show_button_export-=0.1; if(index_show_button_export<0.0){index_show_button_export=0.0;}}

Export.Draw(CouleurFader.WithAlpha(index_show_button_export));

Export.DrawOutline(CouleurPlotLine);
petitchiffrenoir.Print("Export View As",plotx+625,ploty+8);

ol::Rect NameExport(ol::Vec2D(plotx+730,ploty-5),ol::Vec2D(220,20));
NameExport.SetRoundness(5);
NameExport.Draw(CouleurBleuProcedure.WithAlpha(0.2));
if(window_focus_id==W_PLOT && mouse_x>plotx+730 && mouse_x<plotx+950 && mouse_x<plotx+plot_window_x_size-position_plan_x && mouse_y>ploty-5 && mouse_y<ploty+15)
{
 NameExport.DrawOutline(CouleurLigne.WithAlpha(0.5));
}
petitchiffrenoir.Print( plot_name_of_capture, plotx+730+5,ploty+8);
ol::Canvas::DisableClipping();
}


return(0);
}


int deroule_repertoire_plans(int xrep, int yrep, char name_of_rep[25])
{

//////////////////////LISTE DOSSIERS ETC///////////////////////////////////////

ol::Rect BackDeroule(ol::Vec2D(xrep,yrep+155),ol::Vec2D(210,185));
BackDeroule.SetRoundness(5);
BackDeroule.Draw(CouleurBleuProcedure.WithAlpha(0.4));
BackDeroule.Draw(CouleurPlotLine.WithAlpha(0.4));
petitchiffre.Print("Plans folder (.jpg .bmp .png .tga)",xrep+10,yrep+170);

ol::Canvas::SetClipping( xrep,yrep,xrep+230,yrep+175);

for (int y=0;y<8;y++)
{
ol::Rect OverFile(ol::Vec2D(xrep+5,(yrep+185+(y*20)-10)),ol::Vec2D(180,20));
OverFile.SetRoundness(7.5);
//affichage qui est selectionné
if(strcmp( Name_of_plane_is,list_import_plans[importplan_selected])==0 && (importplan_selected==(y+line_importplan)))
{OverFile.Draw(CouleurFond.WithAlpha(0.5));}

petitchiffrenoir.Print(list_import_plans[line_importplan+y],xrep+10,yrep+187+(y*20));
if(window_focus_id==W_PLOT &&  index_editing_theatre_plan==0 && mouse_x>xrep+5 && mouse_x<xrep+155 && mouse_y>(yrep+175+(y*20)) && mouse_y<(yrep+190+(y*20)))
{
OverFile.DrawOutline(CouleurLigne);
}

}


ol::Canvas::DisableClipping();

//////////////////UP DOWN LINE IMPORT////////////////////
ol::Circle LineUp(ol::Vec2D(xrep+190,yrep+200),12);
LineUp.Draw(CouleurFond);
ol::Circle LineDown(ol::Vec2D(xrep+190,yrep+310),12);
LineDown.Draw(CouleurFond);

petitchiffre.Print("-",xrep+186,yrep+205);
petitchiffre.Print("+",xrep+186,yrep+315);
LineUp.DrawOutline(CouleurLigne);
LineDown.DrawOutline(CouleurLigne);

ol::Rect BoutonRescan(ol::Vec2D(xrep+150,yrep+345),ol::Vec2D(60,20));
BoutonRescan.SetRoundness(4);
BoutonRescan.Draw(CouleurPlotLine.WithAlpha(0.6));
petitchiffre.Print("Rescan !",xrep+155,yrep+357);

if( window_focus_id==W_PLOT &&  index_editing_theatre_plan==0 && mouse_x>xrep+150 && mouse_x<xrep+210 && mouse_y>(yrep+345) && mouse_y<(yrep+365))
{
BoutonRescan.DrawOutline(CouleurLigne);
}

return(0);
}

int menu_plan(int plotx, int ploty)
{

ol::Rect PlotSymbList(ol::Vec2D(plotx,ploty),ol::Vec2D(205,220));
PlotSymbList.SetRoundness(15);
PlotSymbList.Draw(CouleurBleuProcedure.WithAlpha(0.4));
PlotSymbList.DrawOutline(CouleurPlotLine.WithAlpha(0.4));


petitchiffrenoir.Print("BACKGROUND:",plotx+10,ploty+15);
ol::Line(ol::Vec2D(plotx+10,ploty+20),ol::Vec2D(plotx+130,ploty+20)).Draw(CouleurPlotLine);


petitchiffrenoir.Print("Theatre Picture",plotx+5,ploty+40);
petitchiffrenoir.Print("Ratio:",plotx+110,ploty+40);
petitchiffrenoir.Print(ol::ToString(ratio_lock_plot_scale),plotx+150,ploty+40);

ol::Rect UnderName(ol::Vec2D(plotx+5,ploty+50),ol::Vec2D(190,20));
UnderName.SetRoundness(5);
UnderName.Draw(CouleurFond);
ol::Canvas::SetClipping( plotx+5,ploty+50,plotx+195,ploty+70);
petitchiffre.Print(Name_of_plane_is,plotx+10,ploty+63);
ol::Canvas::DisableClipping();

char coodr[8];
ol::Rect GridBGSelector(ol::Vec2D(plotx+40,ploty+145),ol::Vec2D(40,20));
GridBGSelector.SetRoundness(3.0);
for(int i=0;i<4;i++)
{

GridBGSelector.MoveTo(ol::Vec2D(plotx+5+(50*i),ploty+90));
GridBGSelector.Draw(CouleurPlotLine.WithAlpha(0.3));

if(window_focus_id==W_PLOT && mouse_x>plotx+5+(50*i) && mouse_x<plotx+50+(50*i) && mouse_y>ploty+90 && mouse_y<ploty+110)
{GridBGSelector.DrawOutline(CouleurLigne.WithAlpha(0.3));}

if(i+1==editing_plan_data_type)
{
GridBGSelector.Draw(CouleurFader);
}
switch(i)
{
case 0:
petitchiffrenoir.Print("X",plotx+20+(50*i),ploty+85);
sprintf(coodr,"%d",position_relative_plan_theatre[0]);
break;
case 1:
petitchiffrenoir.Print("Y",plotx+20+(50*i),ploty+85);
sprintf(coodr,"%d",position_relative_plan_theatre[1]);
break;
case 2:
petitchiffrenoir.Print("Scale X",plotx+5+(50*i),ploty+85);
sprintf(coodr,"%d",taille_relative_plan_theatre[0]);
break;
case 3:
petitchiffrenoir.Print("Scale Y",plotx+5+(50*i),ploty+85);
sprintf(coodr,"%d",taille_relative_plan_theatre[1]);
break;
default:
break;
}
petitchiffrenoir.Print(coodr, plotx+15+(50*i), ploty+102);
}

ol::Line(ol::Vec2D(plotx+130,ploty+110),ol::Vec2D(plotx+130,ploty+115)).Draw(CouleurPlotLine);
ol::Line(ol::Vec2D(plotx+130,ploty+115),ol::Vec2D(plotx+146,ploty+115)).Draw(CouleurPlotLine);
ol::Line(ol::Vec2D(plotx+154,ploty+115),ol::Vec2D(plotx+171,ploty+115)).Draw(CouleurPlotLine);
ol::Line(ol::Vec2D(plotx+171,ploty+115),ol::Vec2D(plotx+171,ploty+110)).Draw(CouleurPlotLine);
ol::Rect LockProp(ol::Vec2D(plotx+146,ploty+111),ol::Vec2D(8,8));
LockProp.Draw(CouleurBlind.WithAlpha(lock_background_proportions));
LockProp.DrawOutline(CouleurPlotLine);

//rotation
petitchiffrenoir.Print("Rotate",plotx+5,ploty+133);
petitchiffrenoir.Print(ol::ToString(orientation_plan_theatre),plotx+95,ploty+133);
ol::Rect AlphaFrame(ol::Vec2D(plotx+5,ploty+140),ol::Vec2D(100,10));
ol::Rect AlphaLevel(ol::Vec2D(plotx+5,ploty+140),ol::Vec2D((orientation_plan_theatre*100),10));
AlphaLevel.Draw(CouleurGreen);
AlphaFrame.DrawOutline(CouleurPlotLine);


ol::Rect AnglePos(ol::Vec2D(-10,-10),ol::Vec2D(10,10));

for(int l=0;l<3;l++)
{
for(int c=0;c<3;c++)
{
int pos=(l*3) + c;
AnglePos.MoveTo(ol::Vec2D(plotx+140+(c*15),ploty+125+(l*15)));
AnglePos.DrawOutline(CouleurPlotLine.WithAlpha(0.5));
if(window_focus_id==W_PLOT &&  mouse_x>plotx+140+(c*15) && mouse_x<plotx+150+(c*15) && mouse_y>ploty+125+(l*15) && mouse_y<ploty+135+(l*15) )
{
AnglePos.Draw(CouleurFader);
}
}
}
AnglePos.MoveTo(ol::Vec2D(plotx+155,ploty+140));
AnglePos.Draw(CouleurPlotFill);
AnglePos.DrawOutline(CouleurPlotFill);
AnglePos.Draw(CouleurBleuProcedure.WithAlpha(0.4));
AnglePos.DrawOutline(CouleurBleuProcedure.WithAlpha(0.4));
//alpha plan
petitchiffrenoir.Print("Alpha Picture",plotx+5,ploty+170);
petitchiffrenoir.Print(ol::ToString(alpha_plan),plotx+95,ploty+170);
ol::Rect RotateFrame(ol::Vec2D(plotx+5,ploty+180),ol::Vec2D(100,10));
ol::Rect RotateLevel(ol::Vec2D(plotx+5,ploty+180),ol::Vec2D((alpha_plan*100),10));
RotateLevel.Draw(CouleurPlotLine);
RotateFrame.DrawOutline(CouleurPlotLine);



petitchiffrenoir.Print("Grid:",plotx+5,ploty+210);
GridBGSelector.MoveTo(ol::Vec2D(plotx+40,ploty+195));

GridBGSelector.Draw(CouleurPlotLine.WithAlpha(0.3));
petitchiffrenoir.Print(ol::ToString(plot_quadrillage_size),plotx+50,ploty+210);

GridBGSelector.MoveTo(ol::Vec2D(plotx+100,ploty+195));
GridBGSelector.Draw(CouleurPlotLine.WithAlpha(0.3));
switch(plot_grid_type)
{
case 0:
petitchiffrenoir.Print("Dots",plotx+105,ploty+210);
break;
case 1:
petitchiffrenoir.Print("Line",plotx+105,ploty+210);
break;
}

petitchiffrenoir.Print("Alpha Grid",plotx+145,ploty+195);

ol::Rect AlphaG(ol::Vec2D(plotx+150,ploty+200),ol::Vec2D(50,10));
AlphaG.DrawOutline(CouleurPlotLine);
ol::Rect AlphaLev(ol::Vec2D(plotx+150,ploty+200),ol::Vec2D(50*alpha_grille,10));
AlphaLev.Draw(CouleurFader);
//liste des plans
deroule_repertoire_plans(plotx-5,ploty+80,"Plans");


//taille de la fenetre

ol::Rect PlotSize(ol::Vec2D(plotx,ploty+450),ol::Vec2D(205,65));
PlotSize.SetRoundness(7.5);
PlotSize.Draw(CouleurBleuProcedure.WithAlpha(0.4));
PlotSize.DrawOutline(CouleurPlotLine.WithAlpha(0.4));

petitchiffrenoir.Print("Plot Window:",plotx+10,ploty+465);

GridBGSelector.MoveTo(ol::Vec2D(plotx+5,ploty+480));
GridBGSelector.Draw(CouleurPlotLine.WithAlpha(0.3));
petitchiffrenoir.Print("Size X",plotx+7,ploty+478);
if(window_focus_id==W_PLOT && mouse_x>plotx+5 && mouse_x<plotx+45 && mouse_y>ploty+480 && mouse_y<ploty+500)
{
GridBGSelector.DrawOutline(CouleurLigne.WithAlpha(0.5));
}
if(editing_plot_sizex==1){GridBGSelector.Draw(CouleurFader);}
petitchiffrenoir.Print(ol::ToString(plot_window_x_size),plotx+10,ploty+493);


GridBGSelector.MoveTo(ol::Vec2D(plotx+55,ploty+480));
GridBGSelector.Draw(CouleurPlotLine.WithAlpha(0.3));
petitchiffrenoir.Print("Size Y",plotx+7+50,ploty+478);
if(window_focus_id==W_PLOT && mouse_x>plotx+55 && mouse_x<plotx+95 && mouse_y>ploty+480 && mouse_y<ploty+500)
{
GridBGSelector.DrawOutline(CouleurLigne.WithAlpha(0.5));
}
if(editing_plot_sizey==1){GridBGSelector.Draw(CouleurFader);}
petitchiffrenoir.Print(ol::ToString(plot_window_y_size),plotx+60,ploty+493);


GridBGSelector.MoveTo(ol::Vec2D(plotx+105,ploty+480));
GridBGSelector.Draw(CouleurPlotFill);
GridBGSelector.DrawOutline(CouleurLigne);
if(plot_editing_color_background==1)
{GridBGSelector.Draw(CouleurFader);}
petitchiffrenoir.Print("BkgCol",plotx+107,ploty+478);


GridBGSelector.MoveTo(ol::Vec2D(plotx+155,ploty+480));
GridBGSelector.Draw(CouleurPlotLine);
GridBGSelector.DrawOutline(CouleurLigne);
if(plot_editing_color_line==1)
{GridBGSelector.Draw(CouleurFader);}
petitchiffrenoir.Print("LineCol",plotx+157,ploty+478);

 return(0);
}



int plot_draw_plan_grid_for_view_point(int plotx, int ploty, int viewpx, int viewpy)
{
if(plot_quadrillage_size!=0)
{

plot_nbre_col=(plot_window_x_size-position_plan_x)/plot_quadrillage_size;
plot_nbre_lin=(plot_window_y_size-position_plan_y)/plot_quadrillage_size;

float reliquatx=(viewpx-plotx)%plot_quadrillage_size;
float reliquaty=(viewpy-ploty)%plot_quadrillage_size;
for(int i=0;i<=plot_nbre_col;i++)
{
switch(plot_grid_type)
{
case 0:
plot_draw_shape_stripline(plotx+(i*plot_quadrillage_size)+reliquatx,ploty,plotx+(i*plot_quadrillage_size)+reliquatx,ploty+(plot_window_y_size-position_plan_y), 1.0, alpha_grille,0,0, 0, 0);
break;
case 1:
ol::Line(ol::Vec2D(plotx+(i*plot_quadrillage_size)+reliquatx,ploty),ol::Vec2D(plotx+(i*plot_quadrillage_size)+reliquatx,ploty+(plot_window_y_size-position_plan_y))).Draw(CouleurPlotLine.WithAlpha(alpha_grille));
break;
}
}
for(int i=0;i<=plot_nbre_lin;i++)
{
switch(plot_grid_type)
{
case 0:
plot_draw_shape_stripline(plotx,ploty+(i*plot_quadrillage_size)+reliquaty,plotx+(plot_window_x_size-position_plan_x),ploty+(i*plot_quadrillage_size)+reliquaty, 1.0,  alpha_grille,0, 0,0, 0);
break;
case 1:
ol::Line(ol::Vec2D(plotx,ploty+(i*plot_quadrillage_size)+reliquaty),ol::Vec2D(plotx+(plot_window_x_size-position_plan_x),ploty+(i*plot_quadrillage_size)+reliquaty)).Draw(CouleurPlotLine.WithAlpha(alpha_grille));
}
}
}


return(0);
}




int plot_draw_shapes(int plotx, int ploty)
{

for(int i=1;i<=nbre_shapes_on_plot;i++)
{
//affichage symbole
switch(shape_type[i])
{
case 0:    //curtain
plot_draw_shape_curtain(plotx+ shape_position[i][0], ploty+shape_position[i][1] ,  plotx+shape_position[i][2], ploty+shape_position[i][3], shape_attributes[i][0]/5 ,  general_alpha_for_shape,shape_color_type[i] ,shape_selected[i]);
break;
case 1:     //ligne  plain
plot_draw_shape_line(plotx+ shape_position[i][0],  ploty+shape_position[i][1] , plotx+ shape_position[i][2],ploty+shape_position[i][3], shape_attributes[i][0]/10,  general_alpha_for_shape, shape_color_type[i],shape_selected[i]);
break;
case 2:     //stripline 1  int plotx1, int ploty1, int plotx2, int ploty2, float sizeshape,  float alphag, short typeofline,int col pattren bool isselected, bool extreme)
plot_draw_shape_stripline(plotx+ shape_position[i][0],  ploty+shape_position[i][1] , plotx+ shape_position[i][2],ploty+shape_position[i][3], shape_attributes[i][0]/10,  general_alpha_for_shape,0, shape_color_type[i] ,shape_selected[i], 1);
break;
case 3:     //stripline 2
plot_draw_shape_stripline(plotx+ shape_position[i][0],  ploty+shape_position[i][1] , plotx+ shape_position[i][2],ploty+shape_position[i][3], shape_attributes[i][0]/10,  general_alpha_for_shape,1, shape_color_type[i] ,shape_selected[i], 1);
break;
case 4:     //stripline 3
plot_draw_shape_stripline(plotx+ shape_position[i][0],  ploty+shape_position[i][1] , plotx+ shape_position[i][2],ploty+shape_position[i][3], shape_attributes[i][0]/10,  general_alpha_for_shape,2, shape_color_type[i] ,shape_selected[i], 1);
break;
case 5:     //stripline 4
plot_draw_shape_stripline(plotx+ shape_position[i][0],  ploty+shape_position[i][1] , plotx+ shape_position[i][2],ploty+shape_position[i][3], shape_attributes[i][0]/10,  general_alpha_for_shape,3, shape_color_type[i] ,shape_selected[i], 1);
break;
case 6:     //stripline 5
plot_draw_shape_stripline(plotx+ shape_position[i][0],  ploty+shape_position[i][1] , plotx+ shape_position[i][2],ploty+shape_position[i][3], shape_attributes[i][0]/10,  general_alpha_for_shape,4, shape_color_type[i] ,shape_selected[i], 1);
break;
case 7://plot_draw_rectangle(int plotx1, int ploty1, int shapesizex, int shapesizey, float shaperotation, float alphashape,  int colorpattern, bool isselected)
plot_draw_rectangle(plotx+ shape_position[i][0],  ploty+shape_position[i][1] , shape_attributes[i][2]*shape_attributes[i][0],shape_attributes[i][3]*shape_attributes[i][0], shape_attributes[i][1],  general_alpha_for_shape,shape_color_type[i],shape_selected[i]);
break;
case 8://plot_draw_circle(int plotx1, int ploty1, float sizeshape,  float alphashape,   int colorpattern, bool isselected)
plot_draw_circle(plotx+shape_position[i][0],  ploty+shape_position[i][1] ,5*shape_attributes[i][0], general_alpha_for_shape,shape_color_type[i],shape_selected[i]);
break;
case 9://plot_draw_slice(int plotx1, int ploty1, float sizeshape, float opening_angle, float shaperotation, float alphashape,   int colorpattern, bool isselected)
plot_draw_slice(plotx+shape_position[i][0],  ploty+shape_position[i][1] ,5*shape_attributes[i][0], shape_attributes[i][4]*6.5,shape_attributes[i][1],general_alpha_for_shape,shape_color_type[i],shape_selected[i]);
break;
case 10://polygon int plotx1, int ploty1, float sizeshape, float shaperotation,int numPoints,  float alphashape,   int colorpattern, bool isselected)
plot_draw_polygon(plotx+shape_position[i][0],  ploty+shape_position[i][1], shape_attributes[i][0]*5,shape_attributes[i][1],(int)shape_attributes[i][2], general_alpha_for_shape,shape_color_type[i],shape_selected[i]);
break;
case 11://(int plotx,int ploty, float fontsize,int fonttype,  char *text,float alphatext, bool isselected)
plot_draw_text(plotx+shape_position[i][0],  ploty+shape_position[i][1],(int)shape_attributes[i][0],(int)shape_attributes[i][2],shape_legend_name[i],general_alpha_for_shape, shape_selected[i]);
break;
default:
break;
}

if(index_show_shape_id==1)
{
petitchiffrenoir.Print(ol::ToString(i), plotx+shape_position[i][0]+10,  ploty+shape_position[i][1]+10);
if(shape_groups[i]!=0)
{
char Gr[8];
sprintf(Gr,"Gr. %d",shape_groups[i]);
petitchiffrenoir.Print(Gr, plotx+shape_position[i][0]+10,  ploty+shape_position[i][1]+20);
}
}
if(shape_type[i]!=11){petitchiffrenoir.Print(shape_legend_name[i], plotx+shape_position[i][0]+shape_relativ_position_legend_name[i][0],  ploty+shape_position[i][1]+shape_relativ_position_legend_name[i][1]);}
}
 return(0);
}


int plot_draw_plan(int plotx, int ploty, int plot_calc_number_is)
{

for(int i=1;i<=nbre_symbols_on_plot[plot_calc_number_is];i++)
{
//affichage symbole
switch(symbol_type[plot_calc_number_is][i])
{
case 0:     //PC 500 ou 650
plot_draw_symbol_pc(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 1:     //PC 1kw
plot_draw_symbol_pc(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 2:     //PC2kw
plot_draw_symbol_pc(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i],i, plot_calc_number_is);
break;
case 3:    //fresnel 1kw
plot_draw_symbol_fresnel(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 4:    //fresnel 2kw
plot_draw_symbol_fresnel(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1],size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 5:    //fresnel 5kw
plot_draw_symbol_fresnel(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 6: //dec source four
plot_draw_symbol_decoupe_etc_1(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1],size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 7: //dec source four
plot_draw_symbol_decoupe_etc_2(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1],size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 8: //dec source four
plot_draw_symbol_decoupe_etc_3(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1],size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 9: //dec 1kw longue
plot_draw_symbol_decoupe_1(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1],size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 10: //dec 1kw mid
plot_draw_symbol_decoupe_2(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1],size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 11: //dec 1kw wide
plot_draw_symbol_decoupe_3(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1],size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 12: //dec 2kw
plot_draw_symbol_decoupe_1(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1],size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 13: //dec 2kw
plot_draw_symbol_decoupe_2(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 14: //dec 2kw
plot_draw_symbol_decoupe_3(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 15: //PAR CP60
plot_draw_symbol_Par_1(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 16: //PAR CP61
plot_draw_symbol_Par_2(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 17: //PAR CP62
plot_draw_symbol_Par_3(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 18: //PAR CP95
plot_draw_symbol_Par_4(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1],size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 19://par 56 CP60
plot_draw_symbol_Par_1(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1],size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 20://par 56 CP61
plot_draw_symbol_Par_2(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 21://par 56 CP62
 plot_draw_symbol_Par_3(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 22://Par 36
plot_draw_symbol_Par_4(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 23://par 20
plot_draw_symbol_Par_3(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 24://par 16
plot_draw_symbol_Par_1(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 25://horiziode assymetrique
plot_draw_symbol_Horiziode_assym(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 26://horiziode symetrique
plot_draw_symbol_Horiziode_sym(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 27://hallogene de chantier
plot_draw_symbol_Horiziode_sym(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 28://BT250w
plot_draw_symbol_BT(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 29://BT500W
plot_draw_symbol_BT(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 30://rampe brabo
plot_draw_symbol_T8(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 31://blinder
plot_draw_symbol_Blinder(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 32://Svob
plot_draw_symbol_Svoboda(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 33://ACL
plot_draw_symbol_ACL(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 34://FLUO
plot_draw_symbol_fluo_little(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 35://FLUO
plot_draw_symbol_fluo_big(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 36://Follow Spot
plot_draw_symbol_FollowSpot1(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 37://Follow Spot
plot_draw_symbol_FollowSpot2(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 38://VP1
plot_draw_symbol_VP1(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 39://VP2
plot_draw_symbol_VP2(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 40://slide projector
plot_draw_symbol_slideprojector(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 41://retro projector
plot_draw_symbol_retroprojector(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 42://volets
plot_draw_symbol_barndoors(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 43://tophat
plot_draw_symbol_top_hat(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 44://color extender
plot_draw_symbol_color_extender(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 45://color changer
plot_draw_symbol_colorchanger(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 46://jalousie
plot_draw_symbol_jalousie(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 47://iris
plot_draw_symbol_iris(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 48://gobo
plot_draw_symbol_goboholder(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 49://shutter
plot_draw_symbol_shutter(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 50://mirror
plot_draw_symbol_motorized_mirror(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 51://smoke machine
plot_draw_symbol_smokemachine(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 52://fog machine
plot_draw_symbol_fogmachine(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 53://DIIMMER LINE
plot_draw_symbol_dimmerline(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 54://direct
plot_draw_symbol_direct(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 55://service
plot_draw_symbol_serviceligth(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 56://platine
plot_draw_symbol_platine_de_sol(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 57://pied
plot_draw_symbol_littlestand(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 58://grand pied
plot_draw_symbol_bigstand(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 59://barre de couplage
plot_draw_symbol_barre_de_couplage(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 60://Echelle
plot_draw_symbol_echelle(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 61://pont 50 1m
plot_draw_symbol_pont50_1m(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 62://pont 50 3m
plot_draw_symbol_pont50_3m(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 63://pont 50 jonction
plot_draw_symbol_pont50_jonction(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 64://pont 30 1m
plot_draw_symbol_pont30_1m(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 65://pont 30 3m
plot_draw_symbol_pont30_3m(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;
case 66://pont 30 jonction
plot_draw_symbol_pont30_jonction(plotx+pos_symbol[plot_calc_number_is][i][0], ploty+pos_symbol[plot_calc_number_is][i][1], size_symbol[(symbol_type[plot_calc_number_is][i])]*global_symbol_size,  angle_symbol[plot_calc_number_is][i], i, plot_calc_number_is);
break;

default:
break;
}


if( index_edit_light_plot==1 && (index_menus_lighting_plot==2 || index_menus_lighting_plot==4))//visualition du centre projo
{
ol::Line(ol::Vec2D(plotx+pos_symbol[plot_calc_number_is][i][0]-5,ploty+pos_symbol[plot_calc_number_is][i][1]-5),ol::Vec2D(plotx+pos_symbol[plot_calc_number_is][i][0]+5,ploty+pos_symbol[plot_calc_number_is][i][1]+5)).Draw(CouleurBlind);
ol::Line(ol::Vec2D(plotx+pos_symbol[plot_calc_number_is][i][0]+5,ploty+pos_symbol[plot_calc_number_is][i][1]-5),ol::Vec2D(plotx+pos_symbol[plot_calc_number_is][i][0]-5,ploty+pos_symbol[plot_calc_number_is][i][1]+5)).Draw(CouleurBlind);
if(plot_show_options[6]==1)
{
petitchiffrenoir.Print(ol::ToString(i),plotx+pos_symbol[plot_calc_number_is][i][0],ploty+pos_symbol[plot_calc_number_is][i][1]);
if(symbol_is_linked_to[plot_calc_number_is][i]>0)
{
//groupes de projos
char temp_txt_grp[16];
sprintf(temp_txt_grp,"Grp to %d",symbol_is_linked_to[plot_calc_number_is][i]);
petitchiffrenoir.Print(temp_txt_grp,plotx+pos_symbol[plot_calc_number_is][i][0],ploty+10+pos_symbol[plot_calc_number_is][i][1]);
}

}//show ID
if( plot_show_options[7]==1)
{
switch(plot_name_text_size)
{
case 0:
for(int n=0;n<4;n++)
{
petitchiffrenoir.Print(symbol_note[plot_calc_number_is][i][n], plotx+pos_symbol[plot_calc_number_is][i][0]+relatif_plot_xy[plot_calc_number_is][i][4][0],ploty+pos_symbol[plot_calc_number_is][i][1]+relatif_plot_xy[plot_calc_number_is][i][4][1]+(n*10));
}
break;
case 1:
for(int n=0;n<4;n++)
{
minichiffre.Print(symbol_note[plot_calc_number_is][i][n], plotx+pos_symbol[plot_calc_number_is][i][0]+relatif_plot_xy[plot_calc_number_is][i][4][0],ploty+pos_symbol[plot_calc_number_is][i][1]+relatif_plot_xy[plot_calc_number_is][i][4][1]+(n*10));
}
break;
}
}
}


//si cicuit
if( symbol_channel_is[plot_calc_number_is][i]!=0 && i>0)
{

bool way=0;
if(plot_show_options[0]==1)
{

if(symbol_is_linked_to[plot_calc_number_is][i]==0)
{

if(relatif_plot_xy[plot_calc_number_is][i][0][0]<0) way=1; //( right to left)
draw_line_to_channel_of_symbol(plotx+pos_symbol[plot_calc_number_is][i][0],ploty+pos_symbol[plot_calc_number_is][i][1],
plotx+pos_symbol[plot_calc_number_is][i][0]+relatif_plot_xy[plot_calc_number_is][i][0][0],
ploty+pos_symbol[plot_calc_number_is][i][1]+relatif_plot_xy[plot_calc_number_is][i][0][1], way);

draw_channel_of_symbol(plot_calc_number_is,i,
plotx+pos_symbol[plot_calc_number_is][i][0]+relatif_plot_xy[plot_calc_number_is][i][0][0],
ploty+pos_symbol[plot_calc_number_is][i][1]+relatif_plot_xy[plot_calc_number_is][i][0][1]);
}
else
{
int dest_s=symbol_is_linked_to[plot_calc_number_is][i];
way=0;
if((plotx+pos_symbol[plot_calc_number_is][dest_s][0]+relatif_plot_xy[plot_calc_number_is][dest_s][0][0])-(plotx+pos_symbol[plot_calc_number_is][i][0])<0)
way=1;
draw_line_to_channel_of_symbol(
plotx+pos_symbol[plot_calc_number_is][i][0],
ploty+pos_symbol[plot_calc_number_is][i][1],
plotx+pos_symbol[plot_calc_number_is][dest_s][0]+relatif_plot_xy[plot_calc_number_is][dest_s][0][0],
ploty+pos_symbol[plot_calc_number_is][dest_s][1]+relatif_plot_xy[plot_calc_number_is][dest_s][0][1],
 0);
}


}

if(plot_show_options[1]==1 )
switch( plot_name_text_size)
{
case 0:
petitchiffrenoir.Print(descriptif_projecteurs[(symbol_channel_is[plot_calc_number_is][i])],
plotx+pos_symbol[plot_calc_number_is][i][0]-50+relatif_plot_xy[plot_calc_number_is][i][1][0],(ploty+50+pos_symbol[plot_calc_number_is][i][1]+relatif_plot_xy[plot_calc_number_is][i][1][1]));
break;
case 1:
minichiffre.Print(descriptif_projecteurs[(symbol_channel_is[plot_calc_number_is][i])],
plotx+pos_symbol[plot_calc_number_is][i][0]-50+relatif_plot_xy[plot_calc_number_is][i][1][0],(ploty+50+pos_symbol[plot_calc_number_is][i][1]+relatif_plot_xy[plot_calc_number_is][i][1][1]));
break;
}
}


if(plot_show_options[5]==1 && symbol_dimmer_is[plot_calc_number_is][i]!=0 )
{
sprintf(temp_plot_info,"d:%d",symbol_dimmer_is[plot_calc_number_is][i]);
petitchiffrenoir.Print(temp_plot_info,plotx+pos_symbol[plot_calc_number_is][i][0]+10+relatif_plot_xy[plot_calc_number_is][i][3][0],ploty+pos_symbol[plot_calc_number_is][i][1]+80+relatif_plot_xy[plot_calc_number_is][i][3][1]);

}
//hors affichage du circuit
draw_gels_of_a_symbol(plot_calc_number_is,i,
plotx+pos_symbol[plot_calc_number_is][i][0]+relatif_plot_xy[plot_calc_number_is][i][2][0],ploty+pos_symbol[plot_calc_number_is][i][1]+relatif_plot_xy[plot_calc_number_is][i][2][1]);


}

return(0);
}

int plot_draw_levels(int plotx, int ploty, int plot_calc_number_is)
{

for(int i=1;i<=nbre_symbols_on_plot[plot_calc_number_is];i++)
{

if(plot_index_show_levels==1 && symbol_channel_is[plot_calc_number_is][i]!=0 && symbol_channel_is[plot_calc_number_is][i]<513 )
{
char lev[8];
ol::Rect etiquette(ol::Vec2D(plotx+pos_symbol[plot_calc_number_is][i][0]-15,ploty+pos_symbol[plot_calc_number_is][i][1]-35),ol::Vec2D(30,20));
etiquette.SetRoundness(5);
switch(index_blind)
{
case 0:
if(bufferSequenciel[( symbol_channel_is[plot_calc_number_is][i])]>0)
{
switch(dmx_view)
{
case 0://stage et faders
sprintf(lev,"%d",(int) ((float)(bufferSequenciel[( symbol_channel_is[plot_calc_number_is][i])])/2.55));
break;
case 1:
sprintf(lev,"%d",bufferSequenciel[( symbol_channel_is[plot_calc_number_is][i])]);
break;
}
etiquette.Draw(CouleurPlotLine.WithAlpha(0.2));
etiquette.Draw(CouleurBleuProcedure.WithAlpha(((float)(bufferSequenciel[( symbol_channel_is[plot_calc_number_is][i])])/255)));
petitchiffre.Print(lev,plotx+pos_symbol[plot_calc_number_is][i][0]-10,ploty+pos_symbol[plot_calc_number_is][i][1]-20);
}
if( plot_index_show_levels_from_faders==1 && bufferFaders[( symbol_channel_is[plot_calc_number_is][i])]>0)
{
etiquette.MoveTo(ol::Vec2D(plotx+pos_symbol[plot_calc_number_is][i][0]-15,ploty+pos_symbol[plot_calc_number_is][i][1]-55));
switch(dmx_view)
{
case 0:
sprintf(lev,"%d",(int) ((float)(bufferFaders[( symbol_channel_is[plot_calc_number_is][i])])/2.55));
break;
case 1:
sprintf(lev,"%d",bufferFaders[( symbol_channel_is[plot_calc_number_is][i])]);
break;
}
etiquette.Draw(CouleurPlotLine.WithAlpha(0.2));
etiquette.Draw(CouleurFader.WithAlpha(((float)(bufferFaders[( symbol_channel_is[plot_calc_number_is][i])])/255)));
etiquette.DrawOutline(CouleurPlotLine.WithAlpha(0.8));
petitchiffre.Print(lev,plotx+pos_symbol[plot_calc_number_is][i][0]-10,ploty+pos_symbol[plot_calc_number_is][i][1]-40);
}
break;
case 1://blind
if(bufferBlind[( symbol_channel_is[plot_calc_number_is][i])]>0)
{
switch(dmx_view)
{
case 0:
sprintf(lev,"%d",(int) ((float)(bufferBlind[( symbol_channel_is[plot_calc_number_is][i])])/2.55));
break;
case 1:
sprintf(lev,"%d",bufferBlind[( symbol_channel_is[plot_calc_number_is][i])]);
break;
}
etiquette.Draw(CouleurPlotLine.WithAlpha(0.2));
etiquette.Draw(CouleurBlind.WithAlpha(((float)(bufferBlind[( symbol_channel_is[plot_calc_number_is][i])])/255)));
petitchiffre.Print(lev,plotx+pos_symbol[plot_calc_number_is][i][0]-10,ploty+pos_symbol[plot_calc_number_is][i][1]-20);
}
break;
}
}

}
return(0);
}

int plot_draw_plan_bitmap(int xplot, int yplot, float alphaplan)
{

LightPlanBitmapMain.BlitTransformed(xplot+position_relative_plan_theatre[0], yplot+position_relative_plan_theatre[1],taille_relative_plan_theatre[0],taille_relative_plan_theatre[1],orientation_plan_theatre*6.5,alphaplan);

return(0);
}



int plot_draw_legend(int xplot, int yplot)
{


ol::Rect CadreDescriptif(ol::Vec2D(xplot,yplot),ol::Vec2D(180,25));
CadreDescriptif.SetRoundness(7.5);
CadreDescriptif.Draw(CouleurBlind.WithAlpha(0.5));
neuromoyen.Print("Spots & Symbols List" ,xplot+5,yplot+14);

int refth=0;
int thx=0; int thy=0;
int the_ecrat=0;
int nbr_sy=0;
int nbre_cl=0;
int typap=0;



switch(legend_view)
{
case 0://liste simple
nbre_cl=nbre_symbols_differents_sur_plot/16;
for(int cl=0;cl<nbre_cl+1;cl++)
{
the_ecrat=0;
for(int i=0;i<16;i++)
{
refth=i+(cl*16);

typap=index_list_appareils_pour_la_legende[refth];
if(refth<nbre_symbols_differents_sur_plot)
{
petitchiffrenoir.Print(ol::ToString(plot_list_appareils[typap]),xplot+(200*cl),yplot+40+(i*15));
petitchiffrenoir.Print(symbol_nickname[typap],xplot+30+(200*cl),yplot+40+(i*15));
}
}
}
break;
case 1://legende projos
nbre_cl=nbre_symbols_differents_sur_plot/8;
for(int cl=0;cl<nbre_cl+1;cl++)
{
the_ecrat=0;
for(int i=0;i<8;i++)
{
refth=i+(cl*8);

typap=index_list_appareils_pour_la_legende[refth];
if(refth<nbre_symbols_differents_sur_plot)
{
thx= xplot+70+(cl*220);
the_ecrat+= 35+ (int)(plot_ecartement_legende[typap]*size_symbol[typap]);
thy= yplot+50+the_ecrat;
plot_print_neutral_symbol(typap,thx,thy   );
petitchiffrenoir.Print(ol::ToString(plot_list_appareils[typap]),thx+40,thy);
petitchiffrenoir.Print(symbol_nickname[typap],thx+60,thy);
}
}
}
break;
}

if(thx==0){thx=200;}


CadreDescriptif.MoveTo(ol::Vec2D(xplot+thx,yplot));
CadreDescriptif.Draw(CouleurBlind.WithAlpha(0.5));
neuromoyen.Print("Sorted Gels List" ,xplot+5+thx,yplot+14);


int clnbre=nbre_ligne_pour_gelats/24;


for(int cl=0;cl<=clnbre;cl++)
{
for(int ln=0;ln<24;ln++)
{
petitchiffrenoir.Print(Geo[ln+(cl*ln)],xplot+thx+(cl*200),yplot+40+(ln*10));
}
}
 return(0);
}


int plot_legend_menu(int xplot, int yplot)
{
ol::Rect Undernamelist(ol::Vec2D(xplot+10,yplot),ol::Vec2D(185,10+(25*18)));
Undernamelist.SetRoundness(5);
Undernamelist.Draw(CouleurPlotLine.WithAlpha(0.05));
ol::Rect UnderName(ol::Vec2D(xplot+10,yplot+5),ol::Vec2D(185,20));
UnderName.SetRoundness(5);
for(int i=0;i<18;i++)
{
UnderName.MoveTo(ol::Vec2D(xplot+10,yplot+(i*25)));
petitchiffrenoir.Print(plot_renseignements[i],xplot+15,yplot+15+(i*25));
if(window_focus_id==W_PLOT && index_edit_light_plot==1 && mouse_x>xplot+10 && mouse_x<xplot+195 && mouse_y>yplot+5+(i*25) && mouse_y<yplot+25+(i*25))
{
UnderName.DrawOutline(CouleurPlotLine.WithAlpha(0.6));
if(mouse_b&1 && mouse_released==0)
{
sprintf(plot_renseignements[i],numeric);
reset_numeric_entry();numeric_postext=0;
if(index_text_auto_close==1){index_type=0;}
mouse_released=1;
}
}
}

UnderName.MoveTo(ol::Vec2D(xplot+10,yplot+470));
UnderName.Draw(CouleurPlotLine.WithAlpha(0.05));
ol::Line(ol::Vec2D(xplot+10, yplot+505), ol::Vec2D(xplot+10 + plot_quadrillage_size, yplot+505)).Draw(CouleurPlotLine);
ol::Line(ol::Vec2D(xplot+10, yplot+495), ol::Vec2D(xplot+10 , yplot+515)).Draw(CouleurPlotLine);
ol::Line(ol::Vec2D(xplot+10+ plot_quadrillage_size, yplot+495), ol::Vec2D(xplot+10+ plot_quadrillage_size , yplot+515)).Draw(CouleurPlotLine);
petitchiffrenoir.Print(plot_renseignements[19],xplot+15,yplot+483);
if(window_focus_id==W_PLOT && index_edit_light_plot==1 && mouse_x>xplot+10 && mouse_x<xplot+195 && mouse_y>yplot+470 && mouse_y<yplot+490)
{
UnderName.DrawOutline(CouleurPlotLine.WithAlpha(0.6));
if(mouse_b&1 && mouse_released==0)
{
sprintf(plot_renseignements[19],numeric);
reset_numeric_entry();numeric_postext=0;
if(index_text_auto_close==1){index_type=0;}
mouse_released=1;
}
}

petitchiffrenoir.Print(plot_renseignements[20],xplot+15,yplot+540);
petitchiffrenoir.Print(plot_renseignements[21],xplot+15,yplot+560);


ol::Rect Mode(ol::Vec2D(xplot+10,yplot+580),ol::Vec2D(95,20));
Mode.SetRoundness(5);


switch(legend_view)
{
case 0://liste
Mode.Draw(CouleurPlotLine.WithAlpha(0.3));
petitchiffrenoir.Print(" LIST MODE",xplot+15,yplot+592);
break;
case 1://appareils
Mode.Draw(CouleurFader);
petitchiffrenoir.Print("LEGEND MODE",xplot+15,yplot+592);
break;
}

if(window_focus_id==W_PLOT && mouse_x>xplot+10 && mouse_x<xplot+105 && mouse_y>yplot+580 && mouse_y<yplot+600)
{
Mode.DrawOutline(CouleurLigne.WithAlpha(0.6));
if(mouse_button==1 && mouse_released==0)
{
legend_view=toggle(legend_view);
mouse_released=1;
}
}

return(0);
}

int Plot_window(int plotx, int ploty)
{

ol::Rect PlotPanel(ol::Vec2D(plotx,  ploty), ol::Vec2D( plot_window_x_size,plot_window_y_size));
PlotPanel.SetRoundness(15);
PlotPanel.SetLineWidth(triple_epaisseur_ligne_fader);
PlotPanel.Draw(CouleurPlotFill);
ol::Rect PlotBehindMove(ol::Vec2D(plotx+3,ploty+5),ol::Vec2D(220,40));
PlotBehindMove.SetRoundness(15);
PlotBehindMove.Draw(CouleurFond.WithAlpha(0.5));
if(window_focus_id==W_PLOT)
{
PlotPanel.DrawOutline(CouleurFader);
}
else {PlotPanel.DrawOutline(CouleurLigne); }
neuro.Print("LIGHT PLOT",plotx+100,ploty+30);

petitchiffrenoir.SetColor(CouleurPlotLine );
minichiffre.SetColor(CouleurPlotLine );

plot_menu_bar(plotx+230,ploty+10);//menus generaux de plot

ol::Canvas::SetClipping( plotx,ploty,plot_window_x_size,plot_window_y_size);


switch(index_menus_lighting_plot)
{
case 0://plan
menu_plan(plotx+10, ploty+50);
logical_deroule_repertoire_plans(plotx+5,ploty+130); //a garder pour rafraichir le plan correctement
break;
case 1://shapes
plot_shape_list(plotx+10, ploty+50);
shape_edition(plotx+10, ploty+200);//editeur des props du shape
//logical_shape_edition(plotx+10, ploty+200);
break;
case 2://symbols
plot_symbol_list(plotx+10, ploty+50);//selecteur symbol
//logical_plot_symbol_list(plotx+10, ploty+50);
plot_symbol_edition(plotx+10, ploty+200);//editeur de symbol
//logical_plot_symbol_edition(plotx+10, ploty+200);//editeur de symbol
symbol_edition_options(plotx+10,ploty+275);
//logical_symbol_edition_options(plotx+10, ploty+275);
break;
case 3://légende
plot_legend_menu(plotx+10,ploty+50);
break;
default:
break;
}
ol::Canvas::DisableClipping();

ol::Canvas::SetClipping( plotx+position_plan_x,ploty+position_plan_y,plot_window_x_size-position_plan_x,plot_window_y_size-position_plan_y);

if(index_menus_lighting_plot!=3 )
{
plot_draw_plan_bitmap(plotx+position_plan_x+plot_view_port_x,ploty+position_plan_y+plot_view_port_y, alpha_plan);
if(index_menus_lighting_plot!=4 ){plot_draw_plan_grid_for_view_point(plotx+position_plan_x,ploty+position_plan_y,plotx+plot_view_port_x,ploty+plot_view_port_y);}
plot_draw_shapes(plotx+position_plan_x+plot_view_port_x,ploty+position_plan_y+plot_view_port_y);
for(int i=0;i<4;i++)
{
if(show_calc_number[i]==1)
{
plot_draw_plan(plotx+position_plan_x+plot_view_port_x,ploty+position_plan_y+plot_view_port_y,i);
}
}
for(int j=0;j<4;j++)
{
if(show_calc_number[j]==1)
{
plot_draw_levels(plotx+position_plan_x+plot_view_port_x,ploty+position_plan_y+plot_view_port_y,j);
}
}
}
else
{
plot_draw_legend(plotx+position_plan_x+plot_view_port_x-plot_correcteur_portx,ploty+position_plan_y+plot_view_port_y-plot_correcteur_porty);
}

ol::Canvas::DisableClipping();


petitchiffrenoir.SetColor(ol::Rgba::BLACK );
minichiffre.SetColor(ol::Rgba::WHITE );
return(0);
}



