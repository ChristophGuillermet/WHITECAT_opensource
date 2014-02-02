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

 \file patch_splines_2.cpp
 \brief {description courte} 
 \author Christoph Guillermet
 \version {numero de version du fichier}
 \date {date description}
 
 White Cat {- catégorie} {- sous catégorie {- sous catégorie}}
 Description détaillée
 
 **/
 
 
/* calculates the distance between two curve_nodes */
fixed curve_node_dist(curve_node n1, curve_node n2)
{
   #define SCALE  64

   fixed dx = itofix(n1.x - n2.x) / SCALE;
   fixed dy = itofix(n1.y - n2.y) / SCALE;

   return fixsqrt(fixmul(dx, dx) + fixmul(dy, dy)) * SCALE;
}


/* constructs curve_nodes to go at the ends of the list, for tangent calculations */
curve_node dummy_curve_node(curve_node curve_Node, curve_node prev)
{
   curve_node n;

   n.x = curve_Node.x - (prev.x - curve_Node.x) / 8;
   n.y = curve_Node.y - (prev.y - curve_Node.y) / 8;
   n.tangent = itofix(0);

   return n;
}



/* calculates a set of curve_node tangents */
void curve_calc_tangents(void)
{  
     
     //pas touche prepa tangentes OK pour sens en avant
   int i;
   
   curve_nodes[0] = dummy_curve_node(curve_nodes[1], curve_nodes[2]);
   curve_nodes[curve_node_count] = dummy_curve_node(curve_nodes[curve_node_count-1], curve_nodes[curve_node_count-2]);
   
   curve_node_count++;

  for (i=1; i<curve_node_count-1; i++)
      curve_nodes[i].tangent = fixatan2(itofix(curve_nodes[i+1].y - curve_nodes[i-1].y),
				  itofix(curve_nodes[i+1].x - curve_nodes[i-1].x));
				  
				  
 			  
}




/* calculates the control points for a spline segment */
void curve_get_control_points(curve_node n1, curve_node n2, int points[8])
{
   fixed dist = fixmul(curve_node_dist(n1, n2), curve_curviness);

   points[0] = n1.x;
   points[1] = n1.y;

   points[2] = n1.x + fixtoi(fixmul(fixcos(n1.tangent), dist));
   points[3] = n1.y + fixtoi(fixmul(fixsin(n1.tangent), dist));

   points[4] = n2.x - fixtoi(fixmul(fixcos(n2.tangent), dist));
   points[5] = n2.y - fixtoi(fixmul(fixsin(n2.tangent), dist));

   points[6] = n2.x;
   points[7] = n2.y;
}



int write_curve()
{
index_writing_curve=1;
 int points[MAX_curve_nodeS ];// etait [8] le 24 aout
 
 
 curve_curviness = ftofix(curve_spline_level); 
 curve_calc_tangents();

 for(int nio=1;nio<(curve_node_count-1);nio++)
 {
 curve_get_control_points(curve_nodes[nio],curve_nodes[nio+1],points);
 
 int resolu= (curve_nodes[nio+1].x) - (curve_nodes[nio].x);
 int temp_curve_x[resolu];
 int temp_curve_y[resolu];
 calc_spline(points,resolu,  temp_curve_x, temp_curve_y); 
 
 int index_sp=0;
 
 for(int cuv=0;cuv<resolu;cuv++)
 {
 index_sp=(curve_nodes[nio].x+cuv)-(xpatch_window+30+455);
 
 if(index_sp>255){index_sp=255;}
 if(index_sp<0){index_sp=0;}
 
 curve_report[curve_selected][index_sp]=temp_curve_y[cuv]-(ypatch_window+50);
 
 if(curve_report[curve_selected][index_sp]<0){curve_report[curve_selected][index_sp]=0;}
 if(curve_report[curve_selected][index_sp]>255){curve_report[curve_selected][index_sp]=255;}
 
 }  
 }
index_writing_curve=0;
 return(0);   
}





