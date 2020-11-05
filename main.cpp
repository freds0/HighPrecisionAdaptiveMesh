/**
 *  \brief     Main file of Project of Adaptive Mesh.
 *  \details   This class is used to demonstrate the refinement of a adaptive mesh.
 *  \author    Frederico S. Oliveira
 *  \author    Sanderson L. Gonzaga de Oliveira
 *  \author    Talles Henrique de Oliveira
 *  \version   2.0
 *  \date      2012-2013-2014
 *  \pre       First initialize the system.
 *  \bug       Not all memory is freed when deleting an object of this class.
 *  \warning   Improper use can crash your application
 *  \copyright GNU Public License.
 */
#include "OpenGL/openGL.h"
#include "Mesh/mesh.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <cmath>
#include <map>
#include <iomanip>      // std::setw
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include <string>
#include <pthread.h>
#include "real.hpp"
#include "Utils/constants.h"
#include "Adjacency/adjacency.h"
#include "FiniteVolumeMethod/FiniteVolume.h"
#include "ConjugateGradient/ConjugateGradient.h"
//
// Auxiliar data structures
//
/** 
 * Data structure information about mesh quality
 */ 
class InfoQualityMesh
{
public:
    int temporalIteration;
    int numberOfVertices;
    float rho_max;
    float upsilon_min;
    float percentageRhoBiggerRhoAlpha;
    float percentageUpsilonLowerEta;
    InfoQualityMesh(int temporalIteration, int numberOfVertices, float rho_max, float upsilon_min, float percentageRhoBiggerRhoAlpha, float percentageUpsilonLowerEta) 
    {
        this->temporalIteration 	  = temporalIteration;
	this->numberOfVertices		  = numberOfVertices;
        this->rho_max           	  = rho_max;       
	this->upsilon_min           	  = upsilon_min;       
	this->percentageRhoBiggerRhoAlpha = percentageRhoBiggerRhoAlpha;       
	this->percentageUpsilonLowerEta   = percentageUpsilonLowerEta;            
    }
};
/** 
 * Data structure information about mesh gradients
 */
class InfoGradientMesh 
{
public:
    int temporalIteration;
    int numberOfVertices;
    float gradientMax;
    float gradientMin;
    float gradientMedia;
    InfoGradientMesh(int i, int numberOfVertices, float gradientMax, float gradientMin, float gradientMedia) 
    {
        this->temporalIteration = i;
	this->numberOfVertices	= numberOfVertices;
        this->gradientMax       = gradientMax;       
	this->gradientMin       = gradientMin;       
	this->gradientMedia     = gradientMedia;       
    }
};
/** 
 * Data structure information about clocks of execution
 */
class InfoClock 
{
public:
    int numberOfVertices;
    clock_t c;
    InfoClock(int n, clock_t cl) 
    {
        this->numberOfVertices = n;
        this->c = cl;       
    }
};
/** 
 * Data structure information about moved vertices
 */
class InfoMovingVertex
{ 
    public:
        mpfr::real<MPFR_BITS_PRECISION> *coordinates;
        Vertex * v;
        ~InfoMovingVertex() 
        {
            delete[] coordinates;
            v = NULL;
        }
};
/** 
 * Data structure used as a function parameter of advanceTimeAndMovingMesh()
 */
class ThreadParameter
{
public:
    Mesh *mesh;
    int monitorFunctionId;
    int threadId;
    string outputFile;
    string outputDirectory;
    ThreadParameter(Mesh *m, int monitorFunctionId, int threadId, string outputFile, string outputDirectory)
    {
      this->mesh              = NULL;
      if (m != NULL)
	  this->mesh          = m;
      this->monitorFunctionId = monitorFunctionId;
      this->threadId          = threadId;
      this->outputFile        = outputFile;
      this->outputDirectory   = outputDirectory;
    }
};
//
// Variables constants of configuration. Can be altered by configuration file ./input/configuration.cfg
//
static int 
    CONFIG_TIME_LIMIT                            = 10,    
    CONFIG_MIN_VERTICES                          = 1000, // \xi
    /**<  CONFIG_MONITOR_FUNCTION
	* 0 = MONITOR_FUNCTION_LAMBDA, 
	  1 = MONITOR_FUNCTION_UPSILON, 
	  2 = MONITOR_FUNCTION_XI, 
	  3 = MONITOR_FUNCTION_GAMMA
	  4 = MONITOR_FUNCTION_PSI
    */        
    CONFIG_MONITOR_FUNCTION_THREAD_A             = 0,
    CONFIG_MONITOR_FUNCTION_THREAD_B		 = 1,    
    CONFIG_MONITOR_FUNCTION_THREAD_C		 = 2,
    CONFIG_MONITOR_FUNCTION_THREAD_D		 = 3,
    CONFIG_MONITOR_FUNCTION_THREAD_E		 = 4,
    CONFIG_NUM_THREADS				 = 5;
static float
    CONFIG_TIME_VARIATION                        = 0.1, // \Delta_t
    CONFIG_MONITOR_FUNCTION_BETA                 = 0.1,
    CONFIG_MONITOR_FUNCTION_MU                   = 0.1,
    CONFIG_LAPLACIAN_SMOOTHING_KAPA		 = 0.1,
    CONFIG_MOVING_MESH_ETA                       = 0.5,
    CONFIG_MESH_MINIMUM_ANGLE_ALPHA              = 30,    
    // Boundary conditions        
    CONFIG_DOMAIN_DIMENSION                      = 50,
    CONFIG_BNDR_CONDITION_BOTTOM_CORNERS         = 10,
    CONFIG_BNDR_CONDITION_TOP_CORNERS            = 10,
    CONFIG_BNDR_CONDITION_TOP                    = 10,         
    CONFIG_BNDR_CONDITION_BOTTOM                 = 10,
    CONFIG_BNDR_CONDITION_SIDES                  = 0,
    CONFIG_BNDR_CONDITION_IN_INITIAL_VALUE       = 0; 
static long double
    CONFIG_CG_PRESICION_EPSILON                  = 1e-10,
    CONFIG_PERC_INCREASE_GRADIENT_THETA          = 1e-10;
static bool
    CONFIG_EXEC_FLIP_OFF_CENTER                  = false, /**<  false to execute off_center (Ungor), true to execute flip (Lawson) */
    CONFIG_EXEC_CUTHILL_MCKEE_ALG                = true,
    CONFIG_CUTHILL_MCKEE_REVERSE                 = true,  /**<  true to reverse cuthill_mckee, false to cuthill_mckee */
    CONFIG_DISPLAY_MESH                          = true,      
    CONFIG_SHOW_TRIANGLE                         = true,  /**< Show triangles in mesh display */
    CONFIG_SHOW_CIRCLE                           = true,  /**< Show circuncicles in mesh display */
    CONFIG_SHOW_VORONOI_DIAGRAM                  = true,  /**< Show voronoi diagram in mesh display */
    CONFIG_SHOW_VERTEX_LABEL                     = true,  /**< Show vertex label in mesh display */
    CONFIG_PRINT_VERTEX_VALUE                    = true,  /**< Print to CONFIG_OUTPUT_FILE_VERTEX_VALUE vertices informations */
    CONFIG_PRINT_TRIANGLE_VALUE                  = true,  /**< Print to CONFIG_OUTPUT_FILE_TRIANGLE_VALUE triangles informations */  
    CONFIG_PRINT_ADJACENCY_VALUE		 = true,  /**< Print to CONFIG_OUTPUT_FILE_ADJACENCY_VALUE adjacencies informations */  
    CONFIG_PRINT_CLOCKS_VALUES			 = true,  /**< Print to CONFIG_OUTPUT_FILE_CLOCK_* execution clocks */  
    CONFIG_PRINT_VERTEX_MOVIMENT                 = true,  /**< Print to CONFIG_OUTPUT_FILE_VERTEX_MOVIMENT movement information of vertices */  
    CONFIG_PRINT_MATRIX_A_B                      = true,  /**< TODO Funcao precisa da biblioteca eigen */
    CONFIG_PRINT_INFO_MESSAGES                   = true,  /**< Information messages will print during execution */ 
    CONFIG_EXEC_THREAD_A        		 = true,  /**< Run Thread A */
    CONFIG_EXEC_THREAD_B			 = true,  /**< Run Thread B */  
    CONFIG_EXEC_THREAD_C			 = true,  /**< Run Thread C */
    CONFIG_EXEC_THREAD_D			 = true,  /**< Run Thread D */
    CONFIG_EXEC_THREAD_E 			 = true;  /**< Run Thread E */   
static string
    CONFIG_MAIN_FILE                             = /*CONFIG_INPUT_DIRECTORY << */ "./input/configuration.cfg",
    CONFIG_OUTPUT_FILE_VERTEX_VALUE              = "VerticesValues.csv",
    CONFIG_OUTPUT_FILE_TRIANGLE_VALUE            = "TrianglesValues.csv",
    CONFIG_OUTPUT_FILE_ADJACENCY_VALUE           = "AdjacenciesValues.csv",
    CONFIG_OUTPUT_FILE_VERTEX_MOVIMENT           = "VerticesMoviment.csv",
    CONFIG_OUTPUT_FILE_QUALITY_MESH		 = "Quality.csv",
    CONFIG_OUTPUT_FILE_GRADIENT_MESH		 = "Gradients.csv",
    CONFIG_OUTPUT_FILE_MATRIX_A                  = "MatrixA.txt",
    CONFIG_OUTPUT_FILE_MATRIX_B                  = "MatrixB.txt",
    CONFIG_OUTPUT_FILE_CLOCK_CMR                 = "Clocks_cmr.csv",
    CONFIG_OUTPUT_FILE_CLOCK_FV                  = "Clocks_fv.csv",
    CONFIG_OUTPUT_FILE_CLOCK_CG                  = "Clocks_cg.txt",
    CONFIG_OUTPUT_FILE_CLOCK_MM                  = "Clocks_mm.txt",
    CONFIG_OUTPUT_MAIN_FILE                      = "saida.txt",
    CONFIG_OUTPUT_FILE_THREAD_A                  = "saida_thread_A.txt",    
    CONFIG_OUTPUT_FILE_THREAD_B                  = "saida_thread_B.txt",
    CONFIG_OUTPUT_FILE_THREAD_C                  = "saida_thread_C.txt",        
    CONFIG_OUTPUT_FILE_THREAD_D                  = "saida_thread_D.txt",
    CONFIG_OUTPUT_FILE_THREAD_E                  = "saida_thread_E.txt",
    CONFIG_OUTPUT_DIR_THREAD_A                   = "Thread_A",    
    CONFIG_OUTPUT_DIR_THREAD_B                   = "Thread_B",
    CONFIG_OUTPUT_DIR_THREAD_C                   = "Thread_C",        
    CONFIG_OUTPUT_DIR_THREAD_D                   = "Thread_D",
    CONFIG_OUTPUT_DIR_THREAD_E                   = "Thread_E"; 
//    
// Main functions
//    
/**
 * Displays the mesh using openGL
 */    
void display();
/**
 * OpenGL function. Creates window with square witch representates the domain
 */
void init();
/**
 * Set initial boundary conditions and initial internal values
 * @param v vertex to set values
 */
void setInitialBoundaryConditions(Vertex *v);
/**
 * Advance timestep and copy value of u to previews u
 * @param m mesh to advance timestep
 * @param timeStep new timestep
 */
void nextTimeStep(Mesh *m, int &timeStep);
/**
 * Creates a visualization of mesh
 * @param argc
 * @param argv
 */
void displayMesh(int argc, char *argv[]);
/**
 * Set zero to values of u and previews_u 
 * @param m mesh to set zero value
 */
void setZero(Mesh *m);
/**
 * Get gradient mesh information stored on array and create a InfoQualityMesh 
 * @param m mesh to get gradients
 * @param timeStep time step of gradients
 */
InfoGradientMesh *getGradientsMesh(Mesh *m, int timeStep);
/**
 * Get quality mesh information stored on array and create a InfoQualityMesh 
 * @param m mesh to get gradients
 * @param timeStep time step of gradients
 */ 
InfoQualityMesh *getQualityMesh(Mesh *m, int timeStep);
/**
 * Print quality information mesh to file
 * @param l list of InfoQualityMesh
 * @param s string with output file
 */ 
void printInformationQualityMesh(list<InfoQualityMesh *> l, string s); 
/**
 * Print gradient information mesh to file
 * @param l list of InfoGradientMesh
 * @param s string with output file
 */ 
void printInformationGradientsMesh(list<InfoGradientMesh *> l, string s); 
/**
 * Print list of clocks to file
 * @param l list of InfoClock
 * @param s string with output file
 */ 
void printListOfClocks(list<InfoClock *> l, string s);
/**
 * Clear list of clocks deleting data
 * @param l list of InfoClock to clear
 */ 
void clearListOfClocks(list<InfoClock *> l);
/**
 * Clear list of QualityMesh deleting data
 * @param l list of InfoQualityMesh to clear
 */
void clearListOfQualityMesh(list<InfoQualityMesh *> l);
/**
 * Clear list of GradientsMesh deleting data
 * @param l list of InfoGradientMesh to clear
 */
void clearListOfGradientsMesh(list<InfoGradientMesh *> l); 
/**
 * Set to map the address of monitorFunctions and names of monitor functions 
 */
void createMonitorFunctionsMap();
/**
 * Read configurations variables from file
 * @param inputFileName input file name with configurations
 */
void readConfigurationsFromFile(string inputFileName);
/**
 * Verify if points x and y are out of domain
 * @param x x coordinate
 * @param y y coordinate
 * @return true if is out of domain
 */
bool isOutOfDomain(mpfr::real<MPFR_BITS_PRECISION> &x, mpfr::real<MPFR_BITS_PRECISION> &y );
/**
 * Verify if adjacency accord distance and gradient criterion refinement
 * @param adjPtr adjacency to verify
 * @return true if adjPtr accord distance and gradient criterion refinement
 */
bool refinementCriterion(Adjacency *adjPtr);
/**
 * Verify if adjacency accord distance criterion refinement
 * @param adjPtr adjacency to verify
 * @return true if adjPtr accord distance criterion refinement
 */
bool refinementDistanceCriterion(Adjacency *adjPtr);
/**
 * Verify if triangle accord criterion refinement
 * @param t triangle to verify
 * @return true if triangle t accord criterion refinement
 */
bool refinementTriangleCriterion(Triangle *t);
/**
 * Get triangle of adjacency with highest difference of u value
 * @param adjPtr adjacency to get triangle with highest difference of u value
 * @return triangle with highest difference of u value
 */
Triangle *getTriangle(Adjacency *adjPtr);
/**
 * Calculates weight values of sum_x_total, sum_y_total and sum_u_total to monitorFunctionGamma()
 * @param v vertex to calculates weight
 * @param sum_x_total variable to set sum of x
 * @param sum_y_total variable to set sum of y
 * @param sum_u_total variable to set sum of u
 * @see monitorFunctionGamma()
 */
