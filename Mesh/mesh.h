#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED
#include "../Vertex/vertex.h"
#include "../Triangle/triangle.h"
#include "../Adjacency/adjacency.h"
#include "../Utils/utils.h"
#include <string>
#include <list>
#include <queue>
#include <fstream> // file stream

typedef bool (functionAdj)(Adjacency *);
typedef Triangle *(functionGetTriangle)(Adjacency *);
typedef bool (functionTri)(Triangle *);
typedef bool (functionR)(Triangle *t);
typedef void (functionV)(Vertex *v);
typedef bool (*monitorFunction)(Mesh *m);
class Mesh
{
public:
    ////////////////////////////////////////////////////////////////////////////    
    // Attributes    
    ////////////////////////////////////////////////////////////////////////////
    Vertex *first;                 /**< First vertex of the list of vertices. */
    Vertex *last;                  /**<  Last vertex of the list of vertices. */    
    list<Triangle *> triangleList; /**<  List of all triangles in the triangulation. */
    list<Adjacency *> edges;       /**<  List of all edges in the triangulation. */
    list<Adjacency *> segments;    /**<  List of all segments. The segment may or may not be present in the triangulation. If the segment is in triangulation, then it is in the list of edges too. */
    int numberOfVertices;          /**<  Total of vertices in the triangulation. */
    int numberOfTriangles;         /**<  Total of triangles in the triangulation. */    
        ////////////////////////////////////////////////////////////////////////////
    // Methods    
    ////////////////////////////////////////////////////////////////////////////
    /**
     * Constructor.
     * Initializes the mesh. Set the variables: first, last, numberOfTriangles, numberOfVertices and vertexFunction with 0.
     */    
    Mesh();
    /**
     * Destructor.
     * Destroy the mesh. Deletes the adjacencies, the triangles and the vertices.
     */        
    ~Mesh();
    /**
     * Function that takes as a parameter the function responsible for setting the initial vertices (boundary conditions).
     * @param function name of function to set Vertex
     */
    void setVertexFunction(functionV *function);
    /**
     * Initialize creating a new mesh from the input PSLG in the file 'fileName'. 
     * @param filename name of the input file. TODO: create a model of file.     
     */
    void initialize(string fileName);
    /**
     * Refines the mesh using the criterion of lowest angle. Refines triangles that have one of a few larger than the value passed as a parameter
     * @param angle is the minimum angle desired.
     * @param refinementType 0 = Ruppert Refinement, otherwise = Off-center (Ungor).    
     */    
    int refine(mpfr::real<MPFR_BITS_PRECISION> &angle, int refinementType);
    /**
     * Refines the triangle by Ruppert's method.
     * @param t triangle to refine.
     */
    void refineTriangle(Triangle *t); 
    /**
     * Remove a triangle of mesh according to a function.
     * @param function removal criterion (in triangle to remove, out bool indicates sucess ou no)
     */
    void removeTriangles(functionR *function);
    /**
     * Applies the Ruppert's refinement  in mesh according to a function. 
     * @param function refinament criterion (in Adjacency to verify, out bool indicates if refinement must be done or no)
     */
    bool applyRefinement( functionAdj *adjCriterion, functionGetTriangle *getTriangle );
    /**
     * Applies the Ruppert's refinement  in mesh according to a function. 
     * @param fucntion refinement criterion.
     */    
    bool applyRefinement( functionTri *triCriterion );
    /**
     * Move de vertices of mesh by a monitor function    
     */
    bool movingMesh( monitorFunction monitorFunction);
    /**
     * Clone the mesh returning pointer to the new mesh
     *
     **/
    Mesh *cloneMesh();
    /**
     * For all edges of mesh, this method verify if the edges are a Delaunay edges. 
     * If is not a Delaunay edge, then the edge is fliped. 
     * Then, the method recursively repeats the procedure for the edges of the triangle that shared the edge with t. 
     * @return number of flips
     */ 
    int maintainsDelaunay();    
    /** 
     * For all triangles and all vertices of mesh, checks whether a vertex inside triangle.  Brute force method 
     * @return true if mesh is OK, false exists a vertex inside triangle
     */     
    bool verifyPointsInsideTriangles();
    /**
     * For all triangles checks the neighbors are setting correctly
     * @return true if neighbors are ok, false exists error.
     */     
    bool verifyNeighbors();
    /**
     * Like verifyPointsInsideTriangles() and verifyNeighbors() but only one method
     * @return true if mesh is OK, false are problem.
     */     
    bool bruteForceCheckMesh();
    /** 
     * For all triangles and vertices neighbors of neighbors, checks whether a vertex inside triangle.  Brute force method 
     * @return true if mesh is OK, false exists a vertex inside triangle
     */     
    bool smartCheckMesh();
    /**
     * Reorders the list of vertices according to the in-depth search
     * @param reverse reverse order
     * @param startingVertex use constant UTILS_CM_RANDOM to use random initial vertex; UTILS_CM_PSEUDOPERIPHERAL to use pseudoperipheral vertex (George and Liu algorithm); UTILS_CM_SMALLER_DEGREE to use vertex with smaller degree
     */     
    void cuthillMcKee(bool reverse, int startingVertex);

private:
    ////////////////////////////////////////////////////////////////////////////    
    // Attributes    
    ////////////////////////////////////////////////////////////////////////////    
    /* List of triangle candidates to be refined (It's a auxiliary list). */
    list<Triangle *> toRefineByAngleCriterion; /** Used in refine. */
    list<Triangle *> toRefineByAdjCriterion; /**<  Used in applyRefinement. */
    list<Adjacency *> encroachedS;
    list<Adjacency *> toFlip;   
    /* List of triangles candidates to be removed (Auxiliary list). */
    list<Triangle *> toRemove;    
    ////////////////////////////////////////////////////////////////////////////    
    // Methods    
    ////////////////////////////////////////////////////////////////////////////
    functionV *vertexFunction;
    /**
     * Lawsons's algorithm. Performs flip for those edges that are not Delaunay. Given a vertex v which is within the triangle t, this method splits t in three triangles.
     * @param v vertex v which is within the triangle t.
     * @param t triangle which will be divided.
     */
    void splitTriangle(Vertex *v, Triangle *t);
    /**
     * Given a vertex in the edge, this method split the edge into two new edges.
     * @param v vertex v wich is in the edge.
     * @param edge which will be divided.      
     */
    void splitEdge(Vertex *v, Adjacency *edge);
    /**
     * Reverses the edge 'edge', and returns the new edge.
     * @param edge edge which will be reverse.
     * @return newEdge new edge after split.
     */    
    Adjacency *swap(Adjacency *edge);
    /**
     * Splits all encroached segments of the mesh.
     */
    void splitEncroachedSegments();
    /**
     * Splits all segments which are encroached by the point (px, py).Returns true if at least one segment was splited. 
     * @param px x coordinate.
     * @param py y coordinate.
     * @return bool indicates fi the segment is encroached.
     */
    bool splitEncroachedSegments(mpfr::real<MPFR_BITS_PRECISION> px, mpfr::real<MPFR_BITS_PRECISION> py);
    /**
     * Verify if the segment 'seg is encroached. Verify segments opposite to the obtuse angles. 
     * @param seg segment to verify.
     * @return 'true' if the segment 'seg' is encroached.
     */    
    bool isEncroached(Adjacency *seg);
    /**
     * Returns 'true' if the segment 'seg' is encroached by the point (px, py).
     * @param px x coordinate of the point (px, py).
     * @param py y coordinate of the point (px, py).
     * @param seg segment to verify if is encroached.
     * @return true if the segment is encroached.
     */
    bool isEncroached(mpfr::real<MPFR_BITS_PRECISION> &px, mpfr::real<MPFR_BITS_PRECISION> &py, Adjacency *seg);
    /**
     * Gives a triangle t and an edge of t, this method verify if the edge is a Delaunay edge. 
     * If is not a Delaunay edge, then the edge is fliped. 
     * Then, the method recursively repeats the procedure for the edges of the triangle that shared the edge with t. 
     * @param t triangle which contain the edge.
     * @param edge edge of t to verify if is a Delaynaty edge. 
     * @return number of flips
     */
    int maintainsDelaunay(Triangle *t, Adjacency *edge);
    /** 
     * Insert a vertex in the circumcenter or off-center of the triangle 't'. 
     * @param t tirnaglo to insert a vertex.
     * @param offcenter if offCenter == 0 -> circuncenter; otherwise create vertex with points (offCenter[0], offCenter[1]), int he off-center. 
     */        
    void insertCircuncenterOff(Triangle *t, mpfr::real<MPFR_BITS_PRECISION> *offCenter);
    /**
     * Find the triangle tha contains the point (px, py), starting the search from the triangle 't'. 
     * @param t triangle to starting the search.
     * @param px coordinate x of the point.
     * @param py coordinate y of the point.
     * \see calculateOffCenter().
     */
    Triangle *findTriangle(Triangle *t, mpfr::real<MPFR_BITS_PRECISION> &px, mpfr::real<MPFR_BITS_PRECISION> &py);
    /**
     * Calculates the off-center of 't' and stores in the array 'offCenter'. b is the bound radius-edge ratio.
     * @param t triangle to calculate.
     * @param b bound radius-edge ratio.
     * @param offCenter array which stores the offCenter.
     */
    void calculateOffCenter(Triangle *t, mpfr::real<MPFR_BITS_PRECISION> &b, mpfr::real<MPFR_BITS_PRECISION> *offCenter);
    /** 
     * Returns a vertex object with coordinates (x,y). Updates the number of vertices, and the linked list. 
     * @param x x coordenate of vertex to create.
     * @param y y coordenate of vertex to create.
     */
    Vertex *createVertex(mpfr::real<MPFR_BITS_PRECISION> &x, mpfr::real<MPFR_BITS_PRECISION> &y);
    /**
     * Creates the triangulation of the bounding box, with two triangles. 
     */
    void createInitialTriangulation();
    /** 
     * Find the triangle that 'v' lies within.
     * @param v vertex to find.
     */
    Triangle *findTriangle( Vertex *v );
    /** 
     * Inserts vertex in the triangulation. 
     * First checks whether the point is located on an edge of the triangle. If true, divide the edge. Otherwise, divide the triangle.
     * @param v vertex to insert
     * @param t triangle to check. If t = 0, find the triangle which contain the vertex.
     */ 
    bool insertVertice(Vertex *v, Triangle *t);
    /** 
     * Creates (or find an existing segment) a edge between 'v0' and 'v1'. 't0' and 't1' must be the triangles that share the edge (can be null). 
     * @param v0 vertex 0 of edge to create or find.
     * @param v1 vertex 1 of edge to create of find.
     * @param t0 triangle that share the edge.
     * @param t1 triangle that share the edge.
     * @param seg 0 = is a edge, 1 = is a segment and is present in the triangulation, 2 = is a segment, but not in the triangulation; 
     */
    Adjacency *createAdjacency(Vertex *v0, Vertex *v1, Triangle *t0, Triangle *t1, int seg);
    /**
     * Deletes an adjacency if is not a segment of the PSLG.
     * @param edge to remove.
     * TODO: verificar se remove apenas se nao for um segmento do PSLG.
     */
    void removeEdge(Adjacency *edge);
    /**
     * Creates and return a triangle with vertex v0, v1 and v2.
     * @param v0 vertex 0 of triangle.
     * @param v1 vertex 1 of triangle.
     * @param v2 vertex 2 of triangle.
     */
    Triangle *createTriangle(Vertex *v0, Vertex *v1, Vertex *v2);
    /**
     * Deletes the triangules 't'. 
     * @param t triangle to delete.
     */
    void deleteTriangle(Triangle *t);
    /**
     * Deletes the vertex 'v'.
     * @param v vertex to delete.
     */
    void deleteVertex(Vertex *v);
    /** 
     * Replaces the triangle 'oldT' that share de edge 'v0v1' (or 'edge') by the triangle 'newT' in the pointes of the adjacency object. 
     * @param v0 vertex 0 of edge shared.
     * @param v1 vertex 1 of edge shared.
     * @param oldT triangle which will be replaced.
     * @param newT triangle which will replace oldT.
     */
    void replaceEdgeTriangle(Vertex *v0, Vertex *v1, Triangle *oldT, Triangle *newT);
    /**
     * Replaces the triangle 'oldT' that share de edge 'edge' by the triangle 'newT' in the pointes of the adjacency object. 
     * @param edge edge shared by oldT and newT.
     * @param oldT triangle to be removed.
     * @param newT triangle to be inserted.
     */
    void replaceEdgeTriangle(Adjacency *edge, Triangle *oldT, Triangle *newT);
    /**
     * Marks t0 and t1 as neighbors.
     * @param t0 triangle neighbor of t1.
     * @param t1 triangle neighbor of t0.
     */
    void setNeighbors(Triangle *t0, Triangle *t1);
    /**
     * Verify if a edge needs to flipped
     * @param edge edge to verify if is encroached
     * @return true need flip, false dont need flipped
     */
    bool needsToFlipped(Adjacency *edge);
};
#endif // MESH_H_INCLUDED