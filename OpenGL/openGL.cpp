#include "openGL.h"
static bool
CONFIG_LOCAL_SHOW_TRIANGLE          = true,
CONFIG_LOCAL_SHOW_CIRCLE            = true,
CONFIG_LOCAL_SHOW_VORONOI_DIAGRAM   = true,
CONFIG_LOCAL_SHOW_VERTEX_LABEL      = true;

void openGL::drawLine( mpfr::real<MPFR_BITS_PRECISION> x1, mpfr::real<MPFR_BITS_PRECISION> y1, mpfr::real<MPFR_BITS_PRECISION> x2, mpfr::real<MPFR_BITS_PRECISION> y2, double r, double g, double b )
{
    double x1_double = 0, y1_double = 0, x2_double = 0, y2_double = 0;    
    x1.conv(x1_double);
    y1.conv(y1_double);
    x2.conv(x2_double);
    y2.conv(y2_double);        
    glColor3f(r, g, b); // Line color
    glBegin(GL_LINES);
    glVertex2d(x1_double,y1_double);
    glVertex2d(x2_double,y2_double);
    glEnd();
}

void openGL::drawVertex( Vertex *v )
{
    double x_double = 0, y_double = 0;    
    mpfr::real<MPFR_BITS_PRECISION> x = v->x;
    mpfr::real<MPFR_BITS_PRECISION> y = v->y;
    x.conv(x_double);   
    y.conv(y_double);      
    glPointSize(5.5);
    glBegin(GL_POINTS);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2d(x_double, y_double);
    glEnd();
    //
    // Print the vertex value
    //
    std::stringstream s1;
    double value_double = 0;
    v->u.conv(value_double);
    //s1 << value_double;
    s1 << v->label;
    string str = s1.str();
    RenderBitmap(x_double, y_double, str);        
}

void openGL::drawVertex( mpfr::real<MPFR_BITS_PRECISION> x, mpfr::real<MPFR_BITS_PRECISION> y  )
{
    double x_double = 0, y_double = 0;    
    x.conv(x_double);
    y.conv(y_double);    
    //glPointSize(5.5);

    glBegin(GL_POINTS);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2d(x_double, y_double);
    glEnd();
    //
    // Print the vertex coordinates
    //
    std::stringstream s1;
    s1 << x << "," << y;
    string str = s1.str();
    RenderBitmap(x_double, y_double, str);        
}

void openGL::RenderBitmap(mpfr::real<MPFR_BITS_PRECISION> x, mpfr::real<MPFR_BITS_PRECISION> y, string str)
{
    double x_double = 0, y_double = 0;    
    x.conv(x_double);
    y.conv(y_double);        
    glColor3f( 255,0,0 );
    const char *c;
    glRasterPos2f (x_double, y_double);
    for (c = str.c_str(); *c != '\0' && *(c-3) != '.'; c++)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
        //glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
}

void openGL::drawTriangle( Vertex *v1, Vertex *v2, Vertex *v3 )
{
    glColor3f( 0, 0, 0 );
    if (CONFIG_LOCAL_SHOW_TRIANGLE || (v1->isBorder && v2->isBorder)) 
    {    
        drawLine( v1->x, v1->y, v2->x, v2->y, 0, 0, 0 );
    }
    if (CONFIG_LOCAL_SHOW_TRIANGLE || (v1->isBorder && v3->isBorder)) 
    {
        drawLine( v1->x, v1->y, v3->x, v3->y, 0, 0, 0 );
    }
    if (CONFIG_LOCAL_SHOW_TRIANGLE || (v3->isBorder && v2->isBorder))
    {
        drawLine( v3->x, v3->y, v2->x, v2->y, 0, 0, 0 );
    }
        
    if (CONFIG_LOCAL_SHOW_VERTEX_LABEL) 
    {    
        drawVertex(v1);
        drawVertex(v2);
        drawVertex(v3);
    }
}

