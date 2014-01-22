#include "GfxRend.hpp"
#include "Transforms.hpp"
#include "Blenders.hpp"
#include "Internal.hpp"
#include "Canvas.hpp"
#include <math.h>
#include <vector>

using namespace ol;
using namespace std;


// CONSTANTS //

static const double MAX_CIRCLE_ANGLE_INCREMENT = 0.35 * AL_PI;
static const float OL_NEAR_ZERO = 0.000001;


// GENERAL FUNCTIONS //

// * GENERAL ROUTINES //

void GfxRend::
RefreshScreen() {
   Canvas::SetTo( SCREEN_BACKBUF );
   Canvas::Refresh();
}



void GfxRend::
FillScreen( Rgba col ) {
   Canvas::SetTo( SCREEN_BACKBUF );
   Canvas::Fill( col );
}



float GfxRend::
GetAngleIncrement( float rad, float accuracy ) {
   return min(( 2.0 * asin( 1.0/rad )/accuracy ), MAX_CIRCLE_ANGLE_INCREMENT );
}


// * CIRCLE ROUTINES //

void GfxRend::
Circle( float x, float y, float rad, Rgba col, float accuracy ) {
   col.Select();
   glDisable( GL_TEXTURE_2D );
   
   float da = GetAngleIncrement( rad, accuracy );
   
   glBegin( GL_TRIANGLE_FAN );
      glVertex2f( x, y );
       
      for( float a = 0.0; a <= 2 * AL_PI; a += da ) {
         glVertex2f( x + cos(a) * rad, y + sin(a) * rad );
      }
      
      glVertex2f( x + rad, y );
   glEnd();
   
   if( Settings::TextureMappingUsed() )
      glEnable( GL_TEXTURE_2D );
}



void GfxRend::
CircleOutline( float x, float y, float rad, Rgba col, float lineWidth, float accuracy ) {
   col.Select();
   glDisable( GL_TEXTURE_2D );
   
   if( lineWidth > 1 + OL_NEAR_ZERO ) {
      Disk( x, y, rad - lineWidth, rad + lineWidth, col, accuracy );
      return;
   }
   
   glLineWidth( lineWidth );
   
   float da = GetAngleIncrement( rad, accuracy );
   
   glBegin( GL_LINE_LOOP );
      for( float a = 0.0; a < 2 * AL_PI; a += da ) {
         glVertex2f( x + cos(a) * rad, y + sin(a) * rad );
      }
   glEnd();
   
   if( Settings::TextureMappingUsed() )
      glEnable( GL_TEXTURE_2D );
}



void GfxRend::
CircleGradient( float x, float y, float rad, Rgba innerCol, Rgba outerCol, float accuracy ) {
   glDisable( GL_TEXTURE_2D );
   
   float da = GetAngleIncrement( rad, accuracy );
   
   glBegin( GL_TRIANGLE_FAN );
      innerCol.Select();
      glVertex2f( x, y );
      
      outerCol.Select();
      
      for( float a = 0.0; a < 2 * AL_PI; a += da ) {
         glVertex2f( x + cos(a) * rad, y + sin(a) * rad );
      }
      
      glVertex2f( x + rad, y );
   glEnd();
   
   if( Settings::TextureMappingUsed() )
      glEnable( GL_TEXTURE_2D );
}



void GfxRend::
DiskRender( float x, float y, float innerRad, float outerRad, Rgba innerCol, Rgba outerCol, float accuracy, bool setCols ) {
   float da = GetAngleIncrement( outerRad, accuracy );
   
   glBegin( GL_QUADS );
      float prevIX = x + innerRad;
      float prevIY = y;
      
      float prevOX = x + outerRad;
      float prevOY = y;
      
      for( float a = 0.0; a <= 2 * AL_PI; a += da ) {
         float cosa = cos(a);
         float sina = sin(a);
         
         float newIX = x + cosa * innerRad;
         float newIY = y + sina * innerRad;
         
         float newOX = x + cosa * outerRad;
         float newOY = y + sina * outerRad;
         
         if( setCols )
            innerCol.Select();
         
         glVertex2f( newIX, newIY );
         glVertex2f( prevIX, prevIY );
         
         if( setCols )
            outerCol.Select();
         
         glVertex2f( prevOX, prevOY );
         glVertex2f( newOX, newOY );
         
         prevIX = newIX;
         prevIY = newIY;
         
         prevOX = newOX;
         prevOY = newOY;
      }
      
      if( setCols )
         innerCol.Select();
         
      glVertex2f( x + innerRad, y );
      glVertex2f( prevIX, prevIY );
      
      if( setCols )
         outerCol.Select();
      
      glVertex2f( prevOX, prevOY );
      glVertex2f( x + outerRad, y );
   glEnd();
}