int draw_curve_node(int n)
{
   if(n<6){
   Circle(curve_nodes[n].x, curve_nodes[n].y, 6).DrawOutline(CouleurBlind);
   petitchiffre.Print(ol::ToString(n),curve_nodes[n].x-7, curve_nodes[n].y-7);
   }
   

   if( window_focus_id==W_PATCH && mouse_x>=((curve_nodes[n].x)-(diam_curve_node/2)) && mouse_x<=((curve_nodes[n].x)+(diam_curve_node/2))
   && mouse_x>=xpatch_window+30+455 && mouse_x<=xpatch_window+30+455+255 
   && mouse_x>(curve_nodes[n-1].x+(diam_curve_node/2)) && mouse_x<(curve_nodes[n+1].x-(diam_curve_node/2))
   && mouse_y>=ypatch_window+50 && mouse_y<=ypatch_window+255+50    )
   {
     Line( Vec2D( curve_nodes[n].x, ypatch_window+50 ), Vec2D(curve_nodes[n].x,ypatch_window+255+50)).Draw(Rgba::YELLOW);
     Circle(curve_nodes[n].x, curve_nodes[n].y, 6).Draw(CouleurBlind); 
    
    if(mouse_b&1 && index_enable_curve_editing==1 )
    {
    if(n>1 && n<5)
    {
    curve_ctrl_pt[curve_selected][n][0]=mouse_x-(xpatch_window+30+455);
    curve_ctrl_pt[curve_selected][n][1]=mouse_y-(ypatch_window+50);  
    }
    if(n==1)
    {
    curve_ctrl_pt[curve_selected][n][0]=0;
    curve_ctrl_pt[curve_selected][n][1]=mouse_y-(ypatch_window+50);          
    }
    if(n==5)
    {
    curve_ctrl_pt[curve_selected][n][0]=255;
    curve_ctrl_pt[curve_selected][n][1]=mouse_y-(ypatch_window+50);  
    }

    write_curve();//ecriture des niveaux
    }                                      
                                           
   }
  
   if (dmx_view==1)//255
   {
   //affichage OUT
   petitpetitchiffrerouge.Print(ol::ToString(255-curve_ctrl_pt[curve_selected][n][1]),xpatch_window+5+455,ypatch_window+50+curve_ctrl_pt[curve_selected][n][1]);
   //affichage IN
   petitpetitchiffre.Print(ol::ToString(curve_ctrl_pt[curve_selected][n][0]),xpatch_window+455+20+curve_ctrl_pt[curve_selected][n][0],ypatch_window+50+270);
   }
   else   if (dmx_view==0)//%
   {
   //affichage OUT
   petitpetitchiffrerouge.Print(ol::ToString((int)((255-curve_ctrl_pt[curve_selected][n][1])/2.55)),xpatch_window+5+455,ypatch_window+50+curve_ctrl_pt[curve_selected][n][1]);
   //affichage IN
   petitpetitchiffre.Print(ol::ToString((int)((curve_ctrl_pt[curve_selected][n][0])/2.55)),xpatch_window+455+20+curve_ctrl_pt[curve_selected][n][0],ypatch_window+50+270);
   }
   
   
 return(0);  
}



/* draws the spline paths */
int curve_draw_splines()
{
    int io;
     curve_nodes[0] = dummy_curve_node(curve_nodes[curve_node_count+1], curve_nodes[curve_node_count]);//curve_node et previous
   for (io=1; io<curve_node_count-1; io++)
   {  
      curve_nodes[io].x=(curve_ctrl_pt[curve_selected][io][0]+xpatch_window+30+455);
      curve_nodes[io].y=(curve_ctrl_pt[curve_selected][io][1]+ypatch_window+50);
      draw_curve_node(io);
   }

return(0);
}


int view_curve_after_draw()//verif du report de ma courbe
{
for (int d=0; d<255;d++)
{
 Point(xpatch_window+30+455+d,ypatch_window+50+curve_report[curve_selected][d] ).Draw(Rgba::GREEN);  
}
   return(0); 
}


int SplineCurve()
{

curve_node_count=6;
curve_curviness = ftofix(curve_spline_level);
curve_calc_tangents();
//allegro_gl_set_allegro_mode(); //melange gl et allegro screen
curve_draw_splines();//l attribution des pas est fait dans drawsplines
//allegro_gl_unset_allegro_mode(); //melange gl et allegro screen
view_curve_after_draw();
return(0);
}


