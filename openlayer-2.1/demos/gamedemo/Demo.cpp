#include <OpenLayer.hpp>
#include <loadpng.h>
#include <string>
#include <vector>
#include <math.h>

using namespace std;
using namespace ol;



// The gravity costant //
const float gravity = 0.03;

// The gravity costant //
const float particleGravity = 0.07;


class Block {
public:
   int imageIndex;
   float opacity;
   bool destroyed;
   bool fading;
};



float FRand( float min, float max ) {
   return min + float(rand())/float(RAND_MAX) * (max-min);
}



const float particleFadeSpeed = 0.01;
const float particleMaxAngleDiff = AL_PI/4.0;

const float particleSpeedMin = 0.1;
const float particleSpeedMax = 5.0;



enum ColorScheme {
   RED_VIOLET,
   GRAY_BLUE
};



class Particle {
public:
   Particle( float x, float y, float speedX, float speedY, Rgba color )
         : x( x ), y( y ), speedX( speedX ), speedY( speedY ), color( color ) {}

   // Construct an usual yellow-orange explosion particle //
   Particle( float x, float y, ColorScheme scheme )
         : x( x ), y( y ) {

      // Get a random angle for that starting speed of the particle //
      float angle = FRand( AL_PI/2.0 - particleMaxAngleDiff, AL_PI/2.0 + particleMaxAngleDiff );

      // Get a random starting speed for the particle //
      float speed = FRand( particleSpeedMin, particleSpeedMax );

      speedX = cos( angle ) * speed;
      speedY = -sin( angle ) * speed;

      switch( scheme ) {
         case RED_VIOLET:
            // Get a random color between violet and red //
            color = Rgba( 1.0, 0.3, 0.7 ).InterpolateWith( Rgba( 1.0, 0.0, 0.0 ), FRand( 0.0, 1.0 ));
           break;
         case GRAY_BLUE:
            // Get a random color between gray and blue //
            color = Rgba( 0.7, 0.7, 0.7 ).InterpolateWith( Rgba( 0.3, 0.3, 1.0 ), FRand( 0.0, 1.0 ));
           break;
      }

      // Get a random opacity for the particle //
      color.a = FRand( 0.5, 1.0 );

      // Get a random radius for the particle
      radius = FRand( 1.0, 3.0 );
   }

   // Returns true if the particle should be destroyed //
   bool Update( float deltaTime ) {
      // Apply the gravity to the speed //
      speedY += deltaTime * particleGravity;

      // Move the particle //
      x += deltaTime * speedX;
      y += deltaTime * speedY;

      color.a -= deltaTime * particleFadeSpeed;

      // If the opacity is zero or less the particle is gone //
      return color.a <= 0.0;
   }

   void Render() {
      // Render the particle dot in the screen with the additive blender //
      Blenders::Set( ADDITIVE_BLENDER );
      Circle( x, y, radius ).Draw( color );

      // Revert back to the alpha blender //
      Blenders::Set( ALPHA_BLENDER );
   }

   Rgba color;
   float x, y;
   float speedX, speedY;
   float radius;
};



void ShowMessage( TextRenderer &textRenderer, string message );