void GfxRend::
Disk( float x, float y, float innerRad, float outerRad, Rgba col, float accuracy ) {
   col.Select();
   glDisable( GL_TEXTURE_2D );
   
   DiskRender( x, y, innerRad, outerRad, col, col, accuracy, false );
   
   if( Settings::TextureMappingUsed() )
      glEnable( GL_TEXTURE_2D );
}



void GfxRend::
DiskGradient( float x, float y, float innerRad, float outerRad, Rgba innerCol, Rgba outerCol, float accuracy ) {
   glDisable( GL_TEXTURE_2D );
   
   DiskRender( x, y, innerRad, outerRad, innerCol, outerCol, accuracy, true );
   
   if( Settings::TextureMappingUsed() )
      glEnable( GL_TEXTURE_2D );
}



// * ELLIPSE ROUTINES //

void GfxRend::
Ellipse( float x, float y, float xRad, float yRad, Rgba col, float angle, float accuracy ) {
   EllipseSlice( x, y, xRad, yRad, col, 0.0, -2.0 * AL_PI, angle, accuracy );
}



void GfxRend::
EllipseSlice( float x, float y, float xRad, float yRad, Rgba col, float startAngle, float sweepAngle, float angle, float accuracy ) {
   col.Select();
   glDisable( GL_TEXTURE_2D );
   
   glPushMatrix();
   glTranslatef( x, y, 0.0 );
   RotateMatrix( angle );
   
   float da = 1.0;
   
   if( xRad < yRad )
      da = GetAngleIncrement( xRad, accuracy );
   else
      da = GetAngleIncrement( yRad, accuracy );
   
   float endAngle = startAngle - sweepAngle; 
   
   if( endAngle < startAngle ){
     float temp = startAngle;
     startAngle = endAngle;
     endAngle = temp;
   }
   
   glBegin( GL_TRIANGLE_FAN );
      glVertex2f( 0.0, 0.0 );
       
      for( float a = startAngle; a < endAngle; a += da ) {
         glVertex2f( cos(a) * xRad, sin(a) * yRad );
      }
      
      glVertex2f( cos( endAngle ) * xRad, sin( endAngle ) * yRad );
   glEnd();
   
   glPopMatrix();
   
   if( Settings::TextureMappingUsed() )
      glEnable( GL_TEXTURE_2D );
}



void GfxRend::
EllipseOutline( float x, float y, float xRad, float yRad,
                Rgba col, float angle, float lineWidth, float accuracy ) {
   col.Select();
   glDisable( GL_TEXTURE_2D );
   
   glPushMatrix();
   glTranslatef( x, y, 0.0 );
   RotateMatrix( angle );
   
   float da = 1.0;
   
   if( xRad < yRad )        
      da = GetAngleIncrement( xRad, accuracy );
   else
      da = GetAngleIncrement( yRad, accuracy );
   
   
   float innerXRad = xRad - lineWidth;
   float outerXRad = xRad + lineWidth;
   
   glBegin( GL_QUADS );
      float prevIX = innerXRad;
      float prevIY = 0.0;
      
      float prevOX = outerXRad;
      float prevOY = 0.0;
      
      for( float a = 0.0; a <= 2 * AL_PI; a += da ) {
         float cosa = cos(a);
         float sina = sin(a);
         
         float newIX = cosa * innerXRad;
         float newIY = sina * (yRad - lineWidth);
         
         float newOX = cosa * outerXRad;
         float newOY = sina * (yRad + lineWidth);
         
         glVertex2f( newIX, newIY );
         glVertex2f( prevIX, prevIY );
         
         glVertex2f( prevOX, prevOY );
         glVertex2f( newOX, newOY );
         
         prevIX = newIX;
         prevIY = newIY;
         
         prevOX = newOX;
         prevOY = newOY;
      }
         
      glVertex2f( innerXRad, 0.0 );
      glVertex2f( prevIX, prevIY );
      
      glVertex2f( prevOX, prevOY );
      glVertex2f( outerXRad, 0.0 );
   glEnd();
   
   glPopMatrix();
   
   if( Settings::TextureMappingUsed() )
      glEnable( GL_TEXTURE_2D );
}