void calculatesWeightGamma(Vertex *v, mpfr::real<MPFR_BITS_PRECISION> &sum_x_total, mpfr::real<MPFR_BITS_PRECISION> &sum_y_total, mpfr::real<MPFR_BITS_PRECISION> &sum_u_total);
/**
 * Calculates weight values of sum_x_total, sum_y_total and sum_u_total to monitorFunctionUpsilon()
 * @param v vertex to calculates weight
 * @param sum_x_total variable to set sum of x of the neighbors of vertex v
 * @param sum_y_total variable to set sum of y of the neighbors of vertex v
 * @param sum_u_total variable to set sum of u of the neighbors of vertex v
 * @see monitorFunctionUpsilon()
 */
void calculatesWeightUpsilon(Vertex *v, mpfr::real<MPFR_BITS_PRECISION> &sum_x_total, mpfr::real<MPFR_BITS_PRECISION> &sum_y_total, mpfr::real<MPFR_BITS_PRECISION> &sum_u_total);
/**
 * Calculates weight values of sum_x_total and sum_y_total to monitorFunctionLambda()
 * @param v vertex to calculates weight
 * @param sum_x_total variable to set sum of x of the neighbors of vertex v
 * @param sum_y_total variable to set sum of y of the neighbors of vertex v
 * @see monitorFunctionLambda()
 */
void calculatesWeightLambda(Vertex *v, mpfr::real<MPFR_BITS_PRECISION> &sum_x_total, mpfr::real<MPFR_BITS_PRECISION> &sum_y_total);
/**
 * Calculates weight values for laplacian smoothing.
 * @param v vertex to calculates weight
 * @param sum_x_total variable to set sum of x of the neighbors of vertex v
 * @param sum_y_total variable to set sum of y of the neighbors of vertex v
 * @return bool if vertex must be moved (if had triangle with bad quality)
 * @see monitorFunctionLambda()
 */
bool calculatesWeightLaplacianSmoothing(Vertex *v, mpfr::real<MPFR_BITS_PRECISION> &sum_x_total, mpfr::real<MPFR_BITS_PRECISION> &sum_y_total, mpfr::real<MPFR_BITS_PRECISION> &sum_u_total);
/**
 * Set new values of x and y stored in list of movingVertexData
 * @param verticesToMoveList list of InfoMovingVertex contening vertices to move
 */
void moveVertices(list<InfoMovingVertex *> &verticesToMoveList);
/**
 * Helps to create a InfoMovingVertex
 * @param v vertex to move
 * @param newCoordinates new coordiantes of vertex to move
 * @return InfoMovingVertex created
 */
InfoMovingVertex *createInfoMovingVertex(Vertex *v, mpfr::real<MPFR_BITS_PRECISION> *newCoordinates);
/**
 * Verify if triangle is out of domain
 * @param t triangle to check
 * @return true if is out of domain
 */
bool isTriangleOutOfDomain(Triangle *t);
/**
 * Moving vertices function guided by based laplacian smoothing function, defined by S. L. G. Oliveira, 2013
 * @param m mesh to move
 * @return true if vertices coordinates has moved. False if vertices coordinates don't change.
 */
bool monitorFunctionLambda(Mesh *m);
/**
 * Moving vertices function guided by based laplacian smoothing function, defined by Thompson 1999
 * @param m mesh to move
 * @return true if vertices coordinates has moved. False if vertices coordinates don't change.
 */
bool monitorFunctionUpsilon(Mesh *m);
/**
 * Moving vertices function guided by laplacian smoothing
 * @param m mesh to move
 * @return true if vertices coordinates has moved. False if vertices coordinates don't change.
 */
bool monitorFunctionGamma(Mesh *m);
/**
 * Moving vertices function guided by based laplacian smoothing function, defined by Taubin 1995
 * @param m mesh to move
 * @return true if vertices coordinates has moved. False if vertices coordinates don't change.
 */
bool monitorFunctionXi(Mesh *m); 
/**
 * Moving vertices function guided by based laplacian smoothing function, defined by Taubin 1995, known as bilaplacian
 * @param m mesh to move
 * @return true if vertices coordinates has moved. False if vertices coordinates don't change.
 */
bool monitorFunctionPsi(Mesh *m);
/**
 * Laplacian smoothing to improve mesh quality
 * @param m mesh to move
 * @return true if vertices coordinates has moved. False if vertices coordinates don't change.
 */
bool laplacianSmoothing(Mesh *m);
/**
 * Mesh generation and refinement
 */
Mesh *createAdaptiveMesh(); 
/**
 * Advances timestep and moving mesh. Running by thread
 * @param arg void pointer to ThreadParameter
 * @return NULL
 */
void *advanceTimeAndMovingMesh(void *arg);
/**
 * Adapter to clone mesh. Running by thread
 * @param arg void pointer to Mesh to clone
 * @return NULL
 */
void *adapterCloneMesh(void *arg);
/**
 * Clean output directory
 */
void cleanOutputDirectories();
/**
 * Creates output directories of threads
 */
void createOutputDirectories();
//
// Static variables
//
static map<int, monitorFunction> monitorFunctions; /**< Map of monitor functions address */
static map<int, string> monitorFunctionsNames; /**< Map of monitor functions names */
static clock_t timeClockToCreateAdaptiveMesh; /**< Store clocks to create adaptive mesh */
static mpfr::real<MPFR_BITS_PRECISION> 
    eta                	 = 0, /**< Minimal quality tolerance. Stoped criterion to moving vertices. Setting on createAdaptiveMesh() */
    rho_alpha          	 = 1, /**< Value of circunradiusToShortestEdgeRatio maximus used to refine mesh. Setting on createAdaptiveMesh() */
    limitDistance        = 0, /**< Limit of Distance is used by refinementCriterion() */
    limitAdjacencyLambda = 0; /**< Limit of Adjacency is used by refinementCriterion() */                                                                                                 
//
// Variables
//    
InfoClock *ic; /**< Information of clocks of execution */    
int vertexCount = 0; /**< Used to assign vertices label by setInitialBoundaryConditions() */
Mesh 
  *displayM,/**< Pointer to mesh display. */
  *m;//, *n, *o, *p, *q; /**< m is original mesh; n, o, p and q are clones, used by threads. */
/**
 * Main function
 * 
 */
int main(int argc, char *argv[])
{   
    stringstream tmp, outputFile; 
    outputFile.str("");    
    // Creates a array of pointers to ThreadParameter, used by AdvancingTimeMovingMes()
    ThreadParameter *threadsArg[CONFIG_NUM_THREADS];
    pthread_t threads[CONFIG_NUM_THREADS];       
    pthread_t threads_clone[CONFIG_NUM_THREADS-1];
    // Arguments of threads execution
    ThreadParameter *argM = NULL, *argN = NULL, *argO = NULL, *argP = NULL, *argQ = NULL;
    // Read configuration file
    CONFIG_MAIN_FILE = (argc == 2) ? argv[1] : CONFIG_MAIN_FILE;  
    // Clean output directory
//    cleanOutputDirectories();
    // Set configuration variables
    readConfigurationsFromFile(CONFIG_MAIN_FILE);
    // Setting output file
    outputFile << CONFIG_OUTPUT_DIRECTORY << "/" << CONFIG_OUTPUT_MAIN_FILE;         
    // Create output directories
    createOutputDirectories();        
    // Map of names and address of monitor functions
    createMonitorFunctionsMap();
    // Create mesh by adaptive refinenment
    m = createAdaptiveMesh(); 
    // Clonning Meshes
    tmp.str("");
    tmp << "\n#### CLONANDO MALHA ####\n\n";
    int totalMeshes = 0;
    if (CONFIG_EXEC_THREAD_A)
    {
        // ArgM use original mesh
	argM = new ThreadParameter(m, CONFIG_MONITOR_FUNCTION_THREAD_A, 0, CONFIG_OUTPUT_FILE_THREAD_A, CONFIG_OUTPUT_DIR_THREAD_A);
	threadsArg[0] = argM;
	tmp << "Thread A OK\n";
	totalMeshes++;
    }
    if (CONFIG_EXEC_THREAD_B)
    {   
	int flag = pthread_create(&threads_clone[0],NULL, &adapterCloneMesh, (void *) m);
	argN = new ThreadParameter(/*n*/ NULL, CONFIG_MONITOR_FUNCTION_THREAD_B, 0, CONFIG_OUTPUT_FILE_THREAD_B, CONFIG_OUTPUT_DIR_THREAD_B);
	threadsArg[1] = argN;
	if (flag == 0)
	    tmp << "Thread B OK\n";      	  
	else 
	{
	    tmp << "Erro clone mesh B\n"; 
	    Utils::printString(tmp.str(), outputFile.str(), CONFIG_PRINT_INFO_MESSAGES); 	            
	    exit(EXIT_FAILURE);		    
	}
	totalMeshes++;
    }
    if (CONFIG_EXEC_THREAD_C)
    {
	int flag = pthread_create(&threads_clone[1],NULL, &adapterCloneMesh, (void *) m);
	argO = new ThreadParameter(/*o*/ NULL, CONFIG_MONITOR_FUNCTION_THREAD_C, 0, CONFIG_OUTPUT_FILE_THREAD_C, CONFIG_OUTPUT_DIR_THREAD_C);
	threadsArg[2] = argO;
	if (flag == 0)
	    tmp << "Thread C OK\n";      	  
	else 
	{
	    tmp << "Erro clone mesh C\n"; 
	    Utils::printString(tmp.str(), outputFile.str(), CONFIG_PRINT_INFO_MESSAGES); 	            
	    exit(EXIT_FAILURE);		    
	}
	totalMeshes++;
    }
    if (CONFIG_EXEC_THREAD_D)
    {
	int flag = pthread_create(&threads_clone[2],NULL, &adapterCloneMesh, (void *) m);
	argP = new ThreadParameter(/*p*/ NULL, CONFIG_MONITOR_FUNCTION_THREAD_D, 0, CONFIG_OUTPUT_FILE_THREAD_D, CONFIG_OUTPUT_DIR_THREAD_D);
	threadsArg[3] = argP;
	if (flag == 0)
	    tmp << "Thread D OK\n";      	  
	else 
	{
	    tmp << "Erro clone mesh D\n"; 
	    Utils::printString(tmp.str(), outputFile.str(), CONFIG_PRINT_INFO_MESSAGES); 	            
	    exit(EXIT_FAILURE);		    
	}
	totalMeshes++;
    }    
    if (CONFIG_EXEC_THREAD_E)
    {
	int flag = pthread_create(&threads_clone[3],NULL, &adapterCloneMesh, (void *) m);
	argQ = new ThreadParameter(/*q*/ NULL, CONFIG_MONITOR_FUNCTION_THREAD_E, 0, CONFIG_OUTPUT_FILE_THREAD_E, CONFIG_OUTPUT_DIR_THREAD_E);
	threadsArg[4] = argQ;
	if (flag == 0)
	    tmp << "Thread E OK\n";      	  
	else 
	{
	    tmp << "Erro clone mesh E\n"; 
	    Utils::printString(tmp.str(), outputFile.str(), CONFIG_PRINT_INFO_MESSAGES); 	            
	    exit(EXIT_FAILURE);		    
	}
	totalMeshes++;
    }            
    for(int i = 0; i < CONFIG_NUM_THREADS - 1; ++i) 
    {
	if ( ((i == 0) &&  (!CONFIG_EXEC_THREAD_B)) ||
	     ((i == 1) &&  (!CONFIG_EXEC_THREAD_C)) ||
	     ((i == 2) &&  (!CONFIG_EXEC_THREAD_D)) ||
	     ((i == 3) &&  (!CONFIG_EXEC_THREAD_E)) )
	    continue;
	Mesh *newMeshTmp = NULL;
	pthread_join(threads_clone[i], (void **)&newMeshTmp);
	ThreadParameter *argTmp = threadsArg[i+1]; // Ignores ArgM
	argTmp->mesh = newMeshTmp;		
    }
    Utils::printString(tmp.str(), outputFile.str(), CONFIG_PRINT_INFO_MESSAGES);      
    // Executing threads
    tmp.str("");
    tmp << "\n#### EXECUTANDO THREADS ####\n\n";   
    int num_threads = 0;
    for(int i = 0; i < CONFIG_NUM_THREADS; i++)
    {
	if ( ((i == 0) &&  (!CONFIG_EXEC_THREAD_A)) ||
	     ((i == 1) &&  (!CONFIG_EXEC_THREAD_B)) ||
	     ((i == 2) &&  (!CONFIG_EXEC_THREAD_C)) ||
	     ((i == 3) &&  (!CONFIG_EXEC_THREAD_D)) ||
	     ((i == 4) &&  (!CONFIG_EXEC_THREAD_E)) )
	    continue;      
        threadsArg[i]->threadId = i;
	int flag = pthread_create(&threads[i],NULL, &advanceTimeAndMovingMesh, (void *)threadsArg[i]);
	if (flag != 0)
	{  
	    tmp << "Erro criacao da thread: " << i << endl;     
	    Utils::printString(tmp.str(), outputFile.str(), CONFIG_PRINT_INFO_MESSAGES); 	            
	    exit(EXIT_FAILURE);		  
	}
	tmp << "Thread: " << i << " OK" << endl;     
	num_threads++;
     }    
    Utils::printString(tmp.str(), outputFile.str(), CONFIG_PRINT_INFO_MESSAGES); 	        
    tmp.str("");
    for(int i = 0; i < CONFIG_NUM_THREADS; ++i) 
    {
	if ( ((i == 0) &&  (!CONFIG_EXEC_THREAD_A)) ||
	     ((i == 1) &&  (!CONFIG_EXEC_THREAD_B)) ||
	     ((i == 2) &&  (!CONFIG_EXEC_THREAD_C)) ||
	     ((i == 3) &&  (!CONFIG_EXEC_THREAD_D)) ||
	     ((i == 4) &&  (!CONFIG_EXEC_THREAD_E)) )
	    continue;      
	tmp << "Join Thread: " << i << " OK" << endl;     	
	pthread_join(threads[i],NULL);
    }
    Utils::printString(tmp.str(), outputFile.str(), CONFIG_PRINT_INFO_MESSAGES); 	            	    
    displayM = m;
    if (CONFIG_DISPLAY_MESH) 
        displayMesh(argc, argv);    
    return EXIT_SUCCESS;    
}

void displayMesh (int argc, char *argv[])
{    
    glutInit( &argc, argv );       
    glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize( 1080, 1080 );
    glutInitWindowPosition( 0,0 );
    glutCreateWindow( "Triangulacao de Delaunay" );
    init();
    glutDisplayFunc( display );
    glutMainLoop();
}

void display ()
{
    glClear (GL_COLOR_BUFFER_BIT);    
    openGL::drawGrid(displayM->triangleList, displayM->edges, CONFIG_SHOW_VERTEX_LABEL, CONFIG_SHOW_TRIANGLE, CONFIG_SHOW_CIRCLE, CONFIG_SHOW_VORONOI_DIAGRAM);
    glFlush();
}

void init (void)
{
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
    glClear( GL_COLOR_BUFFER_BIT );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    //gluOrtho2D ( -0.1, 4000000.1, -0.1, 4000000.1 );
    //gluOrtho2D ( 10000.0, 50000.1, 10000.0, 50000.1 ); // circle
    //gluOrtho2D ( 900000.1, 5100000, 900000.1, 5100000 ); // quadrados
    //gluOrtho2D ( 250.0, 550, 450.0, 750 ); // pslg
    //gluOrtho2D ( 1000.0, 75000, 28000, 96000 ); // pslgr
    //gluOrtho2D ( 160000.0, 500000, 436000.0, 780000 ); // circulo
    gluOrtho2D ( -1.0, CONFIG_DOMAIN_DIMENSION + 1, -1.0, CONFIG_DOMAIN_DIMENSION + 1 ); // quadrado
}

