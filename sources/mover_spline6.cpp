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

 \file mover_spline6.cpp
 \brief {description courte} 
 \author Christoph Guillermet
 \version {numero de version du fichier}
 \date {date description}
 
 White Cat {- catégorie} {- sous catégorie {- sous catégorie}}
 Description détaillée
 
 **/
 
 

/* calculates the distance between two nodes */
fixed node_dist(NODE n1, NODE n2)
{
   #define SCALE  64

   fixed dx = itofix(n1.x - n2.x) / SCALE;
   fixed dy = itofix(n1.y - n2.y) / SCALE;

   return fixsqrt(fixmul(dx, dx) + fixmul(dy, dy)) * SCALE;
}


/* constructs nodes to go at the ends of the list, for tangent calculations */
NODE dummy_node(NODE node, NODE prev)
{
   NODE n;

   n.x = node.x - (prev.x - node.x) / 8;
   n.y = node.y - (prev.y - node.y) / 8;
   n.tangent = itofix(0);

   return n;
}



/* calculates a set of node tangents */
void calc_tangents(void)
{  
     
     //pas touche prepa tangentes OK pour sens en avant
   int i;
   
   nodes[0] = dummy_node(nodes[node_count+1], nodes[node_count]);//node et previous
   nodes[node_count+1] = dummy_node(nodes[node_count], nodes[0]);
   
   node_count++;

  for (i=1; i<node_count-1; i++)
      nodes[i].tangent = fixatan2(itofix(nodes[i+1].y - nodes[i-1].y),
				  itofix(nodes[i+1].x - nodes[i-1].x));
				  
				  
 			  
}




/* calculates the control points for a spline segment */
void get_control_points(NODE n1, NODE n2, int points[8])
{
   fixed dist = fixmul(node_dist(n1, n2), curviness);

   points[0] = n1.x;
   points[1] = n1.y;

   points[2] = n1.x + fixtoi(fixmul(fixcos(n1.tangent), dist));
   points[3] = n1.y + fixtoi(fixmul(fixsin(n1.tangent), dist));

   points[4] = n2.x - fixtoi(fixmul(fixcos(n2.tangent), dist));
   points[5] = n2.y - fixtoi(fixmul(fixsin(n2.tangent), dist));

   points[6] = n2.x;
   points[7] = n2.y;
}


/* calculates the control points for a spline segment */
void get_control_points_backward(NODE n1, NODE n2, int points[8])
{
   fixed dist = fixmul(node_dist(n1, n2), curviness);

   points[0] = n1.x;
   points[1] = n1.y;

   points[2] = n1.x - fixtoi(fixmul(fixcos(n1.tangent), dist));
   points[3] = n1.y - fixtoi(fixmul(fixsin(n1.tangent), dist));

   points[4] = n2.x + fixtoi(fixmul(fixcos(n2.tangent), dist));
   points[5] = n2.y + fixtoi(fixmul(fixsin(n2.tangent), dist));

   points[6] = n2.x;
   points[7] = n2.y;
}


/* draws a spline curve connecting two nodes */
void draw_spline(NODE n1, NODE n2)
{
   int points[8];
   int i;

   get_control_points(n1, n2, points);
   spline(screen, points, makecol(0,200,0));
   if (show_control_points)
      for (i=1; i<=2; i++)
	 circlefill(screen, points[i*2], points[i*2+1], 2, makecol(0,0,0));
}



/* draws the spline paths */
int draw_splines(int move_selected)
{
   int io;
     nodes[0] = dummy_node(nodes[node_count+1], nodes[node_count]);//node et previous
   for (io=1; io<node_count; io++)
   {  
      nodes[io].x=(dock_move_xy[move_selected][io][0]+xmover_window+20);
      nodes[io].y=(dock_move_xy[move_selected][io][1]+ymover_window+20);
   }

   for (io=1; io<node_count-1; io++)
   {
    draw_spline(nodes[io], nodes[io+1]);    
   } 
  
   
  // goback to begin  
  draw_spline(nodes[node_count], nodes[1]);
   
return(0);
}


int ShowSpline(int move_selected)
{

node_count=dock_moves_contains_steps[move_selected];
curviness = ftofix(spline_ratio[move_selected]);
calc_tangents();
if(GotoMoves[move_selected][actual_step_node]==0 || index_move_back==1)
{
draw_splines(move_selected);
}//l attribution des pas est fait dans drawsplines
return(0);
}


#define SPLINE_MAX_RESOLUTION 256

int my_spline_path_X[SPLINE_MAX_RESOLUTION];
int my_spline_path_Y[SPLINE_MAX_RESOLUTION];

int Prepare_Cross_Spline(int move_selected)
{
 int points[8];

 actual_step_node=dock_move_actual_step[move_selected];

 //si tracker pas à l endroit de la memoire
 if(mover_params[0][0]!=(dock_move_xy[move_selected][actual_step_node][0]+xmover_window+20) || mover_params[1][0]!=(dock_move_xy[move_selected][actual_step_node][1]+ymover_window+20)) 
 {    nodes[actual_step_node].x=mover_params[0][0]+xmover_window+20;
      nodes[actual_step_node].y=mover_params[1][0]+ymover_window+20;
 }
 
 
 
 //next step et raffraichissement xy pour cycle mode
  next_step_node=next_step[move_selected];
  if(next_step_node>dock_moves_contains_steps[move_selected]){next_step_node=1;}
  nodes[next_step_node].x=(dock_move_xy[move_selected][next_step_node][0]+xmover_window+20);
  nodes[next_step_node].y=(dock_move_xy[move_selected][next_step_node][1]+ymover_window+20);

 
  //dans le cas d un goto  
 if(GotoMoves[move_selected][actual_step_node]!=0 && index_move_forward==1)
 {
 numero_de_dock_goto_spline=GotoMoves[move_selected][actual_step_node];
 next_step_node=Moves_Inpoint[numero_de_dock_goto_spline];
 nodes[next_step_node].x=(dock_move_xy[numero_de_dock_goto_spline][next_step_node][0]+xmover_window+20);
 nodes[next_step_node].y=(dock_move_xy[numero_de_dock_goto_spline][next_step_node][1]+ymover_window+20);  
 for(int ro=1;ro<dock_moves_contains_steps[numero_de_dock_goto_spline];ro++)
 {
  if(ro!=actual_step_node)
  {
  nodes[ro].x=(dock_move_xy[numero_de_dock_goto_spline][ro][0]+xmover_window+20);
  nodes[ro].y=(dock_move_xy[numero_de_dock_goto_spline][ro][1]+ymover_window+20);       
  }
 }
 }
 
// pas besoin de re calcul des tangentes, il est fait pour l affichage. 			  
 get_control_points(nodes[actual_step_node],nodes[next_step_node],points);

 
 if(index_move_back==1)
 { get_control_points_backward(nodes[actual_step_node],nodes[next_step_node],points);}
 calc_spline(points, SPLINE_MAX_RESOLUTION,  my_spline_path_X, my_spline_path_Y);
 
 actual_spline_tick=0.0;
 spline_tick_fraction=((float)SPLINE_MAX_RESOLUTION)/dock_time[dock_move_selected][(dock_move_actual_step[dock_move_selected])];  
 return(0);   
}

