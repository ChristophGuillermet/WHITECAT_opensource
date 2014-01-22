#include "TexturedPoly.hpp"
#include "Bitmap.hpp"
#include "Circle.hpp"
#include "Rectangle.hpp"


#ifdef OL_DEBUG_TEXTURED_POLY
   #include <iostream>
#endif // OL_DEBUG_TEXTURED_POLY

using namespace ol;
using namespace std;


static const float OL_NEAR_ZERO = 0.001;


void TexturedPoly::
Draw( float opacity ) {
   if( dirty ) {
      Construct();
   }
   
	glPushMatrix();
   placement.Apply();
   
   texture->GetReadyToRender( opacity );
   
   for( std::vector< HSlice *> ::iterator iter = slices.begin(); iter != slices.end(); iter++ ) {
      (*iter)->Render();
   }
   
   glBindTexture( GL_TEXTURE_2D, 0 );
   glPopMatrix();
}


void TexturedPoly::HSlice::
Render() {
   bool skipping = true;
   std::vector< SlicePart *> ::iterator newStartPoint = parts.end();
   
   //glBegin( GL_QUAD_STRIP ); use skipping
      for( std::vector< SlicePart *> ::iterator iter = parts.begin(); iter != parts.end(); iter++ ) {
         bool zeroHeight = (*iter)->IsHeightZero();
         
         if( zeroHeight ) {
            if( skipping ) {
               newStartPoint = iter;
               continue;
            }
         }
         else if( skipping ) {
            skipping = false;
            glBegin( GL_QUAD_STRIP );
            
            if( newStartPoint != parts.end() ) {
               (*newStartPoint)->RenderStripPart();
               newStartPoint = parts.end();
            }
         }
         
         (*iter)->RenderStripPart();
         
         if( zeroHeight ) {
            skipping = true;
            glEnd();
         }
      }
   
   if( !skipping ) {
      glEnd();
   }
   
#ifdef OL_DEBUG_TEXTURED_POLY
   glDisable( GL_TEXTURE_2D );
   glLineWidth( 2.0 );
   
   glBegin( GL_LINE_STRIP );
      for( std::vector< SlicePart *> ::iterator iter = parts.begin(); iter != parts.end(); iter++ ) {
         glTexCoord2f((*iter)->textureTop, (*iter)->textureX );
         glVertex2f((*iter)->x, (*iter)->top );
         glTexCoord2f((*iter)->textureBottom, (*iter)->textureX );
         glVertex2f((*iter)->x, (*iter)->bottom );
      }
   glEnd();
   
   glEnable( GL_TEXTURE_2D );
#endif // OL_DEBUG_TEXTURED_POLY
}


bool TexturedPoly::
LineCollides( const Line &line, std::vector< Vec2D > ::const_iterator start,
                      std::vector< Vec2D > ::const_iterator end ) {
   const std::vector< Vec2D > &vertices = GetVertices();
   
   std::vector< Vec2D > ::const_iterator iter = start;
   std::vector< Vec2D > ::const_iterator iterPrev = iter;
   iter++;
   
   if( iter == vertices.end() ) {
      iter = vertices.begin();
   }
   
   while( true ) {
      if( line.Collides( Line( *iterPrev, *iter ))) {
         return true;
      }
      
      if( iter == end ) {
         break;
      }
      
      iterPrev = iter;
      iter++;
      if( iter == vertices.end() ) {
         iter = vertices.begin();
      }
      
   }
   
   return false;
}


