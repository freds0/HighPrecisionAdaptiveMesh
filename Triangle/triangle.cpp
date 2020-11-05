/****** Implementation of the class Triangle (in file triangle.h) ******/
#define _USE_MATH_DEFINES
#include <cmath>
#include "triangle.h"
#include "../Utils/utils.h"
#include "../Adjacency/adjacency.h"

Triangle::Triangle(Vertex *v1, Vertex *v2, Vertex *v3)
{
    this->v[TRIANGLE_VERTEX_ONE] = this->v[TRIANGLE_VERTEX_TWO] = this->v[TRIANGLE_VERTEX_THREE] = NULL;
    if ((v1 != NULL) && (v2 != NULL) && (v3 != NULL))
        setVertices(v1, v2, v3);
    else {
      cerr << "ERROR - Triangle::Triangle" << endl;
      exit(EXIT_FAILURE);
    }
    // Setting neighbors
    this->neighbor[TRIANGLE_VERTEX_ONE]   = NULL;
    this->neighbor[TRIANGLE_VERTEX_TWO]   = NULL;
    this->neighbor[TRIANGLE_VERTEX_THREE] = NULL;
    this->updateCcenter();
    this->updateAngle();
    this->updateRadiusEdge();  
    this->updateShapeRegularityQuality(); 
}

void Triangle::setVertices(Vertex *v1, Vertex *v2, Vertex *v3)
{
    mpfr::real<MPFR_BITS_PRECISION> *orientation = Utils::orientation(v1, v2, v3);
    if( *orientation > 0 )
    {
        this->v[TRIANGLE_VERTEX_ONE]   = v1;
        this->v[TRIANGLE_VERTEX_TWO]   = v2;
        this->v[TRIANGLE_VERTEX_THREE] = v3;
    }
    else
    {
        this->v[TRIANGLE_VERTEX_ONE]   = v1;
        this->v[TRIANGLE_VERTEX_TWO]   = v3;
        this->v[TRIANGLE_VERTEX_THREE] = v2;
    }
    delete orientation;
}

void Triangle::update() {
    
    this->updateCcenter();
    this->updateAngle();
    this->updateRadiusEdge();    
    this->updateShapeRegularityQuality();   
}

void Triangle::updateSRQ() {
    
    this->updateShapeRegularityQuality();    
}

void Triangle::setVertice( Vertex *v, int number)
{
    if( number >= TRIANGLE_VERTEX_ONE && number <= TRIANGLE_VERTEX_THREE )
    {
        this->v[number] = v;
        setVertices(this->v[TRIANGLE_VERTEX_ONE], this->v[TRIANGLE_VERTEX_TWO], this->v[TRIANGLE_VERTEX_THREE]);
        updateCcenter();
        updateAngle();
        updateRadiusEdge();
    }
    else
    {
        cerr << "ERROR - Triangle::setVertice" << endl;
        exit(EXIT_FAILURE);
    }
}

void Triangle::setNeighbor(Triangle *t)
{
    Vertex *v0 = t->getVertex(TRIANGLE_VERTEX_ONE);
    Vertex *v1 = t->getVertex(TRIANGLE_VERTEX_TWO);
    Vertex *v2 = t->getVertex(TRIANGLE_VERTEX_THREE);    
    if ( (this->hasVertex(v0)) && this->hasVertex(v1) ) 
    {
        int numberV0 = this->getNumber(v0);
        int numberV1 = this->getNumber(v1);
        this->neighbor[3 - (numberV0 + numberV1)] = t; /* Triangle opposite to the vertex v[TRIANGLE_VERTEX_THREE] */
    }    
    else if ( (this->hasVertex(v0)) && this->hasVertex(v2) ) 
    {
        int numberV0 = this->getNumber(v0);
        int numberV2 = this->getNumber(v2);        
        this->neighbor[3 - (numberV0 + numberV2)] = t; /* Triangle opposite to the vertex v[TRIANGLE_VERTEX_TWO] */
    }    
    else if ( (this->hasVertex(v1)) && this->hasVertex(v2) ) 
    {
        int numberV1 = this->getNumber(v1);
        int numberV2 = this->getNumber(v2);            
        this->neighbor[3 - (numberV1 + numberV2)] = t; /* Triangle opposite to the vertex v[TRIANGLE_VERTEX_ONE] */
    }
    else /* t is not a neighbor */
    {
        cerr << "ERROR - Triangle::setNeighbor" << endl;
        exit(EXIT_FAILURE);
    }
}

