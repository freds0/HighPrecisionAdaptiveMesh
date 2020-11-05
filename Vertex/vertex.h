/****** Declaration of the class Vertex ******/
/* The ESSENTIAL attributes of a vertex is only the coordinates x and y. */

#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED
#include "../Utils/constants.h"
#include <list>
#include "real.hpp"
using namespace std;
class Adjacency; // A forward declaration is therefore needed to avoid circular dependencies.
class Triangle;  // A forward declaration is therefore needed to avoid circular dependencies.
class Vertex
{
public:
    //
    // Attributes
    //
    list<Adjacency *> adjList;   /**< List to the adjacent vertices */
    list<Triangle *> incList;    /**< List to the incident triangles */
    Vertex *next;                /**< Pointer used to create a list of vertices */
    int label;                   /**< Vertex label. */
    int levelStructure;          /**< Used in Cuthill–McKee algorithm */
    int eccentricity;            /**< Used in Cuthill–McKee algorithm */
    bool visited;                /**< Used in Cuthill–McKee algorithm */
    bool isBorder;               /**< 0 or false vertex is internal, otherwise (true) vertex is border*/
    /** Data members utilized in the Finite Volume Method and Conjugate Gradient Method **/
    mpfr::real<MPFR_BITS_PRECISION> u, previews_u, b, coefficient, r, d, Au, Ad;    
    /** Vertex coordinates **/
    mpfr::real<MPFR_BITS_PRECISION> x, y;
    //
    // Methods
    //
    /**
     * Constructor. Initialize attributes x, y and isBorder.
     * @param x coordinate.
     * @param y coordinate.
     * @param isBorder isBorder.
     */
    Vertex(mpfr::real<MPFR_BITS_PRECISION> &x, mpfr::real<MPFR_BITS_PRECISION> &y, int isBorder); 
    /**
     * Changes the coordinates x and y.
     * @param x coordinate
     * @param y coordinate
     */
    void setXY(mpfr::real<MPFR_BITS_PRECISION> &x, mpfr::real<MPFR_BITS_PRECISION> &y); 
    /**
     * Get the Adjacency object with vertex v. If there is no
       such adjacency, then returns null.
     * @param v
     * @return the Adjacency object with vertex .
     */
    Adjacency *getAdjacency(Vertex *v);
    /**
     * Get any Adjacency object with vertex v.       
     * @return any Adjacency object.
     */    
    Adjacency *getAdjacency();

private:
    //
    // Attributes
    //
};
#endif // VERTEX_H_INCLUDED
