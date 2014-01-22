/***
OpenLayer Collision Demo
---
Purpose:    This demo demonstrates polygon collision detection in OpenLayer
Created:		2006/02/16
Author: 		Brady Eidson

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

// A helper function to return a random number between min and max //
float FRand( float min, float max ) {
   return min + float( rand() )/float( RAND_MAX ) * ( max - min );
}

using namespace ol;







class MotionPoly : public Poly
{

public:
	MotionPoly(  )
	{}

	void UpdatePosition( float deltaTime )
	{
		position += velocity * deltaTime;
	}

	void Draw()
	{
		Transforms::SetPosition( position );
		Poly::Draw( Rgba::WHITE );
	}

	Placement GetPlacement()
	{
		return Placement( position );
	}

	float    mass;
	Vec2D		velocity;
	Vec2D		position;

};

class BoundingRectangle : public Poly
{

public:
	BoundingRectangle( const Vec2D& topleft, const Vec2D& _size )
		: tl( topleft ), br( topleft + _size )
	{
		Add( tl );
		Add( Vec2D( br.x, tl.y ));
		Add( br );
		Add( Vec2D( tl.x, br.y ));
	}

	bool Contains( MotionPoly& p )
	{
		unsigned int i;
		const std::vector< Vec2D > v = p.GetVertices();
		Vec2D test_tl = tl - p.position;
		Vec2D test_br = br - p.position;


		for( i = 0; i < v.size(); i++ )
		{
			if( v[i].x > test_br.x || v[i].x < test_tl.x ||
			    v[i].y > test_br.y || v[i].y < test_tl.y )
				return false;
		}
		return true;
	}


private:

	Vec2D tl;
	Vec2D br;
};


int main() {
   // SETUP //

   // Set up the program with all possible drivers //
   Setup::SetupProgram();
	printf( "Setup\n" );

   // Set up the screen in windowed mode with the window size of 800 x 600 //
   Setup::SetupScreen( 800, 600, WINDOWED );
	printf( "SetupScreen\n" );


	MotionPoly player;
	player.Add( Vec2D( 0, 0 ));
	player.Add( Vec2D( 30, 0 ));
	player.Add( Vec2D( 30, 30 ));
	player.Add( Vec2D( 0, 30 ));

	player.position.x = 20.0;
	player.position.y = 20.0;
	player.velocity.x = 0.0;
	player.velocity.y = 0.0;
	player.mass = 25.0;

	MotionPoly wall;
	wall.Add( Vec2D( 0.0, 0.0 ));
	wall.Add( Vec2D( SCREEN_W, 0.0 ));
	wall.Add( Vec2D( SCREEN_W, SCREEN_H ));
	wall.Add( Vec2D( 0.0, SCREEN_H ));

	MotionPoly enemy;
	enemy.Add( Vec2D( 15, 0 ));
	enemy.Add( Vec2D( 30, 15 ));
	enemy.Add( Vec2D( 15, 30 ));
	enemy.Add( Vec2D( 0, 15 ));

	enemy.mass = 10.0;
	enemy.position = Vec2D( 700,0 );

	BoundingRectangle goal( Vec2D( 300,200 ), Vec2D( 80.0, 80.0 ));

   // Set up the delta time routines with the default fps of 100.0 //
	const float FRAMERATE = 100.0;
   FpsCounter::Start( FRAMERATE );

	bool collisionzoom = false;
	Collision collision;


	const float THRUST = 5.0;
	const float GRAVITY = .098;
	bool bExit = false;

   // Run the game loop until esc is pressed //
   while( !key[KEY_ESC] && !bExit ) {
      // LOGIC //

		while( keypressed())
		{
			if( readkey() >> 8 == KEY_SPACE )
			{
				FpsCounter::Pause();
				bool pause = true;
				while( pause )
				{
					while( keypressed())
					{
						if( readkey() >> 8 == KEY_SPACE )
						  pause = false;
					}
					rest(1);
				}
				FpsCounter::Resume();
			}
		}

		//printf( "Starting a frame\n" );


      // Inform the fps counter that a new frame has started //
      FpsCounter::NewFrameStarted();

      // Get the delta time //
      float deltaTime = FpsCounter::GetDeltaTime();
		//printf( "Currentfps - %f\n", FpsCounter::GetFps());
      // Move the bitmap with the arrow keys //
		Vec2D force;

      if( key[KEY_LEFT] ) force.x -= 1;
      if( key[KEY_RIGHT] ) force.x += 1;
      if( key[KEY_UP] ) force.y -= 1;
      if( key[KEY_DOWN] ) force.y += 1;

      if( force.x && force.y )
			force *= .7071;

		force *= THRUST;

		Vec2D accel = force / player.mass;
		accel.y += GRAVITY;

		//printf( "Force is %f,%f\n", force.x, force.y );
		//printf( "Accel is %f,%f\n", accel.x, accel.y );
		//printf( "DT is %f\n", deltaTime );

		//printf( "PreVelocity is %+07.3f,%+07.3f\n", player.velocity.x, player.velocity.y );
		player.velocity += accel*deltaTime;
		//printf( "Velocity is    %+07.3f,%+07.3f\n", player.velocity.x, player.velocity.y );
		player.UpdatePosition( deltaTime );



		//accel.x = FRand( -.1, .1 );
		//accel.y = FRand( -.1, .1 );
		accel.x = 0.0;
		accel.y = GRAVITY;
		enemy.velocity += accel * deltaTime;
		enemy.UpdatePosition( deltaTime );


		collision = player.GetCollision( wall, player.GetPlacement(), wall.GetPlacement());
		if( collision.IsCollision())
		{
			Vec2D surfaceNormal = collision.GetSegment( OBJ_B ).GetNormal();
			float speedAgainstSurface = -player.velocity * surfaceNormal;
			player.velocity += 2 * speedAgainstSurface * surfaceNormal;
		}


		collision = enemy.GetCollision( wall, enemy.GetPlacement(), wall.GetPlacement());
		if( collision.IsCollision())
		{
			Vec2D surfaceNormal = collision.GetSegment( OBJ_B ).GetNormal();
			float speedAgainstSurface = -enemy.velocity * surfaceNormal;
			enemy.velocity += 2 * speedAgainstSurface * surfaceNormal;
		}


		collision = player.GetCollision( enemy, player.GetPlacement(), enemy.GetPlacement());
		if( collision.IsCollision())
		{
			Vec2D pp = player.velocity * player.mass;
			Vec2D ep = enemy.velocity * enemy.mass;
			Vec2D vcm = (pp + ep) / (player.mass + enemy.mass );

			Vec2D pvf = -( player.velocity - vcm ) + vcm;
			Vec2D evf = -( enemy.velocity - vcm ) + vcm;

			player.velocity = pvf;
			enemy.velocity = evf;

		}

		if( goal.Contains( player ))
		{
			allegro_message( "You went into the firey goal of death!  You lose!" );
			bExit = true;
		}
		if( goal.Contains( enemy ))
		{
			allegro_message( "You got the enemy into the firey goal of death!  Congrats, you win!" );
			bExit = true;
		}


      // RENDERING //

      // Clear the screen to white //
      Canvas::Fill( Rgba::BLACK );

      // Draw the bitmap to the screen with the top-left coordinates //
		player.Draw();
		enemy.Draw();
		Transforms::SetPosition( 0,0 );
		goal.DrawOutline( Rgba::RED );

      // Refresh the screen contents to show this frame //
      Canvas::Refresh();
      rest(0);
   }
   return 0;
}
END_OF_MAIN()


