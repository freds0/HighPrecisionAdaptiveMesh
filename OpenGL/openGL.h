#ifndef OPENGL_INCLUDED
#define OPENGL_INCLUDED
#include <sstream>
#include <cmath>
#include <list>
#include <string>
#include <GL/glut.h>
#include <GL/gl.h>
#include "../Vertex/vertex.h"
#include "../Triangle/triangle.h"
#include "../Adjacency/adjacency.h"
using namespace std;
class openGL
{
public:
    /**
     * Draws a line for p1(x1, y1) to p2(x2, y2) with the color rgb 
     * @param x1 x coordinate of the point p1
     * @param y1 y coordinate of the point p1
     * @param x2 x coordinate of the point p2
     * @param y2 y coordinate of the point p2
     * @param r r value of rgb
     * @param g g value of rgb
     * @param b b value of rgb
     */  
    static void drawLine( mpfr::real<MPFR_BITS_PRECISION> x1, mpfr::real<MPFR_BITS_PRECISION> y1, mpfr::real<MPFR_BITS_PRECISION> x2, mpfr::real<MPFR_BITS_PRECISION> y2, double r, double g, double b );
    /**
     *  Print the string in the position (x,y)
     * @param x x coordinate
     * @param y y coordinate
     * @param str string to print
     */      
    static void RenderBitmap(mpfr::real<MPFR_BITS_PRECISION> x, mpfr::real<MPFR_BITS_PRECISION> y, string str);
    /**
     *  Draws the vertex v
     * @param v vertex to draw
     */       
    static void drawVertex( Vertex *v );
    /**
     *  Draws the vertex coordinates
     * @param x x coordinate
     * @param y y coordinate
     */           
    static void drawVertex( mpfr::real<MPFR_BITS_PRECISION> x, mpfr::real<MPFR_BITS_PRECISION> y );    
    /**
     *  Draws a triangle
     * @param v1 vertex v1 of triangle
     * @param v2 vertex v2 of triangle
     * @param v3 vertex v1 of triangle
     */       
    static void drawTriangle( Vertex *v1, Vertex *v2, Vertex *v3 );
    /**
     *  Draws the circuncircle
     * @param v1 vertex v of triangle to calculates radius of circuncircle
     * @param x x coordinate of circuncircle
     * @param y y coordinate of circuncircle
     */     
    static void drawCcircle( Vertex *v, mpfr::real<MPFR_BITS_PRECISION> ccenterX, mpfr::real<MPFR_BITS_PRECISION> ccenterY );
    /**
     *  Draws the Grid
     * @param tlist triangles list
     * @param edges edges list
     * @param CONFIG_SHOW_VERTEX_LABEL true to print vertex label
     * @param CONFIG_SHOW_TRIANGLE true to print triangle
     * @param CONFIG_SHOW_CIRCLE true to print circuncircle
     * @param CONFIG_SHOW_VORONOI_DIAGRAM true to print voronoi diagram
     */       
    static void drawGrid( list<Triangle *> &tlist, list<Adjacency *> &edges, bool CONFIG_SHOW_VERTEX_LABEL, bool CONFIG_SHOW_TRIANGLE, bool CONFIG_SHOW_CIRCLE, bool CONFIG_SHOW_VORONOI_DIAGRAM);
};
#endif // OPENGL_INCLUDED