void TexturedPoly::
Construct() {
   const std::vector< Vec2D > &vertices = GetVertices();
   
   // Remove the old slices
   
   for( std::vector< HSlice *> ::iterator iter = slices.begin(); iter != slices.end(); iter++ ) {
      delete *iter;
   }
   
   slices.clear();
   
   if( vertices.size() < 3 ) {
      return;
   }
   
   if( !texture->IsValid() ) {
      OlError( "Polygon texture isn't loaded correctly!" );
      return;
   }
   
   // Find the area of the polygon
   
   const Vec2D &firstVertex = vertices.front();
   
   float minX = firstVertex.x, minY = firstVertex.y;
   float maxX = firstVertex.x, maxY = firstVertex.y;
   
   std::vector< Vec2D > ::const_iterator iter = vertices.begin();
   iter++;
   
   std::vector< Vec2D > ::const_iterator leftmost = vertices.begin(), rightmost = vertices.begin();
   
   for(; iter != vertices.end(); iter++ ) {
      if( iter->x > maxX ) {
         maxX = iter->x;
         rightmost = iter;
      }
      else if( iter->x < minX ) {
         minX = iter->x;
         leftmost = iter;
      }
      
      if( iter->y > maxY ) {
         maxY = iter->y;
      }
      
      if( iter->y < minY ) {
         minY = iter->y;
      }
   }
   
   #ifdef OL_DEBUG_TEXTURED_POLY
      Circle( *leftmost, 20 ).Draw( Rgba::YELLOW );
      Circle( *rightmost, 20 ).Draw( Rgba( 0.0, 1.0, 1.0 ));
   #endif // OL_DEBUG_TEXTURED_POLY
   
   // Cut the polygon to vertical slices, starting from minX and ending at maxX
   
   float bmpW = texture->Width();
   float bmpH = texture->Height();
   
   minX -= 1.0;
   minY -= 1.0;
   
   for( float y = minY; y <= maxY; y += bmpH ) {
      for( float x = minX; x <= maxX; x += bmpW ) {
         #ifdef OL_DEBUG_TEXTURED_POLY
            cout << endl << "NEW LOOP: " << x << ", " << y << endl << endl;
         #endif // OL_DEBUG_TEXTURED_POLY
         
         std::vector< Vec2D > ::const_iterator topStart = rightmost;
         std::vector< Vec2D > ::const_iterator topEnd = leftmost;
         SideProcessed topSide = ProcessSide( topStart, topEnd, x, y, true, false );
         
         std::vector< Vec2D > ::const_iterator bottomStart = leftmost;
         std::vector< Vec2D > ::const_iterator bottomEnd = rightmost;
         SideProcessed bottomSide = ProcessSide( bottomStart, bottomEnd, x, y, false, true );
         
         if( topSide.isEmpty && bottomSide.isEmpty ) {
            // Let's check if the slice is really inside the polygon
            Line topVLine( Vec2D( x, minY ), Vec2D( x, y ));
            Line bottomVLine( Vec2D( x, y+bmpH ), Vec2D( x, maxY ));
               
            #ifdef OL_DEBUG_TEXTURED_POLY
               topVLine.Draw( Rgba( 1.0, 0.0, 1.0 ));
               bottomVLine.Draw( Rgba( 0.0, 1.0, 1.0 ));
            #endif // OL_DEBUG_TEXTURED_POLY
            
            if( !( LineCollides( topVLine, topStart, topEnd ) &&
                   LineCollides( bottomVLine, bottomStart, bottomEnd ))) {
               // The slice is outside the polygon (test works only with convex polygons)
               continue;
            }
         }
         
         // Remember that the top side is right to left
         std::vector< Vec2D > ::reverse_iterator topIter = topSide.vertices.rbegin();
         std::vector< Vec2D > ::reverse_iterator topPrev = topIter;
         topIter++;
         
         std::vector< Vec2D > ::const_iterator bottomIter = bottomSide.vertices.begin();
         std::vector< Vec2D > ::const_iterator bottomPrev = bottomIter;
         bottomIter++;
         
         HSlice *hslice = new HSlice();
         
         float startTop = topPrev->y;
         float startBottom = bottomPrev->y;
         
         if( startTop > startBottom ) {
            float temp = startTop;
            startTop = startBottom;
            startBottom = temp;
         }
         
         hslice->AddPart( new SlicePart( startTop, startBottom, x, x, y, texture ));
         
         while( topIter != topSide.vertices.rend() && bottomIter != bottomSide.vertices.end() ) {
            float top, bottom, sliceX;
            
            // Unoptimized, uses Line::GetIntersectionPoint
            
            if( topIter->x < bottomIter->x ) {
               // Top cuts from bottom
               
               sliceX = topIter->x;
               top = topIter->y;
               
               Line vLine( *topIter, Vec2D( topIter->x, y + bmpH + 1.0 ));
               Line bottomLine( *bottomPrev, *bottomIter );
               bottom = vLine.GetIntersectionPoint( bottomLine ).y;
               
               topPrev = topIter;
               topIter++;
            }
            else  {
               // Bottom cuts from top
               
               sliceX = bottomIter->x;
               bottom = bottomIter->y;
               
               Line vLine( *bottomIter, Vec2D( bottomIter->x, y - 1.0 ));
               Line topLine( *topPrev, *topIter );
               top = vLine.GetIntersectionPoint( topLine ).y;
               
               bottomPrev = bottomIter;
               bottomIter++;
            }
            
            if( top > bottom ) {
               float temp = bottom;
               bottom = top;
               top = temp;
            }
            
            #ifdef OL_DEBUG_TEXTURED_POLY
               cout << "Adding at x =  " << sliceX << ": Top: " << top << " Bottom " << bottom << endl;
            #endif // OL_DEBUG_TEXTURED_POLY
            
            // Construct the SlicePart
            hslice->AddPart( new SlicePart( top, bottom, sliceX, x, y, texture ));
         }
         
         slices.push_back( hslice );
      }
   }
   
   dirty = false;
}