void Triangle::replaceNeighbor(Triangle *oldNeighbor, Triangle *newNeighbor)
{
    if(this->neighbor[TRIANGLE_VERTEX_ONE] == oldNeighbor)
        this->neighbor[TRIANGLE_VERTEX_ONE] = newNeighbor;
    else if(this->neighbor[TRIANGLE_VERTEX_TWO] == oldNeighbor)
        this->neighbor[TRIANGLE_VERTEX_TWO] = newNeighbor;
    else if(this->neighbor[TRIANGLE_VERTEX_THREE] == oldNeighbor)
        this->neighbor[TRIANGLE_VERTEX_THREE] = newNeighbor;
}

Vertex *Triangle::getVertex(int number)
{
    if( !(number >= TRIANGLE_VERTEX_ONE && number <= TRIANGLE_VERTEX_THREE) )
    {
        cerr << "ERROR - Triangle::getVertex" << endl;
        exit(EXIT_FAILURE);
    }
    return this->v[number];

}

Triangle *Triangle::getNeighbor(int number)
{
    if( !(number >= TRIANGLE_VERTEX_ONE && number <= TRIANGLE_VERTEX_THREE) )
    {
        cerr << "ERROR - Triangle::getNeighbor" << endl;
        exit(EXIT_FAILURE);
    }
    return this->neighbor[number];
}

int Triangle::getNumber(Vertex *v)
{
    int number = -1;
    if( this->v[TRIANGLE_VERTEX_ONE] == v )
        number = TRIANGLE_VERTEX_ONE;
    else if( this->v[TRIANGLE_VERTEX_TWO] == v )
        number = TRIANGLE_VERTEX_TWO;
    else if( this->v[TRIANGLE_VERTEX_THREE] == v )
        number = TRIANGLE_VERTEX_THREE;
    else 
    {
        cerr << "ERROR - Triangle::getNumber" << endl;
        exit(EXIT_FAILURE);
    }
    return number;
}

void Triangle::updateCcenter()
{
    mpfr::real<MPFR_BITS_PRECISION> *x1 = NULL, *y1 = NULL, *x2 = NULL, *y2 = NULL, *x3 = NULL, *y3 = NULL, asq = 0, csq = 0, top1 = 0, top2 = 0, bot = 0, *sqrRadius = NULL;
    x1 = &(v[TRIANGLE_VERTEX_ONE]->x);
    y1 = &(v[TRIANGLE_VERTEX_ONE]->y);
    x2 = &(v[TRIANGLE_VERTEX_TWO]->x);
    y2 = &(v[TRIANGLE_VERTEX_TWO]->y);
    x3 = &(v[TRIANGLE_VERTEX_THREE]->x);
    y3 = &(v[TRIANGLE_VERTEX_THREE]->y);
    asq = ( *x2 - *x1 ) * ( *x2 - *x1 ) + ( *y2 - *y1 ) * ( *y2 - *y1 );
    csq = ( *x3 - *x1 ) * ( *x3 - *x1 ) + ( *y3 - *y1 ) * ( *y3 - *y1 );
    top1 = ( *y2 - *y1 ) * csq - ( *y3 - *y1 ) * asq;
    top2 = -( *x2 - *x1 ) * csq + ( *x3 - *x1 ) * asq;
    bot = ( ( *y2 - *y1 ) * ( *x3 - *x1 ) ) - ( ( *y3 - *y1 ) * ( *x2 - *x1 ) );
    this->cCenter[COORDINATE_X] = *x1 +  (0.5*top1)/bot;
    this->cCenter[COORDINATE_Y] = *y1 +  (0.5*top2)/bot;
    if( bot == 0.0 )
    {
        cerr << "ERROR - UpdateCcenter(): Possible two points equals." << endl;
        exit(EXIT_FAILURE);
    }
    sqrRadius = Utils::distanceSQR(cCenter[COORDINATE_X], cCenter[COORDINATE_Y], v[TRIANGLE_VERTEX_ONE]);
    this->sqrRadius = *sqrRadius;
    delete sqrRadius;
}

