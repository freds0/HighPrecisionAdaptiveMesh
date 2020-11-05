#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED
#include <list>
#include "../Vertex/vertex.h"
#include <fstream> // file stream
using namespace std;
class Mesh;
class Utils
{
public:
    /**
     * Checks if a list of triangles contains a given triangle.
     * @param l list of triangles.
     * @param t triangle to search.
     * @return 
     */
    static bool contains(list<Triangle *> &l, Triangle *t);
    /**
     * Checks if a list of vertices contains a given vertice.
     * @param l list of vertices.
     * @param v vertice to search.
     * @return 
     */
    static bool contains(list<Vertex *> &l, Vertex *v);
    /**
     * Checks if a list of adjacencies contains a given adjacency
     * @param l
     * @param adj
     * @return 
     */
    static bool contains(list<Adjacency *> &l, Adjacency *adj);
    /**
     * Calculate the orientation of vertices of a triangle.
     * @param v1
     * @param v2
     * @param v3
     * @return 0 if counter-clockwise. <0 if clockwise. 0 if collinear. 
     */    
    static mpfr::real<MPFR_BITS_PRECISION> *orientation(Vertex *v1, Vertex *v2, Vertex *v3);
    /**
     * Calculate the orientation of vertices of a triangle.
     * @param ax
     * @param ay
     * @param v2
     * @param v3
     * @return >0 if counter-clockwise. <0 if clockwise. 0 if collinear. 
     */
    static mpfr::real<MPFR_BITS_PRECISION> *orientation(mpfr::real<MPFR_BITS_PRECISION> &ax, mpfr::real<MPFR_BITS_PRECISION> &ay, Vertex *v2, Vertex *v3);
    /**
     * Calculate the gradient between v1 and v2.
     * @param v1
     * @param v2
     * @return gradient between v1 and v2.
     */
    static mpfr::real<MPFR_BITS_PRECISION> *gradient(Vertex *v1, Vertex *v2);
    /**
     * Calculate the distance between v1 and v2. 
     * @param v1
     * @param v2
     * @return 
     */
    static mpfr::real<MPFR_BITS_PRECISION> *distance(Vertex *v1, Vertex *v2);    
    /**
     * Calculate the distance between the point (px, py) and v.
     * @param px
     * @param py
     * @param v
     * @return distance between the point.
     */
    
