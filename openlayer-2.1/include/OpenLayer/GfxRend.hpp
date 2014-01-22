#ifndef OL_GFX_REND_HPP
#define OL_GFX_REND_HPP


#include "Includes.hpp"
#include "Rgba.hpp"
#include "Settings.hpp"
#include "Declspec.hpp"


namespace ol {

// THIS CLASS IS DEPRECATED //
// Use Shape and Canvas instead //
// GfxRend - Primitive graphics rendering functions //

class OL_LIB_DECLSPEC GfxRend {
public:
   // Call this after you've finished rendering the current frame //
   static void RefreshScreen();
   
   
   // Rectangle functions //
   
   // Draws a filled rectangle //
   static void Rect( float x, float y, float width, float height, Rgba color );
   
   // Draws a non-filled rectangle //
   static void RectOutline( float x, float y, float width, float height,
                            Rgba color, float lineWidth = 1.0 );
   
   // Draws a rectangle filled with a gradient such that each corner of the //
   // rectangle is given a color and the colors of all pixels inside the rectangle //
   // are interpolated along the colors of the corners //
   // Colors should be an array with 4 color values such that colors[0] is the color //
   // of the top-left corner, colors[1] top-right corner and so forth in clockwise order //
   static void RectGradient( float x, float y, float width, float height, Rgba *colors );
   
   
   // Line functions //
   
   // Draws a line between (x1, y1) and (x2, y2) //
   static void Line( float x1, float y1, float x2, float y2, Rgba col, float lineWidth = 1.0 );
   
   // Draws a line such that the color of the starting point (x1, y1) is color1 //
   // and the color of the ending point (x2, y2) is color2 and the colors of all //
   // the pixels in-between change smoothly from color1 to color2 //
   static void LineGradient( float x1, float y1, float x2, float y2, Rgba color1,
                             Rgba color2, float lineWidth = 1.0 );
   
   // Renders a series of lines between (xCoordinates[i], yCoodrinates[i]) and //
   // (xCoordinates[i+1], yCoodrinates[i+1]) //
   static void LineStrip( float *xCoordinates, float *yCoordinates, int numCoordinates,
                          Rgba color, float lineWidth = 1.0 ); 
   
   // Like LineStrip but with colors changing smoothly from startCol to endCol //
   // along the line //
   static void LineStripGradient( float *xCoordinates, float *yCoordinates, int numCoordinates,
                                  Rgba startCol, Rgba endCol, float lineWidth = 1.0 ); 
   
   // Like LineStrip but with a different color for each coordinate //
   // The colors along the series of lines will be interpolated between the colors //
   // of the first and last points of the line series as well as between the colors //
   // in of left and right sides //
   static void LineStripGradient( float *xCoordinates, float *yCoordinates, int numCoordinates,
                                  Rgba startCol, Rgba endCol, Rgba leftCol, Rgba rightCol, float lineWidth = 1.0 ); 
   
   // Like LineStrip but with a different color for each coordinate //
   // The colors along the lines will be interpolated between the colors //
   // of the end points of the line //
   static void LineStripGradient( float *xCoordinates, float *yCoordinates, int numCoordinates,
                                  Rgba *colors, float lineWidth = 1.0 ); 
   
   
   // Circle functions //
   
   // Accuracy is between 0 and 1, though it should never be zero //
   // If no accuracy is specified, the default accuracy is used (change it in Settings) //
   
   // Draws a filled circle //
   static void Circle( float x, float y, float radius, Rgba col, float accuracy = Settings::GetCircleAccuracy() );
   
   // Draws a non-filled circle //
   static void CircleOutline( float x, float y, float radius, Rgba col,
                              float lineWidth = 1.0, float accuracy = Settings::GetCircleAccuracy() );
   
   // Draws a circle filled with a circular gradient such that the color of the centre is //
   // innerColor and the color of the circumference of the circle is outerColor //
   // and the colors inside the circle change smoothly from innerColor to outerColor //
   static void CircleGradient( float x, float y, float radius, Rgba innerColor,
                               Rgba outerColor, float accuracy = Settings::GetCircleAccuracy() );
   
   // Draws a filled circle with a circular hole in it //
   // The radius of the hole is innerRadius and the radius of the circle is outerRadius //
   static void Disk( float x, float y, float innerRadius, float outerRadius, Rgba color,
                     float accuracy = Settings::GetCircleAccuracy() );
   
   // Draws a disk with colors changing smoothly from innerColor to outerColor along the radius //
   static void DiskGradient( float x, float y, float innerRadius, float outerRadius,
                             Rgba innerColor, Rgba outerColor, float accuracy = Settings::GetCircleAccuracy() );
   
   
   // Ellipse routines //
   
   // The given angle rotates the ellipse around its centre //
   
   // Draws a filled ellipse //
   static void Ellipse( float x, float y, float xRadius, float yRadius, Rgba col,
                        float angle = 0.0, float accuracy = Settings::GetCircleAccuracy() );
   