Mesh *createAdaptiveMesh()
{
    bool refined = true;   
    stringstream tmp, outputFile;
    int countRefinement = 0, numberOfIterationsCG = 0;
    clock_t timeClock;
    // List of clocks information
    list<InfoClock *> 
	listClockCMr, // Clocks to Cuthill Mckee Reverse
	listClockFV,  // Clocks to Finite Volume Method
	listClockCG,  // Clocks to Conjugate Gradient
	listClockMM;  // Clocks to Moving Mesh
    // List of data quality mesh
    list<InfoQualityMesh *> listQualityMesh;
    // List of data gradients mesh
    list<InfoGradientMesh *> listGradients;
    mpfr::real<MPFR_BITS_PRECISION> 
	delta_t 	   = 0,
	*diferenceGradient = NULL, 
        *biggerDistance    = NULL;
    // Creating new mesh
    Mesh *m = new Mesh();
    // Configuring outputfile
    outputFile.str("");
    outputFile << CONFIG_OUTPUT_DIRECTORY << "/" << CONFIG_OUTPUT_MAIN_FILE;    
    timeClockToCreateAdaptiveMesh = clock(); // Getting initial clocks
    delta_t   = CONFIG_TIME_VARIATION; // Static variable
    eta       = CONFIG_MOVING_MESH_ETA; // Static variable
    rho_alpha = fabs( 1 / (2*sin( CONFIG_MESH_MINIMUM_ANGLE_ALPHA * (mpfr::const_pi<MPFR_BITS_PRECISION, MPFR_RNDN>()/180) ) ) ); // Static variable   
    mpfr_free_cache();   // Clear const_pi 	    
    // Set vertex function to define initial values of u
    m->setVertexFunction(setInitialBoundaryConditions);
    // Create the initial mesh from a inputfile
    m->initialize("input/quadrado.ini");        
    // Remove triangles located out of domain
    m->removeTriangles(isTriangleOutOfDomain);     
    // Limit of distance
    biggerDistance = Utils::getBiggerDistance(m->triangleList);
    limitDistance  = (*biggerDistance)*CONFIG_PERC_INCREASE_GRADIENT_THETA; // Used only in the first iteration by refinementDistanceCriterion
    limitAdjacencyLambda = 0;
    delete biggerDistance;
    tmp.str("");    
    tmp << "#### INICIO REFINAMENTO MALHA ###########\n\n";
    while ( (m->numberOfVertices < CONFIG_MIN_VERTICES) && (refined) )           
    {           
        countRefinement++;    
	tmp << "\nETAPA REFINAMENTO: " << countRefinement << endl;
        if (countRefinement == 1) 
        {
            tmp << "Executando algoritmo de Ruppert: Criterio distancia.\n";   
            refined = m->applyRefinement(refinementDistanceCriterion, getTriangle); // In the first refinement step ignores the adjacency condition
        } else 
        {
            // Calculates (Gamma_max - Gamma_min)
            diferenceGradient = Utils::getDiferenceGradient(m->edges);
	    // Lambda = Lambda + Theta*(Gamma_max - Gamma_min)
            limitAdjacencyLambda  += CONFIG_PERC_INCREASE_GRADIENT_THETA*(*diferenceGradient); 
            delete diferenceGradient;
            tmp << "Executando algoritmo de Ruppert:  Criterio gradiente.\n";   
            // Refine the mesh
            refined = m->applyRefinement( refinementTriangleCriterion);                             
        }   
        tmp << "Executando algoritmo de Ungor: Off-centers.\n";             
        // Refinenment by angle criterion
        m->refine(rho_alpha, MESH_UNGOR_REFINEMENT); //  MESH_RUPPERT_REFINEMENT or MESH_UNGOR_REFINEMENT. 
	// Solve mesh
        tmp << "Executando MVF.\n";   
        timeClock = clock();	
        FiniteVolume::solve(m->first, delta_t); // Generates linear system by Finite Volume
        timeClock = clock() - timeClock;
        ic = new InfoClock(m->numberOfVertices, timeClock);
        listClockFV.push_back(ic);         	
        if (CONFIG_EXEC_CUTHILL_MCKEE_ALG)
        {
            tmp << "Executando CMr.\n";              
            timeClock = clock();
            m->cuthillMcKee(CONFIG_CUTHILL_MCKEE_REVERSE, UTILS_CM_PSEUDOPERIPHERAL);                
            timeClock = clock() - timeClock;   
            ic = new InfoClock(m->numberOfVertices, timeClock);
            listClockCMr.push_back(ic);                                  
        }        
        tmp << "Executando MGC.\n";                               
        timeClock = clock();
        numberOfIterationsCG = ConjugateGradient::solve( m->first, m->numberOfVertices, &CONFIG_CG_PRESICION_EPSILON); // Solve the mesh by Conjugate Gradient                                    
        timeClock = clock() - timeClock;
        ic = new InfoClock(m->numberOfVertices, timeClock);
        listClockCG.push_back(ic);       
        tmp << "Total iteracoes: " << numberOfIterationsCG << endl;                           
        tmp << "Total vertices: " << m->numberOfVertices << endl;      
	Utils::printString(tmp.str(), outputFile.str(), CONFIG_PRINT_INFO_MESSAGES);     
	tmp.str("");
	// Get gradients mesh
	listGradients.push_back(getGradientsMesh(m, 0));	
	// Get quality mesh
	listQualityMesh.push_back(getQualityMesh(m, 0));	  
    // End of refinement stage     		
    }  
    // Calculates clocks to create adaptive mesh
    timeClockToCreateAdaptiveMesh = clock() - timeClockToCreateAdaptiveMesh;        
    tmp.str("");
    tmp << CONFIG_OUTPUT_DIRECTORY << "/" << CONFIG_OUTPUT_FILE_QUALITY_MESH;
    printInformationQualityMesh(listQualityMesh, tmp.str());
    clearListOfQualityMesh(listQualityMesh); 
    tmp.str("");
    tmp << CONFIG_OUTPUT_DIRECTORY << "/" << CONFIG_OUTPUT_FILE_GRADIENT_MESH;	
    printInformationGradientsMesh(listGradients, tmp.str());  
    clearListOfGradientsMesh(listGradients);
    if (CONFIG_PRINT_CLOCKS_VALUES)
    {    
	tmp.str(""); // Clear varable
        tmp << CONFIG_OUTPUT_DIRECTORY << CONFIG_OUTPUT_FILE_CLOCK_CMR;
	printListOfClocks(listClockCMr, tmp.str()); 
	tmp.str(""); // Clear varable	
	tmp << CONFIG_OUTPUT_DIRECTORY << CONFIG_OUTPUT_FILE_CLOCK_FV;
	printListOfClocks(listClockFV, tmp.str()); 
	tmp.str(""); // Clear varable	
	tmp << CONFIG_OUTPUT_DIRECTORY << CONFIG_OUTPUT_FILE_CLOCK_CG;
	printListOfClocks(listClockCG, tmp.str()); 
	tmp.str(""); // Clear varable	
	tmp << CONFIG_OUTPUT_DIRECTORY << CONFIG_OUTPUT_FILE_CLOCK_MM;
	printListOfClocks(listClockMM, tmp.str());     
	// Cleaning list of clocks
	clearListOfClocks(listClockCMr);
	clearListOfClocks(listClockFV);
	clearListOfClocks(listClockCG);
	clearListOfClocks(listClockMM); 
    }        
    return m;
}

void *advanceTimeAndMovingMesh(void *arg)
{
    stringstream outputFile, outputDir, tmp;
    clock_t timeClockToAdvanceTimeAndMovingMesh, timeClock; 
    int timeStep = 1, monitorFunctionId = 0, threadId = 0, numberOfIterationsCG = 0;
    list<InfoClock *> 
	listClockCMr, // Clocks to Cuthill Mckee Reverse
	listClockFV,  // Clocks to Finite Volume Method
	listClockCG,  // Clocks to Conjugate Gradient
	listClockMM;  // Clocks to Moving Mesh   
    // List of data quality mesh
    list<InfoQualityMesh *> listQualityMesh;
    // List of data gradients mesh
    list<InfoGradientMesh *> listGradients;        
    mpfr::real<MPFR_BITS_PRECISION>
	delta_t = 0, /**< Time variation of heat equation used in finite volume method. */
	*upsilon = NULL;    
    // Parse void pointer to data pointer
    ThreadParameter *argData = (ThreadParameter *) arg;    
    // Parameters variables
    Mesh *m           = argData->mesh;
    monitorFunctionId = argData->monitorFunctionId;
    threadId          = argData->threadId;        
    outputDir         << argData->outputDirectory;
    outputFile        << argData->outputFile;
    delete argData;   
    tmp.str("");
    tmp << CONFIG_OUTPUT_DIRECTORY << "/" << outputDir.str() << "/" << outputFile.str();                   
    outputFile.str(tmp.str());
    tmp.str("");
    tmp << CONFIG_OUTPUT_DIRECTORY << "/" << outputDir.str() << "/";                        
    outputDir.str(tmp.str());
    timeClockToAdvanceTimeAndMovingMesh = clock();
    while (timeStep <= CONFIG_TIME_LIMIT)   
    {
	delta_t = timeStep*CONFIG_TIME_VARIATION;
	int countMoviment = 0;
	bool hasMov = false;
	// Get gradients mesh
	listGradients.push_back(getGradientsMesh(m, timeStep));
	// Get quality mesh
	listQualityMesh.push_back(getQualityMesh(m, timeStep));	
	do 
	{	   	  
	    if (upsilon != NULL) delete upsilon;     
	    // Verify if monitorFunctionId is valid and moving mesh
	    if ( (monitorFunctionId >= 0) && (monitorFunctionId < (int)monitorFunctionsNames.size() ) )        
	    {
	      	tmp.str("");
		tmp << "\n########### THREAD " << threadId << " ###########\n";
		tmp << "Movimentando malha (Funcao "<< monitorFunctionsNames[monitorFunctionId] << "). Etapa: " << timeStep << "/" << CONFIG_TIME_LIMIT << " - " << countMoviment << "\n";     
		timeClock = clock();            
		hasMov = m->movingMesh(monitorFunctions[monitorFunctionId]);  // Moving the Mesh    
		timeClock = clock() - timeClock;
		ic = new InfoClock(m->numberOfVertices, timeClock);
		listClockMM.push_back(ic);                      
	    } else 
	    {
		cerr << "ERRO - Funcao monitora invalida. " << endl;     
		exit(EXIT_FAILURE);
	    }	 
	    if (hasMov) tmp << "Movimento OK\n"; else tmp << "Nao Movimentou\n";
	    countMoviment++;
	    tmp <<  "Verificando Malha.\n";
	    // Verify mesh pos moviment  
	    if (m->smartCheckMesh() )
		tmp << "Malha OK.\n";	   
	    else {
		tmp << "Erro Malha.\n";  
		Utils::printString(tmp.str(), outputFile.str(), CONFIG_PRINT_INFO_MESSAGES);
		pthread_exit(NULL);
		
	    }
	    tmp << "Executando MVF.\n";     
	    timeClock = clock();
	    FiniteVolume::solve(m->first, delta_t); // Generates linear system by Finite Volume
	    timeClock = clock() - timeClock;
	    ic = new InfoClock(m->numberOfVertices, timeClock);
	    listClockFV.push_back(ic);           
	    if (CONFIG_EXEC_CUTHILL_MCKEE_ALG)
	    {
		tmp << "Executando CMr.\n";  
		timeClock = clock();
		m->cuthillMcKee(CONFIG_CUTHILL_MCKEE_REVERSE, UTILS_CM_PSEUDOPERIPHERAL);                
		timeClock = clock() - timeClock;   
		ic = new InfoClock(m->numberOfVertices, timeClock);
		listClockCMr.push_back(ic);                                  
	    }	                
	    tmp << "Executando MGC.\n"; 
	    timeClock = clock();
	    numberOfIterationsCG = ConjugateGradient::solve( m->first, m->numberOfVertices, &CONFIG_CG_PRESICION_EPSILON); // Solve the mesh by Conjugate Gradient                                    
	    timeClock = clock() - timeClock;
	    ic = new InfoClock(m->numberOfVertices, timeClock);
	    listClockCG.push_back(ic);    
	    timeClock = clock();	    
	    tmp << "Total iteracoes: " << numberOfIterationsCG << endl;
	    tmp << "Total vertices: " << m->numberOfVertices << endl;
	    Utils::printString(tmp.str(), outputFile.str(), CONFIG_PRINT_INFO_MESSAGES);
	    tmp.str("");	    
	    // Get mesh quality Shape Regularity Quality
	    upsilon = Utils::shapeRegularityQualityMesh(*m);	    
 	    if ( *upsilon < eta )
 	    {
		// Get gradients mesh
		listGradients.push_back(getGradientsMesh(m, timeStep));	
		// Get quality mesh
		listQualityMesh.push_back(getQualityMesh(m, timeStep));		
 	    }
	} while ( (hasMov) && ( *upsilon > eta ) ); // End moving mesh
	if (CONFIG_LAPLACIAN_SMOOTHING_KAPA != 0)
	{
	    int count = 0;	  
	    do 
	    {
		count++;
		tmp.str("");
		tmp << "Executando suavizacao laplaciana." << endl;      
		laplacianSmoothing(m);
		upsilon = Utils::shapeRegularityQualityMesh(*m);		
		tmp << "Upsilon min = " << *upsilon << endl;
		Utils::printString(tmp.str(), outputFile.str(), CONFIG_PRINT_INFO_MESSAGES);
	    } while ( (*upsilon < eta) && (count < 5));
	}
	if (upsilon != NULL) delete upsilon;
	upsilon = NULL;
	tmp.str("");
	tmp << "\n########### THREAD " << threadId << " ###########\n";	
	tmp << "FIM DA ETAPA: " << timeStep << "/" << CONFIG_TIME_LIMIT << endl;       	
	int totalFlips = 0, totalRefined = 0;
	if (CONFIG_EXEC_FLIP_OFF_CENTER) // Lawson's algorithm
	{
	    tmp << "Executando algoritmo de Lawson: flips.\n";
	    totalFlips = m->maintainsDelaunay();
	    tmp << "Total flips: " << totalFlips << endl;   
	}
	else // Refinenment by angle criterion        
	{
	    tmp << "EXECUTANDO ALGORITMO DE UNGOR: Off-centers.\n";            
	    totalRefined = m->refine(rho_alpha, MESH_UNGOR_REFINEMENT); /* MESH_RUPPERT_REFINEMENT or MESH_UNGOR_REFINEMENT. */                               
	}
	tmp << "Executando MVF.\n";
	timeClock = clock();
	FiniteVolume::solve(m->first, delta_t); // Generates linear system by Finite Volume
	timeClock = clock() - timeClock;
	ic = new InfoClock(m->numberOfVertices, timeClock);
	listClockFV.push_back(ic);                   	
	if ( (CONFIG_EXEC_CUTHILL_MCKEE_ALG) && ( (totalRefined != 0) || (totalFlips != 0) ) )
	{            
	    tmp << "Executando CMr.\n"; 
	    timeClock = clock();
	    m->cuthillMcKee(CONFIG_CUTHILL_MCKEE_REVERSE, UTILS_CM_PSEUDOPERIPHERAL);                
	    timeClock = clock() - timeClock;  
	    ic = new InfoClock(m->numberOfVertices, timeClock);
	    listClockCMr.push_back(ic);                 
	}
	tmp << "Executando MGC.\n";
	timeClock = clock();
	numberOfIterationsCG = ConjugateGradient::solve( m->first, m->numberOfVertices, &CONFIG_CG_PRESICION_EPSILON); // Solve the mesh by Conjugate Gradient                                    
	timeClock = clock() - timeClock;
	ic = new InfoClock(m->numberOfVertices, timeClock);
	listClockCG.push_back(ic); 
	tmp << "Total refinados: " << totalRefined << endl;   	
	tmp << "Total iteracoes: " << numberOfIterationsCG << endl;      
	tmp << "Total vertices: " << m->numberOfVertices << endl; 
	Utils::printString(tmp.str(), outputFile.str(), CONFIG_PRINT_INFO_MESSAGES);
	tmp.str("");	
	tmp << "\n########### THREAD " << threadId << " ###########\n";
	tmp << "AVANCANDO PARA ETAPA TEMPORAL : " << timeStep + 1 << "/" << CONFIG_TIME_LIMIT  << endl;          
        nextTimeStep(m, timeStep); // Next Time STep
	delta_t = timeStep*CONFIG_TIME_VARIATION;
        tmp << "Executando MVF.\n";	
        timeClock = clock();
        FiniteVolume::solve(m->first, delta_t); // Generates linear system by Finite Volume
        timeClock = clock() - timeClock;
        ic = new InfoClock(m->numberOfVertices, timeClock);
        listClockFV.push_back(ic);           
        if (CONFIG_EXEC_CUTHILL_MCKEE_ALG)
        {            
            tmp << "Executando CMr.\n";          
            timeClock = clock();
            m->cuthillMcKee(CONFIG_CUTHILL_MCKEE_REVERSE, UTILS_CM_PSEUDOPERIPHERAL);                
            timeClock = clock() - timeClock;  
            ic = new InfoClock(m->numberOfVertices, timeClock);
            listClockCMr.push_back(ic);                 
        }
	tmp << "Executando MGC.\n";
        timeClock = clock();        
        numberOfIterationsCG = ConjugateGradient::solve( m->first, m->numberOfVertices, &CONFIG_CG_PRESICION_EPSILON); // Solve the mesh by Conjugate Gradient                                    
	timeClock = timeClock - clock();
        ic = new InfoClock(m->numberOfVertices, timeClock);
        listClockCG.push_back(ic);   
	tmp << "Total iteracoes: " << numberOfIterationsCG << endl;
	tmp << "Total vertices: " << m->numberOfVertices << endl;
	Utils::printString(tmp.str(), outputFile.str(), CONFIG_PRINT_INFO_MESSAGES);
	tmp.str("");	
    }    
    tmp << "\n########### THREAD " << threadId << " ###########\n";  
    if (m->smartCheckMesh() )
	tmp << "Malha OK.\n";
    else 
	tmp << "Erro Malha.\n" ;    
    timeClockToAdvanceTimeAndMovingMesh = clock() - timeClockToAdvanceTimeAndMovingMesh;
    tmp << "Total CLOCKS: " <<  timeClockToAdvanceTimeAndMovingMesh + timeClockToCreateAdaptiveMesh << endl; 
    // Get gradients mesh
    listGradients.push_back(getGradientsMesh(m, timeStep));	
    // Get quality mesh
    listQualityMesh.push_back(getQualityMesh(m, timeStep));	    
    Utils::printString(tmp.str(), outputFile.str(), CONFIG_PRINT_INFO_MESSAGES);
    tmp.str("");
    tmp << outputDir.str() << "/" << CONFIG_OUTPUT_FILE_GRADIENT_MESH;	
    printInformationGradientsMesh(listGradients, tmp.str());
    clearListOfGradientsMesh(listGradients);
    tmp.str("");
    tmp << outputDir.str() << "/" << CONFIG_OUTPUT_FILE_QUALITY_MESH;    
    printInformationQualityMesh(listQualityMesh, tmp.str());  
    clearListOfQualityMesh(listQualityMesh);
    // Print results to files
    if (CONFIG_PRINT_VERTEX_VALUE) 
    {
	tmp.str(""); // Clear varable
        tmp << outputDir.str() << "/" << CONFIG_OUTPUT_FILE_VERTEX_VALUE;
	Utils::printValuesVertices(m->first, tmp.str());   
    }
    if (CONFIG_PRINT_TRIANGLE_VALUE)
    {
	tmp.str(""); // Clear varable
	tmp << outputDir.str() << CONFIG_OUTPUT_FILE_TRIANGLE_VALUE;
	Utils::printValuesTriangles(m->triangleList, tmp.str());           
    }
    if (CONFIG_PRINT_ADJACENCY_VALUE)
    {
	tmp.str(""); // Clear varable
	tmp << outputDir.str() << CONFIG_OUTPUT_FILE_ADJACENCY_VALUE;	
	Utils::printValuesAdjacencies(m->edges, tmp.str());
    }
    /*    Utils::printLinearSystem(m->first, delta_t, "output/sistema_linear.txt");     
    if (CONFIG_PRINT_MATRIX_A_B)
      Utils::printMatrixAB(m, CONFIG_OUTPUT_FILE_MATRIX_A, CONFIG_OUTPUT_FILE_MATRIX_B, CONFIG_OUTPUT_PRECISION);              
    */    
    if (CONFIG_PRINT_CLOCKS_VALUES)
    {    
	tmp.str(""); // Clear varable
        tmp << outputDir.str() << CONFIG_OUTPUT_FILE_CLOCK_CMR;
	printListOfClocks(listClockCMr, tmp.str()); 
	tmp.str(""); // Clear varable	
	tmp << outputDir.str() << CONFIG_OUTPUT_FILE_CLOCK_FV;
	printListOfClocks(listClockFV, tmp.str()); 
	tmp.str(""); // Clear varable	
	tmp << outputDir.str() << CONFIG_OUTPUT_FILE_CLOCK_CG;
	printListOfClocks(listClockCG, tmp.str()); 
	tmp.str(""); // Clear varable	
	tmp << outputDir.str() << CONFIG_OUTPUT_FILE_CLOCK_MM;
	printListOfClocks(listClockMM, tmp.str());     
	// Cleaning list of clocks
	clearListOfClocks(listClockCMr);
	clearListOfClocks(listClockFV);
	clearListOfClocks(listClockCG);
	clearListOfClocks(listClockMM); 
    }    
    pthread_exit(NULL);
}