int build_square_curve(int curve)
{
 //points
 curve_ctrl_pt[curve][1][0]=0; curve_ctrl_pt[curve][1][1]=255;//point 1 en 0 0 

  curve_ctrl_pt[curve][2][0]=27; curve_ctrl_pt[curve][2][1]=255-89;//point 1 en 0 0 
  curve_ctrl_pt[curve][3][0]=55; curve_ctrl_pt[curve][3][1]=255-153;//point 1 en 0 0 
  curve_ctrl_pt[curve][4][0]=118; curve_ctrl_pt[curve][4][1]=255-213;//point 1 en 0 0 
 
 curve_ctrl_pt[curve][5][0]=255; curve_ctrl_pt[curve][5][1]=0;//point 5 en 255 255 

 the_curve_spline_level[curve]=176;
 index_curve_spline_level=176;
 curve_spline_level=(((float)index_curve_spline_level)/127)-1;
 curve_node_count=6;
 curve_curviness = ftofix(curve_spline_level);
 curve_calc_tangents();
 curve_draw_splines();
//write_curve(); //fait planter si debordement de memoire
 view_curve_after_draw();
 return(0);
}


int build_fluo_curve(int curve)
{
 //points
 curve_ctrl_pt[curve][1][0]=0; curve_ctrl_pt[curve][1][1]=255;//point 1 en 0 0 

  curve_ctrl_pt[curve][2][0]=12; curve_ctrl_pt[curve][2][1]=255-45;//point 1 en 0 0 
  curve_ctrl_pt[curve][3][0]=45; curve_ctrl_pt[curve][3][1]=255-103;//point 1 en 0 0 
  curve_ctrl_pt[curve][4][0]=157; curve_ctrl_pt[curve][4][1]=255-178;//point 1 en 0 0 
 
 curve_ctrl_pt[curve][5][0]=255; curve_ctrl_pt[curve][5][1]=0;//point 5 en 255 255 

 the_curve_spline_level[curve]=176;
 index_curve_spline_level=176;
 curve_spline_level=(((float)index_curve_spline_level)/127)-1;
 curve_node_count=6;
 curve_curviness = ftofix(curve_spline_level);
 curve_calc_tangents();
 curve_draw_splines();
//write_curve(); //fait planter si debordement de memoire
 view_curve_after_draw();
 return(0);
}

int build_preheat_curve(int curve)
{
 //points
 curve_ctrl_pt[curve][1][0]=0; curve_ctrl_pt[curve][1][1]=255-28;//point 1 en 0 0 

  curve_ctrl_pt[curve][2][0]=40; curve_ctrl_pt[curve][2][1]=255-86;//point 1 en 0 0 
  curve_ctrl_pt[curve][3][0]=102; curve_ctrl_pt[curve][3][1]=255-163;//point 1 en 0 0 
  curve_ctrl_pt[curve][4][0]=187; curve_ctrl_pt[curve][4][1]=255-235;//point 1 en 0 0 
 
 curve_ctrl_pt[curve][5][0]=255; curve_ctrl_pt[curve][5][1]=0;//point 5 en 255 255 

 the_curve_spline_level[curve]=176;
 index_curve_spline_level=178;
 curve_spline_level=(((float)index_curve_spline_level)/127)-1;
 curve_node_count=6;
 curve_curviness = ftofix(curve_spline_level);
 curve_calc_tangents();
 curve_draw_splines();
//write_curve(); //fait planter si debordement de memoire
 view_curve_after_draw();
 return(0);
}


int build_inverse_curve(int curve)
{
 //points
 for (int pt=1;pt<MAX_curve_nodeS-1;pt++)
 {
  curve_ctrl_pt[curve][pt][0]=(int)((((float)255)/6) *pt);
  curve_ctrl_pt[curve][pt][1]=(int)((((float)255)/6) *pt); 
 }   
 curve_ctrl_pt[curve][1][0]=0; curve_ctrl_pt[curve][1][1]=0;//point 1 en 0 0 
 curve_ctrl_pt[curve][5][0]=255; curve_ctrl_pt[curve][5][1]=255;//point 5 en 255 255 
 the_curve_spline_level[curve]=168;
 index_curve_spline_level=168;
 curve_spline_level=(((float)index_curve_spline_level)/127)-1;
 curve_node_count=6;
curve_curviness = ftofix(curve_spline_level);
curve_calc_tangents();
curve_draw_splines();
//write_curve(); //fait planter si debordement de memoire
view_curve_after_draw();
 return(0);
}