void openGL::drawCcircle( Vertex *v, mpfr::real<MPFR_BITS_PRECISION> ccenterX, mpfr::real<MPFR_BITS_PRECISION> ccenterY )
{
    double ccenterX_double = 0, ccenterY_double = 0, angle_double = 0, raio_double = 0;      
    mpfr::real<MPFR_BITS_PRECISION> raio = sqrt( ( v->x - ccenterX )*( v->x - ccenterX ) +
                                                 ( v->y - ccenterY )*( v->y - ccenterY ) ); 
    raio.conv(raio_double);
    mpfr::real<MPFR_BITS_PRECISION> angle;
    glColor3f(0, 255, 0);
    glBegin(GL_LINE_LOOP);
    if (CONFIG_LOCAL_SHOW_CIRCLE) {
        for( int i=0; i < 300; i++ )
        {
            angle = (2*3.1415926535*i)/300;
            angle.conv(angle_double);
            ccenterX.conv(ccenterX_double);
            ccenterY.conv(ccenterY_double);
            glVertex2d(ccenterX_double + raio_double*cos(angle_double), ccenterY_double + raio_double*sin(angle_double));
        }        
    }
    glEnd();
}

void openGL::drawGrid( list<Triangle *> &tlist, list<Adjacency *> &edges, bool CONFIG_SHOW_VERTEX_LABEL, bool CONFIG_SHOW_TRIANGLE, bool CONFIG_SHOW_CIRCLE, bool CONFIG_SHOW_VORONOI_DIAGRAM)
{
    CONFIG_LOCAL_SHOW_TRIANGLE          = CONFIG_SHOW_TRIANGLE,
    CONFIG_LOCAL_SHOW_CIRCLE            = CONFIG_SHOW_CIRCLE,
    CONFIG_LOCAL_SHOW_VORONOI_DIAGRAM   = CONFIG_SHOW_VORONOI_DIAGRAM,
    CONFIG_LOCAL_SHOW_VERTEX_LABEL      = CONFIG_SHOW_VERTEX_LABEL;    
    //list<Adjacency *>::iterator ite;
    // TODO: descobrir o que faz esse codigo
    /*for( ite = edges.begin(); ite != edges.end(); ite++ )
    {
        Vertex *v0 = (*ite)->getVertex(0);
        Vertex *v1 = (*ite)->getVertex(1);
        if( (*ite)->type == 1 )
        {
            glLineWidth(3);
            drawLine( v0->x, v0->y, v1->x, v1->y, 0, 0, 0 );
            glLineWidth(1);
        }
        else
            drawLine( v0->x, v0->y, v1->x, v1->y, 0, 0, 0 );
    }*/
    list<Triangle *>::iterator ite;
    for( ite = tlist.begin(); ite != tlist.end(); ite++ )
    {
        Triangle *it = *ite;
        Vertex *v1 = it->getVertex(0), *v2 = it->getVertex(1), *v3 = it->getVertex(2);
        //
        // Draw Triangle 
        // 
        drawTriangle(v1, v2, v3);
        //
        // Draw Circle
        //
        drawCcircle(it->getVertex(0), it->cCenter[0], it->cCenter[1]);

        // TODO: Descobrir o que faz esse codigo
//        drawLine( it->cCenter[0], it->cCenter[1], it->getVertex(0)->x, it->getVertex(0)->y, 255,0,255 );
//        drawLine( it->cCenter[0], it->cCenter[1], it->getVertex(1)->x, it->getVertex(1)->y, 255,0,255 );
//        drawLine( it->cCenter[0], it->cCenter[1], it->getVertex(2)->x, it->getVertex(2)->y, 255,0,255 );        
        //
        // Draws the Voronoi diagram
        //
        if (CONFIG_LOCAL_SHOW_VORONOI_DIAGRAM) {
            if( it->getNeighbor(0) != 0 )
            {
                drawLine( it->cCenter[COORDINATE_X], it->cCenter[COORDINATE_Y], it->getNeighbor(0)->cCenter[COORDINATE_X], it->getNeighbor(0)->cCenter[COORDINATE_Y], 0,0,255 );
            }
            if( it->getNeighbor(1) != 0 )
            {
                drawLine( it->cCenter[COORDINATE_X], it->cCenter[COORDINATE_Y], it->getNeighbor(1)->cCenter[COORDINATE_X], it->getNeighbor(1)->cCenter[COORDINATE_Y], 0,0,255 );
            }
            if( it->getNeighbor(2) != 0 )
            {
                drawLine( it->cCenter[COORDINATE_X], it->cCenter[COORDINATE_Y],it->getNeighbor(2)->cCenter[COORDINATE_X], it->getNeighbor(2)->cCenter[COORDINATE_Y], 0,0,255 );
            }
        }         
    }
}