void Triangle::updateAngle()
{
    mpfr::real<MPFR_BITS_PRECISION> *V1V2 = Utils::distance(v[TRIANGLE_VERTEX_ONE], v[TRIANGLE_VERTEX_TWO]);
    mpfr::real<MPFR_BITS_PRECISION> *V1V3 = Utils::distance(v[TRIANGLE_VERTEX_ONE], v[TRIANGLE_VERTEX_THREE]);
    mpfr::real<MPFR_BITS_PRECISION> *V2V3 = Utils::distance(v[TRIANGLE_VERTEX_TWO], v[TRIANGLE_VERTEX_THREE]);
    mpfr::real<MPFR_BITS_PRECISION> cosV1 = ( (*V1V2)*(*V1V2) + (*V1V3)*(*V1V3) - (*V2V3)*(*V2V3) )/ (2*(*V1V2)*(*V1V3));
    mpfr::real<MPFR_BITS_PRECISION> cosV2 = ( (*V1V2)*(*V1V2) + (*V2V3)*(*V2V3) - (*V1V3)*(*V1V3) )/ (2*(*V1V2)*(*V2V3));
    mpfr::real<MPFR_BITS_PRECISION> cosV3 = ( (*V1V3)*(*V1V3) + (*V2V3)*(*V2V3) - (*V1V2)*(*V1V2) )/ (2*(*V1V3)*(*V2V3));    
    delete V1V2;
    delete V1V3;
    delete V2V3;
    mpfr::real<MPFR_BITS_PRECISION> r = 180 / mpfr::const_pi<MPFR_BITS_PRECISION, MPFR_RNDN> (); // RADIAN TO DEGREES 57.295779513082320876; 
    this->angles[TRIANGLE_VERTEX_ONE]   = acos(cosV1) * r;
    this->angles[TRIANGLE_VERTEX_TWO]   = acos(cosV2) * r;
    this->angles[TRIANGLE_VERTEX_THREE] = acos(cosV3) * r;
    mpfr_free_cache();
}

bool Triangle::isInside(mpfr::real<MPFR_BITS_PRECISION> &px, mpfr::real<MPFR_BITS_PRECISION> &py)
{
    mpfr::real<MPFR_BITS_PRECISION> *v1x = &(v[TRIANGLE_VERTEX_ONE]->x);
    mpfr::real<MPFR_BITS_PRECISION> *v1y = &(v[TRIANGLE_VERTEX_ONE]->y);
    mpfr::real<MPFR_BITS_PRECISION> *v2x = &(v[TRIANGLE_VERTEX_TWO]->x);
    mpfr::real<MPFR_BITS_PRECISION> *v2y = &(v[TRIANGLE_VERTEX_TWO]->y);
    mpfr::real<MPFR_BITS_PRECISION> *v3x = &(v[TRIANGLE_VERTEX_THREE]->x);
    mpfr::real<MPFR_BITS_PRECISION> *v3y = &(v[TRIANGLE_VERTEX_THREE]->y);
    //
    /* Barycentre technique */
    //
    /* t->v1 is the origin  */
    mpfr::real<MPFR_BITS_PRECISION> v00x = *v3x - *v1x; // t->v3 - t->v1
    mpfr::real<MPFR_BITS_PRECISION> v00y = *v3y - *v1y;
    mpfr::real<MPFR_BITS_PRECISION> v01x = *v2x - *v1x; // t->v2 - t->v1
    mpfr::real<MPFR_BITS_PRECISION> v01y = *v2y - *v1y;
    mpfr::real<MPFR_BITS_PRECISION> v02x = px - *v1x; // p - t->v1
    mpfr::real<MPFR_BITS_PRECISION> v02y = py - *v1y;
    // Compute dot products
    mpfr::real<MPFR_BITS_PRECISION> dot00 = (v00x * v00x) + (v00y * v00y);
    mpfr::real<MPFR_BITS_PRECISION> dot01 = (v00x * v01x) + (v00y * v01y);
    mpfr::real<MPFR_BITS_PRECISION> dot02 = (v00x * v02x) + (v00y * v02y);
    mpfr::real<MPFR_BITS_PRECISION> dot11 = (v01x * v01x) + (v01y * v01y);
    mpfr::real<MPFR_BITS_PRECISION> dot12 = (v01x * v02x) + (v01y * v02y);
    // Compute barycentric coordinates
    mpfr::real<MPFR_BITS_PRECISION> invDenom = 1/( (dot00 * dot11) - (dot01 * dot01) );
    mpfr::real<MPFR_BITS_PRECISION> u = ( (dot11 * dot02) - (dot01 * dot12) ) * invDenom;
    mpfr::real<MPFR_BITS_PRECISION> v = ( (dot00 * dot12) - (dot01 * dot02) ) * invDenom;
    // Check if point is in triangle
    bool method1 = (u >= 0) && (v >= 0) && (u + v <= 1.0);
    //
    /* Crossing technique */
    //
    int crossings = 0;
    /* EDGE V1 - V2 */
    mpfr::real<MPFR_BITS_PRECISION> slope = 0;
    slope = (*v2y - *v1y)/(*v2x - *v1x);
    bool cond1 = (*v1x <= px) && (px < *v2x);
    bool cond2 = (*v2x <= px) && (px < *v1x);
    bool above = ( py < ( (slope * (px - *v1x)) + *v1y) );
    if( ( cond1 || cond2 ) && above )
        crossings++;
    /* EDGE V1 - V3 */
    slope = (*v3y - *v1y)/(*v3x - *v1x);
    cond1 = (*v1x <= px) && (px < *v3x);
    cond2 = (*v3x <= px) && (px < *v1x);
    above = ( py < ( (slope * (px - *v1x)) + *v1y) );
    if( ( cond1 || cond2 ) && above )
        crossings++;
    /* EDGE V2 - V3 */
    slope = (*v2y - *v3y)/(*v2x - *v3x);
    cond1 = (*v3x <= px) && (px < *v2x);
    cond2 = (*v2x <= px) && (px < *v3x);
    above = ( py < ( (slope * (px - *v3x)) + *v3y) );
    if( ( cond1 || cond2 ) && above )
        crossings++;
    bool method2 = (crossings % 2 != 0);
    //
    /* Orientation technique */
    //
    mpfr::real<MPFR_BITS_PRECISION> *orientation1 = Utils::orientation(px, py, this->v[TRIANGLE_VERTEX_ONE], this->v[TRIANGLE_VERTEX_TWO]);
    mpfr::real<MPFR_BITS_PRECISION> *orientation2 = Utils::orientation(px, py, this->v[TRIANGLE_VERTEX_TWO], this->v[TRIANGLE_VERTEX_THREE]);
    mpfr::real<MPFR_BITS_PRECISION> *orientation3 = Utils::orientation(px, py, this->v[TRIANGLE_VERTEX_THREE], this->v[TRIANGLE_VERTEX_ONE]);
    bool method3 = (*orientation1 >= 0) && (*orientation2 >= 0) && (*orientation3 >= 0);
    delete orientation1;
    delete orientation2;
    delete orientation3;
    //return (method1 && method2) or (method1 && method3) or (method2 && method3);
    return method1 or method2 or method3;
}