TexturedPoly::SideProcessed TexturedPoly::
ProcessSide( std::vector< Vec2D > ::const_iterator start, std::vector< Vec2D > ::const_iterator end,
             float x, float y, bool isUpper, bool leftToRight ) {
   
   TexturedPoly::SideProcessed ret;
   
   float bmpW = texture->Width();
   float bmpH = texture->Height();
   const std::vector< Vec2D > &vertices = GetVertices();
   
   if( end == vertices.end() ) {
      end = vertices.begin();
   }
   
   #ifdef OL_DEBUG_TEXTURED_POLY
      Rect( x, y, bmpW, bmpH ).DrawOutline( Rgba::BLUE );
   #endif // OL_DEBUG_TEXTURED_POLY
   
   Vec2D topLeft = Vec2D( x, y );
   Vec2D topRight = Vec2D( x + bmpW, y );
   Vec2D bottomRight = Vec2D( x + bmpW, y + bmpH );
   Vec2D bottomLeft = Vec2D( x, y + bmpH );
   
   Line *textureEdges[4];
   textureEdges[0] = new Line( topLeft, topRight );
   textureEdges[1] = new Line( topRight, bottomRight );
   textureEdges[2] = new Line( bottomRight, bottomLeft );
   textureEdges[3] = new Line( bottomLeft, topLeft );
   
   std::vector< Vec2D > ::const_iterator beforeIter = start;
   std::vector< Vec2D > ::const_iterator iter = start;
   iter++;
   
   if( iter == vertices.end() ) {
      iter = vertices.begin();
   }
   
   bool inside = start->y > y && start->y < y + bmpH
        && (( leftToRight && start->x >= x ) || ( !leftToRight && start->x <= x + bmpW ));
   
   if( inside ) {
      ret.vertices.push_back( *start );
   }
   else {
      // Find the first vertex which is inside the polygon area
      
      while( iter != end ) {
         if(( leftToRight && iter->x >= x ) || ( !leftToRight && iter->x <= x + bmpW )) {
            AddEdgePoints( ret.vertices, Line( *beforeIter, *iter ), textureEdges, leftToRight );
            //cout << "first last: \n" << ret.vertices.back().ToString() << endl;
            break;
         }
         
         beforeIter = iter;
         
         iter++;
         if( iter == vertices.end() ) {
            iter = vertices.begin();
         }
      }
   }
   
   bool shouldEnd = false;
   std::vector< Vec2D > ::const_iterator beforeBeforeIter = vertices.end();
   
   // Process all vertices inside the polygon area
   
   while( !shouldEnd ) {
      if( iter == end ) {
         shouldEnd = true;
      }
      
      if (( !leftToRight && iter->x < x ) || ( leftToRight && iter->x > x + bmpW )) {
         // Add the point where the side leaves the texture area
         if( inside ) {
            AddEdgePoints( ret.vertices, Line( *beforeIter, *iter ), textureEdges, leftToRight );
         }
         break;
      }
      
      if( iter->y >= y - OL_NEAR_ZERO && iter->y <= y + bmpH + OL_NEAR_ZERO ) {
         if( !inside ) {
            inside = true;
            AddEdgePoints( ret.vertices, Line( *beforeIter, *iter ), textureEdges, leftToRight );
         }
         
         ret.vertices.push_back( *iter );
      }
      else if( inside ) {
         inside = false;
         AddEdgePoints( ret.vertices, Line( *beforeIter, *iter ), textureEdges, leftToRight );
      }
      
      beforeBeforeIter = beforeIter;
      beforeIter = iter;
      
      iter++;
      if( iter == vertices.end() ) {
         iter = vertices.begin();
      }
   }
   
   // Add vertex in the case if the only crossing line crosses both the lower and upper sides of the region
   if( ret.vertices.empty() && beforeBeforeIter != vertices.end() ) {
#ifdef OL_DEBUG_TEXTURED_POLY
      cout << "A1: " << Line( *beforeBeforeIter, *beforeIter ).ToString() << endl;
#endif
      AddEdgePoints( ret.vertices, Line( *beforeBeforeIter, *beforeIter ), textureEdges, leftToRight );
   }
   
   for( int i = 0; i < 4; i++ ) {
      delete textureEdges[i];
   }
   
   // Add a suitable first vertex if it's needed
   if( !ret.vertices.empty() && (( leftToRight && ret.vertices.front().x > x + OL_NEAR_ZERO )
       || ( !leftToRight && ret.vertices.front().x < x + bmpW - OL_NEAR_ZERO ))) {
      vector< Vec2D > newRet;
      if( leftToRight ) {
         newRet.push_back( Vec2D( x, ret.vertices.front().y ));
      }
      else {
         newRet.push_back( Vec2D( x + bmpW, ret.vertices.front().y ));
      }
      
      for( vector< Vec2D > ::iterator iter = ret.vertices.begin(); iter != ret.vertices.end(); iter++ ) {
         newRet.push_back( *iter );
      }
      
      ret.vertices = newRet;
   }
   
   bool wasNoFirst = false;
   
   // If no vertices exist, we need to find suitable start and end vertices
   if( ret.vertices.empty() ) {
      wasNoFirst = true;
      ret.vertices.push_back( Vec2D( leftToRight? x : x+bmpW, isUpper? y : y+bmpH ));
   }
   
   // Add the last vertex if the side has not collided with the left or the right side
   if((( leftToRight && ret.vertices.back().x < x + bmpW - OL_NEAR_ZERO )
       || ( !leftToRight && ret.vertices.back().x > x + OL_NEAR_ZERO ))) {
      ret.vertices.push_back( Vec2D( leftToRight? x+bmpW : x, ret.vertices.back().y ));
      
      if( wasNoFirst ) {
         ret.isEmpty = true;
      }
   }
   
#ifdef OL_DEBUG_TEXTURED_POLY
   glLineWidth( 2.0 );
   glBegin( GL_LINE_STRIP );
   
   if( isUpper )
      Rgba( 0.0, 1.0, 0.5, 0.5 ).Select();
   else
      Rgba( 1.0, 0.7, 0.0, 0.5 ).Select();
   
   for( vector< Vec2D > ::iterator iter = ret.vertices.begin(); iter != ret.vertices.end(); iter++ ) {
      glVertex2f( iter->x, iter->y );
   }
   
   glEnd();
   
   glLineWidth( 1.0 );
   
   for( vector< Vec2D > ::iterator iter = ret.vertices.begin(); iter != ret.vertices.end(); iter++ ) {
      Circle( *iter, isUpper? 5.0 : 7.0 ).Draw( isUpper? Rgba::GREEN.WithAlpha( 0.7 ) : Rgba::RED.WithAlpha( 0.7 ));
   }
#endif // OL_DEBUG_TEXTURED_POLY
   
   return ret;
}