void nextTimeStep(Mesh *m, int &timeStep) {
    Vertex *vertexIterator = m->first;
    while ( vertexIterator != NULL )
    {
        if (!vertexIterator->isBorder) 
        {
            vertexIterator->previews_u = vertexIterator->u;
            vertexIterator->u          = 0;
        }
        vertexIterator = vertexIterator->next;
    } 
    timeStep++;     
}

bool refinementCriterion(Adjacency *adjPtr)
{
    mpfr::real<MPFR_BITS_PRECISION> *distance = NULL, *gradient = NULL;
    Vertex *v0 = adjPtr->getVertex(ADJ_VERTEX_ONE);
    Vertex *v1 = adjPtr->getVertex(ADJ_VERTEX_TWO);
    distance   = Utils::distance(v0, v1);
    gradient   = Utils::gradient(v0, v1);
    bool criterionDistance = (*distance) >= limitDistance;
    bool criterionGradient = (*gradient) >= limitAdjacencyLambda;
    delete distance;
    delete gradient;
    return (criterionDistance && criterionGradient);
}

bool refinementTriangleCriterion(Triangle *t)
{
    mpfr::real<MPFR_BITS_PRECISION> *distance = NULL, *gradient = NULL;
    Vertex *v0 = t->getVertex(TRIANGLE_VERTEX_ONE);
    Vertex *v1 = t->getVertex(TRIANGLE_VERTEX_TWO);
    Vertex *v2 = t->getVertex(TRIANGLE_VERTEX_THREE);
    distance   = Utils::distance(v0, v1);
    gradient   = Utils::gradient(v0, v1);    
    bool criterionDistance = (*distance) >= limitDistance;
    bool criterionGradient = (*gradient) >= limitAdjacencyLambda;
    delete distance;
    delete gradient;
    if (criterionDistance && criterionGradient) 
        return true;
    distance   = Utils::distance(v1, v2);
    gradient   = Utils::gradient(v1, v2);
    criterionDistance = (*distance) >= limitDistance;
    criterionGradient = (*gradient) >= limitAdjacencyLambda;
    delete distance;
    delete gradient;    
    if (criterionDistance && criterionGradient) 
        return true;
    distance   = Utils::distance(v0, v2);
    gradient   = Utils::gradient(v0, v2);
    criterionDistance = (*distance) >= limitDistance;
    criterionGradient = (*gradient) >= limitAdjacencyLambda;
    delete distance;
    delete gradient;    
    if (criterionDistance && criterionGradient) 
        return true;    
    return false;
}

bool refinementDistanceCriterion(Adjacency *adjPtr)
{
    mpfr::real<MPFR_BITS_PRECISION> *distance = NULL;
    Vertex *v0 = adjPtr->getVertex(ADJ_VERTEX_ONE);
    Vertex *v1 = adjPtr->getVertex(ADJ_VERTEX_TWO);
    distance   = Utils::distance(v0, v1);
    bool criterionDistance = (*distance) >= limitDistance;
    delete distance;
    return criterionDistance;
}

Triangle *getTriangle(Adjacency *adjPtr)
{
    mpfr::real<MPFR_BITS_PRECISION> diff_t0 = 0, diff_t1 = 0;
    int number_ve0_t0 = 0, number_ve1_t0 = 0, number_ve0_t1 = 0, number_ve1_t1 = 0;
    Triangle *t0 = adjPtr->getTriangle(ADJ_VERTEX_ONE);
    Triangle *t1 = adjPtr->getTriangle(ADJ_VERTEX_TWO);
    Vertex *ve0 = adjPtr->getVertex(ADJ_VERTEX_ONE);
    Vertex *ve1 = adjPtr->getVertex(ADJ_VERTEX_TWO);
    if ((t0 == NULL) && (t1 != NULL)) return t1; 
    if ((t1 == NULL) && (t0 != NULL)) return t0;
    number_ve0_t0 = t0->getNumber(ve0);
    number_ve1_t0 = t0->getNumber(ve1);
    number_ve0_t1 = t1->getNumber(ve0);
    number_ve1_t1 = t1->getNumber(ve1);
    Vertex *vt0 = t0->getVertex( 3 - (number_ve0_t0 + number_ve1_t0) );
    Vertex *vt1 = t1->getVertex( 3 - (number_ve0_t1 + number_ve1_t1) );
    diff_t0 = abs(vt0->u - ve0->u) + abs(vt0->u - ve1->u);
    diff_t1 = abs(vt1->u - ve0->u) + abs(vt1->u - ve1->u);
    if(diff_t0 > diff_t1)
        return t0;
    else
        return t1;
    return NULL;
}

bool isTriangleOutOfDomain(Triangle *t)
{
    Vertex *v = t->getVertex(TRIANGLE_VERTEX_ONE);
    Vertex *v1 = t->getVertex(TRIANGLE_VERTEX_TWO);
    Vertex *v2 = t->getVertex(TRIANGLE_VERTEX_THREE);
    if( ( v->x < 0 || v->x > CONFIG_DOMAIN_DIMENSION || v->y < 0 || v->y > CONFIG_DOMAIN_DIMENSION) || 
        ( v1->x < 0 || v1->x > CONFIG_DOMAIN_DIMENSION || v1->y < 0 || v1->y > CONFIG_DOMAIN_DIMENSION) ||
        ( v2->x < 0 || v2->x > CONFIG_DOMAIN_DIMENSION || v2->y < 0 || v2->y > CONFIG_DOMAIN_DIMENSION) )
        return true;
    return false;
}

void setInitialBoundaryConditions(Vertex *vPtr)
{
    vPtr->isBorder = true; // Default is vertex at the border
    //
    // Boundary Conditions
    //   
    // Bottom corners
    if( (vPtr->x == 0 && vPtr->y == 0) || (vPtr->x == CONFIG_DOMAIN_DIMENSION && vPtr->y == 0) )
        vPtr->u = CONFIG_BNDR_CONDITION_BOTTOM_CORNERS;
    //  Top corners
    else if(vPtr->y == CONFIG_DOMAIN_DIMENSION && ( vPtr->x == 0 or vPtr->x == CONFIG_DOMAIN_DIMENSION ))
        vPtr->u = CONFIG_BNDR_CONDITION_TOP_CORNERS;
    // Right and left side
    else if( (vPtr->x == 0) || (vPtr->x == CONFIG_DOMAIN_DIMENSION) )
        vPtr->u = CONFIG_BNDR_CONDITION_SIDES; 
    // Bottom
    else if( vPtr->y == 0 )
        vPtr->u = CONFIG_BNDR_CONDITION_BOTTOM;
    // Top
    else if( vPtr->y == CONFIG_DOMAIN_DIMENSION )
        vPtr->u = CONFIG_BNDR_CONDITION_TOP;    
    else if( vPtr->y > 0 && vPtr->y < CONFIG_DOMAIN_DIMENSION && vPtr->x > 0 && vPtr->x < CONFIG_DOMAIN_DIMENSION ) {
        vPtr->isBorder = false; // Vertex is not at the border
        vPtr->u        = CONFIG_BNDR_CONDITION_IN_INITIAL_VALUE;
    }    
    if(vPtr->isBorder == false)
    {
        vPtr->label = vertexCount;
        vertexCount++;
    }    
}

bool isOutOfDomain(mpfr::real<MPFR_BITS_PRECISION> &x, mpfr::real<MPFR_BITS_PRECISION> &y ) 
{
    if ( ( x < 0 ) || ( x > CONFIG_DOMAIN_DIMENSION) || ( y < 0 ) || ( y > CONFIG_DOMAIN_DIMENSION) ) 
	return true;  
    return false;
}