    static mpfr::real<MPFR_BITS_PRECISION> *distance(mpfr::real<MPFR_BITS_PRECISION> &px, mpfr::real<MPFR_BITS_PRECISION> &py, Vertex *v);
    /**
     * Calculate the distance between the point (p1x, p1y) and the point (p2x, p2y).
     * @param p1x
     * @param p1y
     * @param p2x
     * @param p2y
     * @return distance between two points.
     */
    static mpfr::real<MPFR_BITS_PRECISION> *distance(mpfr::real<MPFR_BITS_PRECISION> &p1x, mpfr::real<MPFR_BITS_PRECISION> &p1y, mpfr::real<MPFR_BITS_PRECISION> &p2x, mpfr::real<MPFR_BITS_PRECISION> &p2y);
    /**
     * Calculate the square of the distance between v1 and v2.
     * @param v1
     * @param v2
     * @return square of the distance.
     */
    static mpfr::real<MPFR_BITS_PRECISION> *distanceSQR(Vertex *v1, Vertex *v2);
    /**
     * Calculate the square of the distance between the point (px, py) and v.
     * @param px
     * @param py
     * @param v
     * @return square of the distance.
     */
    static mpfr::real<MPFR_BITS_PRECISION> *distanceSQR(mpfr::real<MPFR_BITS_PRECISION> &px, mpfr::real<MPFR_BITS_PRECISION> &py, Vertex *v);
    /**
     * Calculate the square of the distance between the point (p1x, p1y) and the point (p2x, p2y).
     * @param p1x
     * @param p1y
     * @param p2x
     * @param p2y
     * @return square of the distance.
     */
    static mpfr::real<MPFR_BITS_PRECISION> *distanceSQR(mpfr::real<MPFR_BITS_PRECISION> &p1x, mpfr::real<MPFR_BITS_PRECISION> &p1y, mpfr::real<MPFR_BITS_PRECISION> &p2x, mpfr::real<MPFR_BITS_PRECISION> &p2y);
    /**
     * Checks if the point (px,py) is inside (not on) the diametral circle of
       the edge with endpoints v1 and v2.
     * @param v1
     * @param v2
     * @param px
     * @param py
     * @return true if the point (px,py) is inside (not on) the diametral circle of
       the edge with endpoints v1 and v2; returns false, otherwise.
     */
    static bool inDiametralCircle(Vertex *v1, Vertex *v2, mpfr::real<MPFR_BITS_PRECISION> &px, mpfr::real<MPFR_BITS_PRECISION> &py);
    /**
     * Get the value of the smaller and bigtger gradient in the list of triangles.
     * @param l list of adjacencies.
     * @return gradient_max - gradient_min
     */    
    static mpfr::real<MPFR_BITS_PRECISION> *getDiferenceGradient(list<Adjacency *> &l);    
    /**
     * Get the value of the smaller gradient in the list of triangles.
     * @param l list of triangles.
     * @return lower value of gradient on mesh
     */
    static mpfr::real<MPFR_BITS_PRECISION> *getSmallestGradient(list<Triangle *> &l);
    /**
     * Get the value of the smaller gradient in the list of edges.
     * @param l list of adjacencies.
     * @return lower value of gradient on mesh
     */    
    static mpfr::real<MPFR_BITS_PRECISION> *getSmallestGradient(list<Adjacency *> &l);
    /**
     * Get the value of the bigger gradient in the list of triangles.
     * @param l list of triangles.
     * @return higher value of gradient on mesh
     */
    static mpfr::real<MPFR_BITS_PRECISION> *getBiggerGradient(list<Triangle *> &l);    
    /**
     * Get the value of the bigger gradient in the list of edges.
     * @param l list of adjacencies.
     * @return higher value of gradient on mesh
     */    
    static mpfr::real<MPFR_BITS_PRECISION> *getBiggerGradient(list<Adjacency *> &l);    
    /**
     * Get the value of the media of gradients in the list of edges.
     * @param l list of adjacencies.
     * @return media value of gradient on mesh
     */    
    static mpfr::real<MPFR_BITS_PRECISION> *getMediaGradient(list<Adjacency *> &l);        
    /**
     * Get the value of the bigger distance in the list of triangles.
     * @param l list of triangles.
     * @return higher value of distance of adjacency on mesh
     */
    static mpfr::real<MPFR_BITS_PRECISION> *getBiggerDistance(list<Triangle *> &l);   
    /**
     * Print the value of the higher, lower and media gradient.
     * @param l list of adjacencies
     * @return array of higher, lower and media values of gradientes, like r[0] = higher, r[1] = lower, r[2] = media
     */    
    static float *getGradients(list<Adjacency *> &l);
    /**
     * Print the matrix of mesh with the coeficients.
     * @param mesh
     */       
    static void printMatrixAB(Mesh &mesh, string outputA, string outputB, int outputPrecision);
    /**
     * Print linear system gerated by finite volume method in human format
     * @param firstVertex first vertex of list of vertices
     * @param diferenceTime delta_t
     * @param s output file
     */     
    static void printLinearSystem(Vertex *firstVertex, mpfr::real<MPFR_BITS_PRECISION> *diferenceTime, string s);
    /**
     * Print to file vertices values
     * @param v first vertex of list of vertices
     * @param s output file
     */         
    static void printValuesVertices(Vertex *v, string s);
    /**
     * Print to file adjacencies values
     * @param v adjacencies list
     * @param s output file
     */           
    static void printValuesAdjacencies(list<Adjacency *> &l, string s);    
    /**
     * Print to file triangles values
     * @param v triangles list
     * @param s output file
     */        
    static void printValuesTriangles(list<Triangle *> &l, string s);    
    /**
     * Print to file moviment vertices values
     * @param v first vertex of list of vertices
     * @param newX new value of X of vertex v
     * @param newY new value of Y of vertex v
     * @param s output file
     */    
    static void printVertexMovement(Vertex *v, mpfr::real<MPFR_BITS_PRECISION> &newX, mpfr::real<MPFR_BITS_PRECISION> &newY, string s);
     /**
     * Verify if a file exist
     * @param filename name of file to verify
     */      
    static bool existingFile(string fileName);
    /**
     * Calculate area of polygon (voronoi diagram) around vertex v     
     * @param v vertex to calculate area of voronoi diagram
     * @return area of voronoi diagram
     */               
    static mpfr::real<MPFR_BITS_PRECISION> *areaPolygon(Vertex *v);   
    /**
     * Get incident triangles of Vertex v, like to incList, but triangles are ordened in clockwise or Counter Clockwise
     * @param listTriangles triangles list to put triangles incidents
     * @param v vertex to find incident triangles
     */               
    static void getTrianglesWithVertex(list<Triangle *> &listTriangles, Vertex *v);     
    /**
     * Calculates area of vertices list by shoelace method
     * @param listVertices vertices list form a polygon to calculate area
     * @return area of polygon formed by vertices on listVertices
     */                 
    static mpfr::real<MPFR_BITS_PRECISION> *areaVertices(list<mpfr::real<MPFR_BITS_PRECISION> *> &listVertices);
    /**
     * Get cincuncenters of triangles on listTriangles and put on listVertices
     * @param listVertices vertices list to put circuncenters of triangles list
     * @param listTriangles triangles list to find circuncenters
     */                
    static void getCcentersTriangles(list<mpfr::real<MPFR_BITS_PRECISION> *> &listVertices, list<Triangle *> &listTriangles);
    /**
     * Get higher polygon (voronoi diagram) area found on list of vertices
     * @param v first vertex of list of vertices
     * @return higher area of polygon (voronoi diagram)
     */   
    static mpfr::real<MPFR_BITS_PRECISION> *getBiggerAreaPolygon(Vertex *v);
    /**
     * Get lower polygon (voronoi diagram) area found on list of vertices
     * @param v first vertex of list of vertices
     * @return lower area of polygon (voronoi diagram)
     */ 
    static mpfr::real<MPFR_BITS_PRECISION> *getSmallestAreaPolygon(Vertex *v);
    /**
     * Calculates the euclidian distance of cincuncenters of triangles adjacents which share the edge with vertices v1 and v2.
     * @param v1 vertice of edge shared.
     * @param v2 vertice of edge shared.
     * @return euclidian distance of cincuncenters of triangles.
     */   
    static mpfr::real<MPFR_BITS_PRECISION> *distanceCircuncenterTrianglesShareInterface( Vertex *v1, Vertex *v2 );    
    /**
     * Solve mesh of laplace equation by Method Finite Volume .
     * @param firstVertex first vertice of mesh.
     */      
    static void finiteVolumeLaplaceEquation(Vertex *firstVertex);
    /**
     * Compare degree of vertex, used on cuthillMcKee
     * @param first first vertice to compare.
     * @param second second vertice to compare.
     * @return true if first vertex degree is bigger then secont vertex degree
     */      
    static bool compare(Vertex *first, Vertex *second);
    /**
     * Reordenates the vertices list to pseudoperipherical vertex be the first
     * @param mesh mesh to find pseudoperipherial vertex
     */     
    static void findPseudoPeripheral(Mesh &mesh);
    /**
     * Get maximum value of CircunradiusToShortestEdgeRatio in Mesh
     * @param mesh mesh to find max value of circunradiusToShortestEdgeRatio
     */     
    static mpfr::real<MPFR_BITS_PRECISION> *circunradiusToShortestEdgeRatioMesh(Mesh &m);
    /**
     * Get minimal value of ShapeRegularityQuality in Mesh
     * @param mesh mesh to find minimal value of ShapeRegularityQuality
     * @return minimal value of ShapeRegularityQuality in Mesh
     */      
    static mpfr::real<MPFR_BITS_PRECISION> *shapeRegularityQualityMesh(Mesh &m);
    /**
     * Get ShapeRegularityQuality of triangle
     * @param triangle to get ShapeRegularityQuality
     * @return value of ShapeRegularityQuality of triangle
     */        
    static mpfr::real<MPFR_BITS_PRECISION> *shapeRegularityQuality(Triangle *triangle);
    /**
     * Print string to file and display
     * @param stringToPrint string to print to output file
     * @param outputFileName file name to print string
     * @param print_info_messages true print string to display 
     */       
    static void printString(string stringToPrint, string outputFileName, bool print_info_messages);
    /**
     * Get array with maximum value of CER, mininal value of SRQ, percentage of triangles with CER bigger then maximum, percentage of triangles with SRQ lower then minimal
     * @param toleranceCER maximum tolerance value of CER
     * @param toleranceSRQ minimal tolerance value of SRQ
     * @return array r with r[0] = maximum value of CER, r[1] = mininal value of SRQ, r[2] = percentage of triangles with CER bigger then maximum tolerance, r[3] = percentage of triangles with SRQ lower then minimal tolerance
     */       
    static float *getInformationQualityMesh(Mesh *m,  mpfr::real<MPFR_BITS_PRECISION> &toleranceCER, mpfr::real<MPFR_BITS_PRECISION> &toleranceSRQ);
};

#endif // UTILS_H_INCLUDED