void TexturedPoly::
AddEdgePoints( vector< Vec2D > &vecs, const Line &segment, Line **textureEdges, bool leftToRight ) {
   vector< Vec2D > addList;
   
   for( int i = 0; i < 4; i++ ) {
      if( textureEdges[i]->Collides( segment )) {
         Vec2D collisionPoint = textureEdges[i]->GetIntersectionPoint( segment );
         
         if( vecs.empty() || collisionPoint != vecs.back() ) {
            vector< Vec2D > ::iterator insertPoint = addList.begin();
            
            for( vector< Vec2D > ::iterator iter = addList.begin(); iter != addList.end(); iter++ ) {
               if(( iter->x < collisionPoint.x ) != leftToRight ) {
                  break;
               }
               insertPoint++;
            }
            
            addList.insert( insertPoint, collisionPoint );
         }
      }
   }
   
   for( vector< Vec2D > ::iterator iter = addList.begin(); iter != addList.end(); iter++ ) {
      vecs.push_back( *iter );
   }
}


TexturedPoly::SlicePart::
SlicePart( float top, float bottom, float x, float textureStartX, float textureStartY, const Bitmap *bmp )
   : top( top ), bottom( bottom ), x( x ) {
   const OlRect &textureRect = bmp->textureInfo.rect;
   
   float mul = 1.0 / bmp->Height();
   textureTop = textureRect.y + textureRect.h
      - mul * (top - textureStartY) * textureRect.h;
   
   textureBottom = textureRect.y + textureRect.h
      - mul * (bottom - textureStartY) * textureRect.h;
   
   textureX = textureRect.x + (x - textureStartX) / bmp->Width() * textureRect.w;
}


TexturedPoly::HSlice::
~HSlice() {
   for( std::vector< SlicePart *> ::iterator iter = parts.begin(); iter != parts.end(); iter++ ) {
      delete *iter;
   }
}


void TexturedPoly::
Add( Vec2D vec ) {
   Poly::Add( vec );
   dirty = true;
}


void TexturedPoly::
SetTexture( const Bitmap &texture ) {
   this->texture = &texture;
   dirty = true;
}


void TexturedPoly::
SetVertex( int index, const Vec2D &newValue ) {
   Poly::SetVertex( index, newValue );
   dirty = true;
}