int main() {
   // Choose a random seed from the computer clock
   srand( time( 0 ));

   // Initialize the program //
   // Initialize both mouse and keyboard //
   if( Setup::SetupProgram( true, true ) == false ) {
      allegro_message( "Error during the program startup!" );
      exit( -1 );
   }

   // Open the game in a 800 x 600 window //
   if( Setup::SetupScreen( 800, 600, WINDOWED ) == false ) {
      allegro_message( "Error while trying to set graphics mode!" );
      //exit( -1 );
   }

   // Load the graphics //
   Bitmap background( "Gfx/Background.png" );
   if( !background ) {
      allegro_message( "Couldn't load the background bitmap!" );
      exit( -1 );
   }

   Bitmap paddle( "Gfx/Paddle.png" );
   if( !paddle ) {
      allegro_message( "Couldn't load the paddle bitmap!" );
      exit( -1 );
   }

   Bitmap ball( "Gfx/Ball.png" );
   if( !ball ) {
      allegro_message( "Couldn't load the ball bitmap!" );
      exit( -1 );
   }

   // Load tile graphics, stored as Gfx/Blocks/Block01.png, Gfx/Blocks/Block02.png, etc...
   vector< Bitmap *> blockImages = Bitmap::LoadListOfBitmaps("Gfx/Blocks/Block", "png", 2 );

   if( blockImages.empty() ) {
      allegro_message( "Couldn't load the block bitmaps!" );
      exit( -1 );
   }


   // Block width and height //
   const int BLOCK_W = 40;
   const int BLOCK_H = 20;

   // Load the font with size 17 x 14 and a white color //
   TextRenderer normalTextRenderer( "Fonts/Neuropol.ttf", 17, 12, Rgba::WHITE );

   // Create a bigger sized (40 x 30) copy of the font //
   TextRenderer biggerTextRenderer( normalTextRenderer, 35, 25 );

   // Top-left coordinates of the paddle //
   int paddleXPos = SCREEN_W/2 - paddle.Width()/2;

   int paddleYCentre = SCREEN_H - paddle.Height() -10;
   int paddleYPos = paddleYCentre;

   // The paddle fades away if the ball is destroyed, //
   // thus paddleOpacity is decreased gradually //
   float paddleOpacity = 1.0;
   bool ballDestroyed = false;

   // Maxium paddle movement in y-direction around paddleYCentre //
   const int maxPaddleYChange = 20;

   // Centre coordinates of the ball //
   float ballXPos = paddleXPos, ballYPos = paddleYPos-1;
   // Ball speed //
   float ballXSpeed = 2.0, ballYSpeed = -7.0;

   // The number of extra lifes //
   int extraLifes = 5;

   float maxBallXSpeed = 5.0;

   // Construct a random map //

   const int MAP_W = 20;
   const int MAP_H = 15;

   Block map[MAP_H][MAP_W];

   const int NO_IMAGE = -1;

   // While creating the map we find out the total number of blocks //
   int numTotalBlocks = 0;

   // The number of blocks the player has destroyed yet //
   int numDestroyedBlocks = 0;

   for( int y = 0; y < MAP_H; y++ ) {
      for( int x = 0; x < MAP_W; x++ ) {
         // Should we place a tile here? //
         if( rand()%3 == 0 ) {
            // The tile does exist //
            map[y][x].destroyed = false;

            // Get a random image for the tile //
            map[y][x].imageIndex = rand() % blockImages.size(); // Which one? //

            // One additional block created //
            numTotalBlocks++;
         }
         else {
            // Otherwise the tile does not exist //
            map[y][x].destroyed = true;
            map[y][x].imageIndex = NO_IMAGE;
         }

         // The tile should be fully opaque //
         map[y][x].opacity = 1.0;

         // The tile shouldn't be fading //
         map[y][x].fading = false;
      }
   }


   // The list of active particles //
   vector< Particle *> particles;

   // Original mode has no gravity and a paddle moving in the y-direction //
   bool originalMode = false;

   // Is the player drunk? //
   bool drunkenMode = false;

   float drunkenAngle = 0.0;
   float maxDrunkenAngle = AL_PI/4.0;

   float drunkenAngleSpeed = 0.0;
   float maxDrunkenAngleSpeedChange = 0.003;


   // Let the user choose one of different color schemes //
   // The selection is made by changing the coffecients of the color channels //
   const int numSchemes = 4;
   Rgba colorSchemes[numSchemes] = { Rgba::WHITE, Rgba( 0.0, 1.0, 1.0 ), Rgba::YELLOW, Rgba( 1.0, 0.0, 0.5 ) };
   int selectedScheme = 0;

   // Is the game still running? //
   bool gameRunning = true;

   // The fps we're targetting to run the game at //
   // (The actual fps will be as high as possible with this //
   //  hardware, but the game will run just like if the fps was 100) //
   const float defaultFps = 100.0;

   // FpsCounter to calculates fps and delta time //
   // We should start the counter right before the game loop starts //
   FpsCounter::Start( defaultFps );


   while( gameRunning ) {
      // Tell the fps counter that a new frame has started //
      FpsCounter::NewFrameStarted();


      // PROCESSING PHASE //

      // Check keypresses //

      if( keypressed() ) {
         switch( readkey() >> 8 ) {
            case KEY_ESC:
               // Quit game //
               gameRunning = false;
              break;

            case KEY_D:
               // Toggle the drunken mode //
               drunkenMode = !drunkenMode;

               drunkenAngle = 0.0;
               drunkenAngleSpeed = 0.0;

               if( !drunkenMode ) {
                  // Get rid of the screen rotation and stretching //
                  Transforms::SetRotation( 0.0 );
                  Transforms::SetStretch( 1.0, 1.0 );
               }
              break;
            case KEY_S:
               // Switch to the next color scheme //
               selectedScheme++;

               // Wrap around back to zero if needed //
               if( selectedScheme >= numSchemes ) {
                  selectedScheme = 0;
               }
              break;
         }
      }


      // These values will change in this frame if the level is finished
      // or the ball gets destroyed //
      bool levelFinished = false;
      bool gameOver = false;

      int mouseXMovement, mouseYMovement;

      // Retrieve the mouse movement since the last frame //
      get_mouse_mickeys( &mouseXMovement, &mouseYMovement );

      // Move the paddle //
      paddleXPos += mouseXMovement;

      if( !originalMode )
         paddleYPos += mouseYMovement;

      // Make sure that the paddle doesn't go out of the screen //
      if( paddleXPos < 0 ) {
         paddleXPos = 0;
      }
      else if( paddleXPos + paddle.Width() >= SCREEN_W ) {
         paddleXPos = SCREEN_W - paddle.Width() -1;
      }

      // Make sure the the y-coordinates of the paddle stay in a certain range //
      if( paddleYPos < paddleYCentre - maxPaddleYChange ) {
         paddleYPos = paddleYCentre - maxPaddleYChange;
      }
      else if( paddleYPos > paddleYCentre + maxPaddleYChange ) {
         paddleYPos = paddleYCentre + maxPaddleYChange;
      }


      // Since the last frame, how much time has elapsed (in frames)? //

      float deltaTime = FpsCounter::GetDeltaTime();

      // Store the old ball coordinates //
      float oldBallXPos = ballXPos;
      float oldBallYPos = ballYPos;

      // Apply gravity if we're not in the "original arcanoid mode" //
      if( !originalMode )
         ballYSpeed += deltaTime * gravity;

      // Move the ball
      ballXPos += deltaTime * ballXSpeed;
      ballYPos += deltaTime * ballYSpeed;

      // Make sure that the ball doesn't go out of the screen //
      if( ballXPos < 0.0 ) {
         ballXPos = 0.0;
         ballXSpeed = -ballXSpeed;
      }
      else if( ballXPos >= SCREEN_W ) {
         ballXPos = SCREEN_W-1;
         ballXSpeed = -ballXSpeed;
      }

      if( ballYPos < 0.0 ) {
         ballYPos = 0.0;
         ballYSpeed = -ballYSpeed;
      }

      // Does the ball hit the paddle? //
      if( ballYPos >= paddleYPos && ballYPos < paddleYPos + paddle.Height()
          && ballXPos >= paddleXPos
          && ballXPos <= paddleXPos + paddle.Width() ) {
         ballYPos = paddleYPos - 1;

         // In original mode you don't hit the ball with a paddle //
         if( originalMode ) {
            ballYSpeed = -fabs( ballYSpeed );
         }
         else {
//            ballYSpeed += ((float(mouseYMovement) <? 2.5) <? 0 );
           ballYSpeed += min( mouseYMovement, 0 ); // MAX( MIN( mouseYMovement, 2.5 ), 0 );
            ballYSpeed = -fabs( ballYSpeed ) * 0.6;
         }


         // The speed of the ball along the x-axis should depend on where the ball hit the paddle //
         ballXSpeed = (ballXPos - (paddleXPos + paddle.Width()/2))/(paddle.Width()/2) * maxBallXSpeed;
      } // Has the ball dropped off the bottom of the screen? //
      else if( ballYPos >= SCREEN_H ) {
         ballDestroyed = true;
      }

      // Does the ball hit a block? //

      if( ballYPos > 0.0 && ballYPos < MAP_H * BLOCK_H ) {

         int ballTileX = int( ballXPos )/BLOCK_W;
         int ballTileY = int( ballYPos )/BLOCK_H;

         if( !map[ballTileY][ballTileX].destroyed ) {
            // Destroy the block //
            map[ballTileY][ballTileX].destroyed = true;

            // Fade the block in the screen //
            map[ballTileY][ballTileX].fading = true;

            // One more destroyed block... //
            numDestroyedBlocks++;

            // Have all the blocks been destroyed? //
            if( numDestroyedBlocks >= numTotalBlocks ) {
               levelFinished = true;
            }

            // On which tile the ball was in the previous frame? //
            int oldBallTileX = int( oldBallXPos )/BLOCK_W;
            int oldBallTileY = int( oldBallYPos )/BLOCK_H;

            // Do we have a collision in the x-direction? //
            // (We do, if the ball would not hit the block if it's not moved in x-direction) //
            if( ballTileX != oldBallTileX ) {
               ballXSpeed = -ballXSpeed;
            }

            // The same in the y-direction //
            if( ballTileY != oldBallTileY ) {
               ballYSpeed = -ballYSpeed;
            }
         }
      }


      // Move all particles and destroy those which are faded out //
      for( vector< Particle *> ::iterator iter = particles.begin(); iter != particles.end(); ) {
         bool shouldBeDestroyed = (*iter)->Update( deltaTime );

         if( shouldBeDestroyed ) {
            // Delete the particle and erase it from the list //
            delete *iter;
            iter = particles.erase( iter );
         }
         else {
            iter++;
         }
      }


      // Fade the blocks that need to be faded //
      for( int y = 0; y < MAP_H; y++ ) {
         for( int x = 0; x < MAP_W; x++ ) {
            if( map[y][x].fading ) {
               map[y][x].opacity -= deltaTime * 0.03;

               // Spawn some random explosion particles from the fading block //
               if( map[y][x].opacity > 0.5 && rand()%3 == 0 ) {
                  float particleX = x * BLOCK_W + FRand( 0.0, BLOCK_W );
                  float particleY = y * BLOCK_H + FRand( 0.0, BLOCK_H );

                  particles.push_back( new Particle( particleX, particleY, RED_VIOLET ));
               }

               // Has the tile been completely faded? //
               if( map[y][x].opacity <= 0.0 ) {
                  map[y][x].fading = false;
               }
            }
         }
      }


      if( ballDestroyed ) {
         // Fade the paddle gradualy //
         paddleOpacity -= deltaTime * 0.01;

         // Spit out some explosion particles from the paddle //
         float particleX = FRand( paddleXPos, paddleXPos + paddle.Width() );
         float particleY = FRand( paddleYPos, paddleYPos + paddle.Height() );
         particles.push_back( new Particle( particleX, particleY, GRAY_BLUE ));

         // Shake the screen! //
         Transforms::SetPosition( 0.0, FRand( -2.0, 2.0 ));

         // Has the paddle been completely faded? //
         if( paddleOpacity < 0.0 ) {

            //Restore the old screen position //
            Transforms::SetPosition( 0.0, 0.0 );

            // Decrease the number of lifes left //
            extraLifes--;

            // Out of extra lifes? //
            if( extraLifes < 0 ) {
               // Then it's game over! //
               gameOver = true;
            } // Otherwise, the ball should re-appear on top of the paddle //
            else {
               // New ball should appear! //
               ballDestroyed = false;

               // Select the starting position of the new ball //
               ballYPos = paddleYPos-1;
               ballXPos = paddleXPos + paddle.Width()/2;

               // Select the starting speed of the ball //
               ballXSpeed = -2.0;
               ballYSpeed = -6.0;

               // The paddle should be completely visible again //
               paddleOpacity = 1.0;
            }
         }
      }


      if( drunkenMode ) {
         // Rotate the screen randomly //

         drunkenAngleSpeed += deltaTime * FRand( -maxDrunkenAngleSpeedChange, maxDrunkenAngleSpeedChange );
         drunkenAngle += drunkenAngleSpeed;

         // Make sure that the angle stays in the range //

         if( drunkenAngle > maxDrunkenAngle ) {
            drunkenAngle = maxDrunkenAngle;
            drunkenAngleSpeed = 0.0;
         }
         else if( drunkenAngle < -maxDrunkenAngle ) {
            drunkenAngle = -maxDrunkenAngle;
            drunkenAngleSpeed = 0.0;
         }

         // Rotate and stretch the screen //

         Transforms::SetRotation( drunkenAngle );

         float drunkenMaxStretch = 0.20;

         float drunkenStretch = drunkenAngle/maxDrunkenAngle * drunkenMaxStretch;
         Transforms::SetStretch(( 1.0 - drunkenMaxStretch) + drunkenStretch,
                            ( 1.0 - drunkenMaxStretch) - drunkenStretch );
      }


      // RENDERING PHASE //


      // Select the color scheme by changing the color channels //
      Transforms::SetColorChannels( colorSchemes[selectedScheme] );

      //Transforms::SetPosition( 0.5, 0.5 );

      if( drunkenMode ) {
         // We need to fill the screen with back as we can see the things outside the background //
         Canvas::Fill( Rgba::BLACK );
      }

      // Draw the background //
      background.Blit( 0, 0 );

      // Draw the map //

      for( int y = 0; y < MAP_H; y++ ) {
         for( int x = 0; x < MAP_W; x++ ) {
            // Is the block not destroyed or destroyed but fading? //
            if( !map[y][x].destroyed || map[y][x].fading ) {
               // Get the bitmap index of this block from the map //
               int imageIndex = map[y][x].imageIndex;

               // How opaque should the block be? //
               float blockOpacity = map[y][x].opacity;

               // Render the block image that corresponds to that index to the screen //
               blockImages[imageIndex]->Blit( x * BLOCK_W, y * BLOCK_H, blockOpacity );
            }
         }
      }

      // Construct the tinting color of the paddle //
      Rgba paddleNormalColor( 0.0, 0.3, 0.6, 0.3 );
      Rgba paddleTintColor = paddleNormalColor;

      if( ballDestroyed) {
         // Get the tint factor from the opacity //
         double tintFactor = ((1.0 - paddleOpacity) * 2.0 );

         // We don't want it to exceed 0.5 //
         if( tintFactor > 0.5 )
            tintFactor = 0.5;

         // Construct the final tint color which is mix of paddleNormalColor and white //
         paddleTintColor = paddleNormalColor.InterpolateWith( Rgba::WHITE, tintFactor );
      }

      // Draw the paddle //
      paddle.Blit( paddleXPos, paddleYPos, TintMode( paddleTintColor ), paddleOpacity );

      // Draw the ball //
      ball.Blit( ballXPos - ball.Width()/2, ballYPos - ball.Height()/2, TintMode( Rgba( 1.0, 1.0, 1.0, 0.3 )));

      // Render the particles //
      for( vector< Particle *> ::iterator iter = particles.begin(); iter != particles.end(); iter++ ) {
         (*iter)->Render();
      }

      string text = "Blocks Destroyed: " + ToString( numDestroyedBlocks ) + " / "
                    + ToString( numTotalBlocks ) + "\nExtra balls: " + ToString( max( extraLifes, 0 ))
                    + "\nFPS: " + ToString( int( FpsCounter::GetFps() ))
                    + "\n" + (drunkenMode? "Press D to restore normal mode" : "Press D for Drunken Mode!" )
                    + "\nPress S to change the color scheme";


      // Make sure that the screen isn't rotated, stretched or moved when the texts are rendered //
      Transforms::ResetTransforms();

      // We don't want the color channel modifications to affect text rendering //
      // So we revert all channels back to 1.0 //
      Transforms::SetColorChannels( Rgba::WHITE );

      // Print the shadow of the text to the screen at x = 4, baselineY = 16
      Rgba col = normalTextRenderer.GetColor();
      normalTextRenderer.SetColor( Rgba::BLACK );
      normalTextRenderer.Print( text, 5-1, 15+1 );

      normalTextRenderer.SetColor( col );

      // Print the text to the screen at x = 5, baselineY = 15
      normalTextRenderer.Print( text, 5, 15 );

      // Do we need to display a message in the screen? //

      if( levelFinished ) {
         // Show a message in the screen (the function to do this is there below the main()-function)
         ShowMessage( biggerTextRenderer, "Level Finished!" );
         // Sorry, but only one level in this demo :P //
         gameRunning = false;
      }

      if( gameOver ) {
         // Show a message in the screen (the function to do this is there below the main()-function)
         ShowMessage( biggerTextRenderer, "Game Over!" );
         // Sorry, but you need to restart the game in to play again :P //
         gameRunning = false;
      }

      // Show the current frame in the screen //
	  Canvas::Refresh();
   }


   // Destroy the block images //
   for( vector< Bitmap *> ::iterator iter = blockImages.begin(); iter != blockImages.end(); iter++ ) {
      delete *iter;
   }


   return 0;
}
END_OF_MAIN()



void ShowMessage( TextRenderer &textRenderer, string message ) {
   // Retrieve the width and the height of the text //
   int textWidth = textRenderer.Width( message );
   int textHeight = textRenderer.Height( message );

   // The amount of empty space around the text in the box //
   const int paddingSpace = 20;

   int msgBoxX = SCREEN_W/2 - textWidth/2 - paddingSpace;
   int msgBoxY = SCREEN_H/2 - textHeight/2 - paddingSpace;

   // Tint the screen slightly to black to darken it //
   Transforms::SetTintColor( Rgba( 0.0, 0.0, 0.0, 0.30 ));

   // Draw the box below the text //
   Rect boxRect( msgBoxX, msgBoxY, textWidth + 2 * paddingSpace, textHeight + 2 * paddingSpace );
   boxRect.Draw( Rgba( 0.4, 0.4, 0.4, 0.5 ));
   boxRect.DrawOutline( Rgba( 0.1, 0.1, 0.1, 0.8 ));

   // Draw the text //
   textRenderer.Print( message, msgBoxX + paddingSpace, msgBoxY + paddingSpace + 25 );

   // Show the current frame in the screen //
   Canvas::Refresh();

   // Wait for a keypress //
   clear_keybuf();
   readkey();

}
