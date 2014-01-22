/***
OpenLayer Shape Demo
---
Purpose:     This demo demonstrates usage of the ol::Shape class and its
				 derivitives
Author: 		 Brady Eidson

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
***/


#include <OpenLayer.hpp>
#include <OpenLayer/Circle.hpp>


using namespace ol;


/*
int main()
{
	Setup::SetupProgram();
	Setup::SetupScreen( 800,600,WINDOWED );

	Bitmap bmp( 296, 83, Rgba( 0.0,0.0,0.0,.3));
	bmp.Save( "Gfx/AlphaBitmap.png" );
	return 0;

}
END_OF_MAIN()
*/

void mousePointerLoop( Shape& mouseShape )
{
	while( !keypressed() && !mouse_b )
	{

		Canvas::Fill( Rgba::BLACK );

		Transforms::SetPosition( mouse_x, mouse_y );
		mouseShape.Draw( Rgba::WHITE );
		Transforms::ResetTransforms();

		Canvas::Refresh();

		rest(1);
	}
	while(mouse_b){}
}


bool global_pressed;
bool global_released;

void mouse_function( int flags )
{
	if( flags & MOUSE_FLAG_LEFT_DOWN )
		global_pressed = true;
	else if( flags & MOUSE_FLAG_LEFT_UP )
		global_released = true;

}
END_OF_FUNCTION(mouse_function)

void polygonLoop( Bitmap& pointer )
{

	LineStrip polygon;
	Line line( 0,0,0,0 );

	global_pressed = false;
	global_released = false;

	bool drawing = false;

	while( !keypressed())
	{

		Canvas::Fill( Rgba::BLACK );

		polygon.Draw( Rgba::WHITE );
		if( drawing )
		{
			line.end = Vec2D( mouse_x, mouse_y );
			line.Draw( Rgba::WHITE );
		}
		pointer.Blit( mouse_x, mouse_y );
		Canvas::Refresh();


		if( global_pressed )
		{
			if( !polygon.GetNumOfVertices())
				polygon.AddToEnd( Vec2D( mouse_x, mouse_y ));
			line.start = polygon.GetVertex( polygon.GetNumOfVertices() - 1 );
			global_pressed = false;
			drawing = true;
		}

		if( global_released )
		{
			polygon.AddToEnd( Vec2D( mouse_x, mouse_y ));
			global_released = false;
			drawing = false;
		}






		rest(1);
	}


}


int main() {
   // SETUP //

   // Set up the program with all possible drivers //
   Setup::SetupProgram();

   LOCK_VARIABLE( global_pressed );
   LOCK_VARIABLE( global_released );
   LOCK_FUNCTION( mouse_function );

	mouse_callback = mouse_function;

   // Set up the screen in windowed mode with the window size of 800 x 600 //
   Setup::SetupScreen( 800, 600, WINDOWED );

   // Load a bitmap //
   Bitmap pointer;
   pointer.Load( "Gfx/PointerAlpha.png", "Gfx/PointerAlpha.bmp" );

   // Test if the bitmap was really loaded (maybe it didn't exist) //
   if( !pointer ) {
      allegro_message( "Couldn't load the bitmap!" );
      exit( -1 );
   }


   // RENDERING //

   // Draw the bitmap to the screen with the top-left coordinates of x = 200.0, y = 100.0 //

	Circle cPointer( Vec2D( 10,10 ), 10 );
	mousePointerLoop( cPointer );
	while( keypressed())
		readkey();

	Rect rPointer( 0.0, 0.0, 20.0, 15.0 );
	mousePointerLoop( rPointer );
	while( keypressed())
		readkey();

	ol::Ellipse ePointer( 20.0, 10.0, 20.0, 10.0 );
	mousePointerLoop( ePointer );
	while( keypressed())
		readkey();

	polygonLoop( pointer );

   return 0;
}
END_OF_MAIN()