bool monitorFunctionUpsilon(Mesh *m) 
{        
    // New values
    list<InfoMovingVertex *> verticesToMoveList;  // List containing data of vertices to move     
    Vertex *vertexIterator = m->first;
    bool mov = false;
    while( vertexIterator != NULL ) 
    {                    
        vertexIterator->visited = true;
        if (!vertexIterator->isBorder) 
        {            
            mpfr::real<MPFR_BITS_PRECISION> sum_x_total = 0, sum_y_total = 0, sum_u_total = 0;                           
            // Calculates weights
	    calculatesWeightUpsilon(vertexIterator, sum_x_total, sum_y_total, sum_u_total);                      
            // New coordinates of monitor function
            mpfr::real<MPFR_BITS_PRECISION> *newCoordinates = new mpfr::real<MPFR_BITS_PRECISION>[2];          
            newCoordinates[COORDINATE_X] = vertexIterator->x + (CONFIG_MONITOR_FUNCTION_BETA * sum_x_total) /sum_u_total ;
            newCoordinates[COORDINATE_Y] = vertexIterator->y + (CONFIG_MONITOR_FUNCTION_BETA * sum_y_total) /sum_u_total ;                           
            if ( !isOutOfDomain( newCoordinates[COORDINATE_X] , newCoordinates[COORDINATE_Y] ) )
            {
		if ( (!mov) && (newCoordinates[COORDINATE_X] != vertexIterator->x) && (newCoordinates[COORDINATE_Y] != vertexIterator->y) ) mov = true;
                // Print vertex movement for analysis
                if (CONFIG_PRINT_VERTEX_MOVIMENT)
                    Utils::printVertexMovement(vertexIterator,newCoordinates[COORDINATE_X], newCoordinates[COORDINATE_Y], CONFIG_OUTPUT_FILE_VERTEX_MOVIMENT);            
                // Creates an object containing Information to move the vertex, and inserts this object in the list of moved vertices.
                verticesToMoveList.push_back( createInfoMovingVertex(vertexIterator, newCoordinates) );                
            } else 
            {
                delete[] newCoordinates;
                //cerr << "ERROR - Mesh::movingMesh: new coordinate out of domain. " << endl;                      
            }
        }
        vertexIterator = vertexIterator->next;        
    }             
    moveVertices(verticesToMoveList);        
    return mov;
}

bool monitorFunctionLambda(Mesh *m) 
{  
    // New values
    list<InfoMovingVertex *> verticesToMoveList;  // List containing data of vertices to move         
    bool mov = false;
    mpfr::real<MPFR_BITS_PRECISION> u_max = 0;
    for (list<Adjacency *>::iterator it = m->edges.begin(); it != m->edges.end(); ++it)
    {        
        Vertex *v1 = (*it)->getVertex(ADJ_VERTEX_ONE);
        Vertex *v2 = (*it)->getVertex(ADJ_VERTEX_TWO);	
	if ( abs(v1->u - v2->u)  > u_max )  
	  u_max = abs(v1->u - v2->u);
    }      
    Vertex *vertexIterator = m->first;
    while( vertexIterator != NULL ) 
    {                    
        if (!vertexIterator->isBorder) 
        {                   
            mpfr::real<MPFR_BITS_PRECISION> sum_x_total = 0, sum_y_total = 0, sum_u_total = 0;                           
            // Calculates weights
	    calculatesWeightLambda(vertexIterator, sum_x_total, sum_y_total);                   
            // New coordinates of monitor function
            mpfr::real<MPFR_BITS_PRECISION> *newCoordinates = new mpfr::real<MPFR_BITS_PRECISION>[2];          
            newCoordinates[COORDINATE_X] = vertexIterator->x + (CONFIG_MONITOR_FUNCTION_BETA* sum_x_total)/u_max;
            newCoordinates[COORDINATE_Y] = vertexIterator->y + (CONFIG_MONITOR_FUNCTION_BETA* sum_y_total)/u_max;               
            if ( !isOutOfDomain( newCoordinates[COORDINATE_X] , newCoordinates[COORDINATE_Y] ) )
            {
	        if ( (!mov) && (newCoordinates[COORDINATE_X] != vertexIterator->x) && (newCoordinates[COORDINATE_Y] != vertexIterator->y) ) mov = true;
                // Print vertex movement for analysis
                if (CONFIG_PRINT_VERTEX_MOVIMENT)
                    Utils::printVertexMovement(vertexIterator,newCoordinates[COORDINATE_X], newCoordinates[COORDINATE_Y], CONFIG_OUTPUT_FILE_VERTEX_MOVIMENT);            
                // Creates an object containing Information to move the vertex, and inserts this object in the list of moved vertices.
                verticesToMoveList.push_back( createInfoMovingVertex(vertexIterator, newCoordinates) );                               
            } else 
            {
                delete[] newCoordinates;
                //cerr << "ERROR - Mesh::movingMesh: new coordinate out of domain. " << endl;                      
            }
        }
        vertexIterator = vertexIterator->next;        
    }             
    moveVertices(verticesToMoveList);        
    return mov;
}

bool monitorFunctionGamma(Mesh *m) 
{        
    // New values
    list<InfoMovingVertex *> verticesToMoveList;  // List containing data of vertices to move     
    Vertex *vertexIterator = m->first;
    bool mov = false;
    while( vertexIterator != NULL ) 
    {                    
        if (!vertexIterator->isBorder) 
        {            
            mpfr::real<MPFR_BITS_PRECISION> sum_x_total = 0, sum_y_total = 0, sum_u_total = 0;                           
            // Calculates weights
	    calculatesWeightGamma(vertexIterator, sum_x_total, sum_y_total, sum_u_total);                                                         
            // New coordinates of monitor function
            mpfr::real<MPFR_BITS_PRECISION> *newCoordinates = new mpfr::real<MPFR_BITS_PRECISION>[2];          
            newCoordinates[COORDINATE_X] = vertexIterator->x + (CONFIG_MONITOR_FUNCTION_BETA * sum_x_total) /sum_u_total ;
            newCoordinates[COORDINATE_Y] = vertexIterator->y + (CONFIG_MONITOR_FUNCTION_BETA * sum_y_total) /sum_u_total ;                           
	    if ( !isOutOfDomain( newCoordinates[COORDINATE_X] , newCoordinates[COORDINATE_Y] ) )
	    {
	        if ( (!mov) && (newCoordinates[COORDINATE_X] != vertexIterator->x) && (newCoordinates[COORDINATE_Y] != vertexIterator->y) ) mov = true;
                // Print vertex movement for analysis
                if (CONFIG_PRINT_VERTEX_MOVIMENT)
                    Utils::printVertexMovement(vertexIterator,newCoordinates[COORDINATE_X], newCoordinates[COORDINATE_Y], CONFIG_OUTPUT_FILE_VERTEX_MOVIMENT);            
                // Creates an object containing Information to move the vertex, and inserts this object in the list of moved vertices.
                verticesToMoveList.push_back( createInfoMovingVertex(vertexIterator, newCoordinates) );                
            } else 
            {
                delete[] newCoordinates;
                //cerr << "ERROR - Mesh::movingMesh: new coordinate out of domain. " << endl;                      
            }
        }
        vertexIterator = vertexIterator->next;        
    }             
    moveVertices(verticesToMoveList); 
    return mov;
}
//
// Laplacian Smoothing called Xi defined by Taubin
//
bool monitorFunctionXi(Mesh *m) 
{        
    // New values
    list<InfoMovingVertex *> verticesToMoveList;  // List containing data of vertices to move     
    Vertex *vertexIterator = m->first;
    bool movBeta = false, movMu = false;
    mpfr::real<MPFR_BITS_PRECISION> *newCoordinates = NULL;
    while( vertexIterator != NULL ) 
    {                    
        if (!vertexIterator->isBorder) 
        {            
            mpfr::real<MPFR_BITS_PRECISION> sum_x_total = 0, sum_y_total = 0, sum_u_total = 0;                           
            // Calculates weights
	    calculatesWeightGamma(vertexIterator, sum_x_total, sum_y_total, sum_u_total);                             
            // New coordinates of monitor function
            newCoordinates = new mpfr::real<MPFR_BITS_PRECISION>[2];          
            newCoordinates[COORDINATE_X] = vertexIterator->x + (CONFIG_MONITOR_FUNCTION_BETA * sum_x_total) /sum_u_total ;
            newCoordinates[COORDINATE_Y] = vertexIterator->y + (CONFIG_MONITOR_FUNCTION_BETA * sum_y_total) /sum_u_total ;                           
            if ( !isOutOfDomain( newCoordinates[COORDINATE_X] , newCoordinates[COORDINATE_Y] ) )
            {
	        if ( (!movBeta) && (newCoordinates[COORDINATE_X] != vertexIterator->x) && (newCoordinates[COORDINATE_Y] != vertexIterator->y) ) movBeta = true;
                // Print vertex movement for analysis
                if (CONFIG_PRINT_VERTEX_MOVIMENT)
                    Utils::printVertexMovement(vertexIterator,newCoordinates[COORDINATE_X], newCoordinates[COORDINATE_Y], CONFIG_OUTPUT_FILE_VERTEX_MOVIMENT);            
                // Creates an object containing Information to move the vertex, and inserts this object in the list of moved vertices.
                verticesToMoveList.push_back( createInfoMovingVertex(vertexIterator, newCoordinates) );                
            } else 
            {
                delete[] newCoordinates;
                //cerr << "ERROR - Mesh::movingMesh: new coordinate out of domain. " << endl;                      
            }
        }
        vertexIterator = vertexIterator->next;        
    }     
    moveVertices(verticesToMoveList);   
    vertexIterator = m->first;
    while( vertexIterator != NULL ) 
    {                    
        if (!vertexIterator->isBorder) 
        {            
            mpfr::real<MPFR_BITS_PRECISION> sum_x_total = 0, sum_y_total = 0, sum_u_total = 0;                           
            // Calculates weights
	    calculatesWeightGamma(vertexIterator, sum_x_total, sum_y_total, sum_u_total);                             
            // New coordinates of monitor function
            newCoordinates = new mpfr::real<MPFR_BITS_PRECISION>[2];          
            newCoordinates[COORDINATE_X] = vertexIterator->x - (CONFIG_MONITOR_FUNCTION_MU * sum_x_total) /sum_u_total ;
            newCoordinates[COORDINATE_Y] = vertexIterator->y - (CONFIG_MONITOR_FUNCTION_MU * sum_y_total) /sum_u_total ;                           
            if ( !isOutOfDomain( newCoordinates[COORDINATE_X] , newCoordinates[COORDINATE_Y] ) )
            {
	        if ( (!movMu) && (newCoordinates[COORDINATE_X] != vertexIterator->x) && (newCoordinates[COORDINATE_Y] != vertexIterator->y) ) movMu = true;
                // Print vertex movement for analysis
                if (CONFIG_PRINT_VERTEX_MOVIMENT)
                    Utils::printVertexMovement(vertexIterator,newCoordinates[COORDINATE_X], newCoordinates[COORDINATE_Y], CONFIG_OUTPUT_FILE_VERTEX_MOVIMENT);            
                // Creates an object containing Information to move the vertex, and inserts this object in the list of moved vertices.
                verticesToMoveList.push_back( createInfoMovingVertex(vertexIterator, newCoordinates) );                
            } else 
            {
                delete[] newCoordinates;
                //cerr << "ERROR - Mesh::movingMesh: new coordinate out of domain. " << endl;                      
            }
        }
        vertexIterator = vertexIterator->next;        
    }      
    moveVertices(verticesToMoveList);
    return movBeta && movMu;
}

bool monitorFunctionPsi(Mesh *m) 
{  
     // New values
    list<InfoMovingVertex *> verticesToMoveList;  // List containing data of vertices to move     
    Vertex *vertexIterator = m->first;
    bool movBeta = false, movMu = false;
    mpfr::real<MPFR_BITS_PRECISION> *newCoordinates = NULL;
    while( vertexIterator != NULL ) 
    {                    
        if (!vertexIterator->isBorder) 
        {            
            mpfr::real<MPFR_BITS_PRECISION> sum_x_total = 0, sum_y_total = 0, sum_u_total = 0;                           
            // Calculates weights
	    calculatesWeightGamma(vertexIterator, sum_x_total, sum_y_total, sum_u_total);                             
            // New coordinates of monitor function
            newCoordinates = new mpfr::real<MPFR_BITS_PRECISION>[2];          
            newCoordinates[COORDINATE_X] = vertexIterator->x + (CONFIG_MONITOR_FUNCTION_BETA * sum_x_total) /sum_u_total ;
            newCoordinates[COORDINATE_Y] = vertexIterator->y + (CONFIG_MONITOR_FUNCTION_BETA * sum_y_total) /sum_u_total ;                           
            if ( !isOutOfDomain( newCoordinates[COORDINATE_X] , newCoordinates[COORDINATE_Y] ) )
            {
	        if ( (!movBeta) && (newCoordinates[COORDINATE_X] != vertexIterator->x) && (newCoordinates[COORDINATE_Y] != vertexIterator->y) ) movBeta = true;
                // Print vertex movement for analysis
                if (CONFIG_PRINT_VERTEX_MOVIMENT)
                    Utils::printVertexMovement(vertexIterator,newCoordinates[COORDINATE_X], newCoordinates[COORDINATE_Y], CONFIG_OUTPUT_FILE_VERTEX_MOVIMENT);            
                // Creates an object containing Information to move the vertex, and inserts this object in the list of moved vertices.
                verticesToMoveList.push_back( createInfoMovingVertex(vertexIterator, newCoordinates) );                
            } else 
            {
                delete[] newCoordinates;
                //cerr << "ERROR - Mesh::movingMesh: new coordinate out of domain. " << endl;                      
            }
        }
        vertexIterator = vertexIterator->next;        
    }     
    moveVertices(verticesToMoveList);   
    vertexIterator = m->first;
    while( vertexIterator != NULL ) 
    {                    
        if (!vertexIterator->isBorder) 
        {            
            mpfr::real<MPFR_BITS_PRECISION> sum_x_total = 0, sum_y_total = 0, sum_u_total = 0;                           
            // Calculates weights
	    calculatesWeightGamma(vertexIterator, sum_x_total, sum_y_total, sum_u_total);                             
            // New coordinates of monitor function
            newCoordinates = new mpfr::real<MPFR_BITS_PRECISION>[2];          
            newCoordinates[COORDINATE_X] = vertexIterator->x - (CONFIG_MONITOR_FUNCTION_BETA * sum_x_total) /sum_u_total ;
            newCoordinates[COORDINATE_Y] = vertexIterator->y - (CONFIG_MONITOR_FUNCTION_BETA * sum_y_total) /sum_u_total ;                           
            if ( !isOutOfDomain( newCoordinates[COORDINATE_X] , newCoordinates[COORDINATE_Y] ) )
            {
	        if ( (!movMu) && (newCoordinates[COORDINATE_X] != vertexIterator->x) && (newCoordinates[COORDINATE_Y] != vertexIterator->y) ) movMu = true;
                // Print vertex movement for analysis
                if (CONFIG_PRINT_VERTEX_MOVIMENT)
                    Utils::printVertexMovement(vertexIterator,newCoordinates[COORDINATE_X], newCoordinates[COORDINATE_Y], CONFIG_OUTPUT_FILE_VERTEX_MOVIMENT);            
                // Creates an object containing Information to move the vertex, and inserts this object in the list of moved vertices.
                verticesToMoveList.push_back( createInfoMovingVertex(vertexIterator, newCoordinates) );                
            } else 
            {
                delete[] newCoordinates;
                //cerr << "ERROR - Mesh::movingMesh: new coordinate out of domain. " << endl;                      
            }
        }
        vertexIterator = vertexIterator->next;        
    }      
    moveVertices(verticesToMoveList);
    return movBeta && movMu;
}

