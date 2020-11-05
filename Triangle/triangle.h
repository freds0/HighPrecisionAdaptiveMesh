/***** Declaration of the class Triangle. *****/
/* A triangle has three pointes for its vertices and
   three pointes for its neighbors. */

#ifndef TRIANGLE_H_INCLUDED
#define TRIANGLE_H_INCLUDED

#include <cmath>
#include <cstdlib>
#include <iostream>
using namespace std;
#include "../Vertex/vertex.h"
class Triangle
{
public:
    //
    // Attributes
    //
    mpfr::real<MPFR_BITS_PRECISION> sqrRadius;       	    /**< Square of the circumradius. */
    mpfr::real<MPFR_BITS_PRECISION> shapeRegularityQuality; /**< Square of the circumradius. */
    mpfr::real<MPFR_BITS_PRECISION> cCenter[2];             /**< Coordinates of the circumcenter. */
    mpfr::real<MPFR_BITS_PRECISION> angles[3];              /**< angle[i] is the angle in the vertex v[i]*/
    mpfr::real<MPFR_BITS_PRECISION> radiusEdgeRatio;        /**< Radius-to-shortest edge ratio. */
    //
    // Methods
    //
    /**
     * Constructor. 
     * Initializes the triangle setting the vertices.
     * @param vertex TRIANGLE_VERTEX_ONE.
     * @param vertex TRIANGLE_VERTEX_TWO.
     * @param vertex TRIANGLE_VERTEX_THREE.
     */
    Triangle(Vertex *, Vertex *, Vertex *);
    /**
     * Set vertices of triangle.
     * @param vertex TRIANGLE_VERTEX_ONE.
     * @param vertex TRIANGLE_VERTEX_TWO.
     * @param vertex TRIANGLE_VERTEX_THREE.
     */
    void setVertices( Vertex *, Vertex *, Vertex * );
    /**
     * Sets a vertex with number equal to TRIANGLE_VERTEX_ONE, TRIANGLE_VERTEX_TWO or TRIANGLE_VERTEX_THREE, in other words v[number] = v.
     * @param v vertex to set
     * @param number number of vertice to set.
     */
    void setVertice( Vertex *v, int number);
    /**
     * Add a new neighbor, checking if the triangle has two vertices equal
       to this triangle.
     * @param triangle
     */
    void setNeighbor(Triangle *);
    /**
     * Set the vertice with number 'number' to null.
     * @param number of vertice.
     */
    void setVerticeToNull(int number);
    /**
     * Replace the neighbor "oldNeighbor" for "newNeighbor".
     * @param oldNeighbor
     * @param newNeighbor
     */
    void replaceNeighbor(Triangle *oldNeighbor, Triangle *newNeighbor);
    /**
     * Get the vertex with index 'number'. Returns a error if the number is other than 0, 1 or 2
     * @param number
     * @return the vertex or neighbor with index 'number' or error.
     */
    Vertex *getVertex(int number);
    /**
     * Get the neighbor with index 'number'. Returns a error if the number is other than 0, 1 or 2
     * @param number
     * @return the vertex or neighbor with index 'number' or error.
     */    
    Triangle *getNeighbor(int number);
    /**
     * Get the smaller edge of the triangle.
     * @return the smaller triangle's edge.
     */
    Adjacency *getSmallerEdge();
    /**
     * Get the number of vertex 'v'. If 'v' is a vertex of the triangle. Otherwise, returns -1.
     * @param v vertex
     * @return number of vertex or -1 (if vertex not found). 
     */
    int getNumber(Vertex *v);
    /**
     * Check if the point (px, py) is inside the triangle.
     * @param px x coordenate of vertex.
     * @param py y coordenate of vertex.
     * @return number of the vertex or -1 (if vertex not found).
     */
    bool isInside(mpfr::real<MPFR_BITS_PRECISION> &px, mpfr::real<MPFR_BITS_PRECISION> &py);
    /**
     * Get nearest vertex of a point which is inside the triangle.
     * @param px x coordenate of vertex.
     * @param py y coordenate of vertex.
     * @return u value.
     */
    Vertex *getNearestVertexInsideTriangle(mpfr::real<MPFR_BITS_PRECISION> &px, mpfr::real<MPFR_BITS_PRECISION> &py, Triangle *t);    
    /**
     * Checks if a triangle has a vertex.
     */
    bool hasVertex(Vertex *v);    
    /**
     * Get neighbors of triangle and put on triangles list
     * @param listTriangles triangles list to put neighbors
     */
    void getNeighbors(list<Triangle *> &listTriangles);
    /**
     *  Update circuncenter, angle and CircunradiusToShortestEdgeRatio;
     */
    void update();
    /**
     *  Update ShapeRegularityQuality;
     */
    void updateSRQ();    

private:
    //
    // Attributes
    //
    Vertex *v[3]; /**< Vertices of triangle */
    Triangle *neighbor[3]; /**< Neighbors => Neighbor t[i] is the neighbor opposite to the vertex v[i] */
    //
    // Methods
    //
    /**
     * Update the circumcenter coordinates.
     */
    void updateCcenter();
    /**
     * Update the angles of the triangle.
     */
    void updateAngle();
    /**
     * Update the radius-to-shortest edge ratio.
     */
    void updateRadiusEdge();    
    /**
     * Update the Shape Regularity Quality.
     */
    void updateShapeRegularityQuality();    

};

#endif // TRIANGLE_H_INCLUDED
