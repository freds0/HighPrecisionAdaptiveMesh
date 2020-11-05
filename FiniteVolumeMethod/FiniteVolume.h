#ifndef FINITEVOLUME_H_INCLUDED
#define FINITEVOLUME_H_INCLUDED

#include <list>
#include "../Triangle/triangle.h"
#include "../Vertex/vertex.h"
#include "../Mesh/mesh.h"

class FiniteVolume
{
public:  
    /**
     * Solve the mesh by Method Finite Volume.
     * @param firstVertex first vertice of mesh.
     */    
    static void solve(Vertex *firstVertex, mpfr::real<MPFR_BITS_PRECISION> &delta_t);

private:
    /**
     * Calculates the euclidian distance of cincuncenters of triangles adjacents which share the edge with vertices v1 and v2.
     * @param v1 vertice of edge shared.
     * @param v2 vertice of edge shared.
     * @return euclidian distance of cincuncenters of triangles.
     */
    static mpfr::real<MPFR_BITS_PRECISION> *distCircuncenterTrianglesShareInterface( Vertex *v1, Vertex *v2 );
    /**
     * Computer a border vertex
     * @param v1 vertice
     * @param v2 vertice
     */
    static void computeBorderV( Vertex *p, Vertex *v);
    /**
     * Computer a internal vertex
     * @param p
     * @param listV
     */
    static void computeInternalV( Vertex *p, Adjacency *listV);
    /**
     * Get area of a vertex and divide by delta_t
     * @param v vertex to calculates area of voronoi diagram
     * @param delta_t value of diference of time
     * @return area divided by delta_t
     */
    static mpfr::real<MPFR_BITS_PRECISION> *areaDivDeltaT(Vertex *v, mpfr::real<MPFR_BITS_PRECISION> &delta_t);

};
#endif // FINITEVOLUME_H_INCLUDED