   // Draws a non-filled ellipse //
   static void EllipseOutline( float x, float y, float xRadius, float yRadius, Rgba col, float angle = 0.0,
                               float lineWidth = 1.0, float accuracy = Settings::GetCircleAccuracy() );
   
   // Draws a gradient filled ellipse with colors changing smoothly from innerColor to outerColor along the radius //
   static void EllipseGradient( float x, float y, float xRadius, float yRadius, Rgba innerCol,
                                Rgba outerCol, float angle = 0.0, float accuracy = Settings::GetCircleAccuracy() );
   
   
   // Slice and arc functions //
   
   // The rendered range is startAngle -> startAngle + sweepAngle //
   
   // Draws a filled circle slice //
   static void Slice( float x, float y, float radius, float startAngle, float sweepAngle,
                      Rgba color, float accuracy = Settings::GetCircleAccuracy() );
   
   
   // Draws a gradient filled circle slice with colors changing smoothly //
   // from innerColor to outerColor along the radius //
   static void SliceGradient( float x, float y, float radius, float startAngle, float sweepAngle,
                              Rgba innerColor, Rgba outerColor, float accuracy = Settings::GetCircleAccuracy() );
   
   // Draws a filled ellipse slice //
   static void EllipseSlice( float x, float y, float xRad, float yRad, Rgba col,
                             float startAngle, float sweepAngle, float angle = 0.0, float accuracy = Settings::GetCircleAccuracy() );
   
   // Draws a gradient filled ellipse slice //
   static void EllipseSliceGradient( float x, float y, float xRad, float yRad, Rgba innerColor,
                                     Rgba outerColor, float startAngle, float sweepAngle,
                                     float angle = 0.0, float accuracy = Settings::GetCircleAccuracy() );
   
   // Draws a double-gradient filled ellipse slice //
   static void EllipseSliceGradient( float x, float y, float xRad, float yRad, Rgba innerStartColor,
                                     Rgba outerStartColor, Rgba innerEndColor, Rgba outerEndColor, float startAngle, float sweepAngle,
                                     float angle = 0.0, float accuracy = Settings::GetCircleAccuracy() );
   
   // Draws a filled arc //
   static void Arc( float x, float y, float innerRad, float outerRad, 
                    float startAngle, float sweepAngle, Rgba color, float accuracy = Settings::GetCircleAccuracy() );
   
   // Draws a gradient filled arc with colors changing smoothly //
   // from innerColor to outerColor along the radius //
   static void ArcGradient( float x, float y, float innerRad, float outerRad,
                            float startAngle, float sweepAngle, Rgba innerCol,
                            Rgba outerCol, float accuracy = Settings::GetCircleAccuracy() );
   
   
   // Triangle functions //
   
   // Draws a filled triangle //
   static void Triangle( float x1, float y1, float x2, float y2, float x3, float y3, Rgba color );
   
   // Draws a non-filled triangle //
   static void TriangleOutline( float x1, float y1, float x2, float y2, float x3, float y3, Rgba color, float lineWidth = 1.0 );
   
   // Renders a gradient filled triangle such that the colors of the pixels inside the triangle //
   // are interpolated along the colors of the vertices //
   static void TriangleGradient( float x1, float y1, float x2, float y2, float x3, float y3, Rgba *colors );
   
   
   // Polygon functions //
   
   // Renders a filled polygon //
   static void Polygon( float *xCoordinates, float *yCoordinates, int numCoordinates, Rgba color );
   
   // Renders a non-filled polygon //
   static void PolygonOutline( float *xCoordinates, float *yCoordinates, int numCoordinates, Rgba color, float lineWidth = 1.0 );
   
   // Renders a gradient filled polygon such that the colors of the pixels inside the polygon //
   // are interpolated along the colors of the vertices //
   static void PolygonGradient( float *xCoordinates, float *yCoordinates, int numCoordinates, Rgba *colors );
   
   
   // Other drawing functions //
   
   // Draws a single point in the screen //
   static void Point( float x, float y, Rgba color );
   
   // Fills the entire screen with the specified color //
   // Ignores the alpha value of the color //
   static void FillScreen( Rgba color );
   
   
   // Returns the OpenGL error as text or null if no error has occured //
   static const char *ErrorString();
   
   
private:
   
   static float GetAngleIncrement( float rad, float accuracy );
   
   static void DiskRender( float x, float y, float innerRad, float outerRad, Rgba innerCol, Rgba outerCol, float accuracy, bool setCols = true );
   
   static void LineStripRender( float *xCoordinates, float *yCoordinates, int numCoordinates,
                                Rgba *colors, float lineWidth, bool singleColor, bool useSideColors = false,
                                Rgba leftCol = Rgba::BLACK, Rgba rightCol = Rgba::BLACK );
   
   static Rgba *GenerateColors( float *xCoordinates, float *yCoordinates, int numCoordinates, Rgba col1, Rgba col2 );
};



}


#endif // OL_GFX_REND_HPP