Adjacency *Triangle::getSmallerEdge()
{
    mpfr::real<MPFR_BITS_PRECISION> *v0v1 = Utils::distanceSQR(v[TRIANGLE_VERTEX_ONE],v[TRIANGLE_VERTEX_TWO]);
    mpfr::real<MPFR_BITS_PRECISION> *v0v2 = Utils::distanceSQR(v[TRIANGLE_VERTEX_ONE],v[TRIANGLE_VERTEX_THREE]);
    mpfr::real<MPFR_BITS_PRECISION> *v1v2 = Utils::distanceSQR(v[TRIANGLE_VERTEX_TWO],v[TRIANGLE_VERTEX_THREE]);
    Adjacency *edge;
    if( (*v0v1 <= *v0v2) && (*v0v1 <= *v1v2) )
        edge = v[TRIANGLE_VERTEX_ONE]->getAdjacency(v[TRIANGLE_VERTEX_TWO]);
    else if( (*v0v2 <= *v0v1) && (*v0v2 <= *v1v2) )
        edge = v[TRIANGLE_VERTEX_ONE]->getAdjacency(v[TRIANGLE_VERTEX_THREE]);
    else
        edge = v[TRIANGLE_VERTEX_TWO]->getAdjacency(v[TRIANGLE_VERTEX_THREE]);
    delete v0v1;
    delete v0v2;
    delete v1v2;
    return edge;    
}