bool laplacianSmoothing(Mesh *m) 
{        
    // New values
    list<InfoMovingVertex *> verticesToMoveList;  // List containing data of vertices to move     
    Vertex *vertexIterator = m->first;
    bool mov = false;
    while( vertexIterator != NULL ) 
    {    
        if (!vertexIterator->isBorder) 
        {            
	    bool badQuality = false;
            mpfr::real<MPFR_BITS_PRECISION> sum_x_total = 0, sum_y_total = 0, sum_u_total = 0;                           
            // Calculates weights
	    badQuality = calculatesWeightLaplacianSmoothing(vertexIterator, sum_x_total, sum_y_total, sum_u_total);                                                         
	    if (badQuality) 
	    {
		// New coordinates of monitor function
		mpfr::real<MPFR_BITS_PRECISION> *newCoordinates = new mpfr::real<MPFR_BITS_PRECISION>[2];          
		newCoordinates[COORDINATE_X] = vertexIterator->x + (CONFIG_LAPLACIAN_SMOOTHING_KAPA * sum_x_total) /sum_u_total ;
		newCoordinates[COORDINATE_Y] = vertexIterator->y + (CONFIG_LAPLACIAN_SMOOTHING_KAPA * sum_y_total) /sum_u_total ;                           
		if ( !isOutOfDomain( newCoordinates[COORDINATE_X] , newCoordinates[COORDINATE_Y] ) )
		{
		    if ( (!mov) && (newCoordinates[COORDINATE_X] != vertexIterator->x) && (newCoordinates[COORDINATE_Y] != vertexIterator->y) ) mov = true;
		    // Print vertex movement for analysis
		    if (CONFIG_PRINT_VERTEX_MOVIMENT)
			Utils::printVertexMovement(vertexIterator,newCoordinates[COORDINATE_X], newCoordinates[COORDINATE_Y], CONFIG_OUTPUT_FILE_VERTEX_MOVIMENT);            
		    // Creates an object containing Information to move the vertex, and inserts this object in the list of moved vertices.
		    verticesToMoveList.push_back( createInfoMovingVertex(vertexIterator, newCoordinates) );                
		} else 
		{
		    delete[] newCoordinates;
		    //cerr << "ERROR - Mesh::movingMesh: new coordinate out of domain. " << endl;                      
		}	   	      
	    }   
        }
        vertexIterator = vertexIterator->next;        
    }             
    moveVertices(verticesToMoveList); 
    for( list<Triangle *>::iterator it = m->triangleList.begin(); it != m->triangleList.end(); ++it )   
        (*it)->update();       
    return mov;
}

void calculatesWeightLambda(Vertex *v, mpfr::real<MPFR_BITS_PRECISION> &sum_x_total, mpfr::real<MPFR_BITS_PRECISION> &sum_y_total) 
{
    sum_x_total = 0;
    sum_y_total = 0;
    mpfr::real<MPFR_BITS_PRECISION> sub_xi_xc = 0, sub_yi_yc = 0, sub_ui_uc = 0, sum_ui_uc = 0;                    
    // Calc new coordinates
    for( list<Adjacency *>::iterator it = v->adjList.begin(); it != v->adjList.end(); it++ )
    {  
	int vertexNumber = ADJ_VERTEX_ONE;
	vertexNumber = ( (*it)->getVertex(ADJ_VERTEX_ONE) == v ) ? ADJ_VERTEX_TWO : ADJ_VERTEX_ONE;                
	sub_xi_xc = ((*it)->getVertex(vertexNumber)->x - v->x );
	sub_yi_yc = ((*it)->getVertex(vertexNumber)->y - v->y );
	sub_ui_uc = abs((*it)->getVertex(vertexNumber)->u - v->u );       
	sum_ui_uc = abs((*it)->getVertex(vertexNumber)->u + v->u );       
	sum_x_total += sub_xi_xc * sub_ui_uc ;
	sum_y_total += sub_yi_yc * sub_ui_uc ;
    }  
}

void calculatesWeightGamma(Vertex *v, mpfr::real<MPFR_BITS_PRECISION> &sum_x_total, mpfr::real<MPFR_BITS_PRECISION> &sum_y_total, mpfr::real<MPFR_BITS_PRECISION> &sum_u_total) 
{
    sum_x_total = 0;
    sum_x_total = 0;
    sum_u_total = 0;
    mpfr::real<MPFR_BITS_PRECISION> sub_xi_xc = 0, sub_yi_yc = 0, sub_ui_uc = 0, sum_ui_uc = 0;
    for( list<Adjacency *>::iterator it = v->adjList.begin(); it != v->adjList.end(); it++ )
    {   
	int vertexNumber = ( (*it)->getVertex(ADJ_VERTEX_ONE) == v ) ? ADJ_VERTEX_TWO : ADJ_VERTEX_ONE;                
	Vertex *vertexAdj = (*it)->getVertex(vertexNumber);
	sub_xi_xc = ( vertexAdj->x - v->x ); // Get sum of (x_i - x_c)       
	sub_yi_yc = ( vertexAdj->y - v->y ); // Get sum of (y_i - y_c)    
	sub_ui_uc = abs( vertexAdj->u - v->u );// Get sum of (u_i - u_c)        
	sum_ui_uc = abs( vertexAdj->u + v->u );// Get sum of (u_i - u_c)        
	sum_x_total += sub_xi_xc * ( sub_ui_uc ) ;
	sum_y_total += sub_yi_yc * ( sub_ui_uc ) ;
	sum_u_total += sub_ui_uc;                     
    }         
}

void calculatesWeightUpsilon(Vertex *v, mpfr::real<MPFR_BITS_PRECISION> &sum_x_total, mpfr::real<MPFR_BITS_PRECISION> &sum_y_total, mpfr::real<MPFR_BITS_PRECISION> &sum_u_total) 
{
    sum_x_total = 0;
    sum_x_total = 0;
    sum_u_total = 0;
    mpfr::real<MPFR_BITS_PRECISION> sub_xi_xc = 0, sub_yi_yc = 0, sub_ui_uc = 0, sum_ui_uc = 0;
    // Calc new coordinates
    for( list<Adjacency *>::iterator it = v->adjList.begin(); it != v->adjList.end(); it++ )
    {   
	int vertexNumber = ( (*it)->getVertex(ADJ_VERTEX_ONE) == v ) ? ADJ_VERTEX_TWO : ADJ_VERTEX_ONE;                
	Vertex *vertexAdj = (*it)->getVertex(vertexNumber);
	sub_xi_xc = ( vertexAdj->x - v->x ); // Get sum of (x_i - x_c)       
	sub_yi_yc = ( vertexAdj->y - v->y ); // Get sum of (y_i - y_c)    
	sub_ui_uc = abs( vertexAdj->u - v->u );// Get sum of (u_i - u_c)        
	sum_ui_uc = abs( vertexAdj->u + v->u );// Get sum of (u_i - u_c)        
	sum_x_total += sub_xi_xc * ( sub_ui_uc/sum_ui_uc ) ;
	sum_y_total += sub_yi_yc * ( sub_ui_uc/sum_ui_uc ) ;
	sum_u_total += sub_ui_uc / sum_ui_uc;                    
    }     
}

bool calculatesWeightLaplacianSmoothing(Vertex *v, mpfr::real<MPFR_BITS_PRECISION> &sum_x_total, mpfr::real<MPFR_BITS_PRECISION> &sum_y_total, mpfr::real<MPFR_BITS_PRECISION> &sum_u_total) 
{
    sum_x_total = 0;
    sum_x_total = 0;
    sum_u_total = 0;
    mpfr::real<MPFR_BITS_PRECISION> sub_xi_xc = 0, sub_yi_yc = 0, sum_cer_i_cer_c = 0;
    bool badQuality = false;
    for( list<Adjacency *>::iterator it = v->adjList.begin(); it != v->adjList.end(); it++ )
    {   
	int vertexNumber = ( (*it)->getVertex(ADJ_VERTEX_ONE) == v ) ? ADJ_VERTEX_TWO : ADJ_VERTEX_ONE;                
	Vertex *vertexAdj = (*it)->getVertex(vertexNumber);
	Triangle *t1 = (*it)->getTriangle(ADJ_TRIANGLE_ONE);
	Triangle *t2 = (*it)->getTriangle(ADJ_TRIANGLE_TWO);
	if ( (t1->shapeRegularityQuality > eta) || (t2->shapeRegularityQuality > eta) ) 
	  badQuality = true;
	sub_xi_xc = ( vertexAdj->x - v->x ); // Get sum of (x_i - x_c)       
	sub_yi_yc = ( vertexAdj->y - v->y ); // Get sum of (y_i - y_c)    
	sum_cer_i_cer_c = abs( (t1->shapeRegularityQuality) + (t2->shapeRegularityQuality) );// Get sum of (cer_i + cer_c)        
	sum_x_total += sub_xi_xc * ( sum_cer_i_cer_c ) ;
	sum_y_total += sub_yi_yc * ( sum_cer_i_cer_c ) ;
	sum_u_total += sum_cer_i_cer_c;                     
    }         
    return badQuality;
}
void moveVertices(list<InfoMovingVertex *> &verticesToMoveList) 
{
    // Update vertices coordinates        
    while (!verticesToMoveList.empty()) 
    {
	InfoMovingVertex *it = verticesToMoveList.front();    
	verticesToMoveList.pop_front(); 
	Vertex *vAux = it->v;
	vAux->setXY( it->coordinates[COORDINATE_X], it->coordinates[COORDINATE_Y] ); 
	delete it;
    }    
}

InfoMovingVertex *createInfoMovingVertex(Vertex *v, mpfr::real<MPFR_BITS_PRECISION> *newCoordinates) 
{
    InfoMovingVertex *mv = new InfoMovingVertex();
    mv->coordinates = newCoordinates;
    mv->v = v;  
    return mv;
}

void setZero(Mesh *m) 
{
    Vertex *vertexIterator = m->first;
    while ( vertexIterator != NULL )
    {
        if (!vertexIterator->isBorder) 
        {
            vertexIterator->previews_u  = 0;
            vertexIterator->u           = 0;
        }
        vertexIterator                  = vertexIterator->next;        
    }       
}

void printListOfClocks(list<InfoClock *> l, string s) 
{
    ofstream outputFile;  
    outputFile.open(s.c_str(), fstream::in | fstream::out | fstream::app);    
    outputFile.precision(CONFIG_OUTPUT_PRECISION);   
    outputFile << QUOTES << "Numero de Vertices" << QUOTES << COMMA << "Clocks" << QUOTES << endl;
    for( list<InfoClock *>::iterator it = l.begin(); it != l.end(); it++ )
    { 
//        float seconds = (float)(*it)->c /CLOCKS_PER_SEC;
        outputFile << QUOTES << (*it)->numberOfVertices << QUOTES << COMMA << QUOTES <<  (float)(*it)->c << QUOTES << endl;         
    }  
    outputFile.close();
}

void clearListOfClocks(list<InfoClock *> l) 
{
    for( list<InfoClock *>::iterator it = l.begin(); it != l.end(); it++ )
        delete (*it);
}

void clearListOfQualityMesh(list<InfoQualityMesh *> l) 
{
    for( list<InfoQualityMesh *>::iterator it = l.begin(); it != l.end(); it++ )
        delete (*it);
}

void clearListOfGradientsMesh(list<InfoGradientMesh *> l) 
{
    for( list<InfoGradientMesh *>::iterator it = l.begin(); it != l.end(); it++ )
        delete (*it);
}

InfoQualityMesh *getQualityMesh(Mesh *m, int timeStep)
{
    // Get array of quality
    float *qualityArray = Utils::getInformationQualityMesh(m, rho_alpha, eta);
    InfoQualityMesh *iqm = new InfoQualityMesh(timeStep, m->numberOfVertices, qualityArray[0], qualityArray[1], qualityArray[2], qualityArray[3]);
    delete[] qualityArray;
    return iqm;
}

InfoGradientMesh *getGradientsMesh(Mesh *m, int timeStep)
{
    // Get array of gradients
    float *gradientsArray = Utils::getGradients(m->edges);
    InfoGradientMesh *gradientMesh = new InfoGradientMesh(timeStep, m->numberOfVertices, gradientsArray[0], gradientsArray[1], gradientsArray[2]);  
    delete[] gradientsArray;
    return gradientMesh;
}

void printInformationQualityMesh(list<InfoQualityMesh *> l, string s) 
{
    bool printHeader = false;
    if ( !Utils::existingFile(s) ) 
    {
        printHeader = true;              
    }
    ofstream outputFile;  
    outputFile.open(s.c_str(), fstream::in | fstream::out | fstream::app);    
    if (printHeader) 
    {
        vector<string> listString;
        listString.push_back("temporal_iteration");
	listString.push_back("number_of_vertices");
        listString.push_back("rho_max");
        listString.push_back("upsilon_min");
        listString.push_back("%_rho_>_rho_alpha");
        listString.push_back("%_upsilon_<_eta");        
        // Header
        for( vector<string>::const_iterator it = listString.begin(); it != listString.end(); ++it ) 
            outputFile << QUOTES << (*it) << QUOTES << COMMA;   
        outputFile << endl;
    }
    // Content
    outputFile.precision(CONFIG_OUTPUT_PRECISION);   
    for( list<InfoQualityMesh *>::const_iterator it = l.begin(); it != l.end(); ++it ) 
    {
        outputFile << QUOTES << (*it)->temporalIteration << QUOTES << COMMA 
		   << QUOTES << (*it)->numberOfVertices << QUOTES << COMMA
                   << QUOTES << (*it)->rho_max << QUOTES << COMMA
		   << QUOTES << (*it)->upsilon_min << QUOTES << COMMA
		   << QUOTES << (*it)->percentageRhoBiggerRhoAlpha << QUOTES << COMMA
		   << QUOTES << (*it)->percentageUpsilonLowerEta << QUOTES << endl;
    }
    outputFile << endl;
    outputFile.close();
}

void printInformationGradientsMesh(list<InfoGradientMesh *> l, string s) 
{
    bool printHeader = false;
    if ( !Utils::existingFile(s) ) 
    {
        printHeader = true;              
    }
    ofstream outputFile;  
    outputFile.open(s.c_str(), fstream::in | fstream::out | fstream::app);    
    if (printHeader) 
    {
        vector<string> listString;
        listString.push_back("temporal_iteration");
	listString.push_back("number_of_vertices");
        listString.push_back("gradient_max");
        listString.push_back("gradient_min");
        listString.push_back("gradient_media");
        // Header
        for( vector<string>::const_iterator it = listString.begin(); it != listString.end(); ++it ) 
            outputFile << QUOTES << (*it) << QUOTES << COMMA;   
        outputFile << endl;
    }
    // Content
    outputFile.precision(CONFIG_OUTPUT_PRECISION);   
    for( list<InfoGradientMesh *>::const_iterator it = l.begin(); it != l.end(); ++it ) 
    {
        outputFile << QUOTES << (*it)->temporalIteration << QUOTES << COMMA 
		   << QUOTES << (*it)->numberOfVertices << QUOTES << COMMA
                   << QUOTES << (*it)->gradientMax << QUOTES << COMMA
		   << QUOTES << (*it)->gradientMin << QUOTES << COMMA
		   << QUOTES << (*it)->gradientMedia << QUOTES << endl;
    }
    outputFile << endl;
    outputFile.close();
}

