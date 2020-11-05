/****** Declaration of the class Adjacency. Represents a edge. ******/

#ifndef ADJACENCY_H_INCLUDED
#define ADJACENCY_H_INCLUDED

#include "../Vertex/vertex.h"

#include <cstdlib>
#include <iostream>
#include <list>
using namespace std;
class Adjacency
{
public:
    //
    // Attributes
    //
    int type; /**< ADJ_SEG_ONLY_EDGE (0) = is a edge;
                   ADJ_SEG_IN_TRI    (1) = is a segment and is present in the triangulation;
                   ADJ_SEG_NOT_TRI   (2) = is a segment, but not in the triangulation; */
    
    // Data members utilized in the Finite Volume Method and in Conjugate Gradiente Method.
    mpfr::real<MPFR_BITS_PRECISION> coefficient; /**coeficient of conjugate gradient **/
    //
    // Methods
    //    
    /**
     * Constructor. Initialize attribute 'seg' with 0. The adjacency lists of v1 and v2 are updated.
     * @param v1 endpoints of the edge.
     * @param v2 endpoints of the edge.
     */
    Adjacency( Vertex *v1, Vertex *v2);     
    /**
     * Destructor. Remove the adjaceny lists of v1 and v2.
     */
    ~Adjacency();
    /**
     * Get the vertex with number 'number' ('number' must be ADJ_VERTEX_ONE or ADJ_VERTEX_TWO).
     * @param number.
     * @return Returns the vertex number (ADJ_VERTEX_ONE or ADJ_VERTEX_TWO).
     */
    Vertex *getVertex(int number);          
    /**
     * Adds the triangle only if some pointer (attribute) is null.
     * @param triangle
     */
    void addTriangle(Triangle *triangle);   
    /**
     * Remove a triangle changing to null the pointer for this triangle.
     * @param triangle to change the pointer to null.
     */
    void removeTriangle(Triangle *triangle); 
    /**
     * Get the triangle with number 'number'.
     * @param number
     * @return the triangle with number 'number'.
     */
    Triangle *getTriangle(int number);      

private:
    //
    // Attributes
    //
    Vertex   *v[2]; /**< Endpoints of the edge. */
    Triangle *t[2]; /**< Pointers to the triangles that share the edge. */

};
#endif // ADJACENCY_H_INCLUDED