void Triangle::updateRadiusEdge()
{
    mpfr::real<MPFR_BITS_PRECISION> *v0v1 = Utils::distanceSQR(v[TRIANGLE_VERTEX_ONE],v[TRIANGLE_VERTEX_TWO]);
    mpfr::real<MPFR_BITS_PRECISION> *v0v2 = Utils::distanceSQR(v[TRIANGLE_VERTEX_ONE],v[TRIANGLE_VERTEX_THREE]);
    mpfr::real<MPFR_BITS_PRECISION> *v1v2 = Utils::distanceSQR(v[TRIANGLE_VERTEX_TWO],v[TRIANGLE_VERTEX_THREE]);
    Vertex *v0, *v1;
    if( (*v0v1 <= *v0v2) && (*v0v1 <= *v1v2) )
    {
        v0 = v[TRIANGLE_VERTEX_ONE];
        v1 = v[TRIANGLE_VERTEX_TWO];
    }
    else if( (*v0v2 <= *v0v1) && (*v0v2 <= *v1v2) )
    {
        v0 = v[TRIANGLE_VERTEX_ONE];
        v1 = v[TRIANGLE_VERTEX_THREE];
    }
    else
    {
        v0 = v[TRIANGLE_VERTEX_TWO];
        v1 = v[TRIANGLE_VERTEX_THREE];
    }
    mpfr::real<MPFR_BITS_PRECISION> *length = Utils::distance(v0, v1);
    this->radiusEdgeRatio = sqrt(this->sqrRadius);
    this->radiusEdgeRatio = this->radiusEdgeRatio/(*length);
    delete length;
    delete v0v1;
    delete v0v2; 
    delete v1v2;
}

void Triangle::setVerticeToNull(int number)
{
    this->v[number] = NULL;
}

Vertex *Triangle::getNearestVertexInsideTriangle(mpfr::real<MPFR_BITS_PRECISION> &px , mpfr::real<MPFR_BITS_PRECISION> &py, Triangle *t) 
{
    mpfr::real<MPFR_BITS_PRECISION> *distance = NULL, *shortestDistanceSQR = NULL;
    Vertex *nearestVertex = t->v[TRIANGLE_VERTEX_ONE];
    shortestDistanceSQR = Utils::distanceSQR(px, py, t->v[TRIANGLE_VERTEX_ONE]); 
    distance = Utils::distanceSQR(px, py, t->v[TRIANGLE_VERTEX_TWO]);
    if (*shortestDistanceSQR > *distance) 
    {
        delete shortestDistanceSQR;
        shortestDistanceSQR = distance;
	distance = NULL;
        nearestVertex = t->v[TRIANGLE_VERTEX_TWO];
    } else 
    {
        delete distance;
	distance = NULL;
    }        
    distance = Utils::distanceSQR(px, py, t->v[TRIANGLE_VERTEX_THREE]);
    if (*shortestDistanceSQR > *distance) 
    {
        delete shortestDistanceSQR;
        shortestDistanceSQR = distance;
	distance = NULL;
        nearestVertex = t->v[TRIANGLE_VERTEX_THREE];
    } else 
    {
      delete distance;
      distance = NULL;
    }    
    delete shortestDistanceSQR;
    return nearestVertex;
}

bool Triangle::hasVertex(Vertex *v)
{
    if ((this->v[TRIANGLE_VERTEX_ONE] == v ) ||
        (this->v[TRIANGLE_VERTEX_TWO] == v ) || 
        (this->v[TRIANGLE_VERTEX_THREE] == v )) {
        return true;        
    }
    return false;
}

void Triangle::getNeighbors(list<Triangle*> &listTriangles) {
    
    Triangle *t;
    t = this->getNeighbor(TRIANGLE_VERTEX_ONE);
    if (t != NULL) 
        listTriangles.push_back(t);        
    t = this->getNeighbor(TRIANGLE_VERTEX_TWO);
    if (t != NULL) 
        listTriangles.push_back(t);        
    t = this->getNeighbor(TRIANGLE_VERTEX_THREE);
    if (t != NULL) 
        listTriangles.push_back(t);        
}

void Triangle::updateShapeRegularityQuality() {
  
    mpfr::real<MPFR_BITS_PRECISION> *l1 = NULL, *l2 = NULL, *l3 = NULL, t;
    // Counter Clockwise t < 0; Clockwise t > 0;
    Vertex *v1 = this->v[TRIANGLE_VERTEX_ONE];
    Vertex *v2 = this->v[TRIANGLE_VERTEX_TWO];
    Vertex *v3 = this->v[TRIANGLE_VERTEX_THREE];
    t = abs( ((v2->x - v1->x)*(v3->y - v1->y)) - ((v3->x - v1->x)*(v2->y - v1->y)) );
    l1 = Utils::distanceSQR(v3, v2);
    l2 = Utils::distanceSQR(v1, v3);
    l3 = Utils::distanceSQR(v1, v2);
    this->shapeRegularityQuality = (2 * sqrt(3) * t) / ( (*l1) + (*l2) + (*l3) ); 
}  