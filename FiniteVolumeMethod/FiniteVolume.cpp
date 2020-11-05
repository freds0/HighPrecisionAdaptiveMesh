#include <cmath>
#include "FiniteVolume.h"
#include "../Utils/utils.h"

#include <sstream>
#include <vector>
void FiniteVolume::solve(Vertex *firstVertex, mpfr::real<MPFR_BITS_PRECISION> &delta_t)
{                          
    mpfr::real<MPFR_BITS_PRECISION> *k = NULL;

    Vertex *vertexIterator = firstVertex;    
    while( vertexIterator != 0 )
    {       
        if(vertexIterator->isBorder == false) /* is not at the border */
        {
            k = areaDivDeltaT(vertexIterator, delta_t);                
            vertexIterator->coefficient   = *k; // Coeficient of U_i                       
            vertexIterator->b             = ( (*k) * vertexIterator->previews_u); // Independent term                           
            delete k;
            // For all vertices adjacents to Vertex nodeV
            for(list<Adjacency *>::iterator it = vertexIterator->adjList.begin(); it != vertexIterator->adjList.end(); it++)
            {                
                (*it)->coefficient = 0;               
                Vertex *adjVertex  = (*it)->getVertex(ADJ_VERTEX_ONE);
                if(adjVertex == vertexIterator)
                    adjVertex = (*it)->getVertex(ADJ_VERTEX_TWO);
                if( adjVertex->isBorder ) // If the adjacent vertex is a border vertex
                {
                    computeBorderV(vertexIterator, adjVertex);
                }
                else // If the adjacent vertex is internal
                {
                    computeInternalV( vertexIterator, (*it));
                }
            }                        
        }
        vertexIterator = vertexIterator->next;
    }
}

void FiniteVolume::computeBorderV( Vertex *p, Vertex *v)
{
    // Distance of circuncenter of triangles which shared edge (p, v) / Distance of p to v
    mpfr::real<MPFR_BITS_PRECISION> *distance = NULL, *distanceInterfaceDivDistanceVertices = NULL;
    distance = Utils::distance(p, v);
    distanceInterfaceDivDistanceVertices = distCircuncenterTrianglesShareInterface(p, v);     
    *distanceInterfaceDivDistanceVertices = (*distanceInterfaceDivDistanceVertices) / (*distance);
    delete distance;
    // Updates vertex p
    p->coefficient += *distanceInterfaceDivDistanceVertices;
    p->b           += (v->u * (*distanceInterfaceDivDistanceVertices));    
    delete distanceInterfaceDivDistanceVertices;
}

void FiniteVolume::computeInternalV( Vertex *p, Adjacency *listV)
{
    mpfr::real<MPFR_BITS_PRECISION> *distance = NULL, *distanceInterfaceDivDistanceVertices = NULL;
    Vertex *adjVertex = listV->getVertex(ADJ_VERTEX_ONE);  
    if(adjVertex == p) // If true, get next vertice of edge
        adjVertex = listV->getVertex(ADJ_VERTEX_TWO);
    // Distance of circuncenter of triangles which shared edge (p, adjVertex) / Distance of p to adjVertex
    distance = Utils::distance(p, adjVertex);
    distanceInterfaceDivDistanceVertices = distCircuncenterTrianglesShareInterface(p, adjVertex);   
    *distanceInterfaceDivDistanceVertices = (*distanceInterfaceDivDistanceVertices) / (*distance);
    delete distance;
    // Updates vertex p
    p->coefficient     += *distanceInterfaceDivDistanceVertices;
    // Updates adjacency listV
    listV->coefficient -= *distanceInterfaceDivDistanceVertices;    
    delete distanceInterfaceDivDistanceVertices;   
}

mpfr::real<MPFR_BITS_PRECISION> *FiniteVolume::distCircuncenterTrianglesShareInterface( Vertex *v1, Vertex *v2 )
{
    mpfr::real<MPFR_BITS_PRECISION> *distanceCircuncenterTrianglesShareInterface = new mpfr::real<MPFR_BITS_PRECISION> (0);
    Adjacency *adj = v1->getAdjacency(v2); // Get the Adjacency with vertex v1 and v2. 
    Triangle *c1 = adj->getTriangle(ADJ_TRIANGLE_ONE);
    Triangle *c2 = adj->getTriangle(ADJ_TRIANGLE_TWO);  
    // Euclidian distance of circuncenters of c1 e c2 => dist = [  (X_c1 - X_c2)^2 + (Y_c1 + Y_c2)^2  ]^1/2
    *distanceCircuncenterTrianglesShareInterface = sqrt(   ( c1->cCenter[COORDINATE_X] - c2->cCenter[COORDINATE_X] )*( c1->cCenter[COORDINATE_X] - c2->cCenter[COORDINATE_X] )
                                                         + ( c1->cCenter[COORDINATE_Y] - c2->cCenter[COORDINATE_Y] )*( c1->cCenter[COORDINATE_Y] - c2->cCenter[COORDINATE_Y] ) );   
    return distanceCircuncenterTrianglesShareInterface;
}

mpfr::real<MPFR_BITS_PRECISION> *FiniteVolume::areaDivDeltaT(Vertex *v, mpfr::real<MPFR_BITS_PRECISION> &delta_t)
{    
    mpfr::real<MPFR_BITS_PRECISION> *area = Utils::areaPolygon(v);      
    *area = *area / delta_t;
    return area;    
}