void GfxRend::
EllipseGradient( float x, float y, float xRad, float yRad,
                 Rgba innerCol, Rgba outerCol, float angle, float accuracy ) {
   
   EllipseSliceGradient( x, y, xRad, yRad, innerCol, outerCol, 0.0, -2.0 * AL_PI, angle, accuracy );
}



void GfxRend::
EllipseSliceGradient( float x, float y, float xRad, float yRad,
                      Rgba innerCol, Rgba outerCol, float startAngle, float sweepAngle, float angle, float accuracy ) {
   glDisable( GL_TEXTURE_2D );
   
   glPushMatrix();
   glTranslatef( x, y, 0.0 );
   RotateMatrix( angle );
   
   float da = 1.0;
   
   if( xRad < yRad )
      da = GetAngleIncrement( xRad, accuracy );
   else
      da = GetAngleIncrement( yRad, accuracy );
   
   float endAngle = startAngle - sweepAngle; 
   
   if( endAngle < startAngle ){
     float temp = startAngle;
     startAngle = endAngle;
     endAngle = temp;
   }
   
   if( endAngle - startAngle >= da ) {
      
      glBegin( GL_TRIANGLE_FAN );
         innerCol.Select();
         glVertex2f( 0.0, 0.0 );
         
         outerCol.Select();
         
         for( float a = startAngle; a < endAngle; a += da ) {
            glVertex2f( cos(a) * xRad, sin(a) * yRad );
         }
         
         glVertex2f( cos( endAngle ) * xRad, sin( endAngle ) * yRad );
      glEnd();
   }
   
   glPopMatrix();
   
   if( Settings::TextureMappingUsed() )
      glEnable( GL_TEXTURE_2D );
}




void GfxRend::
EllipseSliceGradient( float x, float y, float xRad, float yRad,
                      Rgba innerCol1, Rgba outerCol1, Rgba innerCol2, Rgba outerCol2, float startAngle, float sweepAngle, float angle, float accuracy ) {
   glDisable( GL_TEXTURE_2D );
   
   glPushMatrix();
   glTranslatef( x, y, 0.0 );
   RotateMatrix( angle );
   
   float da = 1.0;
   
   if( xRad < yRad )
      da = GetAngleIncrement( xRad, accuracy );
   else
      da = GetAngleIncrement( yRad, accuracy );
   
   float endAngle = startAngle - sweepAngle; 
   
   if( endAngle < startAngle ){
     float temp = startAngle;
     startAngle = endAngle;
     endAngle = temp;
   }
   
   if( endAngle - startAngle >= da ) {
      glBegin( GL_QUAD_STRIP );
         for( float a = startAngle; a < endAngle; a += da ) {
            float colFact = (a-startAngle)/(endAngle-startAngle);
               
            innerCol1.InterpolateWith( innerCol2, colFact ).Select();
            glVertex2f( 0.0, 0.0 );
            
            outerCol1.InterpolateWith( outerCol2, colFact ).Select();
            glVertex2f( cos(a) * xRad, sin(a) * yRad );
         }
         
         glVertex2f( cos( endAngle ) * xRad, sin( endAngle ) * yRad );
      glEnd();
   }
   
   glPopMatrix();
   
   if( Settings::TextureMappingUsed() )
      glEnable( GL_TEXTURE_2D );
}


// * SLICE ROUTINES //


void GfxRend::
Slice( float x, float y, float rad, float startAngle, float sweepAngle,
       Rgba col, float accuracy ) {
   col.Select();
   glDisable( GL_TEXTURE_2D );
   
   float da = GetAngleIncrement( rad, accuracy );
   float endAngle = startAngle - sweepAngle; 
   
   if( endAngle < startAngle ) {
     float temp = startAngle;
     startAngle = endAngle;
     endAngle = temp;
   }
   
   
   glBegin( GL_TRIANGLE_FAN );
      glVertex2f( x, y );
      
      for( float a = startAngle; a < endAngle; a += da ) {
         glVertex2f( x + cos(a) * rad, y + sin(a) * rad );
      }
      
      glVertex2f( x + cos( endAngle ) * rad, y + sin( endAngle ) * rad );
   glEnd();
   
   if( Settings::TextureMappingUsed() )
      glEnable( GL_TEXTURE_2D );
}