void createMonitorFunctionsMap()
{
    monitorFunctions[MONITOR_FUNCTION_LAMBDA]       = monitorFunctionLambda;
    monitorFunctions[MONITOR_FUNCTION_UPSILON]      = monitorFunctionUpsilon;
    monitorFunctions[MONITOR_FUNCTION_XI]           = monitorFunctionXi;
    monitorFunctions[MONITOR_FUNCTION_GAMMA]        = monitorFunctionGamma;
    monitorFunctions[MONITOR_FUNCTION_PSI]          = monitorFunctionPsi;
    monitorFunctionsNames[MONITOR_FUNCTION_LAMBDA]  = "Lambda"; 
    monitorFunctionsNames[MONITOR_FUNCTION_UPSILON] = "Upsilon";    
    monitorFunctionsNames[MONITOR_FUNCTION_XI]      = "Xi";
    monitorFunctionsNames[MONITOR_FUNCTION_GAMMA]   = "Gamma";
    monitorFunctionsNames[MONITOR_FUNCTION_PSI]     = "Psi";      
}
void cleanOutputDirectories()
{
    stringstream tmp;
    // Delete output directory
    tmp << "exec rm -rf " << CONFIG_OUTPUT_DIRECTORY << "/*";
    system(tmp.str().c_str());  
}
void createOutputDirectories()
{
    stringstream tmp;  
    // Make output directory
    if (CONFIG_EXEC_THREAD_A)
    {
	tmp.str(""); // Clear varable
	tmp << "exec mkdir " << CONFIG_OUTPUT_DIRECTORY << CONFIG_OUTPUT_DIR_THREAD_A;  
	system(tmp.str().c_str());
    }
    if (CONFIG_EXEC_THREAD_B)
    {    
	tmp.str(""); // Clear varable
	tmp << "exec mkdir " << CONFIG_OUTPUT_DIRECTORY << "/"<< CONFIG_OUTPUT_DIR_THREAD_B;
	system(tmp.str().c_str());    
    }
    if (CONFIG_EXEC_THREAD_C)
    {    
	tmp.str(""); // Clear varable
	tmp << "exec mkdir " << CONFIG_OUTPUT_DIRECTORY << "/"<< CONFIG_OUTPUT_DIR_THREAD_C;  
	system(tmp.str().c_str());     
    }
    if (CONFIG_EXEC_THREAD_D)
    {    
	tmp.str(""); // Clear varable
	tmp << "exec mkdir " << CONFIG_OUTPUT_DIRECTORY << "/"<< CONFIG_OUTPUT_DIR_THREAD_D;
	system(tmp.str().c_str());      
    }
    if (CONFIG_EXEC_THREAD_E)
    {    
	tmp.str(""); // Clear varable
	tmp << "exec mkdir " << CONFIG_OUTPUT_DIRECTORY << "/"<< CONFIG_OUTPUT_DIR_THREAD_E;  
	system(tmp.str().c_str());     
    }
}

void readConfigurationsFromFile(string inputFileName)
{
    int width = 40;
    ifstream inputFile;       
    stringstream tmp, outputFile;
    string line;
    inputFile.open(inputFileName.data());  
    if (!inputFile.is_open())
    {
        cerr << "ERROR - to open file configuration \"" << inputFileName << "\"" << endl;
        exit(EXIT_FAILURE);            
    } 
    tmp.str("");
    tmp << setw(width) << "VARIABLE " << setw(width) << "VALUE" << endl;    
    while ( getline(inputFile, line) )
    {               
        if (line[0] != '#' )
        {  
            // int values
            size_t found_position = line.find("CONFIG_MONITOR_FUNCTION_THREAD_A "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 33);     
                CONFIG_MONITOR_FUNCTION_THREAD_A = atoi(line.c_str()); 
		tmp << setw(width) << "CONFIG_MONITOR_FUNCTION_THREAD_A: " << setw(width) << CONFIG_MONITOR_FUNCTION_THREAD_A << endl;
                continue;
            } 
            found_position = line.find("CONFIG_MONITOR_FUNCTION_THREAD_B "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 33);     
                CONFIG_MONITOR_FUNCTION_THREAD_B = atoi(line.c_str()); 
		tmp << setw(width) << "CONFIG_MONITOR_FUNCTION_THREAD_B: " << setw(width) << CONFIG_MONITOR_FUNCTION_THREAD_B << endl;
                continue;
            }
            found_position = line.find("CONFIG_MONITOR_FUNCTION_THREAD_C "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 33);     
                CONFIG_MONITOR_FUNCTION_THREAD_C = atoi(line.c_str()); 
		tmp << setw(width) << "CONFIG_MONITOR_FUNCTION_THREAD_C: " << setw(width) << CONFIG_MONITOR_FUNCTION_THREAD_C << endl;
                continue;
            }      
            found_position = line.find("CONFIG_MONITOR_FUNCTION_THREAD_D "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 33);     
                CONFIG_MONITOR_FUNCTION_THREAD_D = atoi(line.c_str()); 
		tmp << setw(width) << "CONFIG_MONITOR_FUNCTION_THREAD_D: " << setw(width) << CONFIG_MONITOR_FUNCTION_THREAD_D << endl;
                continue;
            }               
            found_position = line.find("CONFIG_MONITOR_FUNCTION_THREAD_E "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 33);     
                CONFIG_MONITOR_FUNCTION_THREAD_E = atoi(line.c_str()); 
		tmp << setw(width) << "CONFIG_MONITOR_FUNCTION_THREAD_E: " << setw(width) << CONFIG_MONITOR_FUNCTION_THREAD_E << endl;
                continue;
            }             
            found_position = line.find("CONFIG_TIME_LIMIT "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 18);     
                CONFIG_TIME_LIMIT = atoi(line.c_str()); 
		tmp << setw(width) << "CONFIG_TIME_LIMIT: " << setw(width) << CONFIG_TIME_LIMIT << endl;
                continue;
            }                        
            found_position = line.find("CONFIG_MIN_VERTICES "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 20);     
                CONFIG_MIN_VERTICES = atoi(line.c_str());           
		tmp << setw(width) << "CONFIG_MIN_VERTICES: " << setw(width) << CONFIG_MIN_VERTICES << endl;
                continue;
            }  
