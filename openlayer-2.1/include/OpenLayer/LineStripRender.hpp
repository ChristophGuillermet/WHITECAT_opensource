#ifndef OL_LINE_STRIP_RENDER_HPP
#define OL_LINE_STRIP_RENDER_HPP

#include "Rgba.hpp"
#include "Bitmap.hpp"
#include "RawLineStrip.hpp"
#include "Declspec.hpp"

#define OL_NEAR_ZERO 0.001


namespace ol {



template< class std_container_v, class std_container_l >
OL_LIB_DECLSPEC void RawLineStrip< std_container_v, std_container_l >::
LineStripRender( const Rgba *color1, const Rgba *color2, const Bitmap *texture, float lineWidth,
         const Placement &placement, bool connectFirstAndLast ) const {
   
   if( vertices.size() < 2 ) {
      return;
   }
   
#ifdef OL_NO_STATE_CHANGE
   GLboolean texturesEnabled;
   glGetBooleanv( GL_TEXTURE_2D, &texturesEnabled );
#endif
   if( texture ) {
      texture->GetReadyToRender( 1.0 );
   }
   else {
      glDisable( GL_TEXTURE_2D );
   }
   
   // The sum of lengths thus far //
   float sumLengths = 0.0;
   float texturePos = 0.0;
   
   typename std_container_l::const_iterator lengthIter = lengths.begin();
   
   // Find the normal of the beginning of the strip //
   
   Vec2D lastS, last;
   
   typename std_container_v::const_iterator vertexIter = vertices.begin();
   
   if( connectFirstAndLast ) {
      last = vertices.back();
      lastS = vertices.front() - last;
   }
   else {
      last = vertices.front();
      typename std_container_v::const_iterator lastSIter = vertices.begin();
      lastSIter++;
      
      if( *lastSIter == last ) {
   		unsigned int index = 2;
   		while( *lastSIter == last ) {
   			if( vertices.size() <= index )
   				return;
   			lastSIter++;
   			index++;
   			lengthIter++;
   		}
      }
      lastS = *lastSIter - last;
      
      vertexIter++;
   }
   
   lastS /= lastS.GetMagnitude();
   
   glPushMatrix();
   placement.Apply();
   
   // Render the begin of the strip //
   
   glBegin( GL_QUAD_STRIP );
      if( color1 ) {
         color1->Select();
      }
      
      float lastUpperX = last.x + lastS.y * lineWidth;
      float lastUpperY = last.y - lastS.x * lineWidth;
      
      float lastLowerX = last.x - lastS.y * lineWidth;
      float lastLowerY = last.y + lastS.x * lineWidth;
      
      if( !texture ) {
         glVertex2f( lastUpperX, lastUpperY );
         glVertex2f( lastLowerX, lastLowerY );
      }
      else {
         const OlTextureInfo &textureInfo = texture->textureInfo;
         glTexCoord2f( textureInfo.rect.x, textureInfo.rect.y );
         glVertex2f( lastUpperX, lastUpperY );
         glTexCoord2f( textureInfo.rect.x, textureInfo.rect.y + textureInfo.rect.h );
         glVertex2f( lastLowerX, lastLowerY );
      }
      
      
      bool running = true;
      
      while( running ) {
         float x = vertexIter->x;
         float y = vertexIter->y;
         
         vertexIter++;
         
         if( vertexIter == vertices.end() ) {
            if( connectFirstAndLast ) {
               running = false;
               vertexIter = vertices.begin();
            }
            else {
               break;
            }
         }
         
         if( color1 ) {
            float factor = sumLengths / totalLength;
            sumLengths += *lengthIter;
            
            color1->InterpolateWith( *color2, factor ).Select();
         }
         
         if( fabs( x - last.x ) < OL_NEAR_ZERO && fabs( y - last.y ) < OL_NEAR_ZERO ) {
            continue;
         }
         
         // Find the direction vector from the next point to the current one //
         
         float bx = x - vertexIter->x;
         float by = y - vertexIter->y;
         
         float bLength = sqrt( bx * bx + by * by );
         
         if( bLength > -OL_NEAR_ZERO && bLength < OL_NEAR_ZERO ) {
            continue;
         }
         
         bx /= bLength;
         by /= bLength;
         
         // Find the direction vector of the displacement //
         
         float cx = lastS.x + bx;
         float cy = lastS.y + by;
         
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
            
            float diff1 = lastS.x - bx;
            float diff2 = lastS.y - by;
            float diff3 = cx - bx;
            float diff4 = cy - by;
            
            if(( diff1 * diff4 ) - ( diff2 * diff3 ) > 0 ) {
               cx = -cx;
               cy = -cy;
            }
            
            // Find the displacement multiplicator //
            
            float s = lastS.y * cx + (-lastS.x) * cy;
            
            if( fabs( s ) < OL_NEAR_ZERO ) {
               nx = -by;
               ny = bx;
               
               float nRatio = lineWidth/sqrt( nx * nx + ny * ny );
               
               nx *= nRatio;
               ny *= nRatio;
            }
            else {
               nx = cx * lineWidth / s;
               ny = cy * lineWidth / s;
            }
         }
         
         // Find the displaced coordinates //
         
         float upperX = x + nx;
         float upperY = y + ny;
         
         float lowerX = x - nx;
         float lowerY = y - ny;
         
         if( !texture ) {
            glVertex2f( upperX, upperY );
            glVertex2f( lowerX, lowerY );
         }
         else {
            const OlTextureInfo &textureInfo = texture->textureInfo;
            
            texturePos += *lengthIter / textureInfo.imgWidth;
            
            if( texturePos > textureInfo.rect.w ) {
               texturePos = 0.0;
            }
            
            glTexCoord2f( textureInfo.rect.x + texturePos, textureInfo.rect.y );
            glVertex2f( upperX, upperY );
            glTexCoord2f( textureInfo.rect.x + texturePos, textureInfo.rect.y + textureInfo.rect.h );
            glVertex2f( lowerX, lowerY );
            
            typename std_container_v::const_iterator nextIter = vertexIter;
            nextIter++;
            
            bool renderingAllowed = nextIter == vertices.end();
            
            if( !renderingAllowed && connectFirstAndLast ) {
               renderingAllowed = true;
               nextIter = vertices.begin();
            }
            
            if( nextIter != vertices.end() ) {
               glTexCoord2f( textureInfo.rect.x + texturePos, textureInfo.rect.y );
               glVertex2f( upperX, upperY );
               glTexCoord2f( textureInfo.rect.x + texturePos, textureInfo.rect.y + textureInfo.rect.h );
               glVertex2f( lowerX, lowerY );
            }
         }
         
         // Store the information which can be used when calculating the next point //
         
         last.x = x;
         last.y = y;
         
         lastS.x = -bx;
         lastS.y = -by;
         /*
         lastUpperX = upperX;
         lastUpperY = upperY;
         
         lastLowerX = lowerX;
         lastLowerY = lowerY;
         */
         lengthIter++;
      }
      
      if( !connectFirstAndLast ) {
         // Render the end of the strip //
         
         float x = vertices.back().x;
         float y = vertices.back().y;
         
         float upperX = x + lastS.y * lineWidth;
         float upperY = y - lastS.x * lineWidth;
         
         float lowerX = x - lastS.y * lineWidth;
         float lowerY = y + lastS.x * lineWidth;
         
         if( color2 )
            color2->Select();
         
         glVertex2f( upperX, upperY );
         glVertex2f( lowerX, lowerY );
      }   
   glEnd();
   
   glPopMatrix();
   
#ifdef OL_NO_STATE_CHANGE
   if( texturesEnabled ) 
      glEnable( GL_TEXTURE_2D );
   else
      glDisable( GL_TEXTURE_2D );
#endif
}


}



#endif // OL_LINE_STRIP_RENDER_HPP