void GfxRend::
SliceGradient( float x, float y, float rad, float startAngle, float sweepAngle,
               Rgba innerCol, Rgba outerCol, float accuracy ) {
   glDisable( GL_TEXTURE_2D );
   
   float da = GetAngleIncrement( rad, accuracy );
   float endAngle = startAngle - sweepAngle; 
   
   if(endAngle < startAngle){
     float temp = startAngle;
     startAngle = endAngle;
     endAngle = temp;
   }
   
   
   glBegin( GL_TRIANGLE_FAN );
      innerCol.Select();
      glVertex2f( x, y );
      
      outerCol.Select();
       
      for( float a = startAngle; a < endAngle; a += da ) {
         glVertex2f( x + cos(a) * rad, y + sin(a) * rad );
      }
      
      glVertex2f( x + cos( endAngle ) * rad, y + sin( endAngle ) * rad );
   glEnd();
   
   if( Settings::TextureMappingUsed() )
      glEnable( GL_TEXTURE_2D );
}



// * ARC ROUTINES //


void GfxRend::
Arc( float x, float y, float innerRad, float outerRad, 
     float startAngle, float sweepAngle, Rgba col, float accuracy ) {
   
   glDisable( GL_TEXTURE_2D );
   
   col.Select();
   
   float da = GetAngleIncrement( outerRad, accuracy );
   float endAngle = startAngle - sweepAngle; 
   
   if(endAngle < startAngle){
     float temp = startAngle;
     startAngle = endAngle;
     endAngle = temp;
   }
   
   
   glBegin( GL_QUADS );
      float prevIX = x + cos(startAngle) * innerRad;
      float prevIY = y + sin(startAngle) * innerRad;
      
      float prevOX = x + cos(startAngle) * outerRad;
      float prevOY = y + sin(startAngle) * outerRad;
      
      for( float a = startAngle; a < endAngle; a += da ) {
         float cosa = cos(a);
         float sina = sin(a);
         
         float newIX = x + cosa * innerRad;
         float newIY = y + sina * innerRad;
         
         float newOX = x + cosa * outerRad;
         float newOY = y + sina * outerRad;
         
         glVertex2f( newIX, newIY );
         glVertex2f( prevIX, prevIY );
         
         glVertex2f( prevOX, prevOY );
         glVertex2f( newOX, newOY );
         
         prevIX = newIX;
         prevIY = newIY;
         
         prevOX = newOX;
         prevOY = newOY;
      }
       
      glVertex2f( x + cos(endAngle) * innerRad, y + sin(endAngle) * innerRad);
      glVertex2f( prevIX, prevIY );
      
      glVertex2f( prevOX, prevOY );
      glVertex2f( x + cos(endAngle) * outerRad, y + sin(endAngle) * outerRad); 
   glEnd();
   
   if( Settings::TextureMappingUsed() )
      glEnable( GL_TEXTURE_2D );
}



void GfxRend::
ArcGradient( float x, float y, float innerRad, float outerRad, float startAngle,
             float sweepAngle, Rgba innerCol, Rgba outerCol, float accuracy ) {

   glDisable( GL_TEXTURE_2D );
                   
   float da = GetAngleIncrement( outerRad, accuracy );
   float endAngle = startAngle - sweepAngle; 
   
   if(endAngle < startAngle){
     float temp = startAngle;
     startAngle = endAngle;
     endAngle = temp;
   }
   
   
   glBegin( GL_QUADS );
      float prevIX = x + cos(startAngle) * innerRad;
      float prevIY = y + sin(startAngle) * innerRad;
      
      float prevOX = x + cos(startAngle) * outerRad;
      float prevOY = y + sin(startAngle) * outerRad;
      
      for( float a = startAngle; a < endAngle; a += da ) {
         float cosa = cos(a);
         float sina = sin(a);
         
         float newIX = x + cosa * innerRad;
         float newIY = y + sina * innerRad;
         
         float newOX = x + cosa * outerRad;
         float newOY = y + sina * outerRad;
         
         innerCol.Select();
         glVertex2f( newIX, newIY );
         glVertex2f( prevIX, prevIY );
         
         outerCol.Select();
         glVertex2f( prevOX, prevOY );
         glVertex2f( newOX, newOY );
         
         prevIX = newIX;
         prevIY = newIY;
         
         prevOX = newOX;
         prevOY = newOY;
      }
      
      innerCol.Select();   
      glVertex2f( x + cos(endAngle) * innerRad, y + sin(endAngle) * innerRad);
      glVertex2f( prevIX, prevIY );
      
      outerCol.Select();
      glVertex2f( prevOX, prevOY );
      glVertex2f( x + cos(endAngle) * outerRad, y + sin(endAngle) * outerRad); 
   glEnd();
   
   if( Settings::TextureMappingUsed() )
      glEnable( GL_TEXTURE_2D );
}



// * POLYGON ROUTINES //


void GfxRend::
Polygon( float *xCoordinates, float *yCoordinates, int numCoordinates, Rgba col ) {
   col.Select();
   glDisable( GL_TEXTURE_2D );
   
   glBegin( GL_POLYGON );
      for( int i = 0; i < numCoordinates; i++ ) {
         glVertex2f(xCoordinates[i], yCoordinates[i]);
      }
   glEnd();
   
   if( Settings::TextureMappingUsed() )
      glEnable( GL_TEXTURE_2D );
}


void GfxRend::
PolygonOutline( float *xCoordinates, float *yCoordinates, int numCoordinates, Rgba col, float lineWidth ) {
   col.Select();
   glDisable( GL_TEXTURE_2D );
   glLineWidth( lineWidth );
   
   glBegin( GL_LINE_LOOP );
      for( int i = 0; i < numCoordinates; i++ ) {
         glVertex2f(xCoordinates[i], yCoordinates[i]);
      }
   glEnd();
   
   if( Settings::TextureMappingUsed() )
      glEnable( GL_TEXTURE_2D );
}



void GfxRend::
PolygonGradient( float *xCoordinates, float *yCoordinates, int numCoordinates, Rgba *colors ) {
   glDisable( GL_TEXTURE_2D );
   
   glBegin( GL_POLYGON );
      for( int i = 0; i < numCoordinates; i++ ) {
         colors[i].Select();
         glVertex2f(xCoordinates[i], yCoordinates[i]);
      }
   glEnd();
   
   if( Settings::TextureMappingUsed() )
      glEnable( GL_TEXTURE_2D );
}


// * RECTANGLE ROUTINES //

void GfxRend::
Rect( float x, float y, float w, float h, Rgba col ) {
   col.Select();
   glDisable( GL_TEXTURE_2D );
   
   glBegin( GL_QUADS );
      
      glVertex2f( x, y+h );
      glVertex2f( x+w, y+h );
      glVertex2f( x+w, y );
      glVertex2f( x, y );
      
   glEnd();
   
   if( Settings::TextureMappingUsed() )
      glEnable( GL_TEXTURE_2D );
}



void GfxRend::
RectOutline( float x, float y, float w, float h, Rgba col, float lineWidth ) {
   col.Select();
   glDisable( GL_TEXTURE_2D );
   glLineWidth( lineWidth );
   
   glBegin( GL_QUADS );
      // Top //
      glVertex2f( x -lineWidth, y -lineWidth );
      glVertex2f( x+w +lineWidth, y -lineWidth );
      glVertex2f( x+w +lineWidth, y );
      glVertex2f( x -lineWidth, y );
      // Bottom //
      glVertex2f( x -lineWidth, y+h );
      glVertex2f( x+w +lineWidth, y+h );
      glVertex2f( x+w +lineWidth, y+h +lineWidth );
      glVertex2f( x -lineWidth, y+h +lineWidth );
      // Left //
      glVertex2f( x -lineWidth, y );
      glVertex2f( x, y );
      glVertex2f( x, y+h );
      glVertex2f( x -lineWidth, y+h );
      // Right //
      glVertex2f( x+w, y );
      glVertex2f( x+w +lineWidth, y );
      glVertex2f( x+w +lineWidth, y+h );
      glVertex2f( x+w, y+h );
   glEnd();
   
   if( Settings::TextureMappingUsed() )
      glEnable( GL_TEXTURE_2D );
}



void GfxRend::
RectGradient( float x, float y, float w, float h, Rgba *colors ) {
   glDisable( GL_TEXTURE_2D );
   
   glBegin( GL_QUADS );
      colors[3].Select();
      glVertex2f( x, y+h );
      colors[2].Select();
      glVertex2f( x+w, y+h );
      colors[1].Select();
      glVertex2f( x+w, y );
      colors[0].Select();
      glVertex2f( x, y );
      
   glEnd();
   
   if( Settings::TextureMappingUsed() )
      glEnable( GL_TEXTURE_2D );
}


// * LINE ROUTINES //

void GfxRend::
Line( float x1, float y1, float x2, float y2, Rgba col, float lineWidth ) {
   col.Select();
   glDisable( GL_TEXTURE_2D );
   glLineWidth( lineWidth );
   
   glBegin( GL_LINES );
      glVertex2f( x1, y1 );
      glVertex2f( x2, y2 );
   glEnd();
   
   if( Settings::TextureMappingUsed() )
      glEnable( GL_TEXTURE_2D );
}



void GfxRend::
LineGradient( float x1, float y1, float x2, float y2, Rgba col1, Rgba col2, float lineWidth ) {
   glDisable( GL_TEXTURE_2D );
   glLineWidth( lineWidth );
   
   glBegin( GL_LINES );
      col1.Select();
      glVertex2f( x1, y1 );
      col2.Select();
      glVertex2f( x2, y2 );
   glEnd();
   
   if( Settings::TextureMappingUsed() )
      glEnable( GL_TEXTURE_2D );
}




void GfxRend::
LineStrip( float *xCoordinates, float *yCoordinates, int numCoordinates, Rgba col,
           float lineWidth ) {
   if( numCoordinates <= 0 ) {
      return;
   }
   
   LineStripRender( xCoordinates, yCoordinates, numCoordinates, &col, lineWidth, true );
}




void GfxRend::
LineStripGradient( float *xCoordinates, float *yCoordinates, int numCoordinates, Rgba *colors,
                   float lineWidth ) {
   if( numCoordinates <= 0 ) {
      return;
   }
   
   LineStripRender( xCoordinates, yCoordinates, numCoordinates, colors, lineWidth, false );
}



Rgba *GfxRend::
GenerateColors( float *xCoordinates, float *yCoordinates, int numCoordinates,
                Rgba col1, Rgba col2 ) {
   
   if( numCoordinates <= 0 ) {
      return 0;
   }
   
   std::vector< float > lengths;
   float totalLength = 0.0;
   
   for( int i = 1; i < numCoordinates; i++ ) {
      float dx = xCoordinates[i] - xCoordinates[i-1];
      float dy = yCoordinates[i] - yCoordinates[i-1];
      
      float length = sqrt( dx * dx + dy * dy );
      totalLength += length;
      
      lengths.push_back( length );
   }
   
   Rgba *colors = new Rgba[numCoordinates];
   
   int index = 0;
   float lengthsSum = 0.0;
   
   for( std::vector< float > ::iterator iter = lengths.begin();; iter++ ) {
      float factor = lengthsSum / totalLength;
      colors[index++] = col1.InterpolateWith( col2, factor );
      
      if( iter == lengths.end())
         break;
      else
         lengthsSum += *iter;
   }
   
   return colors;
}



void GfxRend::
LineStripGradient( float *xCoordinates, float *yCoordinates, int numCoordinates,
                   Rgba col1, Rgba col2, float lineWidth ) {
   if( numCoordinates <= 0 ) {
      return;
   }
   
   Rgba *colors = GenerateColors( xCoordinates, yCoordinates, numCoordinates, col1, col2 );
   
   LineStripRender( xCoordinates, yCoordinates, numCoordinates, colors, lineWidth, false );
   
   delete[] colors;
}



   
void GfxRend::
LineStripGradient( float *xCoordinates, float *yCoordinates, int numCoordinates,
                   Rgba col1, Rgba col2, Rgba leftCol, Rgba rightCol, float lineWidth ) {
   if( numCoordinates <= 0 ) {
      return;
   }
   
   Rgba *colors = GenerateColors( xCoordinates, yCoordinates, numCoordinates, col1, col2 );
   
   LineStripRender( xCoordinates, yCoordinates, numCoordinates, colors, lineWidth, false, true, leftCol, rightCol );
   
   delete[] colors;
}



void GfxRend::
LineStripRender( float *xCoordinates, float *yCoordinates, int numCoordinates, Rgba *cols,
                 float lineWidth, bool singleColor, bool useSideColors,
                 Rgba leftCol, Rgba rightCol ) {
   if( singleColor )
      cols->Select();
   else
      cols[0].Select();
   
   glDisable( GL_TEXTURE_2D );
   
   // Find the normal of the beginning of the strip //
   
   float lastX = xCoordinates[0];
   float lastY = yCoordinates[0];
   
   float lastSx = xCoordinates[1] - lastX;
   float lastSy = yCoordinates[1] - lastY;
   
   float lastSLength = sqrt( lastSx * lastSx + lastSy * lastSy );
   
   lastSx /= lastSLength;
   lastSy /= lastSLength;
   
   float lastUpperX = lastX + lastSy * lineWidth;
   float lastUpperY = lastY - lastSx * lineWidth;
   
   float lastLowerX = lastX - lastSy * lineWidth;
   float lastLowerY = lastY + lastSx * lineWidth;
   
   // Render the begin of the strip //
   
   if( !useSideColors ) {
      glBegin( GL_QUAD_STRIP );
         glVertex2f( lastUpperX, lastUpperY );
         glVertex2f( lastLowerX, lastLowerY );
   }
   
   for( int i = 1; i < numCoordinates-1; i++ ) {
      float x = xCoordinates[i];
      float y = yCoordinates[i];
      
      if( x == lastX && y == lastY ) {
         continue;
      }
      
      // Find the direction vector from the next point to the current one //
      
      float bx = x - xCoordinates[i+1];
      float by = y - yCoordinates[i+1];
      
      float bLength = sqrt( bx * bx + by * by );
      
      if( bLength > -OL_NEAR_ZERO && bLength < OL_NEAR_ZERO ) {
         continue;
      }
      
      bx /= bLength;
      by /= bLength;
      
      // Find the direction vector of the displacement //
      
      float cx = lastSx + bx;
      float cy = lastSy + by;
      
      float cLength = sqrt( cx * cx + cy * cy );
      
      float nx, ny;
      
      if( cLength > -OL_NEAR_ZERO && cLength < OL_NEAR_ZERO ) {
         nx = -by;
         ny = bx;
         
         float nRatio = lineWidth/sqrt( nx * nx + ny * ny );
         
         nx *= nRatio;
         ny *= nRatio;
      }
      else {
         cx /= cLength;
         cy /= cLength;
      
         // Make sure that the displacement happens always in the same side //
         
         float diff1 = lastSx - bx;
         float diff2 = lastSy - by;
         float diff3 = cx - bx;
         float diff4 = cy - by;
         
         if(( diff1 * diff4 ) - ( diff2 * diff3 ) > 0) {
            cx = -cx;
            cy = -cy;
         }
         
         // Find the displacement multiplicator //
         
         float s = lastSy * cx + (-lastSx) * cy;
         
         nx = cx * lineWidth / s;
         ny = cy * lineWidth / s;
      }
      
      // Find the displaced coordinates //
      
      float upperX = x + nx;
      float upperY = y + ny;
      
      float lowerX = x - nx;
      float lowerY = y - ny;
      
      
      if( !useSideColors ) {
         if( !singleColor )
            cols[i].Select();
         
         glVertex2f( upperX, upperY );
         glVertex2f( lowerX, lowerY );
      }
      else {
         glBegin( GL_QUAD_STRIP );
            // Upper side //
            leftCol.Select();
            glVertex2f( lastUpperX, lastUpperY );
            glVertex2f( upperX, upperY );
            
            // Middle //
            if( !singleColor )
               cols[i-1].Select();
            else
               cols->Select();
            
            glVertex2f( lastX, lastY );
            
            if( !singleColor )
               cols[i].Select();
            
            glVertex2f( x, y );
            
            // Lower side //
            rightCol.Select();
            glVertex2f( lastLowerX, lastLowerY );
            glVertex2f( lowerX, lowerY );
         glEnd();
      }
      
      // Store the information which can be used when calculating the next point //
      
      lastX = x;
      lastY = y;
      
      lastSx = -bx;
      lastSy = -by;
      
      lastUpperX = upperX;
      lastUpperY = upperY;
      
      lastLowerX = lowerX;
      lastLowerY = lowerY;
   }
   
   // Render the end of the strip //
   
   int lastIndex = numCoordinates-1;
   
   float x = xCoordinates[lastIndex];
   float y = yCoordinates[lastIndex];
   
   float upperX = x + lastSy * lineWidth;
   float upperY = y - lastSx * lineWidth;
   
   float lowerX = x - lastSy * lineWidth;
   float lowerY = y + lastSx * lineWidth;
   
   if( !useSideColors ) {
         if( !singleColor )
            cols[lastIndex].Select();
         
         glVertex2f( upperX, upperY );
         glVertex2f( lowerX, lowerY );
      
      glEnd();
   }
   else {
      glBegin( GL_QUAD_STRIP );
         // Upper side //
         leftCol.Select();
         glVertex2f( lastUpperX, lastUpperY );
         glVertex2f( upperX, upperY );
         
         // Middle //
         if( !singleColor )
            cols[lastIndex-1].Select();
         else
            cols->Select();
         
         glVertex2f( lastX, lastY );
         
         if( !singleColor )
            cols[lastIndex].Select();
         
         glVertex2f( x, y );
         
         // Lower side //
         rightCol.Select();
         glVertex2f( lastLowerX, lastLowerY );
         glVertex2f( lowerX, lowerY );
      glEnd();
   }
   
   if( Settings::TextureMappingUsed() )
      glEnable( GL_TEXTURE_2D );
}


// * TRIANGLE ROUTINES //

void GfxRend::
Triangle( float x1, float y1, float x2, float y2, float x3, float y3, Rgba col ) {
   col.Select();
   glDisable( GL_TEXTURE_2D );
   
   glBegin( GL_TRIANGLES );
      glVertex2f( x1, y1 );
      glVertex2f( x2, y2 );
      glVertex2f( x3, y3 );
   glEnd();
   
   if( Settings::TextureMappingUsed() )
      glEnable( GL_TEXTURE_2D );
}



void GfxRend::
TriangleOutline( float x1, float y1, float x2, float y2, float x3, float y3, Rgba col, float lineWidth ) {
   col.Select();
   glDisable( GL_TEXTURE_2D );
   glLineWidth( lineWidth );
   
   glBegin( GL_LINE_LOOP );
      glVertex2f( x1, y1 );
      glVertex2f( x2, y2 );
      glVertex2f( x3, y3 );
   glEnd();
   
   if( Settings::TextureMappingUsed() )
      glEnable( GL_TEXTURE_2D );
}



void GfxRend::
TriangleGradient( float x1, float y1, float x2, float y2, float x3, float y3, Rgba *cols ) {
   glDisable( GL_TEXTURE_2D );
   
   glBegin( GL_TRIANGLES );
      cols[0].Select();
      glVertex2f( x1, y1 );
      cols[1].Select();
      glVertex2f( x2, y2 );
      cols[2].Select();
      glVertex2f( x3, y3 );
   glEnd();
   
   if( Settings::TextureMappingUsed() )
      glEnable( GL_TEXTURE_2D );
}


// * POINT ROUTINES //



void GfxRend::
Point( float x, float y, Rgba col ) {
   col.Select();
   glDisable( GL_TEXTURE_2D );
   
   glBegin( GL_POINTS );
      glVertex2f( x + 0.5, y + 0.5 );
   glEnd();
   
   if( Settings::TextureMappingUsed() )
      glEnable( GL_TEXTURE_2D );
}



// * ERROR HANDLING ROUTINES //

const char *GfxRend::
ErrorString() {
   OlLog( "Retrieving error" );
   GLenum error = glGetError();
   OlLog( "Done... Got: " + ToString( error ));
   
   if( error == GL_NO_ERROR )
      return 0;
   
   if( error == GL_INVALID_ENUM )
      return "GL_INVALID_ENUM";
   
   if( error == GL_INVALID_VALUE )
      return "GL_INVALID_VALUE";
   
   if( error == GL_INVALID_OPERATION )
      return "GL_INVALID_OPERATION";
   
   if( error == GL_STACK_OVERFLOW )
      return "GL_STACK_OVERFLOW";
   
   if( error == GL_STACK_UNDERFLOW )
      return "GL_STACK_UNDERFLOW";
   
   if( error == GL_OUT_OF_MEMORY )
      return "GL_OUT_OF_MEMORY";
   
   OlLog( "Unknown OpenGL error code" );
   OlLog( ToString( error ));
   
   return "OTHER";
}