/*            
            found_position = line.find("CONFIG_OUTPUT_PRECISION "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 24);     
                CONFIG_OUTPUT_PRECISION = atoi(line.c_str());    
                tmp.str(""); 
                tmp << setw(width) << "CONFIG_OUTPUT_PRECISION: " << setw(width) << CONFIG_OUTPUT_PRECISION << endl;
		Utils::printString(tmp.str(), outputFileName, CONFIG_PRINT_INFO_MESSAGES);
                continue;
            }                          
*/            
            // float values            
            found_position = line.find("CONFIG_MESH_MINIMUM_ANGLE_ALPHA "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 32);     
                CONFIG_MESH_MINIMUM_ANGLE_ALPHA = atof(line.c_str());                 
		tmp << setw(width) << "CONFIG_MESH_MINIMUM_ANGLE_ALPHA: " << setw(width) << CONFIG_MESH_MINIMUM_ANGLE_ALPHA << endl;
                continue;
            }            
            found_position = line.find("CONFIG_PERC_INCREASE_GRADIENT_THETA "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 36);     
                CONFIG_PERC_INCREASE_GRADIENT_THETA = atof(line.c_str());                 
		tmp << setw(width) << "CONFIG_PERC_INCREASE_GRADIENT_THETA: " << setw(width) << CONFIG_PERC_INCREASE_GRADIENT_THETA << endl;
                continue;
            }                        
            found_position = line.find("CONFIG_TIME_VARIATION "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 22);     
                CONFIG_TIME_VARIATION = atof(line.c_str());                 
		tmp << setw(width) << "CONFIG_TIME_VARIATION: " << setw(width) << CONFIG_TIME_VARIATION << endl;
                continue;
            }              
            found_position = line.find("CONFIG_MONITOR_FUNCTION_THREAD_A "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 33);     
                CONFIG_MONITOR_FUNCTION_THREAD_A = atoi(line.c_str());    
		tmp << setw(width) << "CONFIG_MONITOR_FUNCTION_THREAD_A: " << setw(width) << CONFIG_MONITOR_FUNCTION_THREAD_A << endl;    
                continue;
            }         
            found_position = line.find("CONFIG_MONITOR_FUNCTION_THREAD_B "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 33);     
                CONFIG_MONITOR_FUNCTION_THREAD_B = atoi(line.c_str());    
		tmp << setw(width) << "CONFIG_MONITOR_FUNCTION_THREAD_B: " << setw(width) << CONFIG_MONITOR_FUNCTION_THREAD_B << endl;    
                continue;
            }
            found_position = line.find("CONFIG_MONITOR_FUNCTION_THREAD_C "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 33);     
                CONFIG_MONITOR_FUNCTION_THREAD_C = atoi(line.c_str());    
		tmp << setw(width) << "CONFIG_MONITOR_FUNCTION_THREAD_C: " << setw(width) << CONFIG_MONITOR_FUNCTION_THREAD_C << endl;    
                continue;
            }                        
            found_position = line.find("CONFIG_MONITOR_FUNCTION_THREAD_D "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 33);     
                CONFIG_MONITOR_FUNCTION_THREAD_D = atoi(line.c_str());    
		tmp << setw(width) << "CONFIG_MONITOR_FUNCTION_THREAD_D: " << setw(width) << CONFIG_MONITOR_FUNCTION_THREAD_D << endl;    
                continue;
            }                      
            found_position = line.find("CONFIG_MONITOR_FUNCTION_THREAD_E "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 33);     
                CONFIG_MONITOR_FUNCTION_THREAD_E = atoi(line.c_str());    
		tmp << setw(width) << "CONFIG_MONITOR_FUNCTION_THREAD_E: " << setw(width) << CONFIG_MONITOR_FUNCTION_THREAD_E << endl;    
                continue;
            }                        
            found_position = line.find("CONFIG_MONITOR_FUNCTION_BETA "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 29);     
                CONFIG_MONITOR_FUNCTION_BETA = atof(line.c_str());                 
		tmp << setw(width) << "CONFIG_MONITOR_FUNCTION_BETA: " << setw(width) << CONFIG_MONITOR_FUNCTION_BETA << endl;
                continue;
            }
            found_position = line.find("CONFIG_LAPLACIAN_SMOOTHING_KAPA "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 32);     
                CONFIG_LAPLACIAN_SMOOTHING_KAPA = atof(line.c_str());                 
		tmp << setw(width) << "CONFIG_LAPLACIAN_SMOOTHING_KAPA: " << setw(width) << CONFIG_LAPLACIAN_SMOOTHING_KAPA << endl;
                continue;
            }            
            found_position = line.find("CONFIG_MONITOR_FUNCTION_MU "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 27);     
                CONFIG_MONITOR_FUNCTION_MU = atof(line.c_str());                 
		tmp << setw(width) << "CONFIG_MONITOR_FUNCTION_MU: " << setw(width) << CONFIG_MONITOR_FUNCTION_MU << endl;
                continue;
            }                           
            found_position = line.find("CONFIG_MOVING_MESH_ETA "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 23);     
                CONFIG_MOVING_MESH_ETA = atof(line.c_str());                 
		tmp << setw(width) << "CONFIG_MOVING_MESH_ETA: " << setw(width) << CONFIG_MOVING_MESH_ETA << endl;
                continue;
            }    
            found_position = line.find("CONFIG_CG_PRESICION_EPSILON "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 28);     
                CONFIG_CG_PRESICION_EPSILON = atof(line.c_str());                 
		tmp << setw(width) << "CONFIG_CG_PRESICION_EPSILON: " << setw(width) << CONFIG_CG_PRESICION_EPSILON << endl;
                continue;
            }                            
            // Boundary conditions
            found_position = line.find("CONFIG_BNDR_CONDITION_BOTTOM_CORNERS "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 37);     
                CONFIG_BNDR_CONDITION_BOTTOM_CORNERS = atof(line.c_str());                 
		tmp << setw(width) << "CONFIG_BNDR_CONDITION_BOTTOM_CORNERS: " << setw(width) << CONFIG_BNDR_CONDITION_BOTTOM_CORNERS << endl;                    
                continue;
            }         
            found_position = line.find("CONFIG_BNDR_CONDITION_TOP_CORNERS "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 34);     
                CONFIG_BNDR_CONDITION_TOP_CORNERS = atof(line.c_str());   
		tmp << setw(width) << "CONFIG_BNDR_CONDITION_TOP_CORNERS: " << setw(width) << CONFIG_BNDR_CONDITION_TOP_CORNERS << endl;                    
                continue;
            }        
            found_position = line.find("CONFIG_BNDR_CONDITION_TOP "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 26);     
                CONFIG_BNDR_CONDITION_TOP = atof(line.c_str());                 
		tmp << setw(width) << "CONFIG_BNDR_CONDITION_TOP: " << setw(width) << CONFIG_BNDR_CONDITION_TOP << endl;                    
                continue;
            }         
            found_position = line.find("CONFIG_BNDR_CONDITION_BOTTOM "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 29);     
                CONFIG_BNDR_CONDITION_BOTTOM = atof(line.c_str());                 
		tmp << setw(width) << "CONFIG_BNDR_CONDITION_BOTTOM: " << setw(width) << CONFIG_BNDR_CONDITION_BOTTOM << endl;                    
                continue;
            }                     
            found_position = line.find("CONFIG_BNDR_CONDITION_SIDES "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 28);     
                CONFIG_BNDR_CONDITION_SIDES = atof(line.c_str());                 
		tmp << setw(width) << "CONFIG_BNDR_CONDITION_SIDES: " << setw(width) << CONFIG_BNDR_CONDITION_SIDES  << endl;       
                continue;
            }                            
            found_position = line.find("CONFIG_BNDR_CONDITION_IN_INITIAL_VALUE "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 39);     
                CONFIG_BNDR_CONDITION_IN_INITIAL_VALUE = atof(line.c_str());                 
		tmp << setw(width) << "CONFIG_BNDR_CONDITION_IN_INITIAL_VALUE: " << setw(width) << CONFIG_BNDR_CONDITION_IN_INITIAL_VALUE << endl;
                continue;
            }                              
            // bool values  
            found_position = line.find("CONFIG_EXEC_THREAD_A "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 21);     
                CONFIG_EXEC_THREAD_A = !line.compare("true") ? true : false;
		tmp << setw(width) << "CONFIG_EXEC_THREAD_A: " << setw(width) << CONFIG_EXEC_THREAD_A << endl;
                continue;
            }                                  
            found_position = line.find("CONFIG_EXEC_THREAD_B "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 21);     
                CONFIG_EXEC_THREAD_B = !line.compare("true") ? true : false;
		tmp << setw(width) << "CONFIG_EXEC_THREAD_B: " << setw(width) << CONFIG_EXEC_THREAD_B << endl;
                continue;
            }   
            found_position = line.find("CONFIG_EXEC_THREAD_C "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 21);     
                CONFIG_EXEC_THREAD_C = !line.compare("true") ? true : false;
		tmp << setw(width) << "CONFIG_EXEC_THREAD_C: " << setw(width) << CONFIG_EXEC_THREAD_C << endl;
                continue;
            }       
            found_position = line.find("CONFIG_EXEC_THREAD_D "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 21);     
                CONFIG_EXEC_THREAD_D = !line.compare("true") ? true : false;
		tmp << setw(width) << "CONFIG_EXEC_THREAD_D: " << setw(width) << CONFIG_EXEC_THREAD_D << endl;
                continue;
            }    
            found_position = line.find("CONFIG_EXEC_THREAD_E "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 21);     
                CONFIG_EXEC_THREAD_E = !line.compare("true") ? true : false;
		tmp << setw(width) << "CONFIG_EXEC_THREAD_E: " << setw(width) << CONFIG_EXEC_THREAD_E << endl;
                continue;
            }               
            found_position = line.find("CONFIG_DISPLAY_MESH "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 20);     
                CONFIG_DISPLAY_MESH = !line.compare("true") ? true : false;
		tmp << setw(width) << "CONFIG_DISPLAY_MESH: " << setw(width) << CONFIG_DISPLAY_MESH << endl;
                continue;
            }                      
            found_position = line.find("CONFIG_SHOW_TRIANGLE "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 21);     
                CONFIG_SHOW_TRIANGLE = !line.compare("true") ? true : false;
		tmp << setw(width) << "CONFIG_SHOW_TRIANGLE: " << setw(width) << CONFIG_SHOW_TRIANGLE << endl;
                continue;
            }  
            found_position = line.find("CONFIG_SHOW_CIRCLE "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 19);     
                CONFIG_SHOW_CIRCLE = !line.compare("true") ? true : false;
		tmp << setw(width) << "CONFIG_SHOW_CIRCLE: " << setw(width) << CONFIG_SHOW_CIRCLE << endl;
                continue;
            }             
            found_position = line.find("CONFIG_SHOW_VORONOI_DIAGRAM "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 28);     
                CONFIG_SHOW_VORONOI_DIAGRAM = !line.compare("true") ? true : false;
		tmp << setw(width) << "CONFIG_SHOW_VORONOI_DIAGRAM: " << setw(width) << CONFIG_SHOW_VORONOI_DIAGRAM << endl;
                continue;
            }                      
            found_position = line.find("CONFIG_SHOW_VERTEX_LABEL "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 25);     
                CONFIG_SHOW_VERTEX_LABEL = !line.compare("true") ? true : false;
		tmp << setw(width) << "CONFIG_SHOW_VERTEX_LABEL: " << setw(width) << CONFIG_SHOW_VERTEX_LABEL << endl;
                continue;
            }       
            found_position = line.find("CONFIG_PRINT_CLOCKS_VALUES "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 27);     
                CONFIG_PRINT_CLOCKS_VALUES = !line.compare("true") ? true : false;
		tmp << setw(width) << "CONFIG_PRINT_CLOCKS_VALUES: " << setw(width) << CONFIG_PRINT_CLOCKS_VALUES << endl;
                continue;
            }                                         
            found_position = line.find("CONFIG_PRINT_VERTEX_VALUE "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 26);     
                CONFIG_PRINT_VERTEX_VALUE = !line.compare("true") ? true : false;
		tmp << setw(width) << "CONFIG_PRINT_VERTEX_VALUE: " << setw(width) << CONFIG_PRINT_VERTEX_VALUE << endl;
                continue;
            }                     
            found_position = line.find("CONFIG_PRINT_ADJACENCY_VALUE "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 29);     
                CONFIG_PRINT_ADJACENCY_VALUE = !line.compare("true") ? true : false;
		tmp  << setw(width) << "CONFIG_PRINT_ADJACENCY_VALUE: " << setw(width) << CONFIG_PRINT_ADJACENCY_VALUE << endl;
                continue;
            }                
            found_position = line.find("CONFIG_PRINT_TRIANGLE_VALUE "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 28);     
                CONFIG_PRINT_TRIANGLE_VALUE = !line.compare("true") ? true : false;
		tmp << setw(width) << "CONFIG_PRINT_TRIANGLE_VALUE: " << setw(width) << CONFIG_PRINT_TRIANGLE_VALUE << endl;
                continue;
            }    
            found_position = line.find("CONFIG_PRINT_VERTEX_MOVIMENT "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 29);     
                CONFIG_PRINT_VERTEX_MOVIMENT = !line.compare("true") ? true : false;
		tmp << setw(width) << "CONFIG_PRINT_VERTEX_MOVIMENT: " << setw(width) << CONFIG_PRINT_VERTEX_MOVIMENT << endl;
                continue;
            }                      
            found_position = line.find("CONFIG_PRINT_MATRIX_A_B "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 24);     
                CONFIG_PRINT_MATRIX_A_B = !line.compare("true") ? true : false;
		tmp << setw(width) << "CONFIG_PRINT_MATRIX_A_B: " << setw(width) << CONFIG_PRINT_MATRIX_A_B << endl;
                continue;
            }               
            found_position = line.find("CONFIG_PRINT_INFO_MESSAGES "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 27);     
                CONFIG_PRINT_INFO_MESSAGES = !line.compare("true") ? true : false;
		tmp << setw(width) << "CONFIG_PRINT_INFO_MESSAGES: " << setw(width) << CONFIG_PRINT_INFO_MESSAGES << endl;
                continue;
            }
            found_position = line.find("CONFIG_EXEC_CUTHILL_MCKEE_ALG "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 30);     
                CONFIG_EXEC_CUTHILL_MCKEE_ALG = !line.compare("true") ? true : false;
		tmp << setw(width) << "CONFIG_EXEC_CUTHILL_MCKEE_ALG: " << setw(width) << CONFIG_EXEC_CUTHILL_MCKEE_ALG << endl;    
                continue;                
            }                        
            found_position = line.find("CONFIG_CUTHILL_MCKEE_REVERSE "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 29);     
                CONFIG_CUTHILL_MCKEE_REVERSE = !line.compare("true") ? true : false;
		tmp << setw(width) << "CONFIG_CUTHILL_MCKEE_REVERSE: " << setw(width) << CONFIG_CUTHILL_MCKEE_REVERSE << endl;    
                continue;                
            }                               
            // string values  
            found_position = line.find("CONFIG_OUTPUT_MAIN_FILE "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 24);     
                CONFIG_OUTPUT_MAIN_FILE = line;
		tmp << setw(width) << "CONFIG_OUTPUT_MAIN_FILE: " << setw(width) << CONFIG_OUTPUT_MAIN_FILE << endl;
                continue;
            }                
            found_position = line.find("CONFIG_OUTPUT_FILE_QUALITY_MESH "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 32);     
                CONFIG_OUTPUT_FILE_QUALITY_MESH = line;
		tmp << setw(width) << "CONFIG_OUTPUT_FILE_QUALITY_MESH: " << setw(width) << CONFIG_OUTPUT_FILE_QUALITY_MESH << endl;
                continue;
            }  
            found_position = line.find("CONFIG_OUTPUT_FILE_GRADIENT_MESH "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 33);     
                CONFIG_OUTPUT_FILE_GRADIENT_MESH = line;
		tmp << setw(width) << "CONFIG_OUTPUT_FILE_GRADIENT_MESH: " << setw(width) << CONFIG_OUTPUT_FILE_GRADIENT_MESH << endl;
                continue;
            }            
            found_position = line.find("CONFIG_OUTPUT_FILE_THREAD_A "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 28);     
                CONFIG_OUTPUT_FILE_THREAD_A = line;
		tmp << setw(width) << "CONFIG_OUTPUT_FILE_THREAD_A: " << setw(width) << CONFIG_OUTPUT_FILE_THREAD_A << endl;
                continue;
            }    
            found_position = line.find("CONFIG_OUTPUT_FILE_THREAD_B "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 28);     
                CONFIG_OUTPUT_FILE_THREAD_B = line;
		tmp << setw(width) << "CONFIG_OUTPUT_FILE_THREAD_B: " << setw(width) << CONFIG_OUTPUT_FILE_THREAD_B << endl;
                continue;
            }              
            found_position = line.find("CONFIG_OUTPUT_FILE_THREAD_C "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 28);     
                CONFIG_OUTPUT_FILE_THREAD_C = line;
		tmp << setw(width) << "CONFIG_OUTPUT_FILE_THREAD_C: " << setw(width) << CONFIG_OUTPUT_FILE_THREAD_C << endl;
                continue;
            }   
            found_position = line.find("CONFIG_OUTPUT_FILE_THREAD_D "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 28);     
                CONFIG_OUTPUT_FILE_THREAD_D = line;
		tmp << setw(width) << "CONFIG_OUTPUT_FILE_THREAD_D: " << setw(width) << CONFIG_OUTPUT_FILE_THREAD_D << endl;
                continue;
            }    
            found_position = line.find("CONFIG_OUTPUT_FILE_THREAD_E "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 28);     
                CONFIG_OUTPUT_FILE_THREAD_E = line;
		tmp << setw(width) << "CONFIG_OUTPUT_FILE_THREAD_E: " << setw(width) << CONFIG_OUTPUT_FILE_THREAD_E << endl;
                continue;
            }                         
            found_position = line.find("CONFIG_OUTPUT_DIR_THREAD_A "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 27);     
                CONFIG_OUTPUT_DIR_THREAD_A = line;
		tmp << setw(width) << "CONFIG_OUTPUT_DIR_THREAD_A: " << setw(width) << CONFIG_OUTPUT_DIR_THREAD_A << endl;
                continue;
            }  
            found_position = line.find("CONFIG_OUTPUT_DIR_THREAD_B "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 27);     
                CONFIG_OUTPUT_DIR_THREAD_B = line;
		tmp << setw(width) << "CONFIG_OUTPUT_DIR_THREAD_B: " << setw(width) << CONFIG_OUTPUT_DIR_THREAD_B << endl;
                continue;
            }    
            found_position = line.find("CONFIG_OUTPUT_DIR_THREAD_C "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 27);     
                CONFIG_OUTPUT_DIR_THREAD_C = line;
		tmp << setw(width) << "CONFIG_OUTPUT_DIR_THREAD_C: " << setw(width) << CONFIG_OUTPUT_DIR_THREAD_C << endl;
                continue;
            }              
            found_position = line.find("CONFIG_OUTPUT_DIR_THREAD_D "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 27);     
                CONFIG_OUTPUT_DIR_THREAD_D = line;
		tmp << setw(width) << "CONFIG_OUTPUT_DIR_THREAD_D: " << setw(width) << CONFIG_OUTPUT_DIR_THREAD_D << endl;
                continue;
            }             
            found_position = line.find("CONFIG_OUTPUT_DIR_THREAD_E "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 27);     
                CONFIG_OUTPUT_DIR_THREAD_E = line;
		tmp << setw(width) << "CONFIG_OUTPUT_DIR_THREAD_E: " << setw(width) << CONFIG_OUTPUT_DIR_THREAD_E << endl;
                continue;
            }            
            found_position = line.find("CONFIG_OUTPUT_FILE_VERTEX_VALUE "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 32);     
                CONFIG_OUTPUT_FILE_VERTEX_VALUE = line;
		tmp << setw(width) << "CONFIG_OUTPUT_FILE_VERTEX_VALUE: " << setw(width) << CONFIG_OUTPUT_FILE_VERTEX_VALUE << endl;
                continue;
            }            
            found_position = line.find("CONFIG_OUTPUT_FILE_TRIANGLE_VALUE "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 34);     
                CONFIG_OUTPUT_FILE_TRIANGLE_VALUE = line;
		tmp  << setw(width) << "CONFIG_OUTPUT_FILE_TRIANGLE_VALUE: " << setw(width) << CONFIG_OUTPUT_FILE_TRIANGLE_VALUE << endl;    
                continue;
            }            
            found_position = line.find("CONFIG_OUTPUT_FILE_VERTEX_MOVIMENT "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 35);     
                CONFIG_OUTPUT_FILE_VERTEX_MOVIMENT = line;
		tmp << setw(width) << "CONFIG_OUTPUT_FILE_VERTEX_MOVIMENT: " << setw(width) << CONFIG_OUTPUT_FILE_VERTEX_MOVIMENT << endl;                    
                continue;
            }       
            found_position = line.find("CONFIG_OUTPUT_FILE_MATRIX_A "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 28);     
                CONFIG_OUTPUT_FILE_MATRIX_A = line;
		tmp << setw(width) << "CONFIG_OUTPUT_FILE_MATRIX_A: " << setw(width) << CONFIG_OUTPUT_FILE_MATRIX_A << endl;
                continue;
            }            
            found_position = line.find("CONFIG_OUTPUT_FILE_MATRIX_B "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 28);     
                CONFIG_OUTPUT_FILE_MATRIX_B = line;
		tmp << setw(width) << "CONFIG_OUTPUT_FILE_MATRIX_B: " << setw(width) << CONFIG_OUTPUT_FILE_MATRIX_B << endl;    
                continue;
            }                           
            found_position = line.find("CONFIG_OUTPUT_FILE_CLOCK_CMR "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 29);     
                CONFIG_OUTPUT_FILE_CLOCK_CMR = line;
		tmp << setw(width) << "CONFIG_OUTPUT_FILE_CLOCK_CMR: " << setw(width) << CONFIG_OUTPUT_FILE_CLOCK_CMR << endl;    
                continue;
            }                      
            found_position = line.find("CONFIG_OUTPUT_FILE_CLOCK_FV "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 28);     
                CONFIG_OUTPUT_FILE_CLOCK_FV = line;
		tmp << setw(width) << "CONFIG_OUTPUT_FILE_CLOCK_FV: " << setw(width) << CONFIG_OUTPUT_FILE_CLOCK_FV << endl;    
                continue;
            }                
            found_position = line.find("CONFIG_OUTPUT_FILE_CLOCK_CG "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 28);     
                CONFIG_OUTPUT_FILE_CLOCK_CG = line;
		tmp << setw(width) << "CONFIG_OUTPUT_FILE_CLOCK_CG: " << setw(width) << CONFIG_OUTPUT_FILE_CLOCK_CG << endl;    
                continue;
            }                   
            found_position = line.find("CONFIG_OUTPUT_FILE_CLOCK_MM "); 
            if (found_position != string::npos)
            {
                line.erase(found_position, 28);     
                CONFIG_OUTPUT_FILE_CLOCK_MM = line;
		tmp << setw(width) << "CONFIG_OUTPUT_FILE_CLOCK_MM: " << setw(width) << CONFIG_OUTPUT_FILE_CLOCK_MM << endl;    
                continue;
            }                
            cerr << "ERROR - unknown configuration variable \"" << line << "\"" << endl;
            exit(EXIT_FAILURE);                                   
        }        
    }
    inputFile.close();                                                                                              
    tmp << "#################################################################################" << endl;
    // Setting output file
    outputFile.str("");
    outputFile << CONFIG_OUTPUT_DIRECTORY << "/" << CONFIG_OUTPUT_MAIN_FILE;     
    Utils::printString(tmp.str(), outputFile.str(), CONFIG_PRINT_INFO_MESSAGES);
}  

void * adapterCloneMesh(void *arg) {
    Mesh *meshToClone = (Mesh *) arg;
    Mesh *newMesh = meshToClone->cloneMesh();
    pthread_exit( (void *) newMesh